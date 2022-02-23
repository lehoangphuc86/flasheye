
#include "ExportManager.h"
#if (_CONF_EXPORT_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define EXPORT_MANAGER_CONSOLE_DEBUG_ENABLE
#ifdef EXPORT_MANAGER_CONSOLE_DEBUG_ENABLE
char exportMgrLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // EXPORT_MANAGER_CONSOLE_DEBUG_ENABLE
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
/*ExportManager*/
ExportManager& ExportManager::getInstance(void)
{
  static ExportManager instance;
  return instance;
}

ExportManager::ExportManager(void)
{

}

ExportManager::~ExportManager(void)
{
  this->stopTask();
}

bool ExportManager::isValid(void)
{
  return this->control_Task.isValid();
}

bool ExportManager::isRunning(void)
{
  return this->control_Task.isTaskRunning();
}


EventManager* ExportManager::eventManager(void)
{
  return this->control_Task.eventManager();
}

int ExportManager::startTask(ExportManagerConfigTAG& exportManagerConfig)
{
  return this->control_Task.startTask(exportManagerConfig);
}

void ExportManager::stopTask(void)
{
  this->control_Task.stopTask();
}

int ExportManager::exportScanResult(ExportScanResultTAG& scanResult)
{
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

    EventExportScanResultTAG eventData = EventExportScanResultTAG();
    memcpy(&eventData.result, &scanResult, sizeof(ExportScanResultTAG));

    ret = this->control_Task.notify(EventManagerConstant::EventMessageId::ExportScanResult, sizeof(EventExportScanResultTAG), (unsigned char*)&eventData);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

#endif // _CONF_EXPORT_MANAGER_ENABLED