#ifndef _JSON_PARSER_H
#define _JSON_PARSER_H

/////////////////////////////////////////////////
// INCLUDE
#include "../FlashEyesCoreLibConfig.h"
#include "../os_system/SystemCommon.h"
#if (_CONF_JSON_PARSER_ENABLED)
//#include "../libs/jsmn/jsmn.h"
#include "../libs/mjson/mjson.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define JSON_PARSER_NUMERIC_CONVERT_TMP_BUFF_LEN     24
#define JSON_PARSER_MAX_TOKEN_DEFAULT                999
/////////////////////////////////////////////////
// MARCO

/*-`%Q` print quoted escaped string.Expect NUL - terminated `char * `
  - `%.*Q` print quoted escaped string.Expect `int, char* `
  - `%s` print string as is.Expect NUL - terminated `char * `
  - `%.*s` print string as is.Expect `int, char* `
  - `%g`, print floating point number, precision is set to 6. Expect `double`
  - `%.*g`, print floating point number with given precision.Expect `int, double`
  - `%d`, `%u` print signed / unsigned integer.Expect `int`
  - `%ld`, `%lu` print signed / unsigned long integer.Expect `long`
  - `%B` print `true` or `false`. Expect `int`
  - `%V` print quoted base64 - encoded string.Expect `int, char* `
  - `%H` print quoted hex - encoded string.Expect `int, char* `
  - `%M` print using custom print function.Expect `int (*)(mjson_print_fn_t, void*, va_list*)`*/
#define JsonSnprintf(buf, bufLen, format, ...)       mjson_snprintf(buf, bufLen, format, __VA_ARGS__)
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef byte                                         JsTokType_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)
typedef enum _jsTokType
{
  JsTokString = 0,
  JsTokNumber,
  JsTokTrue,
  JsTokFalse,
  JsTokNull,
  JsTokArray,
  JsTokObject,
  JsTokTypeMax
} JsTokType;
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
/*JsonParser*/
class JsonParser
{
public:
  JsonParser(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~JsonParser();
  bool                                                          isValid(void);
  bool                                                          isParsing(void);
  int                                                           initialize(uint16_t tokenMax = JSON_PARSER_MAX_TOKEN_DEFAULT);
  void                                                          finialize(void);
  int                                                           begin(char* jsonStr, DataSize_t jsonStrLen);
  int                                                           find(const char* path, JsTokType_t& jsTokType, const char*& tokPos, int& tokLen);
  template <typename T>
  int                                                           getNumber(const char* path, T& val);
  int                                                           getNumber(const char* path, double& val);
  int                                                           getBool(const char* path, int& val);
  int                                                           getString(const char* path, char* desBuff, DataSize_t desLen, DataSize_t& len);
  int                                                           getHex(const char* path, char* desBuff, DataSize_t desLen, DataSize_t& len);

  int                                                           find(const char* jsStr, DataSize_t jsLen, const char* path, JsTokType_t& jsTokType, const char*& tokPos, int& tokLen);
  template <typename T>
  int                                                           getNumber(const char* jsStr, DataSize_t jsLen, const char* path, T& val);
  int                                                           getNumber(const char* jsStr, DataSize_t jsLen, const char* path, double& val);
  int                                                           getBool(const char* jsStr, DataSize_t jsLen, const char* path, int& val);
  int                                                           getString(const char* jsStr, DataSize_t jsLen, const char* path, char* desBuff, DataSize_t desLen, DataSize_t& len);
  int                                                           getHex(const char* jsStr, DataSize_t jsLen, const char* path, char* desBuff, DataSize_t desLen, DataSize_t& len);
  void                                                          end(void);

protected:
  char*                                                         js_Json_Str;
  DataSize_t                                                    js_Json_Len;

public:
  static JsTokType                                              mjsonType2JsTokType(int mjsonType);
};

template <typename T>
int JsonParser::getNumber(const char* path, T& val)
{
  return this->getNumber((const char*)this->js_Json_Str, this->js_Json_Len, path, val);
}

template <typename T>
int JsonParser::getNumber(const char* jsStr, DataSize_t jsLen, const char* path, T& val)
{
  double dVal = 0.0;
  do
  {
    if (this->getNumber(jsStr, jsLen, path, dVal) != 0)
    {
      break;
    }

    val = (T)dVal;
    return 0;
  } while (0);
  return -1;
}

#endif // _CONF_JSON_PARSER_ENABLED

#endif // _JSON_PARSER_H