
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
        case COMM_MBC_MESS_ID_SYSTEMSETTING:
          ret = this.encodeSystemSetting(commMBCPackage.body, jsonBody);
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
        case COMM_MBC_MESS_ID_SYSTEMSETTING:
          resCommMBCPackage.body = new CommMBCSystemSettingTAG();
          ret = this.decodeSystemSetting(jsonPaser, resCommMBCPackage.body);
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
      console.log("[CommMBCHttpClient::send] req: " + reqEncodedJson.data);
      var resEncodedJson = new JsonDataTAG("");
      ret = this.postData(reqEncodedJson, resEncodedJson, isAsync);
      if (ret != 0) {
        break;
      }
      console.log("[CommMBCHttpClient::send] res: " + resEncodedJson.data);
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