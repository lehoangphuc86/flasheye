
#include "DBManager.h"
#if (_CONF_FILE_SYSTEM_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "..\file_system\FileSystemManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define DB_MANAGER_CONSOLE_DEBUG_ENABLE
#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
char dbMgrLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
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
/*DBManager*/
DBManager& DBManager::getInstance(void)
{
  static DBManager instance;
  return instance;
}

DBManager::DBManager(void)
  : db_Handler(DB_HANDLER_INVALID)
{
  memset(this->sql_Query, 0, DB_TBL_ID_MAX * DB_QUERY_LEN_MAX);
}

DBManager::~DBManager(void)
{
  this->finalize();
}

bool DBManager::isValid(void)
{
  return (this->db_Handler == DB_HANDLER_INVALID ? false : true);
}


DBHandler_t DBManager::dbHandler(void)
{
  return this->db_Handler;
}

int DBManager::initialize(DBManagerConfigTAG& dbConfig)
{
  int result = 0;
  do
  {
    if (this->isValid() != false)
    {
#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %i", "dbini", -11);
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
      return -1; // already initialized
    }

    if (dbConfig.dbPath == NULL)
    {
#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %i", "dbini", -22);
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
      break;
    }

    this->finalize();
    result = sqlite3_initialize();
    if (result != DB_RET_OK)
    {
#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %i", "dbini", -1);
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
      break;
    }

    result = sqlite3_open(dbConfig.dbPath, &this->db_Handler);
    if ((result != DB_RET_OK)
      || (this->db_Handler ==NULL)
      )
    { 
#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %i", "dbini", -2);
      CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %s", "dbini", dbConfig.dbPath);
      CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %s", "dbini", sqlite3_errmsg(this->db_Handler));
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
      break;
    }

    return 0;
  } while (0);

  this->finalize();
  return -1;
}

void DBManager::finalize(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    sqlite3_shutdown();
    this->db_Handler = NULL;
    return;
  } while (0);
  this->db_Handler = NULL;
  return;
}

int DBManager::exec(const char* sql, int (*callback)(void*, int, char**, char**), void* arg, char** errmsg)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (sql == NULL)
    {
      break;
    }

    int ret = sqlite3_exec(this->db_Handler, sql, callback, arg, errmsg);
    if (ret != DB_RET_OK)
    {
      break;
    }
    return 0;
  } while (0);

  return -1;
}

int DBManager::exeScriptFile(const char* scriptFile, char* tmpBuf, DataSize_t tmpBufSize, const char lineTeminator)
{
  FileHandler_t scriptFileHandler = FS_FILE_HANDLER_INVALID;
  size_t fileSize = 0;
  size_t readLen = 0;
  size_t totalReadLen = 0;
  size_t curCmdLen = 0;
  size_t tmpValidSize = 0;
  char readCh = 0;
  bool error = false;
  int ret = 0;
#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] eS2 %i", 0);
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if ((scriptFile == NULL)
      || (tmpBuf == NULL)
      || (tmpBufSize <=1)
      )
    {
      break;
    }

    tmpValidSize = tmpBufSize - 1; // 1 char is required for NULL terminator

    if (FileSystemManager::getInstance().exist(scriptFile) == false)
    {
      break;
    }

    scriptFileHandler = FileSystemManager::getInstance().openFile(scriptFile, "r");
    if (FileSystemManager::getInstance().isValidFileHandler(scriptFileHandler) == false)
    {
      break;
    }

    fileSize = FileSystemManager::getInstance().fileSize(scriptFile);

    while (totalReadLen < fileSize)
    {
      if (curCmdLen >= tmpValidSize)
      {
        error = true;
//#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
//        CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] eS2 %i %i %i", -1, curCmdLen , tmpBufSize);
//#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
        break;
      }
      // read 1 char
      readLen = fread(&readCh, 1, 1, scriptFileHandler);
      if (readLen <= 0)
      {
        error = true;
        break;
      }

      totalReadLen++;

      // check it is a line
      if ( (readCh != lineTeminator) // not teminator
        && (totalReadLen < fileSize) // and not end of file
        )
      {
        tmpBuf[curCmdLen] = readCh;
        curCmdLen++;
        continue;
      }

      // run 
      tmpBuf[curCmdLen] = '\0';
      ret = sqlite3_exec(this->db_Handler, tmpBuf, NULL, NULL, NULL);
//#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] eS2 %i %i %i", 6, ret, curCmdLen);
//      CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", tmpBuf);
//#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
      curCmdLen = 0;
      if (ret != DB_RET_OK)
      {
        error = true;
        break;
      }
    }

    if (error != false)
    {
      break;
    }

    FileSystemManager::getInstance().closeFile(scriptFileHandler);
#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] eS2 %i", 99);
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] eS2 %i", -99);
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
  if (FileSystemManager::getInstance().isValidFileHandler(scriptFileHandler) != false)
  {
    FileSystemManager::getInstance().closeFile(scriptFileHandler);
  }

  return -1;
}

int DBManager::cbSelectCell(void* data, int argc, char** argv, char** azColName)
{
  do
  {
    if (argc <= 0)
    {
      break;
    }
    
    DbCbArgDataTAG* cusData = (DbCbArgDataTAG*)data;
    byte dataType = cusData->dataType;
    void* reqData = cusData->data;
//#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] cb %i %i", 1, dataType);
//    CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] cb %i %s", argc, argv[0]);
//#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
    switch (dataType)
    {
      case TypeClass::TChar_t:
      case TypeClass::TInt8_t:
      case TypeClass::TUInt8_t:
      case TypeClass::TString:
        strcpy((char*)reqData, argv[0]);
        break;
      case TypeClass::TFloat_t:
      case TypeClass::TDouble_t:
        *((float*)reqData) = atof(argv[0]);
        break;
    
        *((char*)reqData) = argv[0][0];
        break;
      case TypeClass::TInt64_t:
      case TypeClass::TUInt64_t:
        *((int64_t*)reqData) = atol(argv[0]);
        break;
      case TypeClass::TInt16_t:
      case TypeClass::TUInt16_t:
        *((int16_t*)reqData) = atoi(argv[0]);
        break;
      case TypeClass::TInt32_t:
      case TypeClass::TUInt32_t:
      default:
        *((int32_t*)reqData) = atoi(argv[0]);
        break;
    }
    return 0;
  } while (0);
  return -1;
}
#endif // _CONF_FILE_SYSTEM_MANAGER_ENABLED