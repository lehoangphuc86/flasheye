#ifndef _SYSTEM_COMMON_H
#define _SYSTEM_COMMON_H

#include "../FlashEyesCoreLibConfig.h"
/////////////////////////////////////////////////
// Preprocessor

/////////////////////////////////////////////////
// INCLUDE

#ifdef SYSTEM_ARDUINO_BASED
#include "arduino/SystemCommonArduino.h"
#elif defined(SYSTEM_PC_BASED)
#include "pc/SystemCommonPC.h"
#endif // SYSTEM_ARDUINO_BASED

#include <stdio.h>
#include "string.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

#define PIN_NUMER_MIN                                     0
#define DIGITIAL_VALUE_MIN                                0
#define DIGITIAL_VALUE_MAX                                255


#define SYS_DATA_T_DOUBLE                                 0
#define SYS_DATA_T_8                                      1
#define SYS_DATA_T_STRING                                 2
#define SYS_DATA_T_16                                     3
#define SYS_DATA_T_32                                     4
#define SYS_DATA_T_64                                     5

/////////////////////////////////////////////////
// MARCO

#define SYSTEM_ALIGNMENT_SIZE                             4

#if defined(_MSC_VER)
#define __ATTRIBUTE_ALIGN                                 __declspec(align(SYSTEM_ALIGNMENT_SIZE))
#elif defined(__GNUC__)
#define __ATTRIBUTE_ALIGN                                 __attribute__((aligned(SYSTEM_ALIGNMENT_SIZE)))
#endif // __GNUC__

#ifndef SYSTEM_MIN
#define SYSTEM_MIN(a,b) ((a)<(b)?(a):(b))
#endif // !SYSTEM_MIN

#ifndef SYSTEM_MAX
#define SYSTEM_MAX(a,b) ((a)>(b)?(a):(b))
#endif // !SYSTEM_MAX

#define SYSTEM_CONST_MILISEC_2_NANOSEC                      1000000
#define SYSTEM_CONST_MHZ_2_HZ                               1000000
// SYSTEM_CONSOLE()
//#define SYSTEM_CONSOLE_OUT_BUF_LEN                        24
#define SYSTEM_CONSOLE_OUT_BUF_LEN                          32
#define CONSOLE_LOG(format, ...)                            SYSTEM_CONSOLE(format, __VA_ARGS__)
#define CONSOLE_LOG_BUF(buf, bufLen, format, ...)           SYSTEM_CONSOLE_BUF(buf, bufLen, format, __VA_ARGS__)
#define CONSOLE_LOG_ISR(buf, bufLen, format, ...)           SYSTEM_CONSOLE_ISR(buf, bufLen, format, __VA_ARGS__)
#define SYSTEM_PRINT_BUF(buf, bufLen, format, ...)          snprintf(buf, bufLen, format, __VA_ARGS__)
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
/////////////////////////////////////////////////
// DATA TYPE (ENUM)
typedef enum _typeClass
{
  TInt8_t = 0,
  TChar_t,
  TInt16_t,
  TInt32_t,
  TInt64_t,
  TUInt8_t,
  TUInt16_t,
  TUInt32_t,
  TUInt64_t,
  TFloat_t,
  TDouble_t,
  TString
} TypeClass;
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
template <class T>
struct TypeClassT
{
  static const int value = TypeClass::TInt32_t;
};

template <>
struct TypeClassT<char>
{
  static const int value = TypeClass::TChar_t;
};

template <>
struct TypeClassT<int8_t>
{
  static const int value = TypeClass::TInt8_t;
};


template <>
struct TypeClassT<byte>
{
  static const int value = TypeClass::TUInt8_t;
};

template <>
struct TypeClassT<int16_t>
{
  static const int value = TypeClass::TInt16_t;
};

template <>
struct TypeClassT<uint16_t>
{
  static const int value = TypeClass::TUInt16_t;
};

template <>
struct TypeClassT<uint32_t>
{
  static const int value = TypeClass::TUInt32_t;
};

template <>
struct TypeClassT<int32_t>
{
  static const int value = TypeClass::TInt32_t;
};

template <>
struct TypeClassT<int64_t>
{
  static const int value = TypeClass::TInt64_t;
};

template <>
struct TypeClassT<uint64_t>
{
  static const int value = TypeClass::TUInt64_t;
};


template <>
struct TypeClassT<float>
{
  static const int value = TypeClass::TFloat_t;
};


template <>
struct TypeClassT<double>
{
  static const int value = TypeClass::TDouble_t;
};

template <>
struct TypeClassT<char*>
{
  static const int value = TypeClass::TString;
};

template <>
struct TypeClassT<const char*>
{
  static const int value = TypeClass::TString;
};
#endif // _SYSTEM_COMMON_H