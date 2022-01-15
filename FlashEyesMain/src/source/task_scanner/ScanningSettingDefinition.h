#include "../../FlashEyesMain.h"
#ifndef _SCANNING_SETTING_DEFINITION_H
#define _SCANNING_SETTING_DEFINITION_H
/////////////////////////////////////////////////
// INCLUDE
#if (_CONF_SCANNING_SETTING_DEFINITION_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#########################HW#########################
//#########################Setting Id#################
#define SCANNING_SET_ID_INVALID                       0
#define SCANNING_SET_ID_COM_BAUDRATE                  1
#define SCANNING_SET_ID_COM_PARITY                    2
#define SCANNING_SET_ID_COM_STOPBIT                   3
#define SCANNING_SET_ID_COM_DATABIT                   4
#define SCANNING_SET_ID_COM_FLOW_CTRL                 5
#define SCANNING_SET_ID_CHARACTER                     6
#define SCANNING_SET_ID_READ_STATE                    7
#define SCANNING_SET_ID_AUTO_SCAN                     8
#define SCANNING_SET_ID_CONT_SCAN                     9
#define SCANNING_SET_ID_SECURITY_LEVEL                10
#define SCANNING_SET_ID_CODE_PREFIX                   11
#define SCANNING_SET_ID_CODE_PREFIX_MOD               12
#define SCANNING_SET_ID_CODE_SUFFIX_MOD               13
#define SCANNING_SET_ID_CODE_CUS_FORMAT               14
#define SCANNING_SET_ID_MESS_TER                      15

//#########################Setting enum###############
//-------------------comm----------------------
// Comm: Baudrate
#define SCANNING_SET_COMM_BAUD_1200                   0
#define SCANNING_SET_COMM_BAUD_2400                   1
#define SCANNING_SET_COMM_BAUD_4800                   2
#define SCANNING_SET_COMM_BAUD_9600                   3
#define SCANNING_SET_COMM_BAUD_14400                  4
#define SCANNING_SET_COMM_BAUD_19200                  5
#define SCANNING_SET_COMM_BAUD_38400                  6
#define SCANNING_SET_COMM_BAUD_57600                  7
#define SCANNING_SET_COMM_BAUD_115200                 8
#define SCANNING_SET_COMM_BAUD_MAX                    9
// Comm: parity
#define SCANNING_SET_COMM_PARITY_NONE                 0
#define SCANNING_SET_COMM_PARITY_ODD                  1
#define SCANNING_SET_COMM_PARITY_EVEN                 2
#define SCANNING_SET_COMM_PARITY_MAX                  3
// Comm: stopbit
#define SCANNING_SET_COMM_STOPBIT_1                   0
#define SCANNING_SET_COMM_STOPBIT_2                   1
#define SCANNING_SET_COMM_STOPBIT_MAX                 2
// Comm: databit
#define SCANNING_SET_COMM_DATABIT_8                   0
#define SCANNING_SET_COMM_DATABIT_7                   1
#define SCANNING_SET_COMM_DATABIT_6                   2
#define SCANNING_SET_COMM_DATABIT_5                   3
#define SCANNING_SET_COMM_DATABIT_MAX                 4
// Comm: control follow
#define SCANNING_SET_COMM_FLOW_CTRL_NONE              0
#define SCANNING_SET_COMM_FLOW_CTRL_RTS_NO            1
#define SCANNING_SET_COMM_FLOW_CTRL_CTS_NO            2
#define SCANNING_SET_COMM_FLOW_CTRL_CTS               3
#define SCANNING_SET_COMM_FLOW_CTRL_MAX               4
//--------------------character format----------------
#define SCANNING_SET_CHARACTER_NO_CONVERT             0
#define SCANNING_SET_CHARACTER_TO_UPPER               1
#define SCANNING_SET_CHARACTER_TO_LOWER               2
#define SCANNING_SET_CHARACTER_REVERSE                3
#define SCANNING_SET_CHARACTER_MAX                    4
//-------------------security level-------------------
#define SCANNING_SET_SECURITY_LEVEL_1                 0
#define SCANNING_SET_SECURITY_LEVEL_2                 1
#define SCANNING_SET_SECURITY_LEVEL_3                 2
#define SCANNING_SET_SECURITY_LEVEL_4                 3
#define SCANNING_SET_SECURITY_LEVEL_MAX               4
//------------------Code cus prefix/suffix format-----
#define SCANNING_SET_CODE_CUS_FORMAT_1                0 // CodeID + Custom + AIMIID
#define SCANNING_SET_CODE_CUS_FORMAT_2                1 // Custom + CodeID + AIMIID
#define SCANNING_SET_CODE_CUS_FORMAT_MAX              2
//-------------------Mess terminator------------------
#define SCANNING_SET_MESS_TER_NONE                    0
#define SCANNING_SET_MESS_TER_ENTER                   1 // 0x0D
#define SCANNING_SET_MESS_TER_ENTER_N_LINE_BREAK      2 // 0x0D 0x0A
#define SCANNING_SET_MESS_TER_LINE_BREAK              3 // 0x0A
#define SCANNING_SET_MESS_TER_MAX                     4

//#########################Setting default###########
#define SCANNING_SET_DF_COM_BAUDRATE                  SCANNING_SET_COMM_BAUD_9600
#define SCANNING_SET_DF_COM_PARITY                    SCANNING_SET_COMM_PARITY_NONE
#define SCANNING_SET_DF_COM_STOPBIT                   SCANNING_SET_COMM_STOPBIT_1
#define SCANNING_SET_DF_COM_DATABIT                   SCANNING_SET_COMM_DATABIT_8
#define SCANNING_SET_DF_COM_FLOW_CTRL                 SCANNING_SET_COMM_FLOW_CTRL_NONE
#define SCANNING_SET_DF_CHARACTER                     SCANNING_SET_CHARACTER_TO_UPPER
#define SCANNING_SET_DF_READ_STATE                    true
#define SCANNING_SET_DF_AUTO_SCAN                     false
#define SCANNING_SET_DF_CONT_SCAN                     false
#define SCANNING_SET_DF_SECURITY_LEVEL                SCANNING_SET_SECURITY_LEVEL_1
#define SCANNING_SET_DF_CODE_PREFIX                   true
#define SCANNING_SET_DF_CODE_PREFIX_MOD               ""
#define SCANNING_SET_DF_CODE_SUFFIX_MOD               ""
#define SCANNING_SET_DF_CODE_CUS_FORMAT               SCANNING_SET_CODE_CUS_FORMAT_2
#define SCANNING_SET_DF_MESS_TER                      SCANNING_SET_MESS_TER_LINE_BREAK
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
//

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION


#endif // _CONF_SCANNING_SETTING_DEFINITION_ENABLED

#endif // _SCANNING_SETTING_DEFINITION_H

