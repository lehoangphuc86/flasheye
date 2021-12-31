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
#define DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
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
typedef struct _dbCbArgDataTAG
{
  uint32_t dataType;
  void* data;
} DbCbArgDataTAG;
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
  template <typename RetTypeT>
  int                                                           selectCell(byte tableId, const char* colName, const char* keyName, unsigned int id, RetTypeT* val);
  template <typename ValTypeT>
  int                                                           updateCell(byte tableId, const char* colName, const char* keyName, unsigned int id, ValTypeT newVal);
  template <typename ValTypeT>
  int                                                           updateCell(const char* format, byte tableId, const char* colName, const char* keyName, unsigned int id, ValTypeT newVal);
  void                                                          finalize(void);
  int                                                           exec(const char* sql, int (*callback)(void*, int, char**, char**), void*, char** errmsg);
  int                                                           exeScriptFile(const char* scriptFile, char* tmpBuf, DataSize_t tmpBufSize, const char lineTeminator = DB_MANAGER_LINE_TERMINATOR_DEFAULT);
private:
  static int                                                    cbSelectCell(void* data, int argc, char** argv, char** azColName);
protected:
  DBHandler_t                                                   db_Handler;
  SystemMutex                                                   db_Locker[DB_TBL_ID_MAX]; // locker for each table
  char                                                          sql_Query[DB_TBL_ID_MAX][DB_QUERY_LEN_MAX];

#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
  char dbMgrLogBuf2[DB_QUERY_LEN_MAX];
#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
};

template <typename RetTypeT>
int DBManager::selectCell(byte tableId, const char* colName, const char* keyName, unsigned int id, RetTypeT* val)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (tableId >= DB_TBL_ID_MAX)
    {
      break;
    }
    
    int ret = 0;
    DbCbArgDataTAG retStruct = DbCbArgDataTAG();
    retStruct.dataType = TypeClassT<RetTypeT>::value;
    retStruct.data = val;
    //char* errorMess = NULL;
    {
      SystemMutexLocker locker(this->db_Locker[tableId]);
      SYSTEM_PRINT_BUF(this->sql_Query[tableId], DB_QUERY_LEN_MAX, "SELECT %s FROM %s WHERE %s=%u", colName, g_Db_Tbl_Names[tableId], keyName, id);
#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      /*CONSOLE_LOG_BUF(this->dbMgrLogBuf2, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db] sC %i", 1);
      CONSOLE_LOG_BUF(this->dbMgrLogBuf2, DB_QUERY_LEN_MAX, "[db] sC %i %s", 2, this->sql_Query[tableId]);
      CONSOLE_LOG_BUF(this->dbMgrLogBuf2, DB_QUERY_LEN_MAX, "[db] sC %i %i %i", 3, retStruct.data, sizeof(retStruct.data));*/
#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
      ret = sqlite3_exec(this->db_Handler, this->sql_Query[tableId], DBManager::cbSelectCell, (void*)&retStruct, NULL);
//#ifdef DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(this->dbMgrLogBuf2, SYSTEM_CONSOLE_OUT_BUF_LEN, "[db]: sC %i %i %i %i", 7, ret , *((RetTypeT*)val), *((RetTypeT*)retStruct.data));
//#endif // DB_MANAGER_2_CONSOLE_DEBUG_ENABLE
    }

    if (ret != DB_RET_OK)
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

template <typename ValTypeT>
int DBManager::updateCell(const char* format, byte tableId, const char* colName, const char* keyName, unsigned int id, ValTypeT newVal)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (tableId >= DB_TBL_ID_MAX)
    {
      break;
    }

    int ret = 0;
    {
      SystemMutexLocker locker(this->db_Locker[tableId]);
      SYSTEM_PRINT_BUF(sql_Query[tableId], DB_QUERY_LEN_MAX, format, g_Db_Tbl_Names[tableId], colName, newVal, keyName, id);
      ret = sqlite3_exec(this->db_Handler, sql_Query[tableId], NULL, NULL, NULL);
    }

    if (ret != DB_RET_OK)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

template <typename ValTypeT>
int DBManager::updateCell(byte tableId, const char* colName, const char* keyName, unsigned int id, ValTypeT newVal)
{
  switch (TypeClassT<ValTypeT>::value)
  {
  case TypeClass::TString:
  case TypeClass::TChar_t:
    return this->updateCell("UPDATE %s SET %s=%ld WHERE %s='%u'", tableId, colName, keyName, id, newVal);
  default:
    return this->updateCell("UPDATE %s SET %s=%ld WHERE %s=%u", tableId, colName, keyName, id, newVal);
    break;
  }
}

#endif // _CONF_DB_MANAGER_ENABLED

#endif // _DB_MANAGER_H