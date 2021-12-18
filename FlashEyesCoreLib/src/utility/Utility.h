#ifndef _UTILITY_H
#define _UTILITY_H

/////////////////////////////////////////////////
// INCLUDE
#include "../FlashEyesCoreLibConfig.h"
#include "../os_system/SystemCommon.h"
#if (_CONF_UTILITY_ENABLED)

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define UTILITY_CHECK_SUM_MAX_VALUE             0xFFFF

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

class Utility
{
public:
  Utility(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~Utility();
  static size_t findSubstring(char* mainStr, size_t mainStrLen, char* subStr, size_t subStrLen);
  static uint16_t calculateSum(uint8_t* inBuffer, size_t inBufferLen, uint16_t initSum = 0);
  static bool isEqualString(const char* str1, size_t str1Len, const char* str2, size_t str2Len);
  static int getFreeMemory(void);
  static uint32_t currentTimeInMilliSec(void);
  static uint32_t currentTimeInMicroSec(void);
  static bool checkFileExtension(const char* filePath, const char* ext);
};
#endif // _CONF_UTILITY_ENABLED

#endif // _UTILITY_H

