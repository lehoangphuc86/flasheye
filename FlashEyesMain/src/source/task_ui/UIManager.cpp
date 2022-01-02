
#include "UIManager.h"
#if (_CONF_UI_MANAGER_ENABLED)
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
  , db_Table_Names(NULL)
  , db_Locker(NULL)
  , sql_Query(NULL)
{
  //memset(this->sql_Query, 0, DB_TBL_ID_MAX * DB_QUERY_LEN_MAX);
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

    if ( (dbConfig.dbPath == NULL)
      || (dbConfig.dbTableName == NULL)
      || (dbConfig.dbTableIdMax <= 0)
      )
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

    
    this->db_Table_Names = dbConfig.dbTableName;
    this->db_Table_Id_Max = dbConfig.dbTableIdMax;
    this->db_Locker = new SystemMutex[this->db_Table_Id_Max];
    this->sql_Query = new char*[this->db_Table_Id_Max];
    
    for (int wk_idx = 0; wk_idx < this->db_Table_Id_Max; wk_idx++)
    {
      this->sql_Query[wk_idx] = new char[DB_QUERY_LEN_MAX];
    }

    if ( (this->db_Locker == NULL)
      || (this->sql_Query == NULL)
      )
    {
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
    this->db_Table_Names = NULL;
    
    if (this->db_Locker != NULL)
    {
      delete[] this->db_Locker;
      this->db_Locker = NULL;
    }
    
    if (this->sql_Query != NULL)
    {
      for (int wk_idx = 0; wk_idx < this->db_Table_Id_Max; wk_idx++)
      {
        if (this->sql_Query[wk_idx] == NULL)
        {
          continue;
        }
        delete[] this->sql_Query[wk_idx];
        this->sql_Query[wk_idx] = NULL;
      }
    }
    
    this->sql_Query = NULL;
    this->db_Table_Id_Max = 0;
    return;
  } while (0);
  this->db_Handler = NULL;
  this->db_Table_Names = NULL;
  this->db_Locker = NULL;
  this->sql_Query = NULL;
  this->db_Table_Id_Max = 0;
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


int DBManager::selectCellStr(DbTableId_t tableId, const char* colName, const char* keyName, unsigned int id, char* val, DataSize_t valLen)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (tableId >= this->db_Table_Id_Max)
    {
      break;
    }

    if ( (val == NULL)
      || (valLen <=0)
      )
    {
      break;
    }

    val[valLen - 1] = '\0';
    DbCbArgStrDataTAG strData = DbCbArgStrDataTAG();
    strData.valid = false;
    strData.sVal = val;
    strData.slen = valLen;
    {
      SystemMutexLocker locker(this->db_Locker[tableId]);
      SYSTEM_PRINT_BUF(this->sql_Query[tableId], DB_QUERY_LEN_MAX, "SELECT %s FROM %s WHERE %s=%u", colName, this->db_Table_Names[tableId], keyName, id);
//#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(this->dbMgrLogBuf2, DB_QUERY_LEN_MAX, "[db] sCS %i %s", 2, this->sql_Query[tableId]);
//#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      ret = sqlite3_exec(this->db_Handler, this->sql_Query[tableId], DBManager::cbSelectCellStr, (void*)&strData, NULL);
//#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(this->dbMgrLogBuf2, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db]: sCS %i %i %s", 7, ret, val);
//#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
    }

    if ((ret != DB_RET_OK)
      || (strData.valid == false)
      )
    {
#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      /*CONSOLE_LOG_BUF(this->dbMgrLogBuf2, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", errorMess);
      sqlite3_free(errorMess);*/
#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int DBManager::updateCellStr(DbTableId_t tableId, const char* colName, const char* keyName, unsigned int id, char* newVal, DataSize_t newValLen)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (tableId >= this->db_Table_Id_Max)
    {
      break;
    }

    {
      SystemMutexLocker locker(this->db_Locker[tableId]);
      SYSTEM_PRINT_BUF(this->sql_Query[tableId], DB_QUERY_LEN_MAX, "UPDATE %s SET %s='%s' WHERE %s=%u", this->db_Table_Names[tableId], colName, newVal, keyName, id);
#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(this->dbMgrLogBuf2, DB_QUERY_LEN_MAX, "[db] uCS %i %s", 2, this->sql_Query[tableId]);
#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      ret = sqlite3_exec(this->db_Handler, sql_Query[tableId], NULL, NULL, NULL);
#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(this->dbMgrLogBuf2, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db]: uCS %i %i", 7, ret);
#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
    }

    if (ret != DB_RET_OK)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}


int DBManager::cbSelectCellNum(void* data, int argc, char** argv, char** azColName)
{
  do
  {
    if ( (argc <= 0)
      || (data == NULL)
      )
    {
      break;
    }

    DbCbArgNumDataTAG* numData = (DbCbArgNumDataTAG*)data;
    numData->valid = true;
    *(numData->dVal) = atof(argv[0]);
#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] cbn %i %f", 1, *(numData->dVal));
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE

    return 0;
  } while (0);
#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] cbn %i", -99);
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int DBManager::cbSelectCellStr(void* data, int argc, char** argv, char** azColName)
{
  do
  {
    if ((argc <= 0)
      || (data == NULL)
      || (argv == NULL)
      || (argv[0] == NULL)
      )
    {
      break;
    }

    DbCbArgStrDataTAG* strData = (DbCbArgStrDataTAG*)data;
    strData->valid = true;
    strncpy(strData->sVal, argv[0], strData->slen);
#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] cbs %i %s", 1, strData->sVal);
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef DB_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(dbMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] cbs %i", -99);
#endif // DB_MANAGER_CONSOLE_DEBUG_ENABLE
  return -1;
}
#endif // _CONF_UI_MANAGER_ENABLED