#pragma once
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define CEC_PHYSADDR_INVALID                           0xFFFF

#define CEC_ADDR_TV                                       0x0
#define CEC_ADDR_RECORDER_1                               0x1
#define CEC_ADDR_RECORDER_2                               0x2
#define CEC_ADDR_TUNER_1                                  0x3
#define CEC_ADDR_PLAYBACK_1                               0x4
#define CEC_ADDR_AUDIO_SYS                                0x5
#define CEC_ADDR_TUNER_2                                  0x6
#define CEC_ADDR_TUNER_3                                  0x7
#define CEC_ADDR_PLAYBACK_2                               0x8
#define CEC_ADDR_RECORDER_3                               0x9
#define CEC_ADDR_TUNER4                                   0xA
#define CEC_ADDR_PLAYBACK_3                               0xB
#define CEC_ADDR_FREE_USE                                 0xE
#define CEC_ADDR_UNREGISTERED                             0xF
#define CEC_ADDR_BROADCAST                                0xF

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Dest can be:
// - u for Unicast only opcodes
// - b for Broadcast only opcodes
// - x for Both

//                                                    OPC   Dest       Arguments                          Expected reply
#define CEC_OPC_FEATURE_ABORT                         0x00  // u  [Opcode],[Reason]                       -
  #define CEC_OPC_ABORT_REASON_UNKNOWN_OPCODE               0x00
  #define CEC_OPC_ABORT_REASON_INCORRECT_MODE               0x01
  #define CEC_OPC_ABORT_REASON_SOURCE                       0x02
  #define CEC_OPC_ABORT_REASON_INVALID_OPERAND              0x03
  #define CEC_OPC_ABORT_REASON_REFUSED                      0x04
// 0x01 - 0x03
#define CEC_OPC_IMAGE_VIEW_ON                         0x04  // u  -                                       -
#define CEC_OPC_TUNER_STEP_INCREMENT                  0x05  // u  -                                       -
#define CEC_OPC_TUNER_STEP_DECREMENT                  0x06  // u  -                                       -
#define CEC_OPC_TUNER_DEVICE_STATUS                   0x07  // u  [Info]                                  -
#define CEC_OPC_GIVE_TUNER_DEVICE_STATUS              0x08  // u  -                                       0x08
#define CEC_OPC_RECORD_ON                             0x09  // u  [Source]                                0x0a
#define CEC_OPC_RECORD_STATUS                         0x0a  // u  [Status]                                -
#define CEC_OPC_RECORD_OFF                            0x0b  // u  -                                       -
// 0x0c
#define CEC_OPC_TEXT_VIEW_ON                          0x0d  // u  -                                       -
// 0x0e
#define CEC_OPC_RECORD_TV_SCREEN                      0x0f  // u  -                                       -
// 0x10 - 0x19
#define CEC_OPC_GIVE_DECK_STATUS                      0x1a  // u  [StatusRequest]                         0x1b
  #define CEC_OPC_STATUS_REQUEST_ON                         0x01
  #define CEC_OPC_STATUS_REQUEST_OFF                        0x02
  #define CEC_OPC_STATUS_REQUEST_ONCE                       0x03
#define CEC_OPC_DECK_STATUS                           0x1b  // u  -                                       -
  #define CEC_OPC_DECK_INFO_PLAY                            0x11
  #define CEC_OPC_DECK_INFO_RECORD                          0x12
  #define CEC_OPC_DECK_INFO_PLAY_REVERSE                    0x13
  #define CEC_OPC_DECK_INFO_STILL                           0x14
  #define CEC_OPC_DECK_INFO_SLOW                            0x15
  #define CEC_OPC_DECK_INFO_SLOW_REVERSE                    0x16
  #define CEC_OPC_DECK_INFO_FAST_FOWARD                     0x17
  #define CEC_OPC_DECK_INFO_FAST_REVERSE                    0x18
  #define CEC_OPC_DECK_INFO_NO_MEDIA                        0x19
  #define CEC_OPC_DECK_INFO_STOP                            0x1a
  #define CEC_OPC_DECK_INFO_WIND                            0x1b
  #define CEC_OPC_DECK_INFO_REWIND                          0x1c
  #define CEC_OPC_DECK_INFO_IDX_FORWARD                     0x1d
  #define CEC_OPC_DECK_INFO_IDX_REVERSE                     0x1e
  #define CEC_OPC_DECK_INFO_OTHER                           0x1f
// 0x1c - 0x31
#define CEC_OPC_SET_MENU_LANGUAGE                     0x32  // b  [Lang]                                  -
#define CEC_OPC_CLEAR_ANALOGUE_TIMER                  0x33  // u  [Day][Month][StartTime][Duration]       0x35
                                                            //    [Seq][Type][Freq][Sys]
#define CEC_OPC_SET_ANALOGUE_TIMER                    0x34  // u  [Day][Month][StartTime][Duration]       0x35
                                                            //    [Seq][Type][Freq][Sys]
#define CEC_OPC_TIMER_STATUS                          0x35  // u  [Status]                                -
#define CEC_OPC_STANDBY                               0x36  // x  -                                       -
// 0x37 - 0x3f
#define CEC_OPC_PLAY                                  0x41  // u  [Mode]                                  -
  #define CEC_OPC_PLAY_MODE_FAST                            0x00
  #define CEC_OPC_PLAY_MODE_MIN_SPEED                       0x01
  #define CEC_OPC_PLAY_MODE_MEDIUM_SPEED                    0x02
  #define CEC_OPC_PLAY_MODE_MAX_SPEED                       0x03
  #define CEC_OPC_PLAY_MODE_FORWARD                         0x04
  #define CEC_OPC_PLAY_MODE_FAST_FORWARD_MIN_SPEED          0x05
  #define CEC_OPC_PLAY_MODE_FAST_FORWARD_MEDIUM_SPEED       0x06
  #define CEC_OPC_PLAY_MODE_FAST_FORWARD_MAX_SPEED          0x07
  #define CEC_OPC_PLAY_MODE_REVERSE                         0x08
  #define CEC_OPC_PLAY_MODE_FAST_REVERSE_MIN_SPEED          0x09
  #define CEC_OPC_PLAY_MODE_FAST_REVERSE_MEDIUM_SPEED       0x0a
  #define CEC_OPC_PLAY_MODE_FAST_REVERSE_MAX_SPEED          0x0b
  #define CEC_OPC_PLAY_MODE_SLOW                            0x10
  #define CEC_OPC_PLAY_MODE_SLOW_FORWARD_MIN_SPEED          0x15
  #define CEC_OPC_PLAY_MODE_SLOW_FORWARD_MEDIUM_SPEED       0x16
  #define CEC_OPC_PLAY_MODE_SLOW_FORWARD_MAX_SPEED          0x17
  #define CEC_OPC_PLAY_MODE_SLOW_REVERSE_MIN_SPEED          0x19
  #define CEC_OPC_PLAY_MODE_SLOW_REVERSE_MEDIUM_SPEED       0x1a
  #define CEC_OPC_PLAY_MODE_SLOW_REVERSE_MAX_SPEED          0x1b
  #define CEC_OPC_PLAY_MODE_PLAY_REVERSE                    0x20
  #define CEC_OPC_PLAY_MODE_PLAY                            0x20
  #define CEC_OPC_PLAY_MODE_PLAY_FORWARD                    0x24
  #define CEC_OPC_PLAY_MODE_PLAY_STILL                      0x25
#define CEC_OPC_DECK_CONTROL                          0x42  // u  [Mode]                                  -
  #define CEC_OPC_DECK_CONTROL_MODE_SKIP_FORWARD            0x01
  #define CEC_OPC_DECK_CONTROL_MODE_SKIP_REVERSE            0x02
  #define CEC_OPC_DECK_CONTROL_MODE_STOP                    0x03
  #define CEC_OPC_DECK_CONTROL_MODE_EJECT                   0x04
#define CEC_OPC_TIMER_CLEARED_STATUS                  0x43  // u  -                                       -
#define CEC_OPC_USER_CONTROL_PRESSED                  0x44  // u  [UICommand]                             -
#define CEC_OPC_USER_CONTROL_RELEASED                 0x45  // u  -                                       -
#define CEC_OPC_GIVE_OSD_NAME                         0x46  // u  -                                       0x47
#define CEC_OPC_SET_OSD_NAME                          0x47  // u  [Name]                                  -
// 0x48 - 0x63
#define CEC_OPC_SET_OSD_STRING                        0x64  // u  [DispCtrl][String]                      -
  #define CEC_OPC_DISP_CONTROL_DEFAULT                      0x00
  #define CEC_OPC_DISP_CONTROL_CLEARED                      0x40
  #define CEC_OPC_DISP_CONTROL_CLEAR_PREV                   0x80
// 0x65 - 0x66
#define CEC_OPC_SET_TIMER_PROGRAM_TITLE               0x67  // u  [Title]                                 -
// 0x68 - 0x6f
#define CEC_OPC_SYSTEM_AUDIO_MODE_REQUEST             0x70  // u  [PhysAddr]                              -
#define CEC_OPC_GIVE_AUDIO_STATUS                     0x71  // u  -                                       0x7a
#define CEC_OPC_SET_SYSTEM_AUDIO_MODE                 0x72  // x  [Status]                                -
// 0x73 - 0x79
#define CEC_OPC_REPORT_AUDIO_STATUS                   0x7a  // u  [Status]                                -
// 0x7b - 0x7c
#define CEC_OPC_GIVE_SYSTEM_AUDIO_MODE_STATUS         0x7d  // u  -                                       -
#define CEC_OPC_SYSTEM_AUDIO_MODE_STATUS              0x7e  // u  [Status]                                -
// 0x7f
#define CEC_OPC_ROUTING_CHANGE                        0x80  // b  [OldPhysAddr][NewPhysAddr]              -
#define CEC_OPC_ROUTING_INFORMATION                   0x81  // b  [PhysAddr]                              -
#define CEC_OPC_ACTIVE_SOURCE                         0x82  // b  [PhysAddr]                              -
#define CEC_OPC_GIVE_PHYSICAL_ADDRESS                 0x83  // u  -                                       0x84
#define CEC_OPC_REPORT_PHYSICAL_ADDRESS               0x84  // b  [PhysAddr][DevType]                     -
  #define CEC_OPC_DEVICE_TYPE_TV                            0x00
  #define CEC_OPC_DEVICE_TYPE_RECORDER                      0x01
  #define CEC_OPC_DEVICE_TYPE_TUNER                         0x03
  #define CEC_OPC_DEVICE_TYPE_PLAYBACK                      0x04
  #define CEC_OPC_DEVICE_TYPE_AUDIO_SYSTEM                  0x05
#define CEC_OPC_REQUEST_ACTIVE_SOURCE                 0x85  // b  -                                       0x82
#define CEC_OPC_SET_STREAM_PATH                       0x86  // b  [PhysAddr]                              -
#define CEC_OPC_DEVICE_VENDOR_ID                      0x87  // b  [VendorID]                              -
// 0x88
#define CEC_OPC_VENDOR_COMMAND                        0x89  // u  [Data]                                  -
#define CEC_OPC_VENDOR_REMOTE_BUTTON_DOWN             0x8a  // x  [RC Code]                               -
#define CEC_OPC_VENDOR_REMOTE_BUTTON_UP               0x8b  // x  -                                       -
#define CEC_OPC_GIVE_DEVICE_VENDOR_ID                 0x8c  // u  -                                       0x87
#define CEC_OPC_MENU_REQUEST                          0x8d  // u  [Type]                                  0x8e
  #define CEC_OPC_MENU_REQUEST_TYPE_ACTIVATE                0x00
  #define CEC_OPC_MENU_REQUEST_TYPE_DEACTIVATE              0x01
  #define CEC_OPC_MENU_REQUEST_TYPE_QUERY                   0x02
#define CEC_OPC_MENU_STATUS                           0x8e  // u  [State]                                 -
  #define CEC_OPC_MENU_STATE_ACTIVATED                      0x00
  #define CEC_OPC_MENU_STATE_DEACTIVATED                    0x01
#define CEC_OPC_GIVE_DEVICE_POWER_STATUS              0x8f  // u  -                                       0x90
#define CEC_OPC_REPORT_POWER_STATUS                   0x90  // u  [Status]                                -
  #define CEC_OPC_POWER_STATUS_ON                           0x00
  #define CEC_OPC_POWER_STATUS_STANDBY                      0x01
  #define CEC_OPC_POWER_STATUS_STANDBY2ON                   0x02
  #define CEC_OPC_POWER_STATUS_ON2STANDBY                   0x03
#define CEC_OPC_GET_MENU_LANGUAGE                     0x91  // u  -                                       -
#define CEC_OPC_SELECT_ANALOGUE_SERVICE               0x92  // u  [Type][Freq][System]                    -
  #define CEC_OPC_ANALOG_TYPE_CABLE                         0x00
  #define CEC_OPC_ANALOG_TYPE_SATELLITE                     0x01
  #define CEC_OPC_ANALOG_TYPE_TERRESTRIAL                   0x02
#define CEC_OPC_SELECT_DIGITAL_SERVICE                0x93  // u  [ID]                                    -
// 0x94 - 0x96
#define CEC_OPC_SET_DIGITAL_TIMER                     0x97  // u  [Day][Month][StartTime][Duration]       0x35
                                                            //    [Seq][SrvId]
// 0x98
#define CEC_OPC_CLEAR_DIGITAL_TIMER                   0x99  // u  [Day][Month][StartTime][Duration]       0x35
                                                            //    [Seq][SrvId]
#define CEC_OPC_SET_AUDIO_RATE                        0x9a  // u  [AudioRate]                             -
  #define CEC_OPC_AUDIO_RATE_OFF                            0x00
  #define CEC_OPC_AUDIO_RATE_WIDE_STD                       0x01
  #define CEC_OPC_AUDIO_RATE_WIDE_FAST                      0x02
  #define CEC_OPC_AUDIO_RATE_WIDE_SLOW                      0x03
  #define CEC_OPC_AUDIO_RATE_NARROW_STD                     0x04
  #define CEC_OPC_AUDIO_RATE_NARROW_FAST                    0x05
  #define CEC_OPC_AUDIO_RATE_NARROW_SLOW                    0x06
// 0x9b - 0x9c
#define CEC_OPC_INACTIVE_SOURCE                       0x9d  // u  [PhysAddr]                              -
#define CEC_OPC_CEC_VERSION                           0x9e  // u  -                                       -
  #define CEC_OPC_CEC_VERSION_1_1                           0x01
  #define CEC_OPC_CEC_VERSION_1_2                           0x02
  #define CEC_OPC_CEC_VERSION_1_2A                          0x03
  #define CEC_OPC_CEC_VERSION_1_3                           0x04
  #define CEC_OPC_CEC_VERSION_1_3A                          0x05
  #define CEC_OPC_CEC_VERSION_1_4                           0x06
#define CEC_OPC_GET_CEC_VERSION                       0x9f  // u  -                                       0x9e
#define CEC_OPC_VENDOR_COMMAND_WITH_ID                0xa0  // x  [VendorID][SpecificData]                -
#define CEC_OPC_CLEAR_EXTERNAL_TIMER                  0xa1  // u  [Day][Month][StartTime][Duration]       0x35
                                                            //    [Seq][ExtSrc][Ext Plug||PhysAddr]
#define CEC_OPC_SET_EXTERNAL_TIMER                    0xa2  // u  [Day][Month][StartTime][Duration]       0x35
                                                            //    [Seq][ExtSrc][Ext Plug||PhysAddr]
// 0xa3 - 0xfe
#define CEC_OPC_ABORT                                 0xff  // u  -                                       -

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define CEC_KEY_SELECT                        0x00
#define CEC_KEY_UP                            0x01
#define CEC_KEY_DOWN                          0x02
#define CEC_KEY_LEFT                          0x03
#define CEC_KEY_RIGHT                         0x04
#define CEC_KEY_RIGHT_UP                      0x05
#define CEC_KEY_RIGHT_DOWN                    0x06
#define CEC_KEY_LEFT_UP                       0x07
#define CEC_KEY_LEFT_DOWN                     0x08
#define CEC_KEY_ROOT_MENU                     0x09
#define CEC_KEY_SETUP_MENU                    0x0a
#define CEC_KEY_CONTENTS_MENU                 0x0b
#define CEC_KEY_FAVORITE_MENU                 0x0c
#define CEC_KEY_EXIT                          0x0d
// 0x0e - 0x1f
#define CEC_KEY_0                             0x20
#define CEC_KEY_1                             0x21
#define CEC_KEY_2                             0x22
#define CEC_KEY_3                             0x23
#define CEC_KEY_4                             0x24
#define CEC_KEY_5                             0x25
#define CEC_KEY_6                             0x26
#define CEC_KEY_7                             0x27
#define CEC_KEY_8                             0x28
#define CEC_KEY_9                             0x29
#define CEC_KEY_DOT                           0x2a
#define CEC_KEY_ENTER                         0x2b
#define CEC_KEY_CLEAR                         0x2c
// 0x2d - 0x2e
#define CEC_KEY_NEXT_FAVORITE                 0x2f
#define CEC_KEY_CHANNEL_UP                    0x30
#define CEC_KEY_CHANNEL_DOWN                  0x31
#define CEC_KEY_PREVIOUS_CHANNEL              0x32
#define CEC_KEY_SOUND_SELECT                  0x33
#define CEC_KEY_INPUT_SELECT                  0x34
#define CEC_KEY_DISPLAY_INFORMATION           0x35
#define CEC_KEY_HELP                          0x36
#define CEC_KEY_PAGE_UP                       0x37
#define CEC_KEY_PAGE_DOWN                     0x38
// 0x39 - 0x3f
#define CEC_KEY_POWER                         0x40
#define CEC_KEY_VOLUME_UP                     0x41
#define CEC_KEY_VOLUME_DOWN                   0x42
#define CEC_KEY_MUTE                          0x43
#define CEC_KEY_PLAY                          0x44
#define CEC_KEY_STOP                          0x45
#define CEC_KEY_PAUSE                         0x46
#define CEC_KEY_RECORD                        0x47
#define CEC_KEY_REWIND                        0x48
#define CEC_KEY_FAST_FORWARD                  0x49
#define CEC_KEY_EJECT                         0x4a
#define CEC_KEY_FORWARD                       0x4b
#define CEC_KEY_BACKWARD                      0x4c
#define CEC_KEY_STOP_RECORD                   0x4d
#define CEC_KEY_PAUSE_RECORD                  0x4e
// 0x4f
#define CEC_KEY_ANGLE                         0x50
#define CEC_KEY_SUB_PICTURE                   0x51
#define CEC_KEY_VIDEO_ON_DEMAND               0x52
#define CEC_KEY_ELECTRONIC_PROGRAM_GUIDE      0x53
#define CEC_KEY_TIMER_PROGRAMMING             0x54
#define CEC_KEY_INITIAL_CONFIGURATION         0x55
// 0x56 - 0x5f
#define CEC_KEY_PLAY_FUNCTION                 0x60
#define CEC_KEY_PAUSE_PLAY_FUNCTION           0x61
#define CEC_KEY_RECORD_FUNCTION               0x62
#define CEC_KEY_PAUSE_RECORD_FUNCTION         0x63
#define CEC_KEY_STOP_FUNCTION                 0x64
#define CEC_KEY_MUTE_FUNCTION                 0x65
#define CEC_KEY_RESTORE_VOLUME_FUNCTION       0x66
#define CEC_KEY_TUNE_FUNCTION                 0x67
#define CEC_KEY_SELECT_MEDIA_FUNCTION         0x68
#define CEC_KEY_SELECT_AV_INPUT_FUNCTION      0x69
#define CEC_KEY_SELECT_AUDIO_INPUT_FUNCTION   0x6a
#define CEC_KEY_POWER_TOGGLE_FUNCTION         0x6b
#define CEC_KEY_POWER_OFF_FUNCTION            0x6c
#define CEC_KEY_POWER_ON_FUNCTION             0x6d
// 0x6e - 0x70
#define CEC_KEY_F1                            0x71
#define CEC_KEY_BLUE                          0x71
#define CEC_KEY_F2                            0x72
#define CEC_KEY_RED                           0x72
#define CEC_KEY_F3                            0x73
#define CEC_KEY_GREEN                         0x73
#define CEC_KEY_F4                            0x74
#define CEC_KEY_YELLOW                        0x74
#define CEC_KEY_F5                            0x75
#define CEC_KEY_DATA                          0x76
// 0x77 - 0xff
