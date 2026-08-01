#pragma once

#include <QMetaType>
#include <QString>

namespace DRAudio
{
enum class Family
{
  FSystem,
  FEffect,
  FMusic,
  FVideo,
  FBlip,
};

enum Option
{
  OSuppressed = 0x1,
  OIgnoreSuppression = 0x2,

  // engine

  /**
   * If enabled, the engine will suppress all audio when the application is inactive.
   */
  OEngineSuppressBackgroundAudio = 0x4,
};
Q_DECLARE_FLAGS(Options, Option)

QString get_audio_error(const int32_t p_error_code);
} // namespace DRAudio
Q_DECLARE_METATYPE(DRAudio::Options)
