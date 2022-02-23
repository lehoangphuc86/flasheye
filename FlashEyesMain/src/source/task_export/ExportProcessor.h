#include "ExportManagerConstant.h"

#if (_CONF_EXPORT_PROCESSOR_ENABLED)

#ifndef _EXPORT_PROCESSOR_H
#define _EXPORT_PROCESSOR_H

/////////////////////////////////////////////////
// INCLUDE

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

/*ExportProcessor*/
class ExportProcessor
{
public:
  ExportProcessor(byte procType);
  virtual ~ExportProcessor(void);
  virtual bool                                                  isValid(void);
  byte                                                          procType(void);
  virtual int                                                   start(ExportProcessorConfigTAG& procConfig) = 0;
  virtual void                                                  stop(void) = 0;
  virtual int                                                   exportScanResult(ExportScanResultTAG* param) = 0;

protected:
  byte                                                          proc_Type;
};

#endif // _EXPORT_PROCESSOR_H

#endif // _CONF_EXPORT_PROCESSOR_ENABLED