#ifndef _NET_MANAGER_CONSTANT_H
#define _NET_MANAGER_CONSTANT_H

#include "../../FlashEyesMain.h"
#if (_CONF_NET_MANAGER_CONSTANT_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "task_manager/TaskManagerConstant.h"
#include "NetIPConstant.h"
//#include "wifi/NetWifiConstant.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)
typedef enum _netInterfaceTypeUN
{
  NetInterfaceWifi = 0,
  NetInterfaceTypeMax
} NetInterfaceTypeUN;
/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
struct _wifiConnectionConfigTAG;
typedef struct _netConnectionConfigTag
{
  union _netConnectionConfigUN
  {
    _wifiConnectionConfigTAG* wifi;
    void* ethernet;
  } config;
} NetConnectionConfigTAG;

typedef struct _netTaskConfigTAG
{
  TaskManagerConfigTAG taskManagerConfig;
  TaskThreadConfigTAG taskThreadConfig;
} NetTaskConfigTAG;

typedef struct _netManagerTaskConfigTag
{
  byte interfaceType;
  NetTaskConfigTAG netTaskConfig;
} NetManagerConfigTAG;

typedef struct _netManagerConnectionConfigTag
{
  NetConnectionConfigTAG netConnConfig;
} NetManagerConnectionConfigTAG;
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

#endif // _CONF_NET_MANAGER_CONSTANT_ENABLED

#endif // _NET_MANAGER_CONSTANT_H