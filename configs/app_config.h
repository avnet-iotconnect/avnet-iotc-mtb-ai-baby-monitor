//
// Copyright: Avnet, Softweb Inc. 2020
// Modified by Nik Markovic <nikola.markovic@avnet.com> on 6/15/20.
//

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "iotconnect.h"

#define IOTCONNECT_CPID "your-cpid"
#define IOTCONNECT_ENV  "your-env"

// from iotconnect.h IotConnectAuthType
#define IOTCONNECT_AUTH_TYPE IOTC_AT_X509

#define IOTCONNECT_DUID_PREFIX "psoc6-"

#define IOTCONNECT_CONNECTION_TYPE IOTC_CT_UNDEFINED

/*
 PEM format certificate and private key
Example:
#define IOTCONNECT_DEVICE_CERT \
"-----BEGIN CERTIFICATE-----\n" \
".... base64 encoded certificate ..."\
"-----END CERTIFICATE-----"
#define IOTCONNECT_DEVICE_KEY \
"-----BEGIN PRIVATE KEY-----\n" \
".... base64 encoded private key ..."\
"-----END PRIVATE KEY-----"
Leave certificate and private key blank if  you wish to use EMEEPROM data to automatically generate the certificate
and use the runtime configuration for all of the configurable values in this file.
IMPORTANT NOTE: If you use the EMEEPROM runtime configuration,
the certificate and private key will be regenerated when you re-flash the board!
In that case, you would need to delete and re-create your device in IoTConnect.
*/
#define IOTCONNECT_DEVICE_CERT ""
#define IOTCONNECT_DEVICE_KEY ""

// you can choose to use your own NTP server to obtain network time, or simply time.google.com for better stability
#define IOTCONNECT_SNTP_SERVER "pool.ntp.org"


#endif // APP_CONFIG_H
