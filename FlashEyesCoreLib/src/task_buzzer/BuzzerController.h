#include "BuzzerTaskConstant.h"

#if (_CONF_BUZZER_CONTROLLER_ENABLED)

#ifndef _BUZZER_CONTROLLER_H
#define _BUZZER_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE
#include "BuzzerDeviceManager.h"
#include "os_system/SystemMutex.h"
#include "../libs/buzzer/BuzzerNotes.h"
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

/*BuzzerController*/

class BuzzerController
{
public:
  BuzzerController(void);
  virtual ~BuzzerController(void);
  bool                                                          isValid(void);
  int                                                           start(BuzzerControllerConfigTAG& controllerConfig);
  void                                                          stop(void);
  int                                                           control(BuzzerControlParamTAG& controlParam);
  int                                                           turnOnAll(BuzzerNoteFreq_t note, BuzzerNoteLen_t duration = BUZZER_NODE_DURATION_DEFAULT);
  int                                                           turnOnAll(BuzzerNoteFreq_t* melody, BuzzerNoteLen_t* noteLen, BuzzerMelodyLen_t molodyLen);
  int                                                           turnOffAll(void);
  int                                                           turnOnMul(BuzzerDeviceIdSet_t deviceIdSet, BuzzerNoteFreq_t note, BuzzerNoteLen_t duration = BUZZER_NODE_DURATION_DEFAULT);
  int                                                           turnOnMul(BuzzerDeviceIdSet_t deviceIdSet, BuzzerNoteFreq_t* melody, BuzzerNoteLen_t* noteLen, BuzzerMelodyLen_t molodyLen);
  int                                                           turnOffMul(BuzzerDeviceIdSet_t deviceIdSet);
  int                                                           turnOn(BuzzerDeviceId_t deviceId, BuzzerNoteFreq_t note, BuzzerNoteLen_t duration = BUZZER_NODE_DURATION_DEFAULT);
  int                                                           turnOn(BuzzerDeviceId_t deviceId, BuzzerNoteFreq_t* melody, BuzzerNoteLen_t* noteLen, BuzzerMelodyLen_t molodyLen);
  int                                                           turnOff(BuzzerDeviceId_t deviceId);
protected:
  SystemMutex                                                   mutex_Key;
};

#endif // _BUZZER_CONTROLLER_H

#endif // _CONF_BUZZER_CONTROLLER_ENABLED