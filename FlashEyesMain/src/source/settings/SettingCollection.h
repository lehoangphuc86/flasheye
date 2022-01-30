#ifndef _SETTING_COLLECTION_H
#define _SETTING_COLLECTION_H

#include "SettingManagerConstant.h"
#if (_CONF_SETTING_COLLECTION_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "SettingItem.h"
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
typedef struct _settingCollectionConfigTag
{
  DbTableId_t dbTableId;
} SettingCollectionConfigTAG;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
/*SettingCollection*/
class SettingCollection
{
public:
  SettingCollection(void);
  virtual ~SettingCollection(void);
  virtual int                                                   initialize(SettingCollectionConfigTAG& settingConfig);
  bool                                                          isValid(void);
  virtual int                                                   load(void);
  virtual int                                                   save(void);
  virtual int                                                   set(SettingParamTAG& settingParam) = 0;
  virtual int                                                   get(SettingParamTAG& settingParam) = 0;
  virtual void                                                  clear(void);
protected:
  int                                                           initItemList(byte itemCount);
  void                                                          clearItemList(void);
protected:
  DbTableId_t                                                   db_Table_Id;
  SettingItem**                                                 item_List;
  byte                                                          item_Count;
  SystemMutex                                                   mutex_Key;
};
#endif // _CONF_SETTING_COLLECTION_ENABLED

#endif // _SETTING_COLLECTION_H