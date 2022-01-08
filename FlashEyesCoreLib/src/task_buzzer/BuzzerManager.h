#include "BuzzerTaskConstant.h"
#ifndef _BUZZER_MANAGER_H
#define _BUZZER_MANAGER_H
#if (_CONF_BUZZER_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "BuzzerController.h"
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

/*BuzzerManager*/
class BuzzerManager
{
private:
  BuzzerManager(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~BuzzerManager(void);

public:
  static BuzzerManager& getInstance(void);
  BuzzerManager(BuzzerManager const&) = delete;
  void operator=(BuzzerManager const&) = delete;
  bool                                                          isValid(void);
  int                                                           start(BuzzerManagerConfigTAG& buzzerManagerConfig);
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
  BuzzerController                                              buzzer_Controller;
};
#endif // _CONF_BUZZER_MANAGER_ENABLED

#endif // _BUZZER_MANAGER_H