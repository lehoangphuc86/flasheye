#include "ExportManagerConstant.h"
#ifndef _EXPORT_CONTROL_TASK_H
#define _EXPORT_CONTROL_TASK_H
#if (_CONF_EXPORT_CONTROL_TASK_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "ExportProcessorFactory.h"
#include "task_manager/TaskManager.h"
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

/*ExportControlTask*/
class ExportControlTask
  : public TaskManager
{
public:
  ExportControlTask(void);
  virtual ~ExportControlTask(void);
  bool                                                          isValid(void);
  int                                                           startTask(ExportManagerConfigTAG& exportManagerConfig);
  void                                                          stopTask(void);
  void                                                          cleanUp(void);

protected:
  void                                                          proc(void) override;
  int                                                           prepare(void);
  int                                                           onEventExportScanResult(unsigned char* data, unsigned int dataSize);
protected:
  ExportProcessor*                                              export_Processor;
};

#endif // _CONF_EXPORT_CONTROL_TASK_ENABLED

#endif // _EXPORT_CONTROL_TASK_H
