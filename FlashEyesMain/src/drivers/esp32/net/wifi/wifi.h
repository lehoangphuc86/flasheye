#ifndef _DRIVER_WIFI_ESP32_H
#define _DRIVER_WIFI_ESP32_H

/////////////////////////////////////////////////
// INCLUDE
#include "esp_event.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_event.h"
//
//#include <string.h>
//#include "esp_event.h"
//#include "esp_log.h"
//#include "esp_system.h"
//#include "esp_wifi.h"
//#include "freertos/FreeRTOS.h"
//#include "freertos/event_groups.h"
//#include "freertos/task.h"
//#include "nvs_flash.h"
//
//#include "lwip/err.h"
//#include "lwip/sys.h"
#include "esp_netif.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define WIFI_DRV_RET_OK                                 ESP_OK
#define WIFI_DRV_HANDLER_INVALID                        NULL
#define WIFI_DRV_SSID_MAX_LEN                           32
#define WIFI_DRV_PWD_MAX_LEN                            64
#define WIFI_DRV_AUTH_MODE_OPEN                         WIFI_AUTH_OPEN
#define WIFI_DRV_AUTH_MODE_WEP                          WIFI_AUTH_WEP
#define WIFI_DRV_AUTH_MODE_WPA_PSK                      WIFI_AUTH_WPA_PSK
#define WIFI_DRV_AUTH_MODE_WPA2_PSK                     WIFI_AUTH_WPA2_PSK
#define WIFI_DRV_AUTH_MODE_WPA_WPA2_PSK                 WIFI_AUTH_WPA_WPA2_PSK
#define WIFI_DRV_AUTH_MODE_WPA2_ENTERPRISE              WIFI_AUTH_WPA2_ENTERPRISE
#define WIFI_DRV_AUTH_MODE_WPA3_PSK                     WIFI_AUTH_WPA3_PSK
#define WIFI_DRV_AUTH_MODE_WPA2_WPA3_PSK                WIFI_AUTH_WPA2_WPA3_PSK
#define WIFI_DRV_AUTH_MODE_WAPI_PSK                     WIFI_AUTH_WAPI_PSK
#define WIFI_DRV_AUTH_MODE_INVALID                      WIFI_AUTH_MAX

#define WIFI_DRV_MODE_AP                                WIFI_MODE_AP
#define WIFI_DRV_MODE_STA                               WIFI_MODE_STA
#define WIFI_DRV_MODE_APSTA                             WIFI_MODE_APSTA
#define WIFI_DRV_MODE_INVALID                           WIFI_MODE_NULL

#define NET_IP4_INVALID                                 0
/////////////////////////////////////////////////
// MARCO
#define WIFI_MAKE_CONFIG_DEFAULT(config)            \
do { \
    config.event_handler = &esp_event_send_internal; \
    config.osi_funcs = &g_wifi_osi_funcs; \
    config.wpa_crypto_funcs = g_wifi_default_wpa_crypto_funcs; \
    config.static_rx_buf_num = CONFIG_ESP32_WIFI_STATIC_RX_BUFFER_NUM;\
    config.dynamic_rx_buf_num = CONFIG_ESP32_WIFI_DYNAMIC_RX_BUFFER_NUM;\
    config.tx_buf_type = CONFIG_ESP32_WIFI_TX_BUFFER_TYPE;\
    config.static_tx_buf_num = WIFI_STATIC_TX_BUFFER_NUM;\
    config.dynamic_tx_buf_num = WIFI_DYNAMIC_TX_BUFFER_NUM;\
    config.cache_tx_buf_num = WIFI_CACHE_TX_BUFFER_NUM;\
    config.csi_enable = WIFI_CSI_ENABLED;\
    config.ampdu_rx_enable = WIFI_AMPDU_RX_ENABLED;\
    config.ampdu_tx_enable = WIFI_AMPDU_TX_ENABLED;\
    config.amsdu_tx_enable = WIFI_AMSDU_TX_ENABLED;\
    config.nvs_enable = WIFI_NVS_ENABLED;\
    config.nano_enable = WIFI_NANO_FORMAT_ENABLED;\
    config.rx_ba_win = WIFI_DEFAULT_RX_BA_WIN;\
    config.wifi_task_core_id = WIFI_TASK_CORE_ID;\
    config.beacon_max_len = WIFI_SOFTAP_BEACON_MAX_LEN; \
    config.mgmt_sbuf_num = WIFI_MGMT_SBUF_NUM; \
    config.feature_caps = g_wifi_feature_caps; \
    config.sta_disconnected_pm = WIFI_STA_DISCONNECTED_PM_ENABLED;  \
    config.magic = WIFI_INIT_CONFIG_MAGIC;\
} while (0)

#define NET_IPV4_ADDR_GET_BYTE(ipaddr, idx)         (((const uint8_t*)(&ipaddr))[idx])
#define NET_IPV4_ADDR1(ipaddr)                      NET_IPV4_ADDR_GET_BYTE(ipaddr, 0)
#define NET_IPV4_ADDR2(ipaddr)                      NET_IPV4_ADDR_GET_BYTE(ipaddr, 1)
#define NET_IPV4_ADDR3(ipaddr)                      NET_IPV4_ADDR_GET_BYTE(ipaddr, 2)
#define NET_IPV4_ADDR4(ipaddr)                      NET_IPV4_ADDR_GET_BYTE(ipaddr, 3)

#define NET_IP4_ADD1_8(ipAdd)                       ((uint8_t)NET_IPV4_ADDR1(ipAdd))
#define NET_IP4_ADD2_8(ipAdd)                       ((uint8_t)NET_IPV4_ADDR2(ipAdd))
#define NET_IP4_ADD3_8(ipAdd)                       ((uint8_t)NET_IPV4_ADDR3(ipAdd))
#define NET_IP4_ADD4_8(ipAdd)                       ((uint8_t)NET_IPV4_ADDR4(ipAdd))

#define NET_IP4_INIT(a,b,c,d)                       ESP_IP4TOADDR(a, b, c, d)

#define NET_IP4_FORMAT                              "%d.%d.%d.%d"
#define NET_IP4_2_STR(ipaddr)                       NET_IP4_ADD1_8(ipaddr), \
                                                    NET_IP4_ADD2_8(ipaddr), \
                                                    NET_IP4_ADD3_8(ipaddr), \
                                                    NET_IP4_ADD4_8(ipaddr)
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef esp_err_t                                   WifiRet_T;
typedef esp_netif_t*                                WifiNetHandler_t;
typedef esp_event_handler_instance_t*               WifiEventHandlerInsance_t;
typedef byte                                        WifiMode_t;
typedef uint8_t                                     WifiConnId_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS
static wifi_auth_mode_t wifiDrvAuthModeConvert(byte appAuthMode)
{
  switch (appAuthMode)
  {
  case WIFI_DRV_AUTH_MODE_OPEN:
    return WIFI_AUTH_OPEN;
  case WIFI_DRV_AUTH_MODE_WEP:
    return WIFI_AUTH_WEP;
  case WIFI_DRV_AUTH_MODE_WPA_PSK:
    return WIFI_AUTH_WPA_PSK;
  case WIFI_DRV_AUTH_MODE_WPA2_PSK:
    return WIFI_AUTH_WPA2_PSK;
  case WIFI_DRV_AUTH_MODE_WPA_WPA2_PSK:
    return WIFI_AUTH_WPA_WPA2_PSK;
  case WIFI_DRV_AUTH_MODE_WPA2_ENTERPRISE:
    return WIFI_AUTH_WPA2_ENTERPRISE;
  case WIFI_DRV_AUTH_MODE_WPA3_PSK:
    return WIFI_AUTH_WPA3_PSK;
  case WIFI_DRV_AUTH_MODE_WPA2_WPA3_PSK:
    return WIFI_AUTH_WPA2_WPA3_PSK;
  case WIFI_DRV_AUTH_MODE_WAPI_PSK:
    return WIFI_AUTH_WAPI_PSK;
  case WIFI_DRV_AUTH_MODE_INVALID:
  default:
    return WIFI_AUTH_MAX;
    break;
  }
}

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION



#endif // _DRIVER_WIFI_ESP32_H