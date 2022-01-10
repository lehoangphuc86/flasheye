#include "DBManagerConstant.h"
#ifndef _DB_MANAGER_H
#define _DB_MANAGER_H
#if (_CONF_DB_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../../drivers/esp32/db/dbms.h"
#include "os_system/SystemMutex.h"
/////////////////////////////////////////////////
// PREPROCESSOR
//#define DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE
#define DB_MANAGER_LINE_TERMINATOR_DEFAULT       '\n'

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef struct _dbCbArgNumDataTAG
{
  bool valid;
  double* dVal;
} DbCbArgNumDataTAG;

typedef struct _dbCbArgStrDataTAG
{
  bool valid;
  char* sVal;
  DataSize_t slen;
} DbCbArgStrDataTAG;
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

/*DBManager*/
class DBManager
{
private:
  DBManager(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~DBManager(void);

public:
  static DBManager& getInstance(void);
  DBManager(DBManager const&) = delete;
  void operator=(DBManager const&) = delete;

  int                                                           initialize(DBManagerConfigTAG& dbConfig);
  bool                                                          isValid(void);
  DBHandler_t                                                   dbHandler(void);
  int                                                           exec(const char* sql, int (*callback)(void*, int, char**, char**), void*, char** errmsg);
  int                                                           exeScriptFile(const char* scriptFile, char* tmpBuf, DataSize_t tmpBufSize, const char lineTeminator = DB_MANAGER_LINE_TERMINATOR_DEFAULT);
  int                                                           selectCellStr(DbTableId_t tableId, const char* colName, const char* keyName, unsigned int id, char* val, DataSize_t valLen);
  template <typename T>
  int                                                           selectCellNum(DbTableId_t tableId, const char* colName, const char* keyName, unsigned int id, T& val);
  int                                                           updateCellStr(DbTableId_t tableId, const char* colName, const char* keyName, unsigned int id, char* newVal, DataSize_t newValLen);
  template <typename T>
  int                                                           updateCellNum(DbTableId_t tableId, const char* colName, const char* keyName, unsigned int id, T newVal);
  void                                                          finalize(void);

private:
  static int                                                    cbSelectCellNum(void* data, int argc, char** argv, char** azColName);
  static int                                                    cbSelectCellStr(void* data, int argc, char** argv, char** azColName);
protected:
  DBHandler_t                                                   db_Handler;
  SystemMutex*                                                  db_Locker; // locker for each table
  char**                                                        sql_Query;//[DB_TBL_ID_MAX][DB_QUERY_LEN_MAX];
  const char**                                                  db_Table_Names;
  DbTableId_t                                                   db_Table_Id_Max;

#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
  char dbMgrLogBuf2[DB_QUERY_LEN_MAX];
#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
};

template <typename T>
int DBManager::selectCellNum(DbTableId_t tableId, const char* colName, const char* keyName, unsigned int id, T& val)
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
    
    val = 0;
    double dVal = 0;
    //char* errorMess = NULL;
    DbCbArgNumDataTAG numData = DbCbArgNumDataTAG();
    numData.valid = false;
    numData.dVal = &dVal;
    {
      SystemMutexLocker locker(this->db_Locker[tableId]);
      SYSTEM_PRINT_BUF(this->sql_Query[tableId], DB_QUERY_LEN_MAX, "SELECT %s FROM %s WHERE %s=%u", colName, this->db_Table_Names[tableId], keyName, id);
//#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(this->dbMgrLogBuf2, DB_QUERY_LEN_MAX, "[db] sCN %i %s", 2, this->sql_Query[tableId]);
//#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      ret = sqlite3_exec(this->db_Handler, this->sql_Query[tableId], DBManager::cbSelectCellNum, (void*)&numData, NULL);
//#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(this->dbMgrLogBuf2, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db]: sCN %i %i %f", 7, ret , dVal);
//#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
    }

    if ((ret != DB_RET_OK)
      || (numData.valid == false)
      )
    {
#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      /*CONSOLE_LOG_BUF(this->dbMgrLogBuf2, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", errorMess);
      sqlite3_free(errorMess);*/
#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      break;
    }
    val = (T)dVal;
    return 0;
  } while (0);
  val = 0;
  return -1;
}

template <typename T>
int DBManager::updateCellNum(DbTableId_t tableId, const char* colName, const char* keyName, unsigned int id, T newVal)
{
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

    int ret = 0;
    {
      double dVal = newVal;
      SystemMutexLocker locker(this->db_Locker[tableId]);
      SYSTEM_PRINT_BUF(sql_Query[tableId], DB_QUERY_LEN_MAX, "UPDATE %s SET %s=%f WHERE %s=%u", this->db_Table_Names[tableId], colName, dVal, keyName, id);
#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(this->dbMgrLogBuf2, DB_QUERY_LEN_MAX, "[db] uCN %i %s", 2, this->sql_Query[tableId]);
#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      ret = sqlite3_exec(this->db_Handler, sql_Query[tableId], NULL, NULL, NULL);
#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(this->dbMgrLogBuf2, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db]: uCN %i %i %f", 7, ret , newVal);
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

// "UPDATE %s SET %s=%ld WHERE %s=%u"

#endif // _CONF_DB_MANAGER_ENABLED

#endif // _DB_MANAGER_H