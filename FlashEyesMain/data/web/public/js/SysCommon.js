
// system 
const SERVER_API_URL_SETTING = '/api/v1/system/update';
const FEM_SCAN_TRG_SRC_HTTP = 1;

const FEM_SCAN_ERR_CODE_NONE = 0;
const FEM_SCAN_ERR_CODE_NOT_AVAI = 1;
const FEM_SCAN_ERR_CODE_NOT_VALID = 2;
const FEM_SCAN_ERR_CODE_OP_ERR = 3;

// data type
const SYS_DATA_T_DOUBLE = 0;
const SYS_DATA_T_8 = 1;
const SYS_DATA_T_STRING = 2;
const SYS_DATA_T_16 = 3;
const SYS_DATA_T_32 = 4;
const SYS_DATA_T_64 = 5;


// MBC 
const COMM_MBC_MESS_ID_SYSTEM_SETTING = 4;
const COMM_MBC_MESS_ID_SCANNING_CONTROL = 5;
const COMM_MBC_MESS_ID_SCANNING_RESULT = 6;

const COMM_MBC_DATA_T_8 = SYS_DATA_T_8;
const COMM_MBC_DATA_T_DOUBLE = SYS_DATA_T_DOUBLE;
const COMM_MBC_DATA_T_STRING = SYS_DATA_T_STRING;
const COMM_MBC_DATA_T_16 = SYS_DATA_T_16;
const COMM_MBC_DATA_T_32 = SYS_DATA_T_32;
const COMM_MBC_DATA_T_64 = SYS_DATA_T_64;


/// Utility

class Utility {
  constructor() {

  }

  static isNumericType(dataType) {
    switch (dataType) {
      case SYS_DATA_T_8:
      case SYS_DATA_T_DOUBLE:
      case SYS_DATA_T_16:
      case SYS_DATA_T_32:
      case SYS_DATA_T_64:
        return true;
      case SYS_DATA_T_STRING:
        return false;
      default:
        return false;
    }
  }

  static isValidType(dataType) {
    if (!dataType) {
      return false;
    }

    switch (dataType) {
      case SYS_DATA_T_8:
      case SYS_DATA_T_DOUBLE:
      case SYS_DATA_T_16:
      case SYS_DATA_T_32:
      case SYS_DATA_T_64:
      case SYS_DATA_T_STRING:
        return true;
      default:
        return false;
    }
  }

  static asyncSleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms))
  }
}