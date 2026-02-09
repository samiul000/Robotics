<img src="/smart_lock_system/pictures/smart_lock_sys.jpg" alt="Smart Lock" align="center" width="300" height="350"/>

## Introduction
Smart Lock is a security lock system with three-way unlocking methods.
```
- HTTP endpoint
- Fingerprint authentication
- PIN via keypad
```
## Components used:
```
1. ESP32
2. Adafruit fingerprint sensor
3. 4×4 matrix keypad
4. 0.96-inch OLED display
5. 5V 2-Relay Module
6. LM2596 DC-DC buck converter
7. 12V UPS module
8. 3S 18650 battery holder (modified to 2P, as the UPS module supports 3.7V–4.2V)
9. DC power adapter module
```
## Setup
<img src="/smart_lock_system/pictures/setup.jpg" alt="Setup" align="center" width="500" height="500"/>

Main diagram of the initial setup can be found in the *smart_lock_system_diagram.pdf* file.

***Note: UPS Module was added later in the system between main power supply, battery holder and buck converter which is not included in the diagram as I could not find any fritzing file of the module.***

## Code
<img src="/smart_lock_system/pictures/code_snippet.png" alt="Code Snippet" align="center" width="350" height="550"/>

## Webpage
<img src="/smart_lock_system/pictures/webpage.jpg" alt="Webpage" align="center" width="350" height="550"/>

User can *unlock* and *lock* using the webpage locally hosted on the ESP32 by entering the pin, setup using the keypad.

## System Overview

This system provides two main menus for user interaction: A-Menu (Fingerprint Options) and B-Menu (PIN Options).
The menus can be accessed using the keypad.

- Press `A` to open *A-Menu*

- Press `B` to open *B-Menu*

### A-Menu: Fingerprint Options
```
1. Unl   - Unlock the system using a fingerprint scan
2. Count - Show the total number of registered fingerprints
3. Del   - Delete a fingerprint using its ID
4. Add   - Add a new fingerprint
```

### Fingerprint Registration

- The user must scan the same fingerprint twice for accuracy.

- After successful scanning, the system assigns a unique ID to the fingerprint.

- This ID is required to delete the fingerprint later.

### Fingerprint Deletion

- The user must enter the fingerprint ID.

- The system deletes the fingerprint associated with the provided ID.

### B-Menu: PIN Options
```
1. Unl   - Unlock the system using the keypad
2. Chng  - Change the existing PIN
3. AP    - Display the webpage access point (AP) address
4. Reset - Reset the entire system
```

### PIN Change

- The user must enter the current PIN.

- If the PIN is correct, the system allows the user to set a new PIN.

- If the PIN is incorrect, the request is denied.

### System Startup Behavior

- On startup, the system checks for an existing configuration file.

- The configuration file contains saved PIN and fingerprint data.

- If no configuration is found, the system prompts the user to complete the initial setup using the menus


