#ifndef _DRIVER_HTTP_ESP32_H
#define _DRIVER_HTTP_ESP32_H

/////////////////////////////////////////////////
// INCLUDE
#include <string.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include "esp_http_server.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define HTTP_HANDLER_INVALID                        NULL
#define HTTP_RET_OK                                 ESP_OK
#define HTTP_RET_ERR                                ESP_FAIL
/////////////////////////////////////////////////
// MARCO
#define HTTP_SERVER_DEFAULT_CONFIG(config)              \
{                                                       \
        config.task_priority      = tskIDLE_PRIORITY+5; \
        config.stack_size         = 4096;               \
        config.core_id            = tskNO_AFFINITY;     \
        config.server_port        = 80;                 \
        config.ctrl_port          = 32768;              \
        config.max_open_sockets   = 7;                  \
        config.max_uri_handlers   = 8;                  \
        config.max_resp_headers   = 8;                  \
        config.backlog_conn       = 5;                  \
        config.lru_purge_enable   = false;              \
        config.recv_wait_timeout  = 5;                  \
        config.send_wait_timeout  = 5;                  \
        config.global_user_ctx = NULL;                  \
        config.global_user_ctx_free_fn = NULL;          \
        config.global_transport_ctx = NULL;             \
        config.global_transport_ctx_free_fn = NULL;     \
        config.open_fn = NULL;                          \
        config.close_fn = NULL;                         \
        config.uri_match_fn = NULL;                     \
}
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef esp_http_client_handle_t                    HttpClientHandler_t;
typedef httpd_handle_t                              HttpServerHandler_t;
typedef esp_err_t                                   HttpRet_t;
typedef esp_http_client_event_t                     HttpClientEvent_t;
typedef httpd_req_t                                 HttpReq_t;
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


#endif // _DRIVER_HTTP_ESP32_H