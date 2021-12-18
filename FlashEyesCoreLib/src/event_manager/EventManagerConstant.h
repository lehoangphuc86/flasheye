#ifndef _EVENT_MANAGER_CONSTANT_H
#define _EVENT_MANAGER_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../FlashEyesCoreLibConfig.h"
#if (_CONF_EVENT_MANAGER_CONSTANT_ENABLED)

#include "../os_system/SystemCommon.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define EVENT_MANAGER_DEBUG_ENABLE
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
typedef struct _eventDataItemConfigTAG
{
  EventSize_t eventItemSize;
} EventDataItemConfigTAG;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
/*EventManagerConstant*/
class EventManagerConstant
{
public:
  enum EventMessageId
  {
    InvalidMessageId = 0
    , TerminateProcess
    , InputReceived
    // For display task
    , DisplayStatus
    , DisplayDistance
    , DisplayTime
    , DisplayControl
    // For distance sensor
    , DistanceRequest
    , DistanceReceived
    , DistanceTriggered
    // CommI2C
    , CommI2COnRequest
    , CommI2COnReceived
    , CommI2CSendRequested
    , CommI2CDataRequested
    // Comm ALL
    , CommOpenConnection
    , CommSendRequested
    , CommDataReceived
    , CommConnectionOpened
    , CommConnectionClosed
    , CommCloseConnection
    // 
    , TestData
    // Console logging
    , ConsoleLogRequest
    , PrepareCompleted
    , ButtonPressed
    // Timer
    , TimerStart
    , TimerStop
    , TimerFired1
    , TimerFired2
    , TimerPause
    , TimerSetAlarm
    // Net Wifi
    , WifiStart
    , WifiStop
    , WifiStarted
    , WifiStopped
    , WifiConnectionOpened
    , WifiConnectionClosed
    , WifiGotIPV4
    // Http
    , CommHttpStart
    , CommHttpConnectionOpened
    , CommHttpDataReceived
    , CommHttpClientRequest
    , CommHttpConnectionClosed
    , CommHttpStop
    , CommHttpFinished
  };

public:
  EventManagerConstant(void) {};
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~EventManagerConstant(void) {};

  static bool isValidMessageId(EventId_t messageId)
  {
    switch (messageId)
    {
    case EventManagerConstant::EventMessageId::TerminateProcess:
    case EventManagerConstant::EventMessageId::InputReceived:
    case EventManagerConstant::EventMessageId::DisplayStatus:
    case EventManagerConstant::EventMessageId::DisplayDistance:
    case EventManagerConstant::EventMessageId::DisplayTime:
    case EventManagerConstant::EventMessageId::DisplayControl:
    case EventManagerConstant::EventMessageId::DistanceRequest:
    case EventManagerConstant::EventMessageId::DistanceReceived:
    case EventManagerConstant::EventMessageId::DistanceTriggered:
    case EventManagerConstant::EventMessageId::CommI2COnRequest:
    case EventManagerConstant::EventMessageId::CommI2COnReceived:
    case EventManagerConstant::EventMessageId::CommI2CSendRequested:
    case EventManagerConstant::EventMessageId::CommI2CDataRequested:
    case EventManagerConstant::EventMessageId::CommOpenConnection:
    case EventManagerConstant::EventMessageId::CommSendRequested:
    case EventManagerConstant::EventMessageId::CommDataReceived:
    case EventManagerConstant::EventMessageId::CommConnectionOpened:
    case EventManagerConstant::EventMessageId::CommConnectionClosed:
    case EventManagerConstant::EventMessageId::CommCloseConnection:
    case EventManagerConstant::EventMessageId::TestData:
    case EventManagerConstant::EventMessageId::ConsoleLogRequest:
    case EventManagerConstant::EventMessageId::PrepareCompleted:
    case EventManagerConstant::EventMessageId::ButtonPressed:
    case EventManagerConstant::EventMessageId::TimerStart:
    case EventManagerConstant::EventMessageId::TimerStop:
    case EventManagerConstant::EventMessageId::TimerFired1:
    case EventManagerConstant::EventMessageId::TimerFired2:
    case EventManagerConstant::EventMessageId::TimerPause:
    case EventManagerConstant::EventMessageId::TimerSetAlarm:
    case EventManagerConstant::EventMessageId::WifiStart:
    case EventManagerConstant::EventMessageId::WifiStop:
    case EventManagerConstant::EventMessageId::WifiStarted:
    case EventManagerConstant::EventMessageId::WifiStopped:
    case EventManagerConstant::EventMessageId::WifiConnectionOpened:
    case EventManagerConstant::EventMessageId::WifiConnectionClosed:
    case EventManagerConstant::EventMessageId::WifiGotIPV4:
    case EventManagerConstant::EventMessageId::CommHttpStart:
    case EventManagerConstant::EventMessageId::CommHttpConnectionOpened:
    case EventManagerConstant::EventMessageId::CommHttpClientRequest:
    case EventManagerConstant::EventMessageId::CommHttpDataReceived:
    case EventManagerConstant::EventMessageId::CommHttpConnectionClosed:
    case EventManagerConstant::EventMessageId::CommHttpStop:
    case EventManagerConstant::EventMessageId::CommHttpFinished:
      return true;
    default:
      return false;
    }
  }
};
#endif // _CONF_EVENT_MANAGER_CONSTANT_ENABLED

#endif // _EVENT_MANAGER_CONSTANT_H

