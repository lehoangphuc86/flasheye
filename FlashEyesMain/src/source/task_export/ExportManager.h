#include "ExportManagerConstant.h"
#ifndef _EXPORT_MANAGER_H
#define _EXPORT_MANAGER_H
#if (_CONF_EXPORT_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "ExportControlTask.h"
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

/*ExportManager*/
class ExportManager
{
private:
  ExportManager(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~ExportManager(void);

public:
  static ExportManager& getInstance(void);
  ExportManager(ExportManager const&) = delete;
  void operator=(ExportManager const&) = delete;
  bool                                                          isRunning(void);
  bool                                                          isValid(void);
  EventManager*                                                 eventManager(void);
  int                                                           startTask(ExportManagerConfigTAG& exportManagerConfig);
  void                                                          stopTask(void);
  int                                                           exportScanResult(ExportScanResultTAG& scanResult);
protected:
  ExportControlTask                                             control_Task;
};
#endif // _CONF_EXPORT_MANAGER_ENABLED

#endif // _EXPORT_MANAGER_H