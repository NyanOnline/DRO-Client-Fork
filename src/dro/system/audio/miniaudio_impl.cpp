#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio_config.h"

#include <miniaudio/extras/decoders/libvorbis/miniaudio_libvorbis.h>
#include <miniaudio/extras/decoders/libopus/miniaudio_libopus.h>

#include <cmath>
#include <cstring>
#include <vector>

namespace
{
ma_decoding_backend_vtable *g_backends[] = {
    ma_decoding_backend_libvorbis,
    ma_decoding_backend_libopus,
};

struct CombFilter
{
  std::vector<float> buffer;
  size_t index = 0;
  float store = 0.0f;
  float feedback = 0.0f;
  float damp = 0.0f;

  void resize(size_t p_size)
  {
    buffer.assign(p_size, 0.0f);
    index = 0;
    store = 0.0f;
  }

  float process(float p_input)
  {
    const float l_output = buffer[index];
    store = l_output * (1.0f - damp) + store * damp;
    buffer[index] = p_input + store * feedback;
    if (++index >= buffer.size())
    {
      index = 0;
    }
    return l_output;
  }
};

struct AllpassFilter
{
  std::vector<float> buffer;
  size_t index = 0;
  float feedback = 0.5f;

  void resize(size_t p_size)
  {
    buffer.assign(p_size, 0.0f);
    index = 0;
  }

  float process(float p_input)
  {
    const float l_buffered = buffer[index];
    const float l_output = -p_input + l_buffered;
    buffer[index] = p_input + l_buffered * feedback;
    if (++index >= buffer.size())
    {
      index = 0;
    }
    return l_output;
  }
};

struct ReverbChannel
{
  CombFilter combs[4];
  AllpassFilter allpasses[2];
};

struct ReverbNode
{
  ma_node_base base;
  ma_uint32 channels = 0;
  std::vector<ReverbChannel> channel_state;
  float wet = 0.28f;
  float dry = 0.72f;
};

const size_t COMB_TUNING[4] = {1116, 1188, 1277, 1356};
const size_t ALLPASS_TUNING[2] = {556, 441};

void reverb_process(ma_node *pNode, const float **ppFramesIn, ma_uint32 *pFrameCountIn, float **ppFramesOut,
                    ma_uint32 *pFrameCountOut)
{
  ReverbNode *l_node = reinterpret_cast<ReverbNode *>(pNode);
  const ma_uint32 l_frame_count = *pFrameCountOut < *pFrameCountIn ? *pFrameCountOut : *pFrameCountIn;
  const ma_uint32 l_channels = l_node->channels;

  const float *l_in = ppFramesIn[0];
  float *l_out = ppFramesOut[0];

  for (ma_uint32 iFrame = 0; iFrame < l_frame_count; ++iFrame)
  {
    for (ma_uint32 iChannel = 0; iChannel < l_channels; ++iChannel)
    {
      const float l_input = l_in[iFrame * l_channels + iChannel];
      ReverbChannel &l_state = l_node->channel_state[iChannel];

      float l_sum = 0.0f;
      for (int i = 0; i < 4; ++i)
      {
        l_sum += l_state.combs[i].process(l_input * 0.015f);
      }
      for (int i = 0; i < 2; ++i)
      {
        l_sum = l_state.allpasses[i].process(l_sum);
      }

      l_out[iFrame * l_channels + iChannel] = l_input * l_node->dry + l_sum * l_node->wet;
    }
  }

  *pFrameCountIn = l_frame_count;
  *pFrameCountOut = l_frame_count;
}

ma_node_vtable g_reverb_vtable = {
    reverb_process,
    nullptr,
    1,
    1,
    0,
};
} // namespace

ma_decoding_backend_vtable **audio_backend::custom_backends()
{
  return g_backends;
}

ma_uint32 audio_backend::custom_backend_count()
{
  return sizeof(g_backends) / sizeof(g_backends[0]);
}

ma_result audio_backend::reverb_node_init(ma_node_graph *p_graph, ma_uint32 p_channels, ma_uint32 p_sample_rate,
                                          ma_node **p_node)
{
  if (p_graph == nullptr || p_node == nullptr || p_channels == 0)
  {
    return MA_INVALID_ARGS;
  }

  ReverbNode *l_node = new (std::nothrow) ReverbNode();
  if (l_node == nullptr)
  {
    return MA_OUT_OF_MEMORY;
  }

  l_node->channels = p_channels;
  l_node->channel_state.resize(p_channels);

  const double l_scale = double(p_sample_rate) / 44100.0;
  for (ma_uint32 iChannel = 0; iChannel < p_channels; ++iChannel)
  {
    ReverbChannel &l_state = l_node->channel_state[iChannel];
    const size_t l_spread = size_t(iChannel) * 23;
    for (int i = 0; i < 4; ++i)
    {
      l_state.combs[i].resize(size_t(double(COMB_TUNING[i] + l_spread) * l_scale) + 1);
      l_state.combs[i].feedback = 0.84f;
      l_state.combs[i].damp = 0.2f;
    }
    for (int i = 0; i < 2; ++i)
    {
      l_state.allpasses[i].resize(size_t(double(ALLPASS_TUNING[i] + l_spread) * l_scale) + 1);
      l_state.allpasses[i].feedback = 0.5f;
    }
  }

  ma_node_config l_config = ma_node_config_init();
  l_config.vtable = &g_reverb_vtable;
  l_config.pInputChannels = &l_node->channels;
  l_config.pOutputChannels = &l_node->channels;

  const ma_result l_result = ma_node_init(p_graph, &l_config, nullptr, &l_node->base);
  if (l_result != MA_SUCCESS)
  {
    delete l_node;
    return l_result;
  }

  *p_node = reinterpret_cast<ma_node *>(l_node);
  return MA_SUCCESS;
}

void audio_backend::reverb_node_uninit(ma_node *p_node)
{
  if (p_node == nullptr)
  {
    return;
  }
  ReverbNode *l_node = reinterpret_cast<ReverbNode *>(p_node);
  ma_node_uninit(&l_node->base, nullptr);
  delete l_node;
}
