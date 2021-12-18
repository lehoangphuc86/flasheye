/////////////////////////////////////////////////
// INCLUDE
#include "FileSystemManager.h"

#if (_CONF_FILE_SYSTEM_MANAGER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define FILE_SYSTEM_MANAGER_CONSOLE_DEBUG_ENABLE

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
#ifdef FILE_SYSTEM_MANAGER_CONSOLE_DEBUG_ENABLE
char fsLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // FILE_SYSTEM_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*FileSystemManager*/
FileSystemManager& FileSystemManager::getInstance(void)
{
  static FileSystemManager instance;
  return instance;
}

FileSystemManager::FileSystemManager(void)
{
  memset(this->partition_Label, 0, FS_MGR_PARTITION_LABEL_LEN_MAX + 1);
}

FileSystemManager::~FileSystemManager(void)
{
  this->finalize();
}

bool FileSystemManager::isValid(void)
{
  return esp_spiffs_mounted(this->partition_Label);
}

//FileSystem_t FileSystemManager::devFileSystem(void)
//{
//  return DevFileSystemMgr;
//}

int FileSystemManager::initialize(FileSystemConfigTAG& fsConfig)
{
  int result = 0;
  FsRet_t fsRet = FS_RET_OK;
  do
  {
    if (this->isValid() != 0)
    {
      return -1; // already initialized
    }

    this->finalize();

    esp_vfs_spiffs_conf_t spiffsConf = esp_vfs_spiffs_conf_t();
    spiffsConf.base_path = fsConfig.basePath;
    spiffsConf.partition_label = fsConfig.partitionLabel;
    spiffsConf.max_files = fsConfig.maxOpenFile;
    spiffsConf.format_if_mount_failed = fsConfig.formatIfFailed;
  
    fsRet = esp_vfs_spiffs_register(&spiffsConf);
    if (fsRet != FS_RET_OK)
    {
      break;
    }
    if (fsConfig.partitionLabel != NULL)
    {
      strlcpy(this->partition_Label, fsConfig.partitionLabel, FS_MGR_PARTITION_LABEL_LEN_MAX);
    }
    return 0;
  } while (0);

  this->finalize();
  return -1;
}

void FileSystemManager::finalize(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    esp_vfs_spiffs_unregister(this->partition_Label);
    this->partition_Label[0] = '\0';
    return;
  } while (0);
  this->partition_Label[0] = '\0';
  return;
}

//
//int FileSystemManager::listDir(const char* dirname, uint8_t levels)
//{
//  DIR* dir = NULL;
//  do
//  {
//    //// list SPIFFS contents
//    if (this->isValid() == false)
//    {
//      break;
//    }
//    
//
//    char logBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
//    CONSOLE_LOG_BUF(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "/\: %s", dirname);
//    dir = opendir(dirname);
//    if (dir == NULL)
//    {
//      break;
//    }
//
//    /*FileHandler_t file = root.openNextFile();
//    while (file != NULL)
//    {
//      if (file.isDirectory() != false)
//      {
//        CONSOLE_LOG_BUF(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "D: %s", file.path());
//        if (levels)
//        {
//          this->listDir(file.path(), levels - 1);
//        }
//      }
//      else
//      {
//        CONSOLE_LOG_BUF(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "F: %s s:%u", file.path(), file.size());
//      }
//      file = root.openNextFile();
//    }*/
//
//    // Read directory entries
//    uint64_t total = 0;
//    int nfiles = 0;
//    struct dirent* ent;
//    char tpath[255];
//    char tbuffer[80];
//    int statok;
//    struct stat sb;
//    struct tm* tm_info;
//    char type;
//    char size[9];
//    while ((ent = readdir(dir)) != NULL)
//    {
//      sprintf(tpath, dirname);
//      if (dirname[strlen(dirname) - 1] != '/')
//      {
//        strcat(tpath, "/");
//      }
//
//      strcat(tpath, ent->d_name);
//      tbuffer[0] = '\0';
//
//      // Get file stat
//      statok = stat(tpath, &sb);
//
//      if (statok == 0)
//      {
//        tm_info = localtime(&sb.st_mtime);
//        strftime(tbuffer, 80, "%d/%m/%Y %R", tm_info);
//      }
//      else
//      {
//        sprintf(tbuffer, "                ");
//      }
//
//      if (ent->d_type == DT_REG)
//      {
//        type = 'f';
//        nfiles++;
//        if (statok)
//        {
//          strcpy(size, "       ?");
//        }
//        else
//        {
//          total += sb.st_size;
//          if (sb.st_size < (1024 * 1024)) sprintf(size, "%8d", (int)sb.st_size);
//          else if ((sb.st_size / 1024) < (1024 * 1024)) sprintf(size, "%6dKB", (int)(sb.st_size / 1024));
//          else sprintf(size, "%6dMB", (int)(sb.st_size / (1024 * 1024)));
//        }
//      }
//      else
//      {
//        type = 'd';
//        strcpy(size, "       -");
//      }
//
//      printf("%c  %s  %s  %s\r\n",
//        type,
//        size,
//        tbuffer,
//        ent->d_name
//      );
//    }
//
//    closedir(dir);
//    return 0;
//  } while (0);
//
//  if (dir != NULL)
//  {
//    closedir(dir);
//  }
//  return -1;
//}
size_t FileSystemManager::fileSize(const char* path)
{
  struct stat statInfo;
  if (stat(path, &statInfo) == 0)
  {
    return 0;
  }
  return statInfo.st_size;
}

bool FileSystemManager::exist(const char* path)
{
  struct stat statInfo;
  return (stat(path, &statInfo) == 0);
}

bool FileSystemManager::isValidFileHandler(FileHandler_t fileHandler)
{
  return (fileHandler == FS_FILE_HANDLER_INVALID? false :true);
}

FileHandler_t FileSystemManager::openFile(const char* path, const char* mode)
{
  return fopen(path, mode);
}

void FileSystemManager::closeFile(FileHandler_t fileHandler)
{
  fclose(fileHandler);
}

int FileSystemManager::renameFile(const char* oldPath, const char* newPath)
{
  return rename(oldPath, newPath);
}

int FileSystemManager::deleteFile(const char* path)
{
  return remove(path);
}

int FileSystemManager::createDir(const char* path, FileMode_t mode)
{
  return mkdir(path, mode);
}

int FileSystemManager::renameDir(const char* oldPath, const char* newPath)
{
  return this->renameFile(oldPath, newPath);
}

int FileSystemManager::deleteDir(const char* path)
{
  return rmdir(path);
}

#endif // _CONF_FILE_SYSTEM_MANAGER_ENABLED