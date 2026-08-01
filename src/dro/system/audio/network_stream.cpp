#include "network_stream.h"

#include <QDeadlineTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace
{
const int HEADER_WAIT_MS = 10000;
const int READ_WAIT_MS = 10000;
const qint64 HEADER_BYTES = 32768;
} // namespace

NetworkStream::NetworkStream(const QUrl &p_url)
    : QObject(nullptr)
{
  m_manager = new QNetworkAccessManager(this);

  QNetworkRequest l_request(p_url);
  l_request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

  m_reply = m_manager->get(l_request);
  connect(m_reply, &QIODevice::readyRead, this, &NetworkStream::handle_ready_read);
  connect(m_reply, &QNetworkReply::finished, this, &NetworkStream::handle_finished);
}

NetworkStream::~NetworkStream()
{
  if (m_reply != nullptr)
  {
    m_reply->disconnect(this);
    m_reply->abort();
    m_reply->deleteLater();
    m_reply = nullptr;
  }

  QMutexLocker l_locker(&m_mutex);
  m_finished = true;
  m_wait.wakeAll();
}

void NetworkStream::handle_ready_read()
{
  const QByteArray l_data = m_reply->readAll();
  if (l_data.isEmpty())
  {
    return;
  }

  QMutexLocker l_locker(&m_mutex);
  m_buffer.append(l_data);
  m_wait.wakeAll();
}

void NetworkStream::handle_finished()
{
  QMutexLocker l_locker(&m_mutex);
  if (m_reply != nullptr && m_reply->error() != QNetworkReply::NoError)
  {
    m_failed = true;
  }
  m_finished = true;
  m_wait.wakeAll();
}

bool NetworkStream::wait_for_header()
{
  QMutexLocker l_locker(&m_mutex);
  QDeadlineTimer l_deadline(HEADER_WAIT_MS);

  while (!m_finished && m_buffer.size() < HEADER_BYTES)
  {
    if (!m_wait.wait(&m_mutex, l_deadline))
    {
      break;
    }
  }

  return !m_failed && !m_buffer.isEmpty();
}

ma_result NetworkStream::read(void *p_buffer, size_t p_bytes, size_t *r_read)
{
  QMutexLocker l_locker(&m_mutex);

  QDeadlineTimer l_deadline(READ_WAIT_MS);
  while (!m_finished && (m_cursor + qint64(p_bytes)) > m_buffer.size())
  {
    if (!m_wait.wait(&m_mutex, l_deadline))
    {
      break;
    }
  }

  const qint64 l_available = m_buffer.size() - m_cursor;
  if (l_available <= 0)
  {
    if (r_read != nullptr)
    {
      *r_read = 0;
    }
    return MA_AT_END;
  }

  const size_t l_to_copy = size_t(qMin(qint64(p_bytes), l_available));
  memcpy(p_buffer, m_buffer.constData() + m_cursor, l_to_copy);
  m_cursor += qint64(l_to_copy);

  if (r_read != nullptr)
  {
    *r_read = l_to_copy;
  }
  return MA_SUCCESS;
}

ma_result NetworkStream::seek(ma_int64 p_offset, ma_seek_origin p_origin)
{
  QMutexLocker l_locker(&m_mutex);

  qint64 l_target = 0;
  if (p_origin == ma_seek_origin_start)
  {
    l_target = p_offset;
  }
  else if (p_origin == ma_seek_origin_current)
  {
    l_target = m_cursor + p_offset;
  }
  else
  {
    if (!m_finished)
    {
      return MA_INVALID_OPERATION;
    }
    l_target = m_buffer.size() + p_offset;
  }

  if (l_target < 0)
  {
    return MA_INVALID_ARGS;
  }

  if (l_target > m_buffer.size())
  {
    QDeadlineTimer l_deadline(READ_WAIT_MS);
    while (!m_finished && l_target > m_buffer.size())
    {
      if (!m_wait.wait(&m_mutex, l_deadline))
      {
        break;
      }
    }
    if (l_target > m_buffer.size())
    {
      return MA_BAD_SEEK;
    }
  }

  m_cursor = l_target;
  return MA_SUCCESS;
}

ma_result NetworkStream::on_read(ma_decoder *pDecoder, void *pBufferOut, size_t bytesToRead, size_t *pBytesRead)
{
  NetworkStream *l_self = static_cast<NetworkStream *>(pDecoder->pUserData);
  if (l_self == nullptr)
  {
    return MA_INVALID_ARGS;
  }
  return l_self->read(pBufferOut, bytesToRead, pBytesRead);
}

ma_result NetworkStream::on_seek(ma_decoder *pDecoder, ma_int64 byteOffset, ma_seek_origin origin)
{
  NetworkStream *l_self = static_cast<NetworkStream *>(pDecoder->pUserData);
  if (l_self == nullptr)
  {
    return MA_INVALID_ARGS;
  }
  return l_self->seek(byteOffset, origin);
}
