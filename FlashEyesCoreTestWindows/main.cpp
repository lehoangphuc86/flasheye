// FlashEyesCoreWindowsTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "../FlashEyesCoreTestArduino/src/FlashEyeCoreTestConfig.h"
#include <iostream>
#ifdef __CONF_TEST_TASK_MANAGER_FUNC_ENABLED
#include "../FlashEyesCoreTestArduino/src/task_manager/TaskManagerTestMain.h"

TaskManagerTest testObject;
#endif // __CONF_TEST_TASK_MANAGER_FUNC_ENABLED


#ifdef __CONF_TEST_TASK_COMM_SERIAL_FUNC_ENABLED
#include "../FlashEyesCoreTestArduino/src/task_comm/task_comm_serial/TaskCommSerialTestMain.h"

TaskCommSerialTest testObject;
#endif // __CONF_TEST_TASK_COMM_SERIAL_FUNC_ENABLED

#ifdef __CONF_TEST_TASK_COMM_MBC_SERIAL_FUNC_ENABLED
#include "../FlashEyesCoreTestArduino/src/task_comm_mbc/task_comm_mbc_serial/TaskCommMBCSerialTestMain.h"

TaskCommMBCSerialTest testObject;
#endif // __CONF_TEST_TASK_COMM_MBC_SERIAL_FUNC_ENABLED

#ifdef __CONF_TEST_TIMER_MANAGER_FUNC_ENABLED
#include "../FlashEyesCoreTestArduino/src/timer_manager/TimerManagerTestMain.h"

TimerManagerMainTest testObject;
#endif // __CONF_TEST_TIMER_MANAGER_FUNC_ENABLED

#include "../FlashEyesCoreLib/src/utility/Utility.h"


int main()
{
  int ret = 0;
  std::cout << "[main] Hello World!\n";
  do
  {
    ret = testObject.setup();
    if (ret != 0)
    {
      break;
    }

    testObject.loop();

    std::cout << "[main] Bye World!\n";
    int input = std::getchar();
    input++;
    return 0;
  } while (0);

  std::cout << "[main] Error!\n";
  int input = std::getchar();
  input++;
  return -1;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
