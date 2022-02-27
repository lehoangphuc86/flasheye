-- ################# SCANNER SETTING#############
DROP TABLE IF EXISTS TBL_CF_SCAN;
CREATE TABLE TBL_CF_SCAN (Id INTEGER, Type INTEGER NOT NULL, Value TEXT NOT NULL);
-- @Type: 1 - NUMERIC, 2 - String with null terminator
-- @Id: FEM_SET_DB_KEY_ID_*
-- @Value: Number or string with null terminator
-- comm_Baudrate
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(1, 1, 3);
-- comm_Parity
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(2, 1, 0);
-- comm_Stopbit
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(3, 1, 0);
-- comm_Databit
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(4, 1, 0);
-- comm_FlowCtrl
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(5, 1, 0);
-- character_Format
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(6, 1, 1);
-- read_State
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(7, 1, 1);
-- auto_Scan
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(8, 1, 0);
-- cont_Scan
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(9, 1, 0);
-- security_Level
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(10, 1, 0);
-- enable_Prefix
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(11, 1, 1);
-- custom_Prefix
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(12, 2, '');
-- custom_Suffix
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(13, 2, '');
-- custom_Code_Format
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(14, 1, 1);
-- mess_Terminator
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(15, 1, 3);
-- trigger_Mode
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(16, 1, 0);
-- dist_SS_Range_Begin
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(17, 1, 5);
-- dist_SS_Range_End
INSERT INTO TBL_CF_SCAN (Id, Type, Value) VALUES(18, 1, 15);