
#ifndef _TASK_CODEC_CONSTANT_H
#define _TASK_CODEC_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../FlashEyesCoreLibConfig.h"
#if (_CONF_TASK_CODEC_CONSTANT_ENABLED)
#include "../os_system/SystemCommon.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define TASK_CODEC_ENCODED_UNIT_LEN_NONE          1
#define TASK_CODEC_ENCODED_UNIT_LEN_HEX           2
#define TASK_CODEC_BASE_HEX                       16
#define TASK_CODEC_DECODED_UNIT_LEN               1
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)


/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
class TaskCodecConstant
{
public:
  enum TaskCodecType
  {
    TASK_CODEC_NONE = 0,
    TASK_CODEC_HEX
  };

  static bool isValidCodecType(byte codecType)
  {
    switch (codecType)
    {
    case (byte)TaskCodecType::TASK_CODEC_HEX:
      return true;
    default:
      return false;
    }
  }
};
#endif // _CONF_TASK_CODEC_CONSTANT_ENABLED

#endif // _TASK_CODEC_CONSTANT_H

