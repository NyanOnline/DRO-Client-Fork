#include "draudio.h"

#include "dro/system/audio/audio_backend.h"

#include <QString>

QString DRAudio::get_audio_error(const int32_t p_error_code)
{
  return audio_backend::error_string(p_error_code);
}
