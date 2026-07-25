#include "HID-Project.h"

void setup() {
  Serial1.begin(115200);
  BootKeyboard.begin();
}

void loop() {
  if (Serial1.available() > 0) {
    String line = Serial1.readStringUntil('\n');
    line.trim();

    if (line.startsWith("STRING ")) {
      BootKeyboard.print(line.substring(7));
    } 
    else if (line.startsWith("KEY ")) {
      pressSpecialKey(line.substring(4), true);
    }
    else if (line.startsWith("COMBO ")) {
      processCombo(line.substring(6));
    }
  }
}

void processCombo(String keys) {
  int start = 0;
  int end = keys.indexOf(' ');
  
  while (end != -1) {
    pressSpecialKey(keys.substring(start, end), false);
    start = end + 1;
    end = keys.indexOf(' ', start);
  }

  pressSpecialKey(keys.substring(start), false);
  
  delay(50);
  BootKeyboard.releaseAll();
}

void pressSpecialKey(String key, bool release) {
  key.toUpperCase();
  
  KeyboardKeycode k = (KeyboardKeycode)0; 

if (key == "ENTER") k = KEY_ENTER;
  else if (key == "ESC") k = KEY_ESC;
  else if (key == "BACKSPACE") k = KEY_BACKSPACE;
  else if (key == "TAB") k = KEY_TAB;
  else if (key == "SPACE") k = KEY_SPACE;
  else if (key == "PRINTSCREEN") k = KEY_PRINTSCREEN;
  else if (key == "SCROLLLOCK") k = KEY_SCROLL_LOCK;
  else if (key == "PAUSE") k = KEY_PAUSE;
  else if (key == "INSERT") k = KEY_INSERT;
  else if (key == "HOME") k = KEY_HOME;
  else if (key == "PAGEUP") k = KEY_PAGE_UP;
  else if (key == "PAGEDOWN") k = KEY_PAGE_DOWN;
  else if (key == "DELETE") k = KEY_DELETE;
  else if (key == "END") k = KEY_END;
  else if (key == "RIGHT") k = KEY_RIGHT_ARROW;
  else if (key == "LEFT") k = KEY_LEFT_ARROW;
  else if (key == "DOWN") k = KEY_DOWN_ARROW;
  else if (key == "UP") k = KEY_UP_ARROW;
  else if (key == "NUMLOCK") k = KEY_NUM_LOCK;
  else if (key == "CAPSLOCK") k = KEY_CAPS_LOCK;
  else if (key == "CTRL") k = KEY_LEFT_CTRL;
  else if (key == "SHIFT") k = KEY_LEFT_SHIFT;
  else if (key == "ALT") k = KEY_LEFT_ALT;
  else if (key == "GUI") k = KEY_LEFT_GUI;
  else if (key == "RCTRL") k = KEY_RIGHT_CTRL;
  else if (key == "RSHIFT") k = KEY_RIGHT_SHIFT;
  else if (key == "RALT") k = KEY_RIGHT_ALT;
  else if (key == "RGUI") k = KEY_RIGHT_GUI;
  else if (key == "F1") k = KEY_F1
  else if (key == "F2") k = KEY_F2
  else if (key == "F3") k = KEY_F3
  else if (key == "F4") k = KEY_F4
  else if (key == "F5") k = KEY_F5
  else if (key == "F6") k = KEY_F6
  else if (key == "F7") k = KEY_F7
  else if (key == "F8") k = KEY_F8
  else if (key == "F9") k = KEY_F9
  else if (key == "F10") k = KEY_F10
  else if (key == "F11") k = KEY_F11
  else if (key == "F12") k = KEY_F12

  else if (key.length() == 1) {
    char c = key[0];
    if (c >= 'A' && c <= 'Z') {

      k = (KeyboardKeycode)(4 + (c - 'A'));
    } else if (c >= '1' && c <= '9') {
      k = (KeyboardKeycode)(30 + (c - '1'));
    } else if (c == '0') {
      k = (KeyboardKeycode)39;
    }
  }

  if (k != (KeyboardKeycode)0) {
    if (release) BootKeyboard.write(k);
    else {
      BootKeyboard.press(k);
      delay(20);
    }
  }
}
