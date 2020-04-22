#pragma once 

void cecKeyUp(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecKeyDown(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecSetMenuLanguage(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecStandBy(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecActiveSource(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecRequestActiveSource(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecSetStreamPath(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecRoutingChange(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecReportPowerStatus(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecGiveDeviceVendorID(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecDeviceVendorID(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecGivePowerStatus(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecGiveOSDName(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecReportPhysAddr(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecGivePhysAddr(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecVendorCommand(const uint8_t st, const uint8_t* data, const uint8_t len);
void cecGiveDeckStatus(const uint8_t st, const uint8_t* data, const uint8_t len);

void cecSpy(const uint8_t st, const uint8_t* data, const uint8_t len);


/**
 * Kodi: Logical Address 0x1, Physical Address 0x1000 (1.0.0.0)
 * // Discover
 *    [0x00 > 0x01 a: 0x83]                       // rxu   Give Physical Address
 *    [0x01 > 0x0F n: 0x84, 0x10 0x00 0x01]       // txb   Report Physical Address    Physical Address (0x1000)
 *    [0x00 > 0x01 a: 0x46]                       // rxu   Give OSD Name
 *    [0x01 > 0x00 a: 0x47, 0x4B 0x6F 0x64 0x69]  // txu   Set OSD Name               'K','o','d','i'
 * // Selecting Kodi
 *    [0x00 > 0x0F n: 0x80, 0x10 0x00 0x10 0x00]  // rxb   Routing Change             Original Address (0x1000), New Address(0x1000)
 *    [0x01 > 0x00 a: 0x04]                       // txu   Image View On
 *    [0x00 > 0x01 a: 0x8F]                       // rxu   Give Device Power Status
 *    [0x01 > 0x0F n: 0x82, 0x10 0x00]            // txb   Set Active Source          Physical Address (0x1000)
 *    [0x01 > 0x00 a: 0x90, 0x00]                 // txu   Report Power Status        ON
 *    [0x00 > 0x0F n: 0x86, 0x10 0x00]            // rxb   Set Stream Path            Physical Address (0x1000)
 *    [0x01 > 0x0F n: 0x82, 0x10 0x00]            // txb   Set Active Source          Physical Address (0x1000)
 *    [0x00 > 0x01 a: 0x8F]                       // rxu   Give Device Power Status
 *    [0x01 > 0x00 a: 0x90, 0x00]                 // txu   Report Power Status        ON
 * // Keys for selecting a movie and playing
 *    [0x00 > 0x01 a: 0x44, 0x00]                 // rxu   Pressing key               Select
 *    [0x00 > 0x01 a: 0x45]                       // rxu   Releasing key
 *    [0x00 > 0x01 a: 0x44, 0x02]                 // rxu   Pressing key               Down
 *    [0x00 > 0x01 a: 0x45]
 *    [0x00 > 0x01 a: 0x44, 0x02]
 *    [0x00 > 0x01 a: 0x45]
 *    [0x00 > 0x01 a: 0x44, 0x02]
 *    [0x00 > 0x01 a: 0x45]
 *    [0x00 > 0x01 a: 0x44, 0x00]
 *    [0x00 > 0x01 a: 0x45]
 *    [0x00 > 0x01 a: 0x44, 0x02]
 *    [0x00 > 0x01 a: 0x45]
 *    [0x00 > 0x01 a: 0x44, 0x00]
 *    [0x00 > 0x01 a: 0x45]
 * // Kodi going to powersave => Powering Off the TV
 *    [0x01 > 0x00 a: 0x36]                       // Standby
 *    [0x00 > 0x0F n: 0x36]
 */

/**
 * Grundig TV remote
 *   Key              CEC code      Kodi Keyboard
 *    1               0x21          KEYPAD_1
 *    2               0x22          KEYPAD_2
 *    3               0x23          KEYPAD_3
 *    4               0x24          KEYPAD_4
 *    5               0x25          KEYPAD_5
 *    6               0x26          KEYPAD_6
 *    7               0x27          KEYPAD_7
 *    8               0x28          KEYPAD_8
 *    9               0x29          KEYPAD_9
 *    0               0x20          KEYPAD_0
 *    Repeat          0x09          KEY_W
 *    EPG             0x53          KEY_C
 *    P+              0x30          KEY_PAGE_UP
 *    P-              0x31          KEY_PAGE_DOWN
 *    Up              0x01          KEY_UP
 *    Down            0x02          KEY_DOWN
 *    Left            0x03          KEY_LEFT
 *    Right           0x05          KEY_RIGHT
 *    Select          0x00          KEY_ENTER
 *    Back            0x0D          KEY_BACKSPACE
 *    Info            0x35          KEY_I
 *    Red/Play/Pause  0x44          KEY_SPACE
 *    Green/Rewind    0x48          KEY_R
 *    Yellow/Stop     0x45          KEY_X
 *    Blue/Forward    0x49          KEY_F
 *    Txt             0x4c          KEY_TAB
 *    Rec             0x4b          KEY_PRINTSCREEN
 *
 */

/**
 * LG ..Smart.. TV
 * 
 * 
 * 
 */
