#ifndef _FEM_MAIN_CPP
#define _FEM_MAIN_CPP
/////////////////////////////////////////////////
// INCLUDE
#include "main.h"
#include "task_opcode/TaskOpcode.h"
#include "task_main/MainControllerFactory.h"
#include "file_system/FileSystemManager.h"
#include "db/DBManager.h"
#include "timer_manager/TimerManager.h"
#include "settings/SettingManager.h"
#include "task_ui/UiManager.h"
#include "task_buzzer/BuzzerManager.h"
#include "task_led/LedManager.h"
#include "task_switch/SwitchManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#define MAIN_CONSOLE_DEBUG_ENABLE
//#define MAIN_DEBUG_RESET_MODE
//#define MAIN_DEBUG_RESET_DB
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
//#ifdef MAIN_CONSOLE_DEBUG_ENABLE
#define MAIN_CONSOLE_DEBUG_BUF_LEN                48
char mainBufLog[MAIN_CONSOLE_DEBUG_BUF_LEN];
//#endif // MAIN_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// STATIC VARIABLES
static MainController* mainController = NULL;
/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// PROTYPES

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/////////////////////////////////////////////////
// CLASS IMPLEMENTATION
/*main*/
int main(void)
{
  int ret = 0;
  SysMode_t systemMode = SYS_MODE_NORMAL;
  do
  {
#ifdef _CONF_SYSTEM_CONSOLE_LOG_ENABLED
    // initialize debug serial communication 
    SYSTEM_CONSOLE_HANDLER.begin(FEM_DEBUG_SERIAL_BAUDRATE);
    while (!SYSTEM_CONSOLE_HANDLER)
    {
      ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
    }
    SYSTEM_SLEEP(FEM_DEBUG_SERIAL_INIT_WAIT);
#endif // _CONF_SYSTEM_CONSOLE_LOG_ENABLED

    CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Initializing...");
    //######################Timer manager: start##########################
    if (TimerManager::getInstance().isValid() == false)
    {
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "TM starting");
      TaskManagerConfigTAG taskConfig = TaskManagerConfigTAG();
      taskConfig.eventUsePool = FEM_TM_USE_POOL;
      taskConfig.eventItemNumber = FEM_TM_TASK_EVENT_NO; // TIMER_MANAGER_TEST_TASK_EVENT_NO;
      TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
      threadConfig.enabled = true;
      threadConfig.useThreadPool = false;
      threadConfig.usStackDepth = FEM_TM_TASK_MEM;
      threadConfig.uxPriority = FEM_TM_TASK_PRIORITY;
      {
        TimerControlTaskConfigTAG timerConfig = TimerControlTaskConfigTAG();
        timerConfig.taskManagerConfig = &taskConfig;
        timerConfig.taskThreadConfig = &threadConfig;
        timerConfig.hwTimerGrpId = FEM_TM_HW_GROUP_ID;
        timerConfig.hwTimerIndex = FEM_TM_HW_INDEX_ID;
        timerConfig.hwTimerDevider = FEM_TM_HW_DEVIDER;
        timerConfig.hwClockMHz = SYSTEM_TIME_BASE_FREQ / 1000000;
        ret = TimerManager::getInstance().initialize(timerConfig);
        if (ret != 0)
        {
          CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "TM init failed");
          break;
        }
      }
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "TM running");
    }

    //######################File system: start##########################
    if (FileSystemManager::getInstance().isValid() == false)
    {
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "FS starting");
      FileSystemConfigTAG fsConfig = FileSystemConfigTAG();
      //memcpy(fsConfig.basePath, FEM_FS_BASE_PATH, SYSTEM_MIN(strlen(FEM_FS_BASE_PATH), FS_MGR_BASE_PATH_LEN_MAX));
      fsConfig.basePath = FEM_FS_BASE_PATH;
      fsConfig.partitionLabel = FEM_FS_PARTITION_LABEL;
      fsConfig.formatIfFailed = FEM_FS_FORMAT;
      fsConfig.maxOpenFile = FEM_FS_MAX_FILE;
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set bp=%s", fsConfig.basePath);
      ret = FileSystemManager::getInstance().initialize(fsConfig);
      if (ret != 0)
      {
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "FS init failed");
        break;
      }
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "FS running");
    }

    //######################DB: start##########################
    {
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "DB starting");
      DBManagerConfigTAG dbManagerConfig = DBManagerConfigTAG();
      dbManagerConfig.dbPath = FEM_DB_PATH;
      dbManagerConfig.dbTableName = g_FEM_Db_Tbl_Names;
      dbManagerConfig.dbTableIdMax = FEM_DB_TBL_ID_MAX;

      //FileSystemManager::getInstance().deleteFile(FEM_DB_PATH);
      bool existingDB = FileSystemManager::getInstance().exist(FEM_DB_PATH);
      ret = DBManager::getInstance().initialize(dbManagerConfig);
      if (ret != 0)
      {
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "DB init failed");
        break;
      }
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "DB running");

      if (existingDB == false)
      {
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Not found db, generating..");
        char* tmpSqlCmdBuf = new char[DB_SCRIPT_LINE_LEN_MAX];
        ret = DBManager::getInstance().resetDB(g_FEM_Db_Tbl_Scripts, FEM_DB_TBL_ID_MAX, tmpSqlCmdBuf, DB_SCRIPT_LINE_LEN_MAX);
        delete[] tmpSqlCmdBuf;
        tmpSqlCmdBuf = NULL;
        if (ret != 0)
        {
          CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "DB generating failed");
          break;
        }
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "DB generating completed.");
      }
    }

    //######################Setting: load##########################
    if (SettingManager::getInstance().isValid() == false)
    {
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Setting starting");
      SettingManagerConfigTAG settingConfig = SettingManagerConfigTAG();
      settingConfig.reserved = 0;
      ret = SettingManager::getInstance().initialize(settingConfig);
      if (ret != 0)
      {
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Setting init failed");
        break;
      }
      
      ret = SettingManager::getInstance().load();
      if (ret != 0)
      {
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Setting fload failed");
        break;
      }
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Setting loaded");

      
      /*CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %i %s", 1, SettingManager::getInstance().scanner().codePrefix());

      SettingManager::getInstance().scanner().codePrefix("new", 3, true);
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %i %s", 2, SettingManager::getInstance().scanner().codePrefix());
      SettingManager::getInstance().scanner().load();
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %i %s", 3, SettingManager::getInstance().scanner().codePrefix());*/
    }
    //######################BUZZER: start##########################
    if (BuzzerManager::getInstance().isValid() == false)
    {
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Buzzer starting");
      BuzzerDeviceConfigTAG buzzerDeviceConfig[FEM_BUZZER_DEVICE_COUNT];
      buzzerDeviceConfig[0].deviceId = FEM_BUZZER_DEVICE_0_DEV_ID;
      buzzerDeviceConfig[0].pin = FEM_BUZZER_DEVICE_0_PIN;
      buzzerDeviceConfig[0].hwChannelId = FEM_BUZZER_DEVICE_0_HW_CHANNEL;
      
      BuzzerManagerConfigTAG buzzerMgrConfig = BuzzerManagerConfigTAG();
      buzzerMgrConfig.controllerConfig.reserved = 0;
      buzzerMgrConfig.deviceCount = FEM_BUZZER_DEVICE_COUNT;
      buzzerMgrConfig.deviceConfig = buzzerDeviceConfig;

      ret = BuzzerManager::getInstance().start(buzzerMgrConfig);
      //BuzzerDeviceManager::getInstance().dump();
      if (ret != 0)
      {
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Buzzer start failed");
        break;
      }
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Buzzer running");
    }
    //######################LED: start##########################
    if (LedManager::getInstance().isValid() == false)
    {
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Led starting");
      LedDeviceConfigTAG ledDeviceConfig[FEM_LED_DEVICE_COUNT];
      ledDeviceConfig[0].channelId = FEM_LED_DEVICE_0_CHANNEL_ID;
      ledDeviceConfig[0].pin = FEM_LED_DEVICE_0_PIN;
      ledDeviceConfig[0].defaultIntensity = FEM_LED_DEVICE_0_DEFAULT_INTENSITY;
      ledDeviceConfig[0].isPWM = FEM_LED_DEVICE_0_IS_PWM;
      ledDeviceConfig[1].channelId = FEM_LED_DEVICE_1_CHANNEL_ID;
      ledDeviceConfig[1].pin = FEM_LED_DEVICE_1_PIN;
      ledDeviceConfig[1].defaultIntensity = FEM_LED_DEVICE_1_DEFAULT_INTENSITY;
      ledDeviceConfig[1].isPWM = FEM_LED_DEVICE_1_IS_PWM;
      ledDeviceConfig[2].channelId = FEM_LED_DEVICE_2_CHANNEL_ID;
      ledDeviceConfig[2].pin = FEM_LED_DEVICE_2_PIN;
      ledDeviceConfig[2].defaultIntensity = FEM_LED_DEVICE_2_DEFAULT_INTENSITY;
      ledDeviceConfig[2].isPWM = FEM_LED_DEVICE_2_IS_PWM;

      LedManagerConfigTAG ledMgrConfig = LedManagerConfigTAG();
      ledMgrConfig.controllerConfig.reserved = 0;
      ledMgrConfig.deviceCount = FEM_LED_DEVICE_COUNT;
      ledMgrConfig.deviceConfig = ledDeviceConfig;

      ret = LedManager::getInstance().start(ledMgrConfig);
      if (ret != 0)
      {
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Led start failed");
        break;
      }
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Led running");
    }
    ////######################UI: start##########################
    if (UiManager::getInstance().isValid() == false)
    {
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "UI starting");
      UiManager::getInstance().regUIMessageId(UIConstant::UIMessageId::UiMessRaw);
      UiManager::getInstance().regUIMessageId(UIConstant::UIMessageId::UiMessMessage);
      UiManager::getInstance().regUIMessageId(UIConstant::UIMessageId::UiMessSysState);

      UiManagerConfigTAG uiConfig = UiManagerConfigTAG();
      uiConfig.dpProcType = FEM_UI_CONTROL_TYPE;
      uiConfig.bufferConfig.dataItemConfig.bufferSize = FEM_UI_DM_BUFF_SIZE;
      uiConfig.bufferConfig.preparedDataNumber = FEM_UI_DM_BUFF_COUNT;
      uiConfig.bufferConfig.usePool = FEM_UI_DM_USE_POOL;

      uiConfig.taskManagerConfig.eventItemNumber = FEM_UI_EM_EVENT_NUM;
      uiConfig.taskManagerConfig.eventUsePool = FEM_UI_EM_USE_POOL;

      uiConfig.taskThreadConfig.enabled = true;
      uiConfig.taskThreadConfig.useThreadPool = FEM_UI_TM_USE_POOL;
      uiConfig.taskThreadConfig.usStackDepth = FEM_UI_TM_MEM;
      uiConfig.taskThreadConfig.uxPriority = FEM_UI_TM_PRIORITY;

      uiConfig.dpProcConfig.deviceConfig.deviceType = FEM_UI_DEV_DEVICE_TYPE;
      uiConfig.dpProcConfig.deviceConfig.columnNo = FEM_UI_DEV_DEVICE_COL;
      uiConfig.dpProcConfig.deviceConfig.rowNo = FEM_UI_DEV_DEVICE_ROW;
      uiConfig.dpProcConfig.deviceConfig.id = FEM_UI_DEV_DEVICE_ID;
      uiConfig.dpProcConfig.deviceConfig.spec.serialConsole.instanceIndex = FEM_UI_DEV_DEVICE_SC_INSTANCE_IDX;
      uiConfig.dpProcConfig.deviceConfig.spec.serialConsole.baudrate = FEM_UI_DEV_DEVICE_SC_BAUDRATE;
      uiConfig.dpProcConfig.deviceConfig.spec.serialConsole.bitPerByte = FEM_UI_DEV_DEVICE_SC_BIT_PER_BYTE;
      uiConfig.dpProcConfig.deviceConfig.spec.serialConsole.parityType = FEM_UI_DEV_DEVICE_SC_PARITY_TYPE;
      uiConfig.dpProcConfig.deviceConfig.spec.serialConsole.stopBitNum = FEM_UI_DEV_DEVICE_SC_STOP_BIT;
      uiConfig.dpProcConfig.deviceConfig.spec.serialConsole.timeout = FEM_UI_DEV_DEVICE_SC_TIMEOUT;
      uiConfig.dpProcConfig.deviceConfig.spec.serialConsole.pin_RX = FEM_UI_DEV_DEVICE_SC_PIN_RX;
      uiConfig.dpProcConfig.deviceConfig.spec.serialConsole.pin_TX = FEM_UI_DEV_DEVICE_SC_PIN_TX;

      ret = UiManager::getInstance().startTask(uiConfig);
      if (ret != 0)
      {
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "UI starting failed");
        break;
      }
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "UI running");
      
      UiManager::getInstance().showSysState(UI_MESS_SYS_STATE_BOOTING, UI_MESS_SYS_STATE_SUB_START);
    }

    //######################SW menu: start##########################
    {
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "SysMode getting");
      //set menu
      SwitchManagerConfigTAG swConfig = SwitchManagerConfigTAG();
      SwitchItemTAG swItems[FEM_SW_BUTTON_COUNT];
      memset(&swItems, 0, FEM_SW_BUTTON_COUNT*sizeof(SwitchItemTAG));
      swItems[0].pin = FEM_PIN_SCAN_BUTTON;
      swItems[0].gpioFunc = FEM_GPIO_FUNC_SCAN_BUTTON;
      swItems[0].triggerType = FEM_ISR_TYPE_SYSMODE_SCAN_BUTTON;
      swItems[0].opCode = FEM_OPCODE_SCAN_BUTTON;
      
      swItems[1].pin = FEM_PIN_RESET_BUTTON;
      swItems[1].gpioFunc = FEM_GPIO_FUNC_RESET_BUTTON;
      swItems[1].triggerType = FEM_ISR_TYPE_SYSMODE_RESET_BUTTON;
      swItems[1].opCode = FEM_OPCODE_RESET_BUTTON;
      
      swConfig.switchConfig.bounceTime = FEM_SW_BUTTON_BOUNCE_TIME;
      swConfig.switchConfig.switchItems = swItems;
      swConfig.switchConfig.switchItemCount = FEM_SW_BUTTON_COUNT;
      ret = SwitchManager::getInstance().start(swConfig);
      if (ret != 0)
      {
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "SW menu setup failed");
        break;
      }

      // get sw mode
#ifdef MAIN_DEBUG_RESET_MODE
      systemMode = SystemMode::ResetMode;
#else // MAIN_DEBUG_RESET_MODE
      SysMode_t lastSystemMode = SettingManager::getInstance().system().lastSysMode();
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set pre mode %d", lastSystemMode);
      // clear last sysmode
      SettingManager::getInstance().system().lastSysMode(SYS_MODE_INVALID, true);
      if ((systemMode == SYS_MODE_NORMAL)
        && (lastSystemMode < SYS_MODE_INVALID)
        )
      {
        systemMode = lastSystemMode;
      }
      else
      {
        systemMode = SwitchManager::getInstance().getSystemMode();
      }
#endif // MAIN_DEBUG_RESET_MODE
      if (systemMode >= SYS_MODE_INVALID)
      {
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "System mode getting failed");
        break;
      }
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set SysMode=%i", systemMode);

      //@@SwitchManager::getInstance().stop();
    }

    // generate main controller
    //######################Main controller: start##########################
    {
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Controller starting");
      mainController = MainControllerFactory::generate(systemMode);
      if (mainController == NULL)
      {
        CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Controller creating failed");
        break;
      }

      // start
      {
        MainControllerConfigTAG mainConfig = MainControllerConfigTAG();
        mainConfig.bufferConfig.dataItemConfig.bufferSize = FEM_MAIN_DM_BUFF_SIZE;
        mainConfig.bufferConfig.preparedDataNumber = FEM_MAIN_DM_BUFF_COUNT;
        mainConfig.bufferConfig.usePool = FEM_MAIN_DM_USE_POOL;

        mainConfig.taskManagerConfig.eventItemNumber = FEM_MAIN_EM_EVENT_NUM;
        mainConfig.taskManagerConfig.eventUsePool = FEM_MAIN_EM_USE_POOL;

        mainConfig.taskThreadConfig.enabled = true;
        mainConfig.taskThreadConfig.useThreadPool = FEM_MAIN_TM_USE_POOL;
        mainConfig.taskThreadConfig.usStackDepth = FEM_MAIN_TM_MEM;
        mainConfig.taskThreadConfig.uxPriority = FEM_MAIN_TM_PRIORITY;
        ret = mainController->start(mainConfig);
        if (ret != 0)
        {
          CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Controller start failed");
          break;
        }
      }
      CONSOLE_LOG_BUF(mainBufLog, MAIN_CONSOLE_DEBUG_BUF_LEN, "[m] set %s", "Controller running");
    }
    //######################Completed##########################
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] set %s", "System running");
    return 0;
  } while (0);

  CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] set %s", "Set up failed");
  UiManager::getInstance().showSysState(UI_MESS_SYS_STATE_BOOTING, UI_MESS_SYS_STATE_SUB_ERROR);
  return -1;
}

#endif // _FEM_MAIN_CPP