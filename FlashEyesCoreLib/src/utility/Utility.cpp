#include "Utility.h"
#if (_CONF_UTILITY_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#ifdef SYSTEM_PC_BASED
#include <chrono>
#else // SYSTEM_PC_BASED

#endif // SYSTEM_PC_BASED
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

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
// CLASS IMPLEMENTAION

/* Utility*/
Utility::Utility(void)
{

}

Utility::~Utility(void)
{

}

size_t Utility::findSubstring(char* mainStr, size_t mainStrLen, char* subStr, size_t subStrLen)
{
  do
  {
    if ( (mainStr == NULL)
      || (subStr == NULL)
      || (mainStrLen == 0)
      || (subStrLen == 0)
      || (mainStrLen < subStrLen)
      )
    {
      break;
    }
    
    size_t mainIndex = 0;
    size_t subIndex = 0;
    for (mainIndex = 0; mainIndex <= mainStrLen - subStrLen; mainIndex++)
    {
      for (subIndex = 0; subIndex < subStrLen; subIndex++)
      {
        if (mainStr[mainIndex + subIndex] != subStr[subIndex])
        {
          break;
        }
      }

      if (subIndex == subStrLen)
      {
        break;
      }
    }

    if (mainIndex > (mainStrLen - subStrLen))
    {
      break; //not found
    }

    return mainIndex;
  } while (0);
  return mainStrLen;
}

uint16_t Utility::calculateSum(uint8_t* inBuffer, size_t inBufferLen, uint16_t initSum)
{
  uint32_t sum = initSum;
  for (size_t indx = 0; indx < inBufferLen / 2; indx++)
  {
    sum += *((uint16_t*)(inBuffer + indx * 2));
  }

  if ((inBufferLen % 2) != 0)
  {
    sum += inBuffer[inBufferLen - 1];
  }
  sum = (sum >> 16) + ((sum << 16) >> 16);
  return (uint16_t)sum;
}

bool Utility::isEqualString(const char* str1, size_t str1Len, const char* str2, size_t str2Len)
{
  do
  {
    if ((str1 == NULL)
      && (str2 == NULL)
      )
    {
      return true;
    }

    if ( (str1Len != str2Len)
      || (str1 == NULL)
      || (str2 == NULL)
      )
    {
      break;
    }

    for (size_t wk_idx = 0; wk_idx < str1Len; wk_idx++)
    {
      if (str1[wk_idx] == str2[wk_idx])
      {
        continue;
      }
      return false;
    }

    return true;
  } while (0);
  return false;
}


int Utility::getFreeMemory(void)
{
#ifdef SYSTEM_PC_BASED
  return 0;
#else // SYSTEM_PC_BASED
  
#ifdef SYSTEM_ESP_PLATFORM
  return ESP.getFreeHeap();
#else // SYSTEM_ESP_PLATFORM
  extern int __heap_start, * __brkval;
  int newVar = 0;
  return (int)&newVar - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
#endif // SYSTEM_ESP_PLATFORM
#endif // SYSTEM_PC_BASED
}

uint32_t Utility::currentTimeInMilliSec(void)
{
#ifdef SYSTEM_PC_BASED
  std::chrono::system_clock::duration duration = std::chrono::system_clock::now().time_since_epoch();
  return (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
#else // SYSTEM_PC_BASED
  return millis();
#endif // SYSTEM_PC_BASED
}

uint32_t Utility::currentTimeInMicroSec(void)
{
#ifdef SYSTEM_PC_BASED
  std::chrono::system_clock::duration duration = std::chrono::system_clock::now().time_since_epoch();
  return (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
#else // SYSTEM_PC_BASED
  return micros();
#endif // SYSTEM_PC_BASED
}

bool Utility::checkFileExtension(const char* filePath, const char* ext)
{
  do
  {
    if ((filePath == NULL)
      || (ext == NULL)
      )
    {
      break;
    }

    return (strcasecmp(&filePath[strlen(filePath) - strlen(ext)], ext) == 0);
  } while (0);
  return false;
  
}

#endif // _CONF_COMM_SERIAL_RECEIVER_ENABLED