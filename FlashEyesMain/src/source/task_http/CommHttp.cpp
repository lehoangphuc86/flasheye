#include "CommHttp.h"
#if (_CONF_COMM_HTTP_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR
#define COMM_HTTP_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// STATIC DATA
#ifdef COMM_HTTP_CONSOLE_DEBUG_ENABLE
char commHttpLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // COMM_HTTP_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*CommHttp*/
CommHttp::CommHttp(bool isServer)
  : TaskManager()
  , is_Server(isServer)
  , is_Started(0)
  , reserved_Bit(0)
  , ret_Start(-1)
  , ret_Stop(-1)
  , current_Req_Id(0)
{
  memset(&this->conn_Config, 0, sizeof(CommHttpConnectionConfigTAG));
}

CommHttp::~CommHttp(void)
{
  this->cleanUp();
}

int CommHttp::inititialize(void)
{
  do
  {
    /*if (this->data_Item.isValid() != false)
    {
      return 0;
    }

    if (result != 0)
    {
      break;
    }*/
    return 0;
    //return result;
  } while (0);
  this->cleanUp();
  return -1;
}

bool CommHttp::isStarted(void)
{
  return this->is_Started;
}

bool CommHttp::isServer(void)
{
  return this->is_Server;
}

int CommHttp::startTask(CommHttpTaskConfigTAG& commHttpConfig)
{
  do
  {
    int result = 0;
    if (this->isTaskRunning() != false)
    {
      return 0; // already running
    }

    result = this->data_Manager.prepare(commHttpConfig.bufferConfig);
    if (result != 0)
    {
      break;
    }

    // Set up tasks
    {
      // self
      this->registerHanldingEventStructSize(sizeof(EventCommHttpStartTAG));
      this->registerHanldingEventStructSize(sizeof(EventCommHttpStopTAG));
      this->regEventSize();
      result = TaskManager::setConfig(commHttpConfig.taskManagerConfig);
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
      result = TaskManager::startProcess(commHttpConfig.taskThreadConfig, true);
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


int CommHttp::startHttp(CommHttpConnectionConfigTAG& commHttpConnConfig, bool waitCompletion)
{
  do
  {
    if (this->isStarted() != false)
    {
      this->stopHttp();
    }
#ifdef COMM_HTTP_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[http] sta %i", 0);
#endif // COMM_HTTP_CONSOLE_DEBUG_ENABLE
    {
      SystemMutexLocker locker(this->mutex_Operating);
      int ret = 0;
      memcpy(&this->conn_Config, &commHttpConnConfig.config, sizeof(commHttpConnConfig.config));
      EventCommHttpStartTAG eventStartData = EventCommHttpStartTAG();
      memcpy(&eventStartData.config, &commHttpConnConfig.config, sizeof(commHttpConnConfig.config));
      this->ret_Start = -1;
      this->mutex_wait_Start.unlock();
      this->mutex_wait_Start.lock();
      ret = this->notify((EventId_t)EventManagerConstant::EventMessageId::CommHttpStart, sizeof(EventCommHttpStartTAG), (unsigned char*)&eventStartData);
#ifdef COMM_HTTP_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[http] sta %i %i", 1, ret);
#endif // COMM_HTTP_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }

      if (waitCompletion == false)
      {
        return 0;
      }

      this->mutex_wait_Start.lock(COMM_HTTP_START_WAIT_TIME);
#ifdef COMM_HTTP_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[http] sta %i %i", 3, this->ret_Start);
#endif // COMM_HTTP_CONSOLE_DEBUG_ENABLE
      if (this->ret_Start != 0)
      {
        break;
      }
    }

    return 0;
  } while (0);
  this->stopHttp();
  return -1;

}

BufferDataItem* CommHttp::getCommData(void)
{
  return (BufferDataItem*)this->data_Manager.get();
}

BufferDataManager* CommHttp::dataManager(void)
{
  return &this->data_Manager;
}

void CommHttp::releaseCommData(BufferDataItem* dataItem)
{
  if (dataItem != NULL)
  {
    this->data_Manager.release(dataItem);
  }
}

void CommHttp::stopHttp(bool waitCompletion)
{
  do
  {
    if (this->isStarted() == false)
    {
      return; // already
    }

    {
      SystemMutexLocker locker(this->mutex_Operating);
      int ret = 0;
      EventCommHttpStopTAG eventStopData = EventCommHttpStopTAG();
      this->mutex_wait_Stop.unlock();
      this->mutex_wait_Stop.lock();
      ret = this->notify((EventId_t)EventManagerConstant::EventMessageId::CommHttpStop, sizeof(EventCommHttpStopTAG), (unsigned char*)&eventStopData);
      if (ret != 0)
      {
        break;
      }

      if (waitCompletion == false)
      {
        this->is_Started = 0;
        return;
      }
      this->mutex_wait_Stop.lock(COMM_HTTP_STOP_WAIT_TIME);
      this->is_Started = 0;
      memset(&this->conn_Config, 0, sizeof(CommHttpConnectionConfigTAG));
    }

    return;
  } while (0);
  this->is_Started = 0;
  return;
}

void CommHttp::stopTask(void)
{
  do
  {
    this->data_Manager.releaseAll();
    this->stopHttp();
    TaskManager::stopProcess();
    return;
  } while (0);
  return;
}


void CommHttp::cleanUp(void)
{
  this->stopTask();
}


HttpReqId_t CommHttp::nextReqId(void)
{
  this->current_Req_Id++;
  if (this->current_Req_Id == 0)
  {
    this->current_Req_Id++;
  }
  return this->current_Req_Id;
}


int CommHttp::prepare(void)
{
  return 0; // do nothing
}

#endif // _CONF_COMM_HTTP_ENABLED