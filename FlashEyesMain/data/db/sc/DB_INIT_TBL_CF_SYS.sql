-- ################# SYSTEM SETTING#############
DROP TABLE IF EXISTS TBL_CF_SYS;
CREATE TABLE TBL_CF_SYS (Id INTEGER, Type INTEGER NOT NULL, Value TEXT NOT NULL);
-- @Type: 1 - NUMERIC, 2 - String with null terminator
-- @Id: FEM_SET_DB_KEY_ID_*
-- @Value: Number or string with null terminator
-- last_Sys_Mode
INSERT INTO TBL_CF_SYS (Id, Type, Value) VALUES(1, 1, 3);
-- wifi_Mode
INSERT INTO TBL_CF_SYS (Id, Type, Value) VALUES(2, 1, 0);
-- wifi_SSID
INSERT INTO TBL_CF_SYS (Id, Type, Value) VALUES(3, 2, 'SamsungJ7');
-- wifi_Pwd
INSERT INTO TBL_CF_SYS (Id, Type, Value) VALUES(4, 2, 'donotconnect');
-- http_Cli_Uri
INSERT INTO TBL_CF_SYS (Id, Type, Value) VALUES(5, 2, 'http://192.168.4.2:3000/scanRes');
-- http_Cli_Data_Type
INSERT INTO TBL_CF_SYS (Id, Type, Value) VALUES(6, 1, 7);
-- http_Cli_Method
INSERT INTO TBL_CF_SYS (Id, Type, Value) VALUES(7, 1, 2);
-- export_Mode
INSERT INTO TBL_CF_SYS (Id, Type, Value) VALUES(8, 1, 0);