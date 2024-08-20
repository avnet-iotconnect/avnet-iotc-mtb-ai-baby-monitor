## Introduction

This demo project is the integration of  
Infineon's [Machine learning: Machine learning: Imagimob MTBML deployment](https://github.com/Infineon/mtb-example-ml-imagimob-mtbml-deploy/tree/release-v1.1.0)
and Avnet's [IoTConnect ModusToolbox&trade; Basic Sample](https://github.com/avnet-iotconnect/avnet-iotc-mtb-basic-example/tree/release-v6.0.0). It demonstrates the ModusToolbox&trade; Machine Learning (MTBML) flow algorithm with "baby cry" model detection by using data from the pulse-density modulation to pulse-code modulation (PDM/PCM) which is sent to the model to detect whether a baby is crying or not.
The AI model's recognized class is then reported to IoTConnect.

This project currently supports the [CY8CKIT-062S2-AI](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-062s2-ai/)
The project supports only Eclipse with GCC_ARM compiler.

## QuickStart Guide

To quickly evaluate this project without compiling code follow the step-by-step instructions in the [QuickStart Guide](QUICKSTART.md).

## Building the Project

[Watch an overview video](https://saleshosted.z13.web.core.windows.net/media/ifx/videos/IFX%20Modus%20with%20IoTConnect.mp4) of creating a new project with IoTConnect in ModusToolbox&trade; then follow the steps below.

To build the project, please refer to the 
[IoTConnect ModusToolbox&trade; Basic Sample Developer Guide](https://github.com/avnet-iotconnect/avnet-iotc-mtb-basic-example/tree/release-v6.0.0/DEVELOPER_GUIDE.md) and note the following:
- Once ModusToolbox has been installed, the [ModusToolbox&trade; for Machine Learning](https://softwaretools.infineon.com/tools/com.ifx.tb.tool.modustoolboxpackmachinelearning) software should be installed as well.
- If using the model generator, you will need to install QEMU and set up the relevant environment variables per the [Machine Learning User Guide](https://www.infineon.com/dgdl/Infineon-Infineon-ModusToolbox_Machine_Learning_User_Guide-UserManual-v02_00-EN-UserManual-v09_00-EN.pdf?fileId=8ac78c8c83cd308101840de7e95a09df)
- Over-the-air updates are not currently supported.
- Use the [psoc6aibm-device-template.json Device Template](files/psoc6aibm-device-template.json) instead of the Basic Sample's template.

## Running the Demo

Once the board connects to IoTConnect, it will start processing microphone input and attempt to detect a baby crying sound. 
This can be tested by placing the board next to the PC speakers and playing an audio file like [this one](https://www.youtube.com/watch?v=j3glwtXrj0c).

The application sends data once every second by default (see commands below), but the AI model recognizes sounds several times per second. 

If a baby cry s recognized, the **baby_cry_detected** attribute will be reported as *true* for at least 3 seconds after the actual detection.

The actual confidence percentage detected at the time of sending the telemetry data will be reported as **confidence_baby_cry**.

Note that due to the fact that *confidence_baby_cry* "lingers" as true for up to 3 seconds so we do not miss a report,
confidence value reported below the threshold may be reported along with confidence_baby_cry being *true*.
This behavior is intended.

One can also supply a smaller number than 1000 (milliseconds) as a parameter to the **set-reporting-interval** command 
at runtime to increase the reporting interval. In that case the confidence value will be more in sync.

The following commands can be sent to the device using the IoTConnect Web UI:

| Command                     | Argument Type     | Description                                                                                              |
|:----------------------------|-------------------|:---------------------------------------------------------------------------------------------------------|
| `board-user-led`            | String (on/off)   | Turn the board LED on or off                                                                             |
| `set-detection-threshold`   | Number (eg. 50)   | Set baby cry detection threshold (default 85) as percentage.                                             |
| `set-reporting-interval`    | Number (eg. 4000) | Set telemetry reporting interval in milliseconds.  By default, the application will report every 1000ms. |
| `demo-mode`                 | String (on/off)   | Enable demo mode. In this mode the application will send telemetry to IoTConnect for a longer period     |

## Additional Model Support

To evaluate other Infineon/Imagimob models with IoTConnect, use the links below:
* [Human Activity Detection (IMU model)](https://github.com/avnet-iotconnect/avnet-iotc-mtb-ai-imu-example)

## Support for Infineon "IoT sense expansion kit"

The project can be modified to support the [CY8CKIT-028-SENSE shield](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-028-sense/) and similar boards.
If introducing support for other boards, please make note that [.cyignore](.cyignore) currently ignores shield files and that CY_IGNORE logic in the makefile has no effect.

## Other IoTConnect-enabled Infineon Kits
See the list [here](https://avnet-iotconnect.github.io/#infineon-technologies)
