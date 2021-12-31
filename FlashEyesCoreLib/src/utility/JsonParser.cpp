#include "JsonParser.h"
#if (_CONF_JSON_PARSER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

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

/* JsonParser*/
JsonParser::JsonParser(void)
  : //tmp_Buff(NULL)
  //, js_Token(NULL)
  //, js_Token_Max(0)
  js_Json_Str(NULL)
  , js_Json_Len(0)
  //, js_Token_Count(0)
{

}

JsonParser::~JsonParser(void)
{
  this->finialize();
}


bool JsonParser::isValid(void)
{
  do
  {
    /*if ( (this->tmp_Buff == NULL)
      || (this->js_Token == NULL)
      )
    {
      break;
    }*/

    return true;
  } while (0);
  return false;
}

bool JsonParser::isParsing(void)
{
  return  (this->js_Json_Len <= 0 ? false : true); // (this->js_Token_Count == 0 ? false : true);
}

int JsonParser::initialize(uint16_t tokenMax)
{
  do
  {
    this->finialize();
    if (tokenMax <= 0)
    {
      break;
    }

    /*this->js_Token = new jsmntok_t[tokenMax + 1];
    if (this->js_Token == NULL)
    {
      break;
    }*/

    //this->js_Token_Max = tokenMax;
    this->end();
    return 0;
  } while (0);

  this->finialize();
  return -1;
}

void JsonParser::finialize(void)
{
  do
  {
    this->end();
    /*if (this->js_Token != NULL)
    {
      delete[] this->js_Token;
    }
    this->js_Token = NULL;*/
    //this->js_Token_Count = 0;
    
    return;
  } while (0);
  return;
}

int JsonParser::begin(char* jsonStr, DataSize_t jsonStrLen)
{
  do
  {
    //jsmn_init(&this->js_Parser);
    //int tokenCount = jsmn_parse(&this->js_Parser, jsonStr, jsonStrLen, this->js_Token, this->js_Token_Max);
    /*if (tokenCount < 0)
    {
      break;
    }*/

    /* Assume the top-level element is an object */
    /*if ((tokenCount < 1)
      || (this->js_Token[0].type != JSMN_OBJECT)
      )
    {
      break;
    }

    this->js_Token_Count = tokenCount;*/
    if (jsonStr == NULL)
    {
      break;
    }

    this->js_Json_Str = jsonStr;
    this->js_Json_Len = jsonStrLen;

    return 0;
  } while (0);
  this->end();
  return -1;
}


int JsonParser::find(const char* path, JsTokType_t& jsTokType, const char*& tokPos, int& tokLen)
{
  return this->find(this->js_Json_Str, this->js_Json_Len, path, jsTokType, tokPos, tokLen);
}

int JsonParser::getNumber(const char* path, double& val)
{
  return this->getNumber(this->js_Json_Str, js_Json_Len, path, val);
}

int JsonParser::getBool(const char* path, int& val)
{
  return this->getBool(this->js_Json_Str, js_Json_Len, path, val);
}

int JsonParser::getString(const char* path, char* desBuff, DataSize_t desLen, DataSize_t& len)
{
  return this->getString(this->js_Json_Str, js_Json_Len, path, desBuff, desLen, len);
}

int JsonParser::getHex(const char* path, char* desBuff, DataSize_t desLen, DataSize_t& len)
{
  return this->getHex(this->js_Json_Str, js_Json_Len, path, desBuff, desLen, len);
}

int JsonParser::find(const char* jsStr, DataSize_t jsLen, const char* path, JsTokType_t& jsTokType, const char*& tokPos, int& tokLen)
{
  do
  {
    int jsType = mjson_find(jsStr, jsLen, path, &tokPos, &tokLen);
    if (jsType == MJSON_TOK_INVALID)
    {
      break;
    }

    jsTokType = JsonParser::mjsonType2JsTokType(jsType);
    return 0;
  } while (0);
  return -1;
}

int JsonParser::getNumber(const char* jsStr, DataSize_t jsLen, const char* path, double& val)
{
  do
  {
    if (mjson_get_number(jsStr, jsLen, path, &val) == 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int JsonParser::getBool(const char* jsStr, DataSize_t jsLen, const char* path, int& val)
{
  do
  {
    if (mjson_get_bool(jsStr, jsLen, path, &val) == 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int JsonParser::getString(const char* jsStr, DataSize_t jsLen, const char* path, char* desBuff, DataSize_t desLen, DataSize_t& len)
{
  do
  {
    int ret = mjson_get_string(jsStr, jsLen, path, desBuff, desLen);
    if (ret == -1)
    {
      break;
    }
    len = (DataSize_t)ret;

    return 0;
  } while (0);
  return -1;
}

int JsonParser::getHex(const char* jsStr, DataSize_t jsLen, const char* path, char* desBuff, DataSize_t desLen, DataSize_t& len)
{
  do
  {
    int ret = mjson_get_hex(jsStr, jsLen, path, desBuff, desLen);
    if (ret == -1)
    {
      break;
    }
    len = (DataSize_t)ret;

    return 0;
  } while (0);
  return -1;
}



void JsonParser::end(void)
{
  this->js_Json_Str = NULL;
  this->js_Json_Len = 0;
}

JsTokType JsonParser::mjsonType2JsTokType(int mjsonType)
{
  switch (mjsonType)
  {
  case MJSON_TOK_STRING:
    return JsTokType::JsTokString;
  case MJSON_TOK_NUMBER:
    return JsTokType::JsTokNumber;
  case MJSON_TOK_TRUE:
    return JsTokType::JsTokTrue;
  case MJSON_TOK_FALSE:
    return JsTokType::JsTokFalse;
  case MJSON_TOK_NULL:
    return JsTokType::JsTokNull;
  case MJSON_TOK_ARRAY:
    return JsTokType::JsTokArray;
  case MJSON_TOK_OBJECT:
    return JsTokType::JsTokObject;
  default:
    return JsTokType::JsTokTypeMax;
  }
}
#endif // _CONF_JSON_PARSER_ENABLED