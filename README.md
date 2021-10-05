# PowerProf Firmware

PowerProf is a current monitor for embedded systems. You can use it to get accurate current consumption data for any workload or operating conditions, and then use that information to design battery-based systems with just the right amount of capacity.

There are currently three supported microcontrollers - the Teensy 3.6, the LOLIN D1 Mini, and the LOLIN D32. The Teensy version supports communication over USB, the LOLIN D1 Mini supports USB and WiFi, and the LOLIN D32 supports USB, WiFi, and BLE. All boards use the [INA260 current sensor](https://www.ti.com/lit/ds/symlink/ina260.pdf) from Texas Instruments.

This repostiory contains firmware for your PowerProf. It is built using [Visual Studio Code](https://code.visualstudio.com/) and [PlatformIO](https://platformio.org/).

To change which device you are targeting, click the environment selector along the bottom bar of VS Code. Change it from "Default" to "teensy," "d1_mini," or "d32" based on the device you are working with.

To build the firmware, simply invoke the "PlatformIO - Build" command. To upload the firmware, invoke the "PlatformIO - Upload" command with your device connected via USB.

## Known Issues

- No WiFi support yet
- No BLE support yet
- Sample rate option in "setup" command is ignored
