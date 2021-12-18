#ifndef _FILE_SYSTME_MANAGER_H
#define _FILE_SYSTME_MANAGER_H

#include "FileSystemManagerConstant.h"
#if (_CONF_FILE_SYSTEM_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../../drivers/esp32/fs/file_system.h"
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

/*FileSystemManager*/
class FileSystemManager
{
private:
  FileSystemManager(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~FileSystemManager(void);

public:
  static FileSystemManager& getInstance(void);
  FileSystemManager(FileSystemManager const&) = delete;
  void operator=(FileSystemManager const&) = delete;

  int                                                           initialize(FileSystemConfigTAG& fsConfig);
  bool                                                          isValid(void);
  //FileSystem_t                                                  devFileSystem(void);
  //int                                                           listDir(const char* dirname, uint8_t levels);
  size_t                                                        fileSize(const char* path);
  bool                                                          exist(const char* path);
  bool                                                          isValidFileHandler(FileHandler_t fileHandler);
  FileHandler_t                                                 openFile(const char* path, const char* mode);
  void                                                          closeFile(FileHandler_t fileHandler);
  int                                                           renameFile(const char* oldPath, const char* newPath);
  int                                                           deleteFile(const char* path);
  int                                                           createDir(const char* path, FileMode_t mode);
  int                                                           renameDir(const char* oldPath, const char* newPath);
  int                                                           deleteDir(const char* path);
  void                                                          finalize(void);
protected:
  //char                                                          base_Path[FS_MGR_BASE_PATH_LEN_MAX + 1];
  char                                                          partition_Label[FS_MGR_PARTITION_LABEL_LEN_MAX + 1];
};
#endif // _CONF_FILE_SYSTEM_MANAGER_ENABLED

#endif // _FILE_SYSTME_MANAGER_H