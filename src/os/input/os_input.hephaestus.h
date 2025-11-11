#ifndef OS_HEPHAESTUS_H
#define OS_HEPHAESTUS_H

typedef u64 Keyboard_Key;
enum
{
  Keyboard_Key_Left_Mouse_Button,
  Keyboard_Key_Right_Mouse_Button,
  Keyboard_Key_Middle_Mouse_Button,
  Keyboard_Key_X1_Mouse_Button,
  Keyboard_Key_X2_Mouse_Button,
  Keyboard_Key_Control_Break,
  Keyboard_Key_Backspace,
  Keyboard_Key_Tab,
  Keyboard_Key_Clear,
  Keyboard_Key_Enter,
  Keyboard_Key_Shift,
  Keyboard_Key_Ctrl,
  Keyboard_Key_Alt,
  Keyboard_Key_Pause,
  Keyboard_Key_Caps_Lock,
  Keyboard_Key_Kana,
  Keyboard_Key_Junja,
  Keyboard_Key_Final,
  Keyboard_Key_Hanja,
  Keyboard_Key_Escape,
  Keyboard_Key_Space,
  Keyboard_Key_Page_Up,
  Keyboard_Key_Page_Down,
  Keyboard_Key_End,
  Keyboard_Key_Home,
  Keyboard_Key_Left_arrow,
  Keyboard_Key_Up_arrow,
  Keyboard_Key_Right_arrow,
  Keyboard_Key_Down_arrow,
  Keyboard_Key_Select,
  Keyboard_Key_Print,
  Keyboard_Key_Execute,
  Keyboard_Key_Print_Screen,
  Keyboard_Key_Insert,
  Keyboard_Key_Delete,
  Keyboard_Key_Help,
  Keyboard_Key_0,
  Keyboard_Key_1,
  Keyboard_Key_2,
  Keyboard_Key_3,
  Keyboard_Key_4,
  Keyboard_Key_5,
  Keyboard_Key_6,
  Keyboard_Key_7,
  Keyboard_Key_8,
  Keyboard_Key_9,
  Keyboard_Key_A,
  Keyboard_Key_B,
  Keyboard_Key_C,
  Keyboard_Key_D,
  Keyboard_Key_E,
  Keyboard_Key_F,
  Keyboard_Key_G,
  Keyboard_Key_H,
  Keyboard_Key_I,
  Keyboard_Key_J,
  Keyboard_Key_K,
  Keyboard_Key_L,
  Keyboard_Key_M,
  Keyboard_Key_N,
  Keyboard_Key_O,
  Keyboard_Key_P,
  Keyboard_Key_Q,
  Keyboard_Key_R,
  Keyboard_Key_S,
  Keyboard_Key_T,
  Keyboard_Key_U,
  Keyboard_Key_V,
  Keyboard_Key_W,
  Keyboard_Key_X,
  Keyboard_Key_Y,
  Keyboard_Key_Z,
  Keyboard_Key_Left_Windows_Key,
  Keyboard_Key_Right_Windows_Key,
  Keyboard_Key_Application_Key,
  Keyboard_Key_Sleep,
  Keyboard_Key_Numeric_Keypad_0,
  Keyboard_Key_Numeric_Keypad_1,
  Keyboard_Key_Numeric_Keypad_2,
  Keyboard_Key_Numeric_Keypad_3,
  Keyboard_Key_Numeric_Keypad_4,
  Keyboard_Key_Numeric_Keypad_5,
  Keyboard_Key_Numeric_Keypad_6,
  Keyboard_Key_Numeric_Keypad_7,
  Keyboard_Key_Numeric_Keypad_8,
  Keyboard_Key_Numeric_Keypad_9,
  Keyboard_Key_Multiply,
  Keyboard_Key_Add,
  Keyboard_Key_Separator,
  Keyboard_Key_Subtract,
  Keyboard_Key_Decimal,
  Keyboard_Key_Divide,
  Keyboard_Key_F1,
  Keyboard_Key_F2,
  Keyboard_Key_F3,
  Keyboard_Key_F4,
  Keyboard_Key_F5,
  Keyboard_Key_F6,
  Keyboard_Key_F7,
  Keyboard_Key_F8,
  Keyboard_Key_F9,
  Keyboard_Key_F10,
  Keyboard_Key_F11,
  Keyboard_Key_F12,
  Keyboard_Key_Num_Lock,
  Keyboard_Key_Scroll_Lock,
  Keyboard_Key_Left_Shift,
  Keyboard_Key_Right_Shift,
  Keyboard_Key_Left_Ctrl,
  Keyboard_Key_Right_Ctrl,
  Keyboard_Key_Left_Alt,
  Keyboard_Key_Right_Alt,
  Keyboard_Key_Browser_Back,
  Keyboard_Key_Browser_Forward,
  Keyboard_Key_Browser_Refresh,
  Keyboard_Key_Browser_Stop,
  Keyboard_Key_Browser_Search,
  Keyboard_Key_Browser_Favorites,
  Keyboard_Key_Browser_Home,
  Keyboard_Key_Volume_Mute,
  Keyboard_Key_Volume_Down,
  Keyboard_Key_Volume_Up,
  Keyboard_Key_Next_Track,
  Keyboard_Key_Previous_Track,
  Keyboard_Key_Stop_Media,
  Keyboard_Key_Play_Pause_Media,
  Keyboard_Key_Launch_Mail,
  Keyboard_Key_Launch_Media_Select,
  Keyboard_Key_Launch_App1,
  Keyboard_Key_Launch_App2,
  Keyboard_Key_Semicolon,
  Keyboard_Key_Plus,
  Keyboard_Key_Comma,
  Keyboard_Key_minus,
  Keyboard_Key_Period,
  Keyboard_Key_Slash,
  Keyboard_Key_Grave_Accent,
  Keyboard_Key_Left_Bracket,
  Keyboard_Key_Backslash,
  Keyboard_Key_Right_Bracket,
  Keyboard_Key_Apostrophe,
  Keyboard_Key_Count,
};

global const char* keyboard_key_strings[] = 
{
  "Left_Mouse_Button",
  "Right_Mouse_Button",
  "Middle_Mouse_Button",
  "X1_Mouse_Button",
  "X2_Mouse_Button",
  "Control_Break",
  "Backspace",
  "Tab",
  "Clear",
  "Enter",
  "Shift",
  "Ctrl",
  "Alt",
  "Pause",
  "Caps_Lock",
  "Kana",
  "Junja",
  "Final",
  "Hanja",
  "Escape",
  "Space",
  "Page_Up",
  "Page_Down",
  "End",
  "Home",
  "Left_arrow",
  "Up_arrow",
  "Right_arrow",
  "Down_arrow",
  "Select",
  "Print",
  "Execute",
  "Print_Screen",
  "Insert",
  "Delete",
  "Help",
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "A",
  "B",
  "C",
  "D",
  "E",
  "F",
  "G",
  "H",
  "I",
  "J",
  "K",
  "L",
  "M",
  "N",
  "O",
  "P",
  "Q",
  "R",
  "S",
  "T",
  "U",
  "V",
  "W",
  "X",
  "Y",
  "Z",
  "Left_Windows_Key",
  "Right_Windows_Key",
  "Application_Key",
  "Sleep",
  "Numeric_Keypad_0",
  "Numeric_Keypad_1",
  "Numeric_Keypad_2",
  "Numeric_Keypad_3",
  "Numeric_Keypad_4",
  "Numeric_Keypad_5",
  "Numeric_Keypad_6",
  "Numeric_Keypad_7",
  "Numeric_Keypad_8",
  "Numeric_Keypad_9",
  "Multiply",
  "Add",
  "Separator",
  "Subtract",
  "Decimal",
  "Divide",
  "F1",
  "F2",
  "F3",
  "F4",
  "F5",
  "F6",
  "F7",
  "F8",
  "F9",
  "F10",
  "F11",
  "F12",
  "Num_Lock",
  "Scroll_Lock",
  "Left_Shift",
  "Right_Shift",
  "Left_Ctrl",
  "Right_Ctrl",
  "Left_Alt",
  "Right_Alt",
  "Browser_Back",
  "Browser_Forward",
  "Browser_Refresh",
  "Browser_Stop",
  "Browser_Search",
  "Browser_Favorites",
  "Browser_Home",
  "Volume_Mute",
  "Volume_Down",
  "Volume_Up",
  "Next_Track",
  "Previous_Track",
  "Stop_Media",
  "Play_Pause_Media",
  "Launch_Mail",
  "Launch_Media_Select",
  "Launch_App1",
  "Launch_App2",
  "Semicolon",
  "Plus",
  "Comma",
  "minus",
  "Period",
  "Slash",
  "Grave_Accent",
  "Left_Bracket",
  "Backslash",
  "Right_Bracket",
  "Apostrophe",
};

#if OS_WINDOWS
global u64 _win32_key_table[Keyboard_Key_Count] = 
{
  VK_LBUTTON, /* Left_Mouse_Button */
  VK_RBUTTON, /* Right_Mouse_Button */
  VK_MBUTTON, /* Middle_Mouse_Button */
  VK_XBUTTON1, /* X1_Mouse_Button */
  VK_XBUTTON2, /* X2_Mouse_Button */
  VK_CANCEL, /* Control_Break */
  VK_BACK, /* Backspace */
  VK_TAB, /* Tab */
  VK_CLEAR, /* Clear */
  VK_RETURN, /* Enter */
  VK_SHIFT, /* Shift */
  VK_CONTROL, /* Ctrl */
  VK_MENU, /* Alt */
  VK_PAUSE, /* Pause */
  VK_CAPITAL, /* Caps_Lock */
  VK_KANA, /* Kana */
  VK_JUNJA, /* Junja */
  VK_FINAL, /* Final */
  VK_HANJA, /* Hanja */
  VK_ESCAPE, /* Escape */
  VK_SPACE, /* Space */
  VK_PRIOR, /* Page_Up */
  VK_NEXT, /* Page_Down */
  VK_END, /* End */
  VK_HOME, /* Home */
  VK_LEFT, /* Left_arrow */
  VK_UP, /* Up_arrow */
  VK_RIGHT, /* Right_arrow */
  VK_DOWN, /* Down_arrow */
  VK_SELECT, /* Select */
  VK_PRINT, /* Print */
  VK_EXECUTE, /* Execute */
  VK_SNAPSHOT, /* Print_Screen */
  VK_INSERT, /* Insert */
  VK_DELETE, /* Delete */
  VK_HELP, /* Help */
  0x30, /* 0 */
  0x31, /* 1 */
  0x32, /* 2 */
  0x33, /* 3 */
  0x34, /* 4 */
  0x35, /* 5 */
  0x36, /* 6 */
  0x37, /* 7 */
  0x38, /* 8 */
  0x39, /* 9 */
  0x41, /* A */
  0x42, /* B */
  0x43, /* C */
  0x44, /* D */
  0x45, /* E */
  0x46, /* F */
  0x47, /* G */
  0x48, /* H */
  0x49, /* I */
  0x4A, /* J */
  0x4B, /* K */
  0x4C, /* L */
  0x4D, /* M */
  0x4E, /* N */
  0x4F, /* O */
  0x50, /* P */
  0x51, /* Q */
  0x52, /* R */
  0x53, /* S */
  0x54, /* T */
  0x55, /* U */
  0x56, /* V */
  0x57, /* W */
  0x58, /* X */
  0x59, /* Y */
  0x5A, /* Z */
  VK_LWIN, /* Left_Windows_Key */
  VK_RWIN, /* Right_Windows_Key */
  VK_APPS, /* Application_Key */
  VK_SLEEP, /* Sleep */
  VK_NUMPAD0, /* Numeric_Keypad_0 */
  VK_NUMPAD1, /* Numeric_Keypad_1 */
  VK_NUMPAD2, /* Numeric_Keypad_2 */
  VK_NUMPAD3, /* Numeric_Keypad_3 */
  VK_NUMPAD4, /* Numeric_Keypad_4 */
  VK_NUMPAD5, /* Numeric_Keypad_5 */
  VK_NUMPAD6, /* Numeric_Keypad_6 */
  VK_NUMPAD7, /* Numeric_Keypad_7 */
  VK_NUMPAD8, /* Numeric_Keypad_8 */
  VK_NUMPAD9, /* Numeric_Keypad_9 */
  VK_MULTIPLY, /* Multiply */
  VK_ADD, /* Add */
  VK_SEPARATOR, /* Separator */
  VK_SUBTRACT, /* Subtract */
  VK_DECIMAL, /* Decimal */
  VK_DIVIDE, /* Divide */
  VK_F1, /* F1 */
  VK_F2, /* F2 */
  VK_F3, /* F3 */
  VK_F4, /* F4 */
  VK_F5, /* F5 */
  VK_F6, /* F6 */
  VK_F7, /* F7 */
  VK_F8, /* F8 */
  VK_F9, /* F9 */
  VK_F10, /* F10 */
  VK_F11, /* F11 */
  VK_F12, /* F12 */
  VK_NUMLOCK, /* Num_Lock */
  VK_SCROLL, /* Scroll_Lock */
  VK_LSHIFT, /* Left_Shift */
  VK_RSHIFT, /* Right_Shift */
  VK_LCONTROL, /* Left_Ctrl */
  VK_RCONTROL, /* Right_Ctrl */
  VK_LMENU, /* Left_Alt */
  VK_RMENU, /* Right_Alt */
  VK_BROWSER_BACK, /* Browser_Back */
  VK_BROWSER_FORWARD, /* Browser_Forward */
  VK_BROWSER_REFRESH, /* Browser_Refresh */
  VK_BROWSER_STOP, /* Browser_Stop */
  VK_BROWSER_SEARCH, /* Browser_Search */
  VK_BROWSER_FAVORITES, /* Browser_Favorites */
  VK_BROWSER_HOME, /* Browser_Home */
  VK_VOLUME_MUTE, /* Volume_Mute */
  VK_VOLUME_DOWN, /* Volume_Down */
  VK_VOLUME_UP, /* Volume_Up */
  VK_MEDIA_NEXT_TRACK, /* Next_Track */
  VK_MEDIA_PREV_TRACK, /* Previous_Track */
  VK_MEDIA_STOP, /* Stop_Media */
  VK_MEDIA_PLAY_PAUSE, /* Play_Pause_Media */
  VK_LAUNCH_MAIL, /* Launch_Mail */
  VK_LAUNCH_MEDIA_SELECT, /* Launch_Media_Select */
  VK_LAUNCH_APP1, /* Launch_App1 */
  VK_LAUNCH_APP2, /* Launch_App2 */
  VK_OEM_1, /* Semicolon */
  VK_OEM_PLUS, /* Plus */
  VK_OEM_COMMA, /* Comma */
  VK_OEM_MINUS, /* minus */
  VK_OEM_PERIOD, /* Period */
  VK_OEM_2, /* Slash */
  VK_OEM_3, /* Grave_Accent */
  VK_OEM_4, /* Left_Bracket */
  VK_OEM_5, /* Backslash */
  VK_OEM_6, /* Right_Bracket */
  VK_OEM_7, /* Apostrophe */
};
#endif// OS_WINDOWS

#if OS_LINUX
global u64 _linux_key_table[Keyboard_Key_Count] = 
{
  BTN_LEFT, /* Left_Mouse_Button */
  BTN_RIGHT, /* Right_Mouse_Button */
  BTN_MIDDLE, /* Middle_Mouse_Button */
  BTN_SIDE, /* X1_Mouse_Button */
  BTN_EXTRA, /* X2_Mouse_Button */
  KEY_CANCEL, /* Control_Break */
  KEY_BACKSPACE, /* Backspace */
  KEY_TAB, /* Tab */
  KEY_REDO, /* Clear */
  KEY_ENTER, /* Enter */
  KEY_LEFTSHIFT, /* Shift */
  KEY_LEFTCTRL, /* Ctrl */
  KEY_LEFTALT, /* Alt */
  KEY_PAUSE, /* Pause */
  KEY_CAPSLOCK, /* Caps_Lock */
  KEY_KATAKANA, /* Kana */
  KEY_ZENKAKUHANKAKU, /* Junja */
  KEY_HENKAN, /* Final */
  KEY_HANGEUL, /* Hanja */
  KEY_ESC, /* Escape */
  KEY_SPACE, /* Space */
  KEY_PAGEUP, /* Page_Up */
  KEY_PAGEDOWN, /* Page_Down */
  KEY_END, /* End */
  KEY_HOME, /* Home */
  KEY_LEFT, /* Left_arrow */
  KEY_UP, /* Up_arrow */
  KEY_RIGHT, /* Right_arrow */
  KEY_DOWN, /* Down_arrow */
  KEY_SELECT, /* Select */
  KEY_PRINT, /* Print */
  KEY_EXECUTE, /* Execute */
  KEY_SYSRQ, /* Print_Screen */
  KEY_INSERT, /* Insert */
  KEY_DELETE, /* Delete */
  KEY_HELP, /* Help */
  KEY_0, /* 0 */
  KEY_1, /* 1 */
  KEY_2, /* 2 */
  KEY_3, /* 3 */
  KEY_4, /* 4 */
  KEY_5, /* 5 */
  KEY_6, /* 6 */
  KEY_7, /* 7 */
  KEY_8, /* 8 */
  KEY_9, /* 9 */
  KEY_A, /* A */
  KEY_B, /* B */
  KEY_C, /* C */
  KEY_D, /* D */
  KEY_E, /* E */
  KEY_F, /* F */
  KEY_G, /* G */
  KEY_H, /* H */
  KEY_I, /* I */
  KEY_J, /* J */
  KEY_K, /* K */
  KEY_L, /* L */
  KEY_M, /* M */
  KEY_N, /* N */
  KEY_O, /* O */
  KEY_P, /* P */
  KEY_Q, /* Q */
  KEY_R, /* R */
  KEY_S, /* S */
  KEY_T, /* T */
  KEY_U, /* U */
  KEY_V, /* V */
  KEY_W, /* W */
  KEY_X, /* X */
  KEY_Y, /* Y */
  KEY_Z, /* Z */
  KEY_LEFTMETA, /* Left_Windows_Key */
  KEY_RIGHTMETA, /* Right_Windows_Key */
  KEY_COMPOSE, /* Application_Key */
  KEY_SLEEP, /* Sleep */
  KEY_KP0, /* Numeric_Keypad_0 */
  KEY_KP1, /* Numeric_Keypad_1 */
  KEY_KP2, /* Numeric_Keypad_2 */
  KEY_KP3, /* Numeric_Keypad_3 */
  KEY_KP4, /* Numeric_Keypad_4 */
  KEY_KP5, /* Numeric_Keypad_5 */
  KEY_KP6, /* Numeric_Keypad_6 */
  KEY_KP7, /* Numeric_Keypad_7 */
  KEY_KP8, /* Numeric_Keypad_8 */
  KEY_KP9, /* Numeric_Keypad_9 */
  KEY_KPASTERISK, /* Multiply */
  KEY_KPPLUS, /* Add */
  KEY_KPCOMMA, /* Separator */
  KEY_KPMINUS, /* Subtract */
  KEY_KPDOT, /* Decimal */
  KEY_KPSLASH, /* Divide */
  KEY_F1, /* F1 */
  KEY_F2, /* F2 */
  KEY_F3, /* F3 */
  KEY_F4, /* F4 */
  KEY_F5, /* F5 */
  KEY_F6, /* F6 */
  KEY_F7, /* F7 */
  KEY_F8, /* F8 */
  KEY_F9, /* F9 */
  KEY_F10, /* F10 */
  KEY_F11, /* F11 */
  KEY_F12, /* F12 */
  KEY_NUMLOCK, /* Num_Lock */
  KEY_SCROLLLOCK, /* Scroll_Lock */
  KEY_LEFTSHIFT, /* Left_Shift */
  KEY_RIGHTSHIFT, /* Right_Shift */
  KEY_LEFTCTRL, /* Left_Ctrl */
  KEY_RIGHTCTRL, /* Right_Ctrl */
  KEY_LEFTALT, /* Left_Alt */
  KEY_RIGHTALT, /* Right_Alt */
  KEY_BACK, /* Browser_Back */
  KEY_FORWARD, /* Browser_Forward */
  KEY_REFRESH, /* Browser_Refresh */
  KEY_STOP, /* Browser_Stop */
  KEY_SEARCH, /* Browser_Search */
  KEY_HOMEPAGE, /* Browser_Favorites */
  KEY_HOMEPAGE, /* Browser_Home */
  KEY_MUTE, /* Volume_Mute */
  KEY_VOLUMEDOWN, /* Volume_Down */
  KEY_VOLUMEUP, /* Volume_Up */
  KEY_NEXTSONG, /* Next_Track */
  KEY_PREVIOUSSONG, /* Previous_Track */
  KEY_STOPCD, /* Stop_Media */
  KEY_PLAYPAUSE, /* Play_Pause_Media */
  KEY_MAIL, /* Launch_Mail */
  KEY_MEDIA, /* Launch_Media_Select */
  KEY_CALC, /* Launch_App1 */
  KEY_PROG1, /* Launch_App2 */
  KEY_SEMICOLON, /* Semicolon */
  KEY_EQUAL, /* Plus */
  KEY_COMMA, /* Comma */
  KEY_MINUS, /* minus */
  KEY_DOT, /* Period */
  KEY_SLASH, /* Slash */
  KEY_GRAVE, /* Grave_Accent */
  KEY_LEFTBRACE, /* Left_Bracket */
  KEY_BACKSLASH, /* Backslash */
  KEY_RIGHTBRACE, /* Right_Bracket */
  KEY_APOSTROPHE, /* Apostrophe */
};
#endif// OS_LINUX

#endif // OS_HEPHAESTUS_H