/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCSetting.h"
#if (_CONF_COMM_MBC_SETTING_ENABLED)
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
//

CommMBCSetting::CommMBCSetting(void)
{
  memset(&this->comm_Mbc_Setting_Params, 0, sizeof(CommMBCSettingParamTAG));
}

CommMBCSetting::~CommMBCSetting(void)
{

}

bool CommMBCSetting::isValidConfig(void)
{
  return CommMBCSetting::isValidConfig(this->comm_Mbc_Setting_Params);
}

bool CommMBCSetting::enabled(void)
{
  return this->comm_Mbc_Setting_Params.enabled;
}

//uint16_t CommMBCSetting::bufferSize(void)
//{
//  return this->comm_Mbc_Setting_Params.bufferSize;
//}

//uint8_t CommMBCSetting::bufferCount(void)
//{
//  return this->comm_Mbc_Setting_Params.bufferCount;
//}

int CommMBCSetting::setConfig(CommMBCSettingParamTAG& commMbcSettingParams)
{
  do
  {
    if (this->isValidConfig(commMbcSettingParams) == false)
    {
      break;
    }
    memcpy(&this->comm_Mbc_Setting_Params, &commMbcSettingParams, sizeof(CommMBCSettingParamTAG));
    this->comm_Mbc_Setting_Params.enabled = true;
    return 0;
  } while (0);
  return -1;
}

bool CommMBCSetting::isValidConfig(CommMBCSettingParamTAG& commMbcSettingParams)
{
  do
  {
    if (commMbcSettingParams.enabled == false)
    {
      break;
    }
   
    return true;
  } while (0);

  return false;
}

#endif // _CONF_COMM_MBC_SETTING_ENABLED