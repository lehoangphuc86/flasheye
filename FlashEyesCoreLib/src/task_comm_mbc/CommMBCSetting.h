#include "CommMBCConstant.h"

#if (_CONF_COMM_MBC_SETTING_ENABLED)

#ifndef _COMM_MBC_SETTING_H
#define _COMM_MBC_SETTING_H

/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCConstant.h"
#if (!_CONF_COMM_MBC_CONSTANT_ENABLED)
#error Comm MBC constant is required
#endif // 
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

/*CommMBCSetting*/
class CommMBCSetting
{
public:
  static bool                                                   isValidConfig(CommMBCSettingParamTAG& commMbcSettingParams);

public:
  CommMBCSetting(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommMBCSetting(void);
  bool                                                          isValidConfig(void);
  int                                                           setConfig(CommMBCSettingParamTAG& commMbcSettingParams);
  bool                                                          enabled(void);
  //uint16_t                                                       bufferSize(void);
  //uint8_t                                                       bufferCount(void);

protected:
  CommMBCSettingParamTAG                                        comm_Mbc_Setting_Params;
};


#endif // _COMM_MBC_SETTING_H

#endif // _CONF_COMM_MBC_SETTING_ENABLED