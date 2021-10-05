# PowerProf Firmware

PowerProf is a current monitor for embedded systems. You can use it to get accurate current consumption data for any workload or operating conditions, and then use that information to design battery-based systems with just the right amount of capacity.

There are currently three supported microcontrollers - the Teensy 3.6, the LOLIN D1 Mini, and the LOLIN D32. The Teensy version supports communication over USB, the LOLIN D1 Mini supports USB and WiFi, and the LOLIN D32 supports USB, WiFi, and BLE. All boards use the [INA260 current sensor](https://www.ti.com/lit/ds/symlink/ina260.pdf) from Texas Instruments.

This repostiory contains firmware for your PowerProf. It is built using [Visual Studio Code](https://code.visualstudio.com/) and [PlatformIO](https://platformio.org/).

To change which device you are targeting, click the environment selector along the bottom bar of VS Code. Change it from "Default" to "teensy," "d1_mini," or "d32" based on the device you are working with.

To build the firmware, simply invoke the "PlatformIO - Build" command. To upload the firmware, invoke the "PlatformIO - Upload" command with your device connected via USB.

## Architecture

The firmware is designed in a modular way, with each significant feature broken out as a separate library which is then consumed by the main source code.

### Blinkenlight

This module is responsible for controlling the onboard LED. It blinks the LED while streaming data.

### Bluetooth

This module is responsible for providing BLE connectivity on the LOLIN D32. It is not used by the Teensy and D1 Mini boards.

### MsgPack

This module is responsible for encoding and decoding messages to/from the [MsgPack](https://msgpack.org/index.html) format. Communications over the USB and WebSocket protocols are entirely encoded in MsgPack.

### Sensor

This module is responsible for interfacing with the INA260 current sensor.

### Main

The main module is responsible for bootstrapping the configured communications protocol, listening for connections, and streaming data when it is available from the INA260.

## Communication

PowerProf supports three communications protocols to provide ultimate flexibility to the end-user: USB, WiFi (WebSockets), and BLE. There are speed, range, and reliability trade-offs between these protocols. USB is the fastest and most reliable, but the least flexible because it requires a wired connection. WiFi sits in the middle - while it is wireless, it is slower and less reliable than USB. BLE is suitable only for lower-speed data acquisition, but is the most flexible protocol because it is wireless and works equally well with desktop and mobile clients.

### USB

When using USB as the communication protocol, the device waits for a serial connection after initializing itself. Once the connection is established, it waits for commands from the host.

### WiFi

When using WiFi as the communication protocol, the device starts up a WebSocket server listening on port 8080 of its public interface. When a connection is established, the device waits for commands from the client.

### BLE

Coming Soon (tm)

## Commands

Commands are sent as JSON objects encoded with MsgPack. An example command would be:

```json
{
  "command": "setup",
  "sampleRate": 500
}
```

This command would change the sampling rate to 500Hz.

There are currently three commands - `start`, `stop`, and `setup`. Only `setup` requires additional parameters.

## Known Issues

- WiFi/BLE are not working
- Sample rate option in "setup" command is ignored
