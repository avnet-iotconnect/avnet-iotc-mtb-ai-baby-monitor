/*******************************************************************************
* Copyright 2020-2021, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/
/* SPDX-License-Identifier: MIT
 * Copyright (C) 2024 Avnet
 * Authors: Nikola Markovic <nikola.markovic@avnet.com>, Shu Liu <shu.liu@avnet.com> et al.
 */
#include "cyhal.h"
#include "cybsp.h"

/* FreeRTOS header files */
#include "FreeRTOS.h"
#include "task.h"

/* Configuration file for Wi-Fi and MQTT client */
#include "wifi_config.h"

/* Middleware libraries */
#include "cy_retarget_io.h"
//#include "clock.h"

/* LwIP header files */
#include "lwip/netif.h"
#include "lwip/apps/sntp.h"

#include "iotconnect.h"
#include "iotc_mtb_time.h"


// AI model related:
#include "mtb_ml_utils.h"
#include "mtb_ml_common.h"
#include "imu.h"
#include "audio.h"
#include "inference.h"
#include "config.h"
#include "imu_model.h"
#include "pdm_model.h"
#include MTB_ML_INCLUDE_MODEL_FILE(MODEL_NAME)


// App related
#include "iotc_gencert.h"
#include "app_eeprom_data.h"
#include "app_config.h"
#include "app_task.h"


#define APP_VERSION "03.01.00"

#define LABEL_BABY_CRY 	1
#define LABEL_UNLABELED	0


typedef enum UserInputYnStatus {
	APP_INPUT_NONE = 0,
	APP_INPUT_YES,
	APP_INPUT_NO
} UserInputYnStatus;

static UserInputYnStatus user_input_status = APP_INPUT_NONE;
static TaskHandle_t model_task_handle = NULL;

// AI Model related values --------------
// NOTE: The ML code logic in inference.c does not seem to be decoupled from what would be "application logic".
// One could say that it is actually an application, but there is some knowledge embedded in it that we should be able to refer to instead.
// We have to duplicate some things locally and hack unfortunately.

// Globals:
volatile bool pdm_pcm_flag;
volatile bool imu_flag;

/* Model information */
static mtb_ml_model_t *imagimob_model_obj;

/* Output/result buffers for the inference engine */
static MTB_ML_DATA_T *result_buffer;

/* Model Output Size */
static int model_output_size;

#if INFERENCE_MODE_SELECT == IMU_INFERENCE
static const char* LABELS[IMAI_IMU_DATA_OUT_COUNT] = IMAI_IMU_SYMBOL_MAP;
#else
static const char* LABELS[IMAI_PDM_DATA_OUT_COUNT] = IMAI_PDM_SYMBOL_MAP;
#endif
// --------------

// Telemetry-related app variables:
// There is a small chance that label and value will not be atomic as combined As this is a demo only,
// for sake of simplicity, we will not be synchronizing tasks to solve for this issues.
static bool is_baby_cry_detected = false;
static float confidence_baby_cry = 0;
static unsigned int highest_confidence_timestamp = 0; // when was the value recorded. Used in the logic to "hold" the value.

static bool is_demo_mode = false;
static int reporting_interval = 1000;
static int detection_threshold = 85;

static void on_connection_status(IotConnectConnectionStatus status) {
    // Add your own status handling
    switch (status) {
        case IOTC_CS_MQTT_CONNECTED:
            printf("IoTConnect Client Connected notification.\n");
            break;
        case IOTC_CS_MQTT_DISCONNECTED:
            printf("IoTConnect Client Disconnected notification.\n");
            break;
        default:
            printf("IoTConnect Client ERROR notification\n");
            break;
    }
}

/******************************************************************************
 * Function Name: wifi_connect
 ******************************************************************************
 * Summary:
 *  Function that initiates connection to the Wi-Fi Access Point using the
 *  specified SSID and PASSWORD. The connection is retried a maximum of
 *  'MAX_WIFI_CONN_RETRIES' times with interval of 'WIFI_CONN_RETRY_INTERVAL_MS'
 *  milliseconds.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  cy_rslt_t : CY_RSLT_SUCCESS upon a successful Wi-Fi connection, else an
 *              error code indicating the failure.
 *
 ******************************************************************************/
static cy_rslt_t wifi_connect(void) {
    cy_rslt_t result = CY_RSLT_SUCCESS;
    cy_wcm_connect_params_t connect_param;
    cy_wcm_ip_address_t ip_address;
    const char* wifi_ssid = app_eeprom_data_get_wifi_ssid(WIFI_SSID);
    const char* wifi_pass = app_eeprom_data_get_wifi_pass(WIFI_PASSWORD);

    /* Check if Wi-Fi connection is already established. */
    if (cy_wcm_is_connected_to_ap() == 0) {
        /* Configure the connection parameters for the Wi-Fi interface. */
        memset(&connect_param, 0, sizeof(cy_wcm_connect_params_t));
        memcpy(connect_param.ap_credentials.SSID, wifi_ssid, strlen(wifi_ssid));
        memcpy(connect_param.ap_credentials.password, wifi_pass, strlen(wifi_pass));
        connect_param.ap_credentials.security = WIFI_SECURITY;

        printf("Connecting to Wi-Fi AP '%s'\n", connect_param.ap_credentials.SSID);

        /* Connect to the Wi-Fi AP. */
        for (uint32_t retry_count = 0; retry_count < MAX_WIFI_CONN_RETRIES; retry_count++) {
            result = cy_wcm_connect_ap(&connect_param, &ip_address);

            if (result == CY_RSLT_SUCCESS) {
                printf("\nSuccessfully connected to Wi-Fi network '%s'.\n", connect_param.ap_credentials.SSID);

                /* Set the appropriate bit in the status_flag to denote
                 * successful Wi-Fi connection, print the assigned IP address.
                 */
                if (ip_address.version == CY_WCM_IP_VER_V4) {
                    printf("IPv4 Address Assigned: %s\n", ip4addr_ntoa((const ip4_addr_t*) &ip_address.ip.v4));
                } else if (ip_address.version == CY_WCM_IP_VER_V6) {
                    printf("IPv6 Address Assigned: %s\n", ip6addr_ntoa((const ip6_addr_t*) &ip_address.ip.v6));
                }
                return result;
            }

            printf("Connection to Wi-Fi network failed with error code 0x%0X. Retrying in %d ms. Retries left: %d\n",
                    (int) result, WIFI_CONN_RETRY_INTERVAL_MS, (int) (MAX_WIFI_CONN_RETRIES - retry_count - 1));
            vTaskDelay(pdMS_TO_TICKS(WIFI_CONN_RETRY_INTERVAL_MS));
        }

        printf("\nExceeded maximum Wi-Fi connection attempts!\n");
        printf("Wi-Fi connection failed after retrying for %d mins\n",
                (int) ((WIFI_CONN_RETRY_INTERVAL_MS * MAX_WIFI_CONN_RETRIES) / 60000u));
    }
    return result;
}

static void on_ota(IotclC2dEventData data) {
    const char *ota_host = iotcl_c2d_get_ota_url_hostname(data, 0);
    if (ota_host == NULL){
        printf("OTA host is invalid.\n");
        return;
    }
    const char *ota_path = iotcl_c2d_get_ota_url_resource(data, 0);
    if (ota_path == NULL) {
        printf("OTA resource is invalid.\n");
        return;
    }
    printf("OTA download request received for https://%s%s, but it is not implemented.\n", ota_host, ota_path);
}

// returns success on matching the expected format. Returns is_on, assuming "on" for true, "off" for false
static bool parse_on_off_command(const char* command, const char* name, bool *arg_parsing_success, bool *is_on, const char** message) {
    *arg_parsing_success = false;
    *message = NULL;
    size_t name_len = strlen(name);
    if (0 == strncmp(command, name, name_len)) {
        if (strlen(command) < name_len + 2) { // one for space and at least one character for the argument
            printf("ERROR: Expected command \"%s\" to have an argument\n", command);
            *message = "Command requires an argument";
            *arg_parsing_success = false;
        } else if (0 == strcmp(&command[name_len + 1], "on")) {
            *is_on = true;
            *message = "Value is now \"on\"";
            *arg_parsing_success = true;
        } else if (0 == strcmp(&command[name_len + 1], "off")) {
            *is_on = false;
            *message = "Value is now \"off\"";
            *arg_parsing_success = true;
        } else {
            *message = "Command argument";
            *arg_parsing_success = false;
        }
        // we matches the command
        return true;
    }

    // not our command
    return false;
}

static void on_command(IotclC2dEventData data) {
    const char * const BOARD_STATUS_LED = "board-user-led";
    const char * const DEMO_MODE_CMD = "demo-mode";
    const char * const QUALIFICATION_START_PREFIX_CMD = "aws-qualification-start "; // with a space
    const char * const SET_DETECTION_THRESHOLD = "set-detection-threshold "; // with a space
    const char * const SET_REPORTING_INTERVAL = "set-reporting-interval "; // with a space

    bool command_success = false;
    const char * message = NULL;

    const char *command = iotcl_c2d_get_command(data);
    const char *ack_id = iotcl_c2d_get_ack_id(data);

    if (command) {
        bool arg_parsing_success;
        printf("Command %s received with %s ACK ID\n", command, ack_id ? ack_id : "no");
        // could be a command without acknowledgment, so ackID can be null
        bool led_on;
        if (parse_on_off_command(command, BOARD_STATUS_LED, &arg_parsing_success, &led_on, &message)) {
            command_success = arg_parsing_success;
            if (arg_parsing_success) {
                cyhal_gpio_write(CYBSP_USER_LED, led_on);
            }
        } else if (parse_on_off_command(command, DEMO_MODE_CMD,  &arg_parsing_success, &is_demo_mode, &message)) {
            command_success = arg_parsing_success;
        } else if (0 == strncmp(QUALIFICATION_START_PREFIX_CMD, command, strlen(QUALIFICATION_START_PREFIX_CMD))) {
        	// Qualification has started. Stop the model task. See Makefile comments about enabling IOTC_AWS_DEVICE_QUALIFICATION
        	if (model_task_handle) {
        		printf("Stopping the model task to start AWS Device Qualification");
            	vTaskDelete(model_task_handle);
        	} else {
        		// Otherwise we will kill the app task with NULL being the argument
        		printf("Model task is NULL. No task stopped.");
        	}
        	// Get out early. Do not send the ack so we don't interfere with SDK taking over from here.
        	return;
        } else if (0 == strncmp(SET_REPORTING_INTERVAL, command, strlen(SET_REPORTING_INTERVAL))) {
        	int value = atoi(&command[strlen(SET_REPORTING_INTERVAL)]);
        	if (0 == value) {
                message = "Argument parsing error";
        	} else {
        		reporting_interval = value;
        		printf("Reporting interval set to %d\n", value);
        		message = "Reporting interval set";
        		command_success =  true;
        	}
        } else if (0 == strncmp(SET_DETECTION_THRESHOLD, command, strlen(SET_DETECTION_THRESHOLD))) {
        	int value = atoi(&command[strlen(SET_REPORTING_INTERVAL)]);
        	if (0 == value) {
                message = "Command argument parsing error";
        	} else {
        		detection_threshold = value;
        		printf("Detection threshold set to %d\n", value);
        		message = "Detection threshold set";
        		command_success =  true;
        	}
        } else {
            printf("Unknown command \"%s\"\n", command);
            message = "Unknown command";
        }
    } else {
        printf("Failed to parse command. Command or argument missing?\n");
        message = "Parsing error";
    }

    // could be a command without ack, so ack ID can be null
    // the user needs to enable acknowledgments in the template to get an ack ID
    if (ack_id) {
        iotcl_mqtt_send_cmd_ack(
                ack_id,
                command_success ? IOTCL_C2D_EVT_CMD_SUCCESS_WITH_ACK : IOTCL_C2D_EVT_CMD_FAILED,
                message // allowed to be null, but should not be null if failed, we'd hope
        );
    } else {
        // if we send an ack
        printf("Message status is %s. Message: %s\n", command_success ? "SUCCESS" : "FAILED", message ? message : "<none>");
    }
}

static cy_rslt_t publish_telemetry(void) {
    IotclMessageHandle msg = iotcl_telemetry_create();
    iotcl_telemetry_set_string(msg, "version", APP_VERSION);
    iotcl_telemetry_set_number(msg, "random", rand() % 100); // test some random numbers
    iotcl_telemetry_set_bool(msg, "baby_cry_detected", is_baby_cry_detected);
    iotcl_telemetry_set_number(msg, "confidence_baby_cry", confidence_baby_cry);

    iotcl_mqtt_send_telemetry(msg, false);
    iotcl_telemetry_destroy(msg);
    return CY_RSLT_SUCCESS;
}

static void app_inference_feed(float *processed_data) {
#if !COMPONENT_ML_FLOAT32
            /* Quantize data before feeding model */
            MTB_ML_DATA_T data_feed_int[PDM_BATCH_SIZE][PDM_NUM_AXIS];
            mtb_ml_utils_model_quantize(imagimob_model_obj, &processed_data[0][0], &data_feed_int[0][0]);

            /* Feed the Model */
            MTB_ML_DATA_T *input_reference = (MTB_ML_DATA_T *) data_feed_int;
            mtb_ml_model_run(imagimob_model_obj, input_reference);
            mtb_ml_utils_model_dequantize(imagimob_model_obj, results);

#else
            MTB_ML_DATA_T *input_reference = (MTB_ML_DATA_T *) processed_data;
            mtb_ml_model_run(imagimob_model_obj, input_reference);
#endif
}


static void app_model_output(void) {

#if !COMPONENT_ML_FLOAT32
    /* Convert 8-bit fixed-point output to floating-point for visualization */
    float *nn_float_buffer = (float *) pvPortMalloc(model_output_size * sizeof(float));
    mtb_ml_utils_model_dequantize(imagimob_model_obj, nn_float_buffer);
#else
    float *nn_float_buffer = result_buffer;
#endif

    if (!nn_float_buffer) {
        printf("Failed to allocate the model data buffer!\n");
        return;
    }

    // the previous algorithm was flexible to detect any class.
    // Keep "LABELS" around, so there's no compiler warning, in case if we ever return to it
    (void) LABELS;

    unsigned int time_now = xTaskGetTickCount() * portTICK_PERIOD_MS;
    confidence_baby_cry = 100.0f * nn_float_buffer[1];
    if (confidence_baby_cry > (float) detection_threshold) {
        printf("confidence: %3.1f%% (detected)\n", confidence_baby_cry);
    	is_baby_cry_detected = true;
    	highest_confidence_timestamp = time_now;
    } else {
        printf("confidence: %3.1f%%\n", confidence_baby_cry);
    	// case where we detected unlabeled, but had an actual detection previously....
    	// Wait until some time before resetting the value. We want to report the last actual detection for some time.

    	if (is_baby_cry_detected && ((time_now - highest_confidence_timestamp) > 3000)) {
    		// ^ really becomes "was_baby_cry_detected"
    		is_baby_cry_detected = false; // Enough time has passed, so reset the value
    	}
    }

#if !COMPONENT_ML_FLOAT32
    vPortFree(nn_float_buffer);
#endif
}


// Feed input data into the model (read from board sensors and write to the model)
static bool app_model_process(void) {

#if INFERENCE_MODE_SELECT == IMU_INFERENCE
    /* Store the processed data from IMAI_IMU_dequeue */
    float processed_data[IMAI_IMU_DATA_OUT_COUNT];
#else
    /* Store processed data from IMAI_dequeue */
    float processed_data[IMAI_PDM_DATA_OUT_COUNT];
#endif


#if INFERENCE_MODE_SELECT == IMU_INFERENCE
        /* Check if the IMU interrupt has triggered */
        if(true == imu_flag) {
            imu_flag = false;

            /* Check if there is enough pre-processed data to start
             * an inference. When there is enough data, processed_data
             * points to the processed IMU data */
            if (IMAI_IMU_RET_SUCCESS == IMAI_IMU_dequeue(processed_data)) {
                /* Feed the neural net the pre-processed data */
                app_inference_feed(processed_data);
                app_model_output();
                return true;
			}
			return false;
        }
#endif

#if INFERENCE_MODE_SELECT == PDM_INFERENCE
        /* Check if the PDM event has triggered */
        if(true == pdm_pcm_flag) {
            /* Check if there is enough pre-processed data to start
             * an inference. When there is enough data, processed_data
             * points to the processed PDM data */
            if(PDM_PROCESSING_COMPLETE == pdm_preprocessing_feed(processed_data)) {
                /* Feed the neural net the pre-processed data */
                app_inference_feed(processed_data);
                app_model_output();
            }
            /* Reset the pdm event flag */
            pdm_pcm_flag = false;
            return true;
        }
        return false;
#endif
}

static cy_rslt_t app_inference_init(void) {
    cy_rslt_t result;

    mtb_ml_model_bin_t imagimob_model_bin = {MTB_ML_MODEL_BIN_DATA(MODEL_NAME)};

    /* Initialize the Neural Network */
    result = mtb_ml_model_init(&imagimob_model_bin, NULL, &imagimob_model_obj);
    if(CY_RSLT_SUCCESS != result) {
        return result;
    }

    mtb_ml_model_get_output(imagimob_model_obj, &result_buffer, &model_output_size);

    return result;
}

static cy_rslt_t app_model_init(void) {
    cy_rslt_t result;

    /* Initialize the inference engine */
    result = app_inference_init();

    if(CY_RSLT_SUCCESS != result) {
        printf("Failed to inference_init()!\n");
        return result;
    }

#if INFERENCE_MODE_SELECT == IMU_INFERENCE
    /* Initialize Imagimob pre-processing library */
    IMAI_IMU_init();
    /* Start the imu, timer, and pre-process the data */
    result = imu_init();
#endif

#if INFERENCE_MODE_SELECT == PDM_INFERENCE
    /* Initialize Imagimob pre-processing library */
    IMAI_PDM_init();
    /* Configure PDM, PDM clocks, and PDM event */
    result = pdm_init();
#endif
    if(CY_RSLT_SUCCESS != result) {
        printf("Failed to init model input!\n");
    }
    return result;
}

static void app_model_task(void *pvParameters) {
    TaskHandle_t *parent_task = pvParameters;

    // Resume the application task. No longer needs to wait for us.
    xTaskNotifyGive(*parent_task);

    while (true) {
        // We guess that sensor data needs data to be passed to the model at 50hz (20ms period)
        // unsure about this....
        vTaskDelay(pdMS_TO_TICKS(20));
        app_model_process();
    }
    while (1) {
        taskYIELD();
    }
}

static void user_input_yn_task (void *pvParameters) {
	TaskHandle_t *parent_task = pvParameters;

	user_input_status = APP_INPUT_NONE;
    printf("Do you wish to configure the device?(y/[n]):\n>");

    int ch = getchar();
    if (EOF == ch) {
        printf("Got EOF?\n");
        goto done;
    }
    if (ch == 'y' || ch == 'Y') {
    	user_input_status = APP_INPUT_YES;
    } else {
    	user_input_status = APP_INPUT_NO;
    }
done:
	xTaskNotifyGive(*parent_task);
    while (1) {
		taskYIELD();
	}
}

void app_task(void *pvParameters) {
    (void) pvParameters;

    UBaseType_t my_priority = uxTaskPriorityGet(NULL);
    TaskHandle_t my_task = xTaskGetCurrentTaskHandle();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence to clear screen. */
    // printf("\x1b[2J\x1b[;H");
    printf("===============================================================\n");
    printf("Starting The App Task\n");
    printf("===============================================================\n\n");

    if (app_model_init()) {
    	// called function will print the error
        return;
    }

    uint64_t hwuid = Cy_SysLib_GetUniqueId();
    uint32_t hwuidhi = (uint32_t)(hwuid >> 32);
    // not using low bytes in the name because they appear to be the same across all boards of the same type
    // feel free to modify the application to use these bytes
    //uint32_t hwuidlo = (uint32_t)(hwuid & 0xFFFFFFFF);

    char iotc_duid[IOTCL_CONFIG_DUID_MAX_LEN];
    sprintf(iotc_duid, IOTCONNECT_DUID_PREFIX"%08lx", hwuidhi);

    printf("Generated device unique ID (DUID) is: %s\n", iotc_duid);

    if (app_eeprom_data_init()){
    	printf("App EEPROM data init failed!\n");
    }
    if (strlen(IOTCONNECT_DEVICE_CERT) > 0) {
    	printf("Using the compiled device certificate.\n");
    } else if (0 == strlen(app_eeprom_data_get_certificate(IOTCONNECT_DEVICE_CERT))) {
	    printf("\nThe board needs to be configured.\n");
	    app_eeprom_data_do_user_input(iotc_x509_generate_credentials);
    } else {
    	// else ask the user if they want to re configure the board. Wait some time for user input...
    	TaskHandle_t user_input_yn_task_handle;
        xTaskCreate(user_input_yn_task, "User Input", 1024, &my_task, (my_priority - 1), &user_input_yn_task_handle);
        ulTaskNotifyTake(pdTRUE, 4000);
        vTaskDelete(user_input_yn_task_handle);

        switch (user_input_status) {
        	case  APP_INPUT_NONE:
        	    printf("Timed out waiting for user input. Resuming...\n");
        	    break;
        	case  APP_INPUT_YES:
        	    app_eeprom_data_do_user_input(iotc_x509_generate_credentials);
        	    break;
        	default:
        	    printf("Bypassing device configuration.\n");
        	    break;
        }
    }

    IotConnectClientConfig config;
    iotconnect_sdk_init_config(&config);
    config.connection_type = app_eeprom_data_get_platform(IOTCONNECT_CONNECTION_TYPE);
    config.cpid = app_eeprom_data_get_cpid(IOTCONNECT_CPID);
    config.env =  app_eeprom_data_get_env(IOTCONNECT_ENV);
    config.duid = iotc_duid;
    config.qos = 1;
    config.verbose = true;
    config.x509_config.device_cert = app_eeprom_data_get_certificate(IOTCONNECT_DEVICE_CERT);
    config.x509_config.device_key = app_eeprom_data_get_private_key(IOTCONNECT_DEVICE_KEY);
    config.callbacks.status_cb = on_connection_status;
    config.callbacks.cmd_cb = on_command;
    config.callbacks.ota_cb = on_ota;

    const char * conn_type_str = "(UNKNOWN)";
    if (config.connection_type == IOTC_CT_AWS) {
        conn_type_str = "AWS";
    } else if(config.connection_type == IOTC_CT_AZURE) {
        conn_type_str = "Azure";
    }

    printf("Current Settings:\n");
    printf("Platform: %s\n", conn_type_str);
    printf("DUID: %s\n", config.duid);
    printf("CPID: %s\n", config.cpid);
    printf("ENV: %s\n", config.env);
    printf("WiFi SSID: %s\n", app_eeprom_data_get_wifi_ssid(WIFI_SSID));
    printf("Device certificate:\n%s\n", app_eeprom_data_get_certificate(IOTCONNECT_DEVICE_CERT));

    cy_wcm_config_t wcm_config = { .interface = CY_WCM_INTERFACE_TYPE_STA };
    if (CY_RSLT_SUCCESS != cy_wcm_init(&wcm_config)) {
        printf("Error: Wi-Fi Connection Manager initialization failed!\n");
        goto exit_cleanup;
    }

    printf("Wi-Fi Connection Manager initialized.\n");

    /* Initiate connection to the Wi-Fi AP and cleanup if the operation fails. */
    if (CY_RSLT_SUCCESS != wifi_connect()) {
        goto exit_cleanup;
    }

    if (0 != iotc_mtb_time_obtain(IOTCONNECT_SNTP_SERVER)) {
        // called function will print errors
        return;
    }

    cy_rslt_t ret = iotconnect_sdk_init(&config);
    if (CY_RSLT_SUCCESS != ret) {
        printf("Failed to initialize the IoTConnect SDK. Error code: %lu\n", ret);
        goto exit_cleanup;
    }

    cyhal_gpio_write(CYBSP_USER_LED, false); // USER_LED is active low

    for (int i = 0; i < 10; i++) {
        ret = iotconnect_sdk_connect();
        if (CY_RSLT_SUCCESS != ret) {
            printf("Failed to initialize the IoTConnect SDK. Error code: %lu\n", ret);
            goto exit_cleanup;
        }
        if (!model_task_handle) {
            xTaskCreate(app_model_task, "Model Task", 1024 * 8, &my_task, my_priority + 1, &model_task_handle);
            ulTaskNotifyTake(pdTRUE, 10000);
        }
        int max_messages = is_demo_mode ? 6000 : 300;
        for (int j = 0; iotconnect_sdk_is_connected() && j < max_messages; j++) {
            cy_rslt_t result = publish_telemetry();
            if (result != CY_RSLT_SUCCESS) {
                break;
            }
            iotconnect_sdk_poll_inbound_mq(reporting_interval);
        }
        iotconnect_sdk_disconnect();
    }
    iotconnect_sdk_deinit();

    printf("\nAppTask Done.\n");
    while (1) {
        taskYIELD();
    }
    return;

    exit_cleanup:
    printf("\nError encountered. AppTask Done.\n");
    while (1) {
        taskYIELD();
    }
}
