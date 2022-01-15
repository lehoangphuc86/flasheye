-- ################# SYSTEM SETTING#############
DROP TABLE IF EXISTS TBL_CF_SYS;
CREATE TABLE TBL_CF_SYS (Id INTEGER, Type INTEGER NOT NULL, Value TEXT NOT NULL);
-- @Type: 1 - NUMERIC, 2 - String with null terminator
-- @Id: FEM_SET_DB_KEY_ID_*
-- @Value: Number or string with null terminator
-- last_Sys_Mode
INSERT INTO TBL_CF_SYS (Id, Type, Value) VALUES(1, 1, 3);
