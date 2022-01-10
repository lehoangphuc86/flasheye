#ifndef _SCANNER_EV_ER080_H
#define _SCANNER_EV_ER080_H

/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define EV_COMM_BUFF_LEN_MAX                            128
#define EV_CMD_OPEN_CHARACTER_NUM                       (3)
#define EV_CMD_CLOSE_CHARACTER_NUM                      (3)
#define EV_CMD_PREFIX_CHARACTER_NUM                     (1)
#define EV_CMD_POSTFIX_CHARACTER_NUM                    (1)
#define EV_CMD_CODE_LEN                                 (4)
#define EV_CMD_RES_CODE_LEN                             (1)
#define EV_REV_CMD_BODY_LEN                             (EV_CMD_RES_CODE_LEN + EV_CMD_PREFIX_CHARACTER_NUM + EV_CMD_CODE_LEN + EV_CMD_POSTFIX_CHARACTER_NUM)
#define EV_SEND_CMD_BUFF_LEN_MAX                        (EV_CMD_OPEN_CHARACTER_NUM + EV_CMD_PREFIX_CHARACTER_NUM + EV_CMD_CODE_LEN + EV_CMD_POSTFIX_CHARACTER_NUM + EV_CMD_CLOSE_CHARACTER_NUM + 1)
#define EV_CMD_OPEN_CHARACTER                           '<'
#define EV_CMD_OPEN_STR                                 "<<<"
#define EV_CMD_CLOSE_CHARACTER                          '>'
#define EV_CMD_CLOSE_STR                                ">>>"
#define EV_CMD_PREFIX_CHARACTER                         'A'
#define EV_CMD_POSTFIX_CHARACTER                        ';'
#define EV_CMD_RES_CODE_SUCCESS                         '!'
#define EV_CMD_RES_CODE_FAIL                            '?'

#define EV_CODE_TERMINATOR_CHARACTER                    0x0A

#define EV_CMD_PRINT_FORMAT                             "%s%c%04d%c%s"

//#########################op command############################3
#define EV_CMD_OP_START_SETTING                         34
#define EV_CMD_OP_STOP_SETTING                          33
#define EV_CMD_OP_FACTORY_RESET                         30
#define EV_CMD_OP_INIT_MODE                             216
#define EV_CMD_OP_START_SCAN                            35
#define EV_CMD_OP_STOP_SCAN                             36

//#########################settings#############################
#define EV_CMD_SET_INVALID                              9999

//-------------------------comm-----------------------------
// Comm::baudrate
#define EV_CMD_SET_COMM_BAUD_1200                       1001
#define EV_CMD_SET_COMM_BAUD_2400                       1002
#define EV_CMD_SET_COMM_BAUD_4800                       1003
#define EV_CMD_SET_COMM_BAUD_9600                       1004
#define EV_CMD_SET_COMM_BAUD_14400                      1005
#define EV_CMD_SET_COMM_BAUD_19200                      1006
#define EV_CMD_SET_COMM_BAUD_38400                      1007
#define EV_CMD_SET_COMM_BAUD_57600                      1008
#define EV_CMD_SET_COMM_BAUD_115200                     1009

// Comm::parity
#define EV_CMD_SET_COMM_PARITY_NONE                     1020
#define EV_CMD_SET_COMM_PARITY_ODD                      1021
#define EV_CMD_SET_COMM_PARITY_EVEN                     1022

// Comm: stopbit
#define EV_CMD_SET_COMM_STOPBIT_1                       1031
#define EV_CMD_SET_COMM_STOPBIT_2                       1033

// Comm: databit
#define EV_CMD_SET_COMM_DATABIT_8                       1050
#define EV_CMD_SET_COMM_DATABIT_7                       1051

// Comm: control follow
#define EV_CMD_SET_COMM_FLOW_CTRL_NONE                  1040
#define EV_CMD_SET_COMM_FLOW_CTRL_RTS_NO                1041
#define EV_CMD_SET_COMM_FLOW_CTRL_CTS_NO                1042
#define EV_CMD_SET_COMM_FLOW_CTRL_CTS                   1043

//-------------------------Character------------------------
#define EV_CMD_SET_CHARACTER_NO_CONVERT                 1130
#define EV_CMD_SET_CHARACTER_TO_UPPER                   1131
#define EV_CMD_SET_CHARACTER_TO_LOWER                   1132
#define EV_CMD_SET_CHARACTER_REVERSE                    1133

//-------------------------Read state-----------------------
#define EV_CMD_SET_READ_STATE_ENABLE                    0042
#define EV_CMD_SET_READ_STATE_DISABLE                   0043

//-------------------------Auto scanning--------------------
#define EV_CMD_SET_AUTO_SCAN_ENABLE                     0211
#define EV_CMD_SET_AUTO_SCAN_TIME_LIMIT                 0250
#define EV_CMD_SET_AUTO_SCAN_REPEAT_FORBIT              0255
#define EV_CMD_SET_AUTO_SCAN_REPEAT_ALLOWED             0256

//-------------------------Continous scanning---------------
#define EV_CMD_SET_CONT_SCAN_ENABLE                     0214
#define EV_CMD_SET_CONT_SCAN_SET_TIME_LEN               0265

//-------------------------Security level-------------------
#define EV_CMD_SET_SECURITY_LEVEL_1                     0220
#define EV_CMD_SET_SECURITY_LEVEL_2                     0221
#define EV_CMD_SET_SECURITY_LEVEL_3                     0222
#define EV_CMD_SET_SECURITY_LEVEL_4                     0223

//-------------------------Code prefix----------------------
#define EV_CMD_SET_CODE_PREFIX_DISABLE                  2040
#define EV_CMD_SET_CODE_PREFIX_ENABLE                   2041
#define EV_CMD_SET_CODE_PREFIX_DEFAULT                  2042

//-------------------------Code prefix mod------------------
/* Send MOD_SHOW -> send character(s) -> send "save" */
#define EV_CMD_SET_CODE_PREFIX_MOD_HIDE                 2020
#define EV_CMD_SET_CODE_PREFIX_MOD_SHOW                 2021
#define EV_CMD_SET_CODE_PREFIX_MOD_ADD                  2022

//-------------------------Code suffix mod------------------
/* Send MOD_SHOW -> send character(s) -> send "save" */
#define EV_CMD_SET_CODE_SUFFIX_MOD_HIDE                 2100
#define EV_CMD_SET_CODE_SUFFIX_MOD_SHOW                 2101
#define EV_CMD_SET_CODE_SUFFIX_MOD_ADD                  2102

//-------------------------Code cus prefix/suffix format----
#define EV_CMD_SET_CODE_CUS_FORMAT_1                    2010 // CodeID + Custom + AIMIID
#define EV_CMD_SET_CODE_CUS_FORMAT_2                    2011 // Custom + CodeID + AIMIID

//-------------------------Message terminator---------------
#define EV_CMD_SET_MESS_TER_NONE                        2110
#define EV_CMD_SET_MESS_TER_ENTER                       2127  // 0x0D
#define EV_CMD_SET_MESS_TER_ENTER_N_LINE_BREAK          2128  // 0x0D;0x0A
#define EV_CMD_SET_MESS_TER_LINE_BREAK                  2129  // 0x0A
#define EV_CMD_SET_MESS_TER_LINE_SKIP                   2130  // 0x0A;0x0A
#define EV_CMD_SET_MESS_TER_ENTER_N_LINE_SKIP           2131  // 0x0D;0x0A;0x0A
#define EV_CMD_SET_MESS_TER_TAB                         2136  // TAB
#define EV_CMD_SET_MESS_TER_CURSOR_RETURN               2125  // HOME

//-------------------------Param code (HEX)-----------------
#define EV_CMD_SET_PARAM_CODE_0                         0
#define EV_CMD_SET_PARAM_CODE_1                         1
#define EV_CMD_SET_PARAM_CODE_2                         2
#define EV_CMD_SET_PARAM_CODE_3                         3
#define EV_CMD_SET_PARAM_CODE_4                         4
#define EV_CMD_SET_PARAM_CODE_5                         5
#define EV_CMD_SET_PARAM_CODE_6                         6
#define EV_CMD_SET_PARAM_CODE_7                         7
#define EV_CMD_SET_PARAM_CODE_8                         8
#define EV_CMD_SET_PARAM_CODE_9                         9
#define EV_CMD_SET_PARAM_CODE_A                         10
#define EV_CMD_SET_PARAM_CODE_B                         11
#define EV_CMD_SET_PARAM_CODE_C                         12
#define EV_CMD_SET_PARAM_CODE_D                         13
#define EV_CMD_SET_PARAM_CODE_E                         14
#define EV_CMD_SET_PARAM_CODE_F                         15
#define EV_CMD_SET_PARAM_CODE_SAVE                      20
#define EV_CMD_SET_PARAM_CODE_RESET                     22  // read this to reset default
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

#endif // _SCANNER_EV_ER080_H
