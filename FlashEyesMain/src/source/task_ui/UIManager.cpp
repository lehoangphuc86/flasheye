
#include "UiManager.h"
#if (_CONF_UI_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define UI_MANAGER_CONSOLE_DEBUG_ENABLE
#ifdef UI_MANAGER_CONSOLE_DEBUG_ENABLE
char uiMgrLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // UI_MANAGER_CONSOLE_DEBUG_ENABLE
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
/*UIManager*/
UiManager& UiManager::getInstance(void)
{
  static UiManager instance;
  return instance;
}

UiManager::UiManager(void)
{

}

UiManager::~UiManager(void)
{
  this->stopTask();
}

bool UiManager::isValid(void)
{
  return this->control_Task.isValid();
}

bool UiManager::isRunning(void)
{
  return this->control_Task.isTaskRunning();
}

DataSize_t UiManager::regUIMessageId(UIMessageId_t messageId, DataSize_t maxRawSize)
{
  return this->control_Task.regUIMessageId(messageId, maxRawSize);
}

DataSize_t UiManager::getUIMessageParamSize(UIMessageId_t messageId)
{
  return this->control_Task.getUIMessageParamSize(messageId);
}

BufferDataItem* UiManager::getBuff(void)
{
  return this->control_Task.getBuff();
}

BufferDataManager* UiManager::dataManager(void)
{
  return this->control_Task.dataManager();
}

void UiManager::releaseBuff(BufferDataItem* dataItem)
{
  return this->control_Task.releaseBuff(dataItem);
}

EventManager* UiManager::eventManager(void)
{
  return this->control_Task.eventManager();
}

int UiManager::startTask(UiManagerConfigTAG& uiManagerConfig)
{
  return this->control_Task.startTask(uiManagerConfig);
}

void UiManager::stopTask(void)
{
  this->control_Task.stopTask();
}

int UiManager::show(UIMessageId_t uiMessageId, DataSize_t dataSize, unsigned char* data)
{
  BufferDataItem* dataItem = NULL;
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (this->isRunning() == false)
    {
      break;
    }

    dataItem = this->getBuff();
    if (dataItem == NULL)
    {
      break;
    }

    ret = dataItem->setData(dataSize, data);
    if (ret != 0)
    {
      break;
    }

    EventUiMessageTAG eventData = EventUiMessageTAG();
    eventData.uiMessId = uiMessageId;
    eventData.buffItem = dataItem;

    ret = this->control_Task.notify(EventManagerConstant::EventMessageId::UiMessage, sizeof(EventUiMessageTAG), (unsigned char*)&eventData);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  if (dataItem != NULL)
  {
    this->releaseBuff(dataItem);
  }
  return -1;
}
#endif // _CONF_UI_MANAGER_ENABLED