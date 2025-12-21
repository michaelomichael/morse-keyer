# morse-keyer

Dizzied by Dvorak? QWERTY too quick? Never fear, now you can focus carefully on every single character you type.

This firmware for ATmega32u4 (e.g. Arduino Leonardo) chips emulates a USB keyboard, and translates your morse code button presses into normal alphanumeric characters that are then sent to your computer as though you had typed them on a "real" keyboard.

It uses the ATmega32u4's built-in USB capabilities to simulate a keyboard using the Human Interface Device (HID) USB profile.

Simply attach a switch between arduino pin D10 and Gnd (no need no add a resistor), and plug the USB cable into your computer.

If you like, you can also attach a speaker or piezo element in series with a resistor (e.g. 150Ω or higher) between arduino pin D9 and Gnd.

## Typing

As is generally the case with morse code, interpretation of the button presses is time dependent.
It's probably best to monitor the serial output (see [Building](#building), below) initially so that you can get a feel for what's going on.

A short tap on the button will produce a dot. A longer tap will produce a dash. You probably knew that.

A short pause after releasing the button will be the trigger to interpret the dots and dashes that have been captured so far, convert them into an alphanumeric character, and send that character as a keypress to the computer.

You can then start inputting dots and dashes for the next letter, or you can first pause a little longer and a single space character will be sent to the computer.

The timings for all of these are [configurable](#configuring), and the built-in LED will flash to give you a visual indication of the "tick" speed.

### Supported Letters

Standard [morse code A-Z and 0-9](https://en.wikipedia.org/wiki/Morse_code#/media/File:International_Morse_Code.svg), plus:

| Morse sequence | Character emitted |
|----------------|-------------------|
| `• — • — • —`  | `.`               |
| `• • — — • •`  | `?`               |
| `• — — — — •`  | `'`               |
| `— • — • — —`  | `!`               |
| _Unrecognised_ | `%`               |

### Backspace

Continuing to hold the button down for a long-ish period (minimum 1,400 milliseconds by default) will cancel all dots and dashes captured since the last alphanumeric character was sent to the computer, and will also send a "backspace"
key press to the computer. If you continue to hold the button down, the device will send repeated "backspace" keys, similar to how it would work with the backspace key on a "real" computer keyboard.

Holding the button down like this is, I'm sure, frowned upon in real morse transmission, but I included it as a practicality.

When the key has been released after typing one or more backspaces, the device will wait forever for you to input the next dot/dash.

Once a new word starts you can wait as long as you like before starting the next one.

## Building

I use the [PlatformIO plugin for Visual Studio Code](https://docs.platformio.org/en/latest/integration/ide/vscode.html). You may also need to install the Arduino [IDE](https://www.arduino.cc/en/software/#ide) or [CLI](https://www.arduino.cc/en/software/#ide) to get the required standard Arduino libraries.

Once you have that, it should just be a matter of plugging in your device, opening the project in VS Code, selecting the "leonardo" environment, and choosing the `PlatformIO: Upload and Monitor` command.

You can run the tests by selecting the "native" environment and running the `PlatformIO: Test` command. This lets the tests run locally on your own computer.

## Configuring

Settings are:

| Name                         | Type    | Description                                                                                                                                                                   | Default value |
|------------------------------|---------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|---------------|
| `loggingEnabled`             | Boolean | Whether to print debug messages on the serial port                                                                                                                            | true          |
| `tickDurationMillis`         | Integer | Number of milliseconds that constitute a "tick" unit for the various timing parameters below. Use this as an overall "speed" control.                                         | 70            |
| `debounceMillis`             | Integer | Number of milliseconds to wait for the button state to [settle](https://en.wikipedia.org/wiki/Switch#Contact_bounce) each time it is pressed or released.                                                                                | 50            |
| `ticksBeforeNewLetter`       | Float   | Number of ticks to wait after the last 'release' before deciding to move to interpret the morse captured so far and type the resulting letter on the keyboard.                | 7             |
| `ticksBeforeNewWord`         | Float   | Number of ticks to wait after we moved to a new letter before deciding to type a space character.                                                                             | 8             |
| `ticksBeforeDash`            | Float   | Minimum duration (in ticks) that the button needs to remain pressed in order to constitute a "dash" rather than a "dot".                                                      | 2.5           |
| `ticksBeforeFirstBackspace`  | Float   | Minimum duration (in ticks) that the button needs to remain pressed in order to type a "backspace".                                                                           | 20            |
| `ticksBeforeSecondBackspace` | Float   | Amount of time (in ticks) that the button needs to remain pressed in order to type a second "backspace", measured from the time that the first "backspace" was typed.         | 13            |
| `ticksBeforeRepeatBackspace` | Float   | Amount of time (in ticks) that the button needs to remain pressed in order to type each subsequent "backspace", measured from the time that the second "backspace" was typed. | 4             |
| `toneEnabled`                | Boolean | Turns the speaker on or off (if connected).                                                                                                                                   | true          |
| `toneVolumePercent`          | Integer | Maximum volume of the tone played by the speaker. (Not currently used.)                                                                                                       | 50            |
| `toneFrequencyHertz`         | Integer | Frequency of the tone played by the speaker.                                                                                                                                  | 440           |
| `keyboardEnabled`            | Boolean | Whether to send keyboard events to the USB host computer.                                                                                                                     | true          |
| `keyboardLayout`             | Integer | Choose between 0 (QWERTY) and 1 (Dvorak).                                                                                                                                     | 0             |

You can configure these settings by sending the following commands using the PlatformIO/Arduino serial monitor, with rate=115,200 baud and line ending=linefeed (`\n`).

Changes should take effect immediately, and are stored in the device's EEPROM so they will persist when the power is switched off.

### List All Settings

Send:

```text
list
```

Example response:

```text
OK: 12
loggingEnabled true
tickDurationMillis 70
debounceMillis 10
ticksBeforeNewLetter 7
ticksBeforeNewWord 8
ticksBeforeDash 2.5
ticksBeforeFirstBackspace 20
ticksBeforeSecondBackspace 13
ticksBeforeRepeatBackspace 7
toneEnabled true
toneVolumePercent 50
toneFrequencyHertz 1220
```

### Set a Single Value

Send a line in the from `set <setting-name> <setting-value>`, e.g.:

```text
set tickDurationMillis 100
```

or:

```text
set ticksBeforeDash 2.5
```

or:

```text
set toneEnabled false
```

The response in each case should be:

```text
OK
```

## Startup

If `toneEnabled` is `true`, you'll hear two quick beeps when the device is first connected to power.

Holding the button down while powering the device up will toggle the `toneEnabled` setting, and persist the new value in EEPROM. When you subsequently release the button, the beeps (or lack thereof) will indicate the new value of this setting.
