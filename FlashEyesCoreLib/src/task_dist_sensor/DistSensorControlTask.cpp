/////////////////////////////////////////////////
// INCLUDE
#include "DistSensorControlTask.h"
#if (_CONF_DIST_SENSOR_CONTROL_TASK_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
//#define DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
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
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
char scanningTaskLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* DistSensorControlTask*/
DistSensorControlTask::DistSensorControlTask(void)
  : TaskManager()
  , ss_Controller(NULL)
  , is_Busy(false)
  , sequence_Id(0)
{
  memset(&this->ctrl_Config, 0, sizeof(DistSensorCtrlConfigTAG));
}
DistSensorControlTask::~DistSensorControlTask(void)
{
  this->stopTask();
}

bool DistSensorControlTask::isBusy(void)
{
  SystemCriticalLocker locker(this->critical_Key);
  return this->is_Busy;
}

bool DistSensorControlTask::isValid(void)
{
  SystemCriticalLocker locker(this->critical_Key);
  return (this->ss_Controller == NULL ? false : true);
}

bool DistSensorControlTask::isTriggerMode(void)
{
  SystemCriticalLocker locker(this->critical_Key);
  return (this->ctrl_Config.opMode == DistSensorOpModeUN::DistSensorRangeMode? true : false);
}

int DistSensorControlTask::control(DistSensorControlParamsTAG& ctrParams)
{
  int ret = 0;
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] req %d", 0);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isTaskRunning() == false)
    {
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] req %d", -1);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      break;
    }

    if (ctrParams.opCode != DIST_SENSOR_CTRL_OPCODE_START)
    {
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] req %d", -20);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      break;
    }

//    if (this->isBusy() != false)
//    {
//#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] req %d", -30);
//#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
//      break;
//    }

    EventDistSensorControlTAG requestEvent = EventDistSensorControlTAG();
    requestEvent.reqParams.sequenceId = ctrParams.sequenceId;
    requestEvent.reqParams.deviceId = ctrParams.deviceId;
    ret = this->notify(EventManagerConstant::EventMessageId::DistanceControl, sizeof(EventDistSensorControlTAG), (unsigned char*)&requestEvent);
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] req %d %d", 98, ret);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] req %i", 99);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] req %i", -99);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}

int DistSensorControlTask::startTask(DistSensorTaskConfigTAG& distSensorConfig)
{
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] sta %i", 0);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  do
  {
    int result = 0;
    if (this->isTaskRunning() != false)
    {
      return 0; // already running
    }

    if (distSensorConfig.config.trgCb == NULL)
    {
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] sta %i", -1);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      break;
    }

    if (distSensorConfig.config.opMode >= DistSensorOpModeUN::DistSensorOpModeMax)
    {
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] sta %i", -2);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      break;
    }

    if ((distSensorConfig.config.measuringInterval < DIST_SENSOR_ENGINE_MEASURING_INTERVAL_MIN)
      || (distSensorConfig.config.measuringInterval > DIST_SENSOR_ENGINE_MEASURING_INTERVAL_MAX)
      )
    {
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] sta %i", -3);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      break;
    }

    if (distSensorConfig.config.validRange.isValid() == false)
    {
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] sta %i", -4);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      break;
    }
    
    this->stopTask();
    memcpy(&this->ctrl_Config, &distSensorConfig, sizeof(DistSensorTaskConfigTAG));
    this->ss_Controller = DistSensorControllerFactory::generate(distSensorConfig.deviceConfig.deviceType);
    if (this->ss_Controller == NULL)
    {
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] sta %i", -5);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      break;
    }

    result = this->ss_Controller->start(distSensorConfig.deviceConfig);
    if (result != 0)
    {
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] sta %i", -6);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      break;
    }
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] sta %i", 70);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE

    // Set up tasks
    {
      this->registerHanldingEventStructSize(sizeof(EventDistSensorGetReadyTAG));
      this->registerHanldingEventStructSize(sizeof(EventDistSensorControlTAG));
      result = TaskManager::setConfig(distSensorConfig.taskManagerConfig);
      if (result != 0)
      {
        break;
      }

      result = TaskManager::prepare();
      if (result != 0)
      {
        break;
      }
    }

    {
      result = TaskManager::startProcess(distSensorConfig.taskThreadConfig, true);
      if (result != 0)
      {
        break;
      }
    }
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] sta %i", 99);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    return result;
  } while (0);

  this->stopTask();
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] sta %i", -99);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}

void DistSensorControlTask::stopTask(void)
{
  do
  {
    TaskManager::stopProcess();
    DistSensorControllerFactory::release(this->ss_Controller);
    this->resetSequence();
    memset(&this->ctrl_Config, 0, sizeof(DistSensorCtrlConfigTAG));
    this->isBusy(false);
    return;
  } while (0);
  return;
}

void DistSensorControlTask::isBusy(bool flag)
{
  SystemCriticalLocker locker(this->critical_Key);
  this->is_Busy = flag;
}


Seq_t DistSensorControlTask::curSeqId(void)
{
  SystemCriticalLocker locker(this->critical_Key);
  return this->sequence_Id;
}

Seq_t DistSensorControlTask::nextSeqId(void)
{
  SystemCriticalLocker locker(this->critical_Key);
  this->sequence_Id++;
  if (this->sequence_Id == 0)
  {
    this->sequence_Id++;
  }
  return this->sequence_Id;
}

Seq_t DistSensorControlTask::nextSeqId(Seq_t inSeqId)
{
  if (inSeqId == 0)
  {
    return this->nextSeqId();
  }
  SystemCriticalLocker locker(this->critical_Key);
  this->sequence_Id = inSeqId;
  return this->sequence_Id;
}

void DistSensorControlTask::proc(void)
{
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] proc %i", 0);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    this->clear();
    return;
  }
  EventDataItem* eventData = NULL;
  bool isStop = false;
  unsigned int timeToWait = (this->isTriggerMode() == false ? TASK_MANAGER_EVENT_WAIT_TIME_DEFAULT : this->ctrl_Config.measuringInterval);
  while (isStop == false) // A Task shall never return or exit.
  {
    eventData = this->event_Manager.wait(timeToWait);
    if (eventData == NULL)
    {
      this->measureDistance();
      continue;
    }

    switch (eventData->messageId())
    {
      case (int)EventManagerConstant::EventMessageId::TerminateProcess:
      {
        this->isTaskRunning(false);
        isStop = true;
        break;
      }
      case (int)EventManagerConstant::EventMessageId::DistanceControl:
      {
        this->onEventDistanceControl(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      default:
        break;
    }
    this->event_Manager.release(eventData);
  }
  this->clear();
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] proc %i", 99);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
}

int DistSensorControlTask::prepare(void)
{
  this->resetSequence();
  return 0; //do nothing
}

void DistSensorControlTask::clear(void)
{
  return;
}

int DistSensorControlTask::measureDistance(void)
{
  int ret = 0;
  do
  {
//#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] meas %i", 0);
//#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    if (this->isValid() == false)
    {
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] meas %i", -10);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      break;
    }

    if (this->isBusy() == false)
    {
//#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] meas %i", -20);
//#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      break;
    }

    double distance = 0.0;
    
    ret = this->ss_Controller->measureDistanceCm(0, distance);
    if (ret != 0)
    {
      break;
    }

    Dist_t distInt = (Dist_t)distance;
//#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] meas %d %d", 70, distInt);
//#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    if (this->ctrl_Config.validRange.isValidDist(distInt) == false)
    {
//#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] meas %d %d", -80, distInt);
//#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
      return 0;
    }

    this->ctrl_Config.trgCb(this->ctrl_Config.trgUserArg, this->curSeqId(), distInt, this->ctrl_Config.opMode);

    this->resetSequence();
//#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] meas %i", 99);
//#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
//#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] meas %i", -99);
//#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}

int DistSensorControlTask::onEventDistanceControl(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] ctr %i", 0);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    if (this->isValid() == false)
    {
      break;
    }

    if (this->isBusy() != false)
    {
      break;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventDistSensorControlTAG))
      )
    {
      break;
    }
    EventDistSensorControlTAG* eventData = (EventDistSensorControlTAG*)data;
    this->nextSeqId(eventData->reqParams.sequenceId);
    
    this->isBusy(true);
    
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] req %i", 99);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssCTsk] req %i", -99);
#endif // DIST_SENSOR_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}

void DistSensorControlTask::resetSequence(void)
{
  this->isBusy(false);
}

#endif // _CONF_DIST_SENSOR_CONTROL_TASK_ENABLED