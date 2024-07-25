## Introduction

This demo project is the integration of the 
Infineon's [Machine learning: Machine learning: Imagimob MTBML deployment](https://github.com/Infineon/mtb-example-ml-imagimob-mtbml-deploy/tree/release-v1.1.0)
and Avnet's [IoTConnect Modus Toolbox&trade; Basic Sample](https://github.com/avnet-iotconnect/avnet-iotc-mtb-basic-example/tree/v6.0.0)
. It demonstrates the ModusToolbox&trade; Machine Learning (MTBML) flow algorithm withbaby crying detection by
using data from the pulse-density modulation to pulse-code modulation (PDM/PCM) which is sent to the model to detect whether a baby is crying or not.
The AI model's recognized class is then reported to IoTConnect.

This project currently supports the [CY8CKIT-062S2-AI](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-062s2-ai/)

The project supports only Eclipse with GCC_ARM compiler.

## Building the Project

To build the project, please refer to the 
[IoTConnect Modus Toolbox&trade; Basic Sample Developer Guide](https://github.com/avnet-iotconnect/avnet-iotc-mtb-basic-example/tree/release-v6.0.0/DEVELOPER_GUIDE.md) 
and note the following:
- Once Modus Toolbox has been installed, the 
    [ModusToolbox&trade; for Machine Learning](https://softwaretools.infineon.com/tools/com.ifx.tb.tool.modustoolboxpackmachinelearning) 
    software should be installed as well.
- If using the model generator, you will need to install QEMU and set up the relevant environment variables
    per [Machine Learning User Guide](https://www.infineon.com/dgdl/Infineon-Infineon-ModusToolbox_Machine_Learning_User_Guide-UserManual-v02_00-EN-UserManual-v09_00-EN.pdf?fileId=8ac78c8c83cd308101840de7e95a09df)
- OTA is not supported.
- Use the [psoc6aibc-device-template.json Device Template](files/psoc6aibc-device-template.json) instead of the Basic Sample's template.


## Running the Demo

Once the board connects to IoTConnect, it will start processing microphone input and attempt to detect a 
baby crying sound. This cab be tested by placing the board next to the PC speakers and
playing an audio file like [this one](https://www.youtube.com/watch?v=j3glwtXrj0c).

The application sends data once every second, but the AI model recognizes sounds several times
per second. If at the time reporting data to IoTConnect the sound is:
 * not recognized - the **class** value will be reported as **unlabelled**
 * recognised as baby crying - the **class** value will be reported as **baby_cry**

The model will report different **confidence** percentages for each case, and the application will report
the highest confidence class along with its confidence percentage.


## Other Supported Boards and IMU

The project  can be modified to support the original Infineon's project supported boards along with the 
[CY8CKIT-028-SENSE shield](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-028-sense/) and similar boards.
If introducing support for those boards, please make note that [.cyignore](.cyignore) currently ignores shield files and 
that CY_IGNORE logic in the makefile has no effect.

The original Infineon's [Machine learning: Imagimob model deployment](https://github.com/Infineon/mtb-example-ml-imagimob-deploy/tree/release-v1.1.0)
project supports both IMU and Audio (PDM) models. Only one model can be run at a time, but 
the user can generate a different set of model files. While the original Infineon's project 
comes with the precompiled IMU model with Human Activity Detection application, this project comes with the pre-generated
PDM model and application. This application is written to support only the PDM model, but can be 
modified to support both, 
similar to our [IoTConnect AI Human Activity Detection AI sample](https://github.com/avnet-iotconnect/avnet-iotc-mtb-ai-imu-example)

