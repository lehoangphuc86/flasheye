/////////////////////////////////////////////////
// INCLUDE
#include "UiControlTask.h"
#if (_CONF_UI_CONTROL_TASK_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
#define UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
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
#ifdef UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
char uiControlTaskLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* UiControlTask*/
UiControlTask::UiControlTask(void)
  : TaskManager()
  , max_Data_Size(0)
  , dp_Processor(NULL)
{

}

UiControlTask::~UiControlTask(void)
{
  this->cleanUp();
}

bool UiControlTask::isValid(void)
{
  do
  {
    if (this->dp_Processor == NULL)
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

DataSize_t UiControlTask::regUIMessageId(UIMessageId_t messageId, DataSize_t maxRawSize)
{
  do
  {
    bool isRaw = (messageId == (UIMessageId_t)UIConstant::UIMessageId::UiMessRaw ? true : false);
    MbcDataSize_t maxParamSize = (isRaw == false ? this->getUIMessageParamSize(messageId) : maxRawSize);
    if (maxParamSize > this->max_Data_Size)
    {
      this->max_Data_Size = maxParamSize;
    }

  } while (0);
  return this->max_Data_Size;
}


DataSize_t UiControlTask::getUIMessageParamSize(UIMessageId_t messageId)
{
  do
  {
    DataSize_t maxSize = 0;

    switch (messageId)
    {
      case (UIMessageId_t)UIConstant::UIMessageId::UiMessRaw:
        maxSize = 0;
        break;
      case (UIMessageId_t)UIConstant::UIMessageId::UiMessMessage:
        maxSize = sizeof(UiMessMessageTAG);
        break;
      case (UIMessageId_t)UIConstant::UIMessageId::UiMessSysState:
        maxSize = sizeof(UiMessSysStateTAG);
        break;
      default:
        maxSize = 0;
        break;
    }

    return maxSize;
  } while (0);

  return 0;
}


BufferDataItem* UiControlTask::getBuff(void)
{
  return (BufferDataItem*)this->data_Manager.get();
}

BufferDataManager* UiControlTask::dataManager(void)
{
  return &this->data_Manager;
}

void UiControlTask::releaseBuff(BufferDataItem* dataItem)
{
  if (dataItem != NULL)
  {
    this->data_Manager.release(dataItem);
  }
}

int UiControlTask::startTask(UiManagerConfigTAG& uiManagerConfig)
{
  do
  {
    int result = 0;
    if (this->isTaskRunning() != false)
    {
      return 0; // already running
    }

    this->stopTask();

    uiManagerConfig.bufferConfig.dataItemConfig.bufferSize = 
      SYSTEM_MAX(uiManagerConfig.bufferConfig.dataItemConfig.bufferSize, this->max_Data_Size);
    result = this->data_Manager.prepare(uiManagerConfig.bufferConfig);
    if (result != 0)
    {
      break;
    }

    this->dp_Processor = UiDpProcessorFactory::generate(uiManagerConfig.dpProcType);
    if (this->dp_Processor == NULL)
    {
      break;
    }

    result = this->dp_Processor->start(uiManagerConfig.dpProcConfig);
    if (result != 0)
    {
      break;
    }

    // Set up tasks
    {
      // self
      this->registerHanldingEventStructSize(sizeof(EventPrepareCompletedTAG));
      this->registerHanldingEventStructSize(sizeof(EventUiMessageTAG));
      result = TaskManager::setConfig(uiManagerConfig.taskManagerConfig);
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
      result = TaskManager::startProcess(uiManagerConfig.taskThreadConfig, true);
      if (result != 0)
      {
        break;
      }
    }

    return result;
  } while (0);

  this->stopTask();
  return -1;
}


void UiControlTask::stopTask(void)
{
  TaskManager::stopProcess();
  if (this->dp_Processor != NULL)
  {
    this->dp_Processor->stop();
  }
  UiDpProcessorFactory::release(this->dp_Processor);
  this->data_Manager.releaseAll();
  return;
}

void UiControlTask::cleanUp(void)
{
  this->stopTask();
}

void UiControlTask::proc(void)
{
#ifdef UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(uiControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiCTsk] proc %i", 0);
#endif // UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    return;
  }
  EventDataItem* eventData = NULL;
  bool isStop = false;
  while (isStop == false) // A Task shall never return or exit.
  {
    eventData = this->event_Manager.wait(TASK_MANAGER_EVENT_WAIT_TIME_DEFAULT);
    if (eventData == NULL)
    {
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
      case (int)EventManagerConstant::EventMessageId::UiMessage:
      {
        this->onEventUiMessage(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      default:
        break;
    }
    this->event_Manager.release(eventData);
  }
#ifdef UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(uiControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiCTsk] proc %i", 99);
#endif // UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
}

int UiControlTask::prepare(void)
{
  return 0; //do nothing
}

int UiControlTask::onEventUiMessage(unsigned char* data, unsigned int dataSize)
{
#ifdef UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(uiControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiCTsk] um %i", 0);
#endif // UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  BufferDataItem* buffItem = NULL;
  do
  {
    if ((dataSize != sizeof(EventUiMessageTAG))
      || (data == NULL))
    {
      break;
    }

    if (this->isValid() == false)
    {
      break;
    }

    EventUiMessageTAG* eventData = (EventUiMessageTAG*)data;
    buffItem = eventData->buffItem;
#ifdef UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiCTsk] um %i %i", 1, eventData->uiMessId);
#endif // UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    switch (eventData->uiMessId)
    {
      case (byte)UIConstant::UIMessageId::UiMessRaw:
      {
        this->onUiMessRaw(eventData);
        break;
      }
      case (byte)UIConstant::UIMessageId::UiMessMessage:
      {
        this->onUiMessMessage(eventData);
        break;
      }
      case (byte)UIConstant::UIMessageId::UiMessSysState:
      {
        this->onUiMessSysState(eventData);
        break;
      }
      default:
        break;
    }

    if (buffItem != NULL)
    {
      this->releaseBuff(buffItem);
    }
#ifdef UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiCTsk] um %i", 99);
#endif // UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

  if (buffItem != NULL)
  {
    this->releaseBuff(buffItem);
  }
#ifdef UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(uiControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiCTsk] um %i", -99);
#endif // UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}


int UiControlTask::onUiMessRaw(EventUiMessageTAG* eventData)
{
  int ret = 0;
  do
  {
    // play sound
    // play led
    if ((eventData->buffItem == NULL)
      || (eventData->buffItem->dataLength() < sizeof(UiMessRawTAG))
      )
    {
      break;
    }

    UiMessRawTAG* param = (UiMessRawTAG*)eventData->buffItem->bufferAddress();
    ret = this->dp_Processor->uiMessRaw(param);
    if (ret != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}

int UiControlTask::onUiMessMessage(EventUiMessageTAG* eventData)
{
  int ret = 0;
  do
  {
    // play sound
    // play led
    if ((eventData->buffItem == NULL)
      || (eventData->buffItem->dataLength() < sizeof(UiMessMessageTAG))
      )
    {
      break;
    }

    UiMessMessageTAG* param = (UiMessMessageTAG*)eventData->buffItem->bufferAddress();
    ret = this->dp_Processor->uiMessMessage(param);
    if (ret != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}

int UiControlTask::onUiMessSysState(EventUiMessageTAG* eventData)
{
#ifdef UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(uiControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiCTsk] oSS %i", 0);
#endif // UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  int ret = 0;
  do
  {
    // play sound
    // play led
    if ((eventData->buffItem == NULL)
      || (eventData->buffItem->dataLength() < sizeof(UiMessSysStateTAG))
      )
    {
      break;
    }

    UiMessSysStateTAG* param = (UiMessSysStateTAG*)eventData->buffItem->bufferAddress();
#ifdef UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiCTsk] oSS %i %i", 1, param->state);
#endif // UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    ret = this->dp_Processor->uiMessSysState(param);
#ifdef UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiCTsk] oSS %i %i", 99, ret);
#endif // UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
    return 0;
  } while (0);
#ifdef UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(uiControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiCTsk] oSS %i", -99);
#endif // UI_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}
#endif // _CONF_UI_CONTROL_TASK_ENABLED