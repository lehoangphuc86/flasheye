
class CommMBCSystemSettingTAG {
  constructor(sectionId = 0, settingId = 0, bitSet1 = 0, errorCode = 0, sLen = 0, data = 0) {
    this.sectionId = sectionId;
    this.settingId = settingId;
    this.bitSet1 = bitSet1;
    this.errorCode = errorCode;
    this.sLen = sLen;
    this.data = data;
  }
}

class CommMBCScanningParamsTAG {
  constructor(enabled, timeout, trgSource, maxScanCount, timeBtwScan) {
    this.timeout = timeout;
    this.enabled = enabled;
    this.trgSource = trgSource;
    this.maxScanCount = maxScanCount;
    this.timeBtwScan = timeBtwScan;
  }
}

class CommMBCScanningControlTAG {
  constructor(sequenceId, bitSet1, errorCode, trgParams) {
    this.sequenceId = sequenceId;
    this.bitSet1 = bitSet1;
    this.errorCode = errorCode;
    this.trgParams = trgParams;
  }
}

class CommMBCScanningBarCodeTAG {
  constructor(codeLen, type, code) {
    this.codeLen = codeLen;
    this.type = type;
    this.code = code;
  }
}

class CommMBCScanningDeviceResultTAG {
  constructor(bitSet1, errorId, code) {
    this.bitSet1 = bitSet1;
    this.errorId = errorId;
    this.code = code;
  }
}

class CommMBCScanningResultTAG {
  constructor(sequenceId, bitSet1, scanIndex, errorCode, deviceResult) {
    this.sequenceId = sequenceId;
    this.bitSet1 = bitSet1;
    this.scanIndex = scanIndex;
    this.errorCode = errorCode;
    this.deviceResult = deviceResult;
  }
}

class CommMBCPackageTAG {
  constructor(messageId, body) {
    this.messageId = messageId;
    this.body = body;
  }
}

class CommMBCHeaderTAG {
  constructor(messageId, dataLen) {
    this.messageId = messageId;
    this.dataLen = dataLen;
  }
}

class JsonDataTAG {
  constructor(data) {
    this.data = data;
  }
}

class CommMBCEngine {
  constructor() {

  }

  encodeData(commMBCPackage, outJson) {
    var ret = 0;
    var jsonHeader = new JsonDataTAG("");
    var jsonBody = new JsonDataTAG("");
    outJson.data = "";
    do {
      switch (commMBCPackage.messageId) {
        case COMM_MBC_MESS_ID_SYSTEM_SETTING:
          ret = this.encodeSystemSetting(commMBCPackage.body, jsonBody);
          break;
        case COMM_MBC_MESS_ID_SCANNING_CONTROL:
          ret = this.encodeScanningControl(commMBCPackage.body, jsonBody);
          break;
        case COMM_MBC_MESS_ID_SCANNING_RESULT:
          ret = this.encodeScanningResult(commMBCPackage.body, jsonBody);
          break;
        default:
          ret = -1;
          break;
      }

      if (ret != 0) {
        break;
      }
      var commMBCHeader = new CommMBCHeaderTAG(commMBCPackage.messageId, jsonBody.data.length);
      ret = this.encodeHeader(commMBCHeader, jsonHeader);
      if (ret != 0) {
        break;
      }

      outJson.data = "{" + jsonHeader.data + "," + jsonBody.data + "}";
      return 0;
    } while (0);
    return -1;
  }

  encodeHeader(commMBCHeader, outJson) {
    outJson.data = "\"h\":" +
      JSON.stringify(
        {
          "id": commMBCHeader.messageId
          , "sz": commMBCHeader.dataLen
        }
      );

    return 0;
  }

  encodeSystemSetting(inCommMBCSystemSetting, outJson) {
    var ret = 0;
    do {
      outJson.data = "\"b\":" +
        JSON.stringify
          (
            {
              "secId": inCommMBCSystemSetting.sectionId
              , "setId": inCommMBCSystemSetting.settingId
              , "bSet1": inCommMBCSystemSetting.bitSet1
              , "eCode": inCommMBCSystemSetting.errorCode
              , "sLen": inCommMBCSystemSetting.sLen
              , "data": inCommMBCSystemSetting.data
            }
          );
      return 0;
    } while (0);
    return -1;
  }

  encodeScanningControl(inCommMBCScanningControl, outJson) {
    var ret = 0;
    do {
      outJson.data = "\"b\":" +
        JSON.stringify
          (
            {
              "seqId": inCommMBCScanningControl.sequenceId
              , "bSet1": inCommMBCScanningControl.bitSet1
              , "eCode": inCommMBCScanningControl.errorCode
              , "trgP": {
                "timO": inCommMBCScanningControl.trgParams.timeout
                , "en": inCommMBCScanningControl.trgParams.enabled
                , "src": inCommMBCScanningControl.trgParams.trgSource
                , "scMax": inCommMBCScanningControl.trgParams.maxScanCount
                , "timW": inCommMBCScanningControl.trgParams.timeBtwScan
              }
            }
        );
      return 0;
    } while (0);
    return -1;
  }

  encodeScanningResult(inCommMBCScanningResult, outJson) {
    var ret = 0;
    do {
      outJson.data = "\"b\":" +
        JSON.stringify
          (
            {
              "seqId": inCommMBCScanningResult.sequenceId
              , "bSet1": inCommMBCScanningResult.bitSet1
              , "sIdx": inCommMBCScanningResult.scanIndex
              , "eCode": inCommMBCScanningResult.errorCode
              , "devR": {
                  "bSet1": inCommMBCScanningResult.deviceResult.bitSet1
                , "eId": inCommMBCScanningResult.deviceResult.errorId
                , "code": {
                  "len": inCommMBCScanningResult.deviceResult.code.codeLen
                  , "type": inCommMBCScanningResult.deviceResult.code.type
                  , "code": inCommMBCScanningResult.deviceResult.code.code
                }
              }
            }
          );
      return 0;
    } while (0);
    return -1;
  }

  // decode
  decodeData(inEncodedJson, resCommMBCPackage) {
    var ret = 0;
    var commMBCHeader = new CommMBCHeaderTAG(0, 0);
    do {
      var jsonPaser = JSON.parse(inEncodedJson.data);
      if (jsonPaser == null) {
        break;
      }

      ret = this.decodeHeader(jsonPaser, commMBCHeader);
      if (ret != 0) {
        break;
      }

      switch (commMBCHeader.messageId) {
        case COMM_MBC_MESS_ID_SYSTEM_SETTING:
          resCommMBCPackage.body = new CommMBCSystemSettingTAG();
          ret = this.decodeSystemSetting(jsonPaser, resCommMBCPackage.body);
          break;
        case COMM_MBC_MESS_ID_SCANNING_CONTROL:
          resCommMBCPackage.body = new CommMBCScanningControlTAG();
          ret = this.decodeScanningControl(jsonPaser, resCommMBCPackage.body);
          break;
        case COMM_MBC_MESS_ID_SCANNING_RESULT:
          resCommMBCPackage.body = new CommMBCScanningResultTAG();
          ret = this.decodeScanningResult(jsonPaser, resCommMBCPackage.body);
          break;
        default:
          ret = -1;
          break;
      }

      if (ret != 0) {
        break;
      }

      resCommMBCPackage.messageId = commMBCHeader.messageId;
      return 0;
    } while (0);
    return -1;
  }

  decodeHeader(inJsonObject, outCommMBCHeader) {

    do {
      var val = inJsonObject["h"]["id"];
      if (val == null) {
        break;
      }
      outCommMBCHeader.messageId = parseInt(val);

      val = inJsonObject["h"]["sz"];
      if (val == null) {
        break;
      }

      outCommMBCHeader.dataLen = parseInt(val);

      return 0;
    } while (0);
    return -1;
  }

  decodeSystemSetting(inJsonObject, outCommMBCSystemSetting) {

    do {
      var val = inJsonObject["b"]["secId"];
      if (val == null) {
        break;
      }
      outCommMBCSystemSetting.sectionId = parseInt(val);
      val = inJsonObject["b"]["setId"];
      if (val == null) {
        break;
      }

      outCommMBCSystemSetting.settingId = parseInt(val);
      val = inJsonObject["b"]["bSet1"];
      if (val == null) {
        break;
      }
      outCommMBCSystemSetting.bSet1 = parseInt(val);

      val = inJsonObject["b"]["eCode"];
      if (val == null) {
        break;
      }
      outCommMBCSystemSetting.errorCode = parseInt(val);

      val = inJsonObject["b"]["sLen"];
      if (val == null) {
        break;
      }
      outCommMBCSystemSetting.sLen = parseInt(val);

      val = inJsonObject["b"]["data"];
      if (val == null) {
        break;
      }

      var bSet = outCommMBCSystemSetting.bSet1;
      var dataType = ((bSet << 3) >> 5);
      var isNumeric = Utility.isNumericType(dataType);
      if (isNumeric == false) {
        outCommMBCSystemSetting.data = val;
      }
      else {
        outCommMBCSystemSetting.data = parseFloat(val);
      }

      return 0;
    } while (0);
    return -1;
  }

  decodeScanningControl(inJsonObject, outCommMBCScanningControl) {

    do {
      var val = inJsonObject["b"]["seqId"];
      if (val == null) {
        break;
      }
      outCommMBCScanningControl.sequenceId = parseInt(val);

      val = inJsonObject["b"]["bSet1"];
      if (val == null) {
        break;
      }
      outCommMBCScanningControl.bSet1 = parseInt(val);

      val = inJsonObject["b"]["eCode"];
      if (val == null) {
        break;
      }
      outCommMBCScanningControl.errorCode = parseInt(val);

      val = inJsonObject["b"]["trgP"]["timO"];
      if (val == null) {
        break;
      }
      outCommMBCScanningControl.trgParams = new CommMBCScanningParamsTAG();
      outCommMBCScanningControl.trgParams.timeout = parseInt(val);

      val = inJsonObject["b"]["trgP"]["en"];
      if (val == null) {
        break;
      }
      outCommMBCScanningControl.trgParams.enabled = parseInt(val);

      val = inJsonObject["b"]["trgP"]["src"];
      if (val == null) {
        break;
      }
      outCommMBCScanningControl.trgParams.trgSource = parseInt(val);

      val = inJsonObject["b"]["trgP"]["scMax"];
      if (val == null) {
        break;
      }
      outCommMBCScanningControl.trgParams.maxScanCount = parseInt(val);

      val = inJsonObject["b"]["trgP"]["timW"];
      if (val == null) {
        break;
      }
      outCommMBCScanningControl.trgParams.timeBtwScan = parseInt(val);

      return 0;
    } while (0);
    return -1;
  }

  decodeScanningResult(inJsonObject, outCommMBCScanningResult) {

    do {
      var val = inJsonObject["b"]["seqId"];
      if (val == null) {
        break;
      }
      outCommMBCScanningResult.sequenceId = parseInt(val);

      val = inJsonObject["b"]["bSet1"];
      if (val == null) {
        break;
      }
      outCommMBCScanningResult.bitSet1 = parseInt(val);

      val = inJsonObject["b"]["eCode"];
      if (val == null) {
        break;
      }
      outCommMBCScanningResult.errorCode = parseInt(val);

      val = inJsonObject["b"]["sIdx"];
      if (val == null) {
        break;
      }
      outCommMBCScanningResult.scanIndex = parseInt(val);

      val = inJsonObject["b"]["devR"]["bSet1"];
      if (val == null) {
        break;
      }

      outCommMBCScanningResult.deviceResult = new CommMBCScanningDeviceResultTAG();
      outCommMBCScanningResult.deviceResult.bitSet1 = parseInt(val);

      val = inJsonObject["b"]["devR"]["eId"];
      if (val == null) {
        break;
      }
      outCommMBCScanningResult.deviceResult.errorId = parseInt(val);

      val = inJsonObject["b"]["devR"]["code"]["len"];
      if (val == null) {
        break;
      }

      outCommMBCScanningResult.deviceResult.code = new CommMBCScanningBarCodeTAG();
      outCommMBCScanningResult.deviceResult.code.codeLen = parseInt(val);

      val = inJsonObject["b"]["devR"]["code"]["type"];
      if (val == null) {
        break;
      }
      outCommMBCScanningResult.deviceResult.code.type = parseInt(val);

      val = inJsonObject["b"]["devR"]["code"]["code"];
      if (val == null) {
        break;
      }
      outCommMBCScanningResult.deviceResult.code.code = val;

      return 0;
    } while (0);
    return -1;
  }
}

class CommMBCHttpClient {
  constructor() {
    this.comm_MBC_Engine = new CommMBCEngine();
  }

  send(reqCommMBCPackage, resCommMBCPackage, isAsync) {
    var ret = 0;
    do {
      var reqEncodedJson = new JsonDataTAG("");
      ret = this.comm_MBC_Engine.encodeData(reqCommMBCPackage, reqEncodedJson);
      if (ret != 0) {
        break;
      }
      //console.log("[CommMBCHttpClient::send] req: " + reqEncodedJson.data);
      var resEncodedJson = new JsonDataTAG("");
      ret = this.postData(reqEncodedJson, resEncodedJson, isAsync);
      if (ret != 0) {
        break;
      }
      //console.log("[CommMBCHttpClient::send] res: " + resEncodedJson.data);
      ret = this.comm_MBC_Engine.decodeData(resEncodedJson, resCommMBCPackage);
      if (ret != 0) {
        break;
      }

      return 0;
    } while (0);
    return -1;

  }

  postData(reqJson, resJson, isAsync) {
    var ret = 0;
    resJson.data = "";
    do {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          resJson.data = this.responseText;
          ret = 0;
        }
      };
      xhttp.open("POST", SERVER_API_URL_SETTING, isAsync);
      xhttp.setRequestHeader("Content-type", "application/json");
      ret = -1;
      xhttp.send(reqJson.data);
      if (ret != 0) {
        break;
      }
      return 0;
    } while (0);
    return -1;
  }
}