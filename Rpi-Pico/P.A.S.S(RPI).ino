#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int chipSelect = 17;
const int btnUp = 2;
const int btnDown = 3;
const int btnSelect = 6;

#define MAX_FILES 30
#define VISIBLE_ROWS 5
String fileNames[MAX_FILES];
int fileCount = 0;
int selectedIndex = 0;
int startWindow = 0;

volatile bool isSending = false;
volatile bool isPaused = false;
volatile int progress = 0;
String fileToProcess = "";

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnSelect, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { for(;;); }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 25);
  display.println("BOOTING PICO...");
  display.display();

  if (!SD.begin(chipSelect)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("SD CARD ERROR!");
    display.display();
    while(1);
  }
  
  readFilesFromSD();
}

void loop() {
  display.clearDisplay();

  if (!isSending) {
    handleMenuNavigation();
    drawMenu();
  } else {
    drawProgressUI();
  }

  display.display();
  delay(30);
}

void readFilesFromSD() {
  File root = SD.open("/");
  fileCount = 0;
  while (true) {
    File entry = root.openNextFile();
    if (!entry || fileCount >= MAX_FILES) break;
    if (!entry.isDirectory()) {
      fileNames[fileCount] = entry.name();
      fileCount++;
    }
    entry.close();
  }
}

void handleMenuNavigation() {
  if (digitalRead(btnUp) == LOW) {
    if (selectedIndex > 0) {
      selectedIndex--;
      if (selectedIndex < startWindow) startWindow = selectedIndex;
    } else {
      selectedIndex = fileCount - 1;
      startWindow = max(0, fileCount - VISIBLE_ROWS);
    }
    delay(150);
  }

  if (digitalRead(btnDown) == LOW) {
    if (selectedIndex < fileCount - 1) {
      selectedIndex++;
      if (selectedIndex >= startWindow + VISIBLE_ROWS) startWindow = selectedIndex - VISIBLE_ROWS + 1;
    } else {
      selectedIndex = 0;
      startWindow = 0;
    }
    delay(150);
  }

  if (digitalRead(btnSelect) == LOW) {
    if (fileCount > 0) {
      fileToProcess = fileNames[selectedIndex];
      isSending = true;
      progress = 0;
      delay(400);
    }
  }
}

void drawMenu() {
  display.setCursor(0, 0);
  display.println("Sellect Macro");
  display.println("--------------------");
  
  for (int i = startWindow; i < min(startWindow + VISIBLE_ROWS, fileCount); i++) {
    if (i == selectedIndex) {
      display.print("> ");
    } else {
      display.print("  ");
    }
    String name = fileNames[i];
    if (name.length() > 17) name = name.substring(0, 14) + "...";
    display.println(name);
  }

  if (fileCount > VISIBLE_ROWS) {
    int barHeight = (VISIBLE_ROWS * 40) / fileCount;
    int barPos = (startWindow * 40) / fileCount;
    display.drawFastVLine(125, 20 + barPos, barHeight, SSD1306_WHITE);
  }
}

void drawProgressUI() {
  display.setCursor(0, 0);
  display.println(" Runing Macro...");
  display.println(fileToProcess);
  display.println("--------------------");

  if (isPaused) {
    display.setCursor(25, 30);
    display.setTextSize(1);
    display.println("!! PAUSED !!");
    display.setCursor(15, 52);
    display.println("Press OK to Resume");
  } else {

    display.drawRect(10, 35, 108, 10, SSD1306_WHITE);
    display.fillRect(10, 35, (progress * 108) / 100, 10, SSD1306_WHITE);
    display.setCursor(55, 48);
    display.print(progress);
    display.print("%");
  }

  if (progress >= 100) {
    display.clearDisplay();
    display.setCursor(20, 30);
    display.println("COMPLETE!");
    display.display();
    delay(1500);
    isSending = false;
  }
}


void setup1() {
  delay(2500);
}

void loop1() {
  if (isSending && fileToProcess != "") {
    File dataFile = SD.open(fileToProcess);
    if (dataFile) {
      unsigned long fileSize = dataFile.size();
      unsigned long bytesTotal = 0;

      while (dataFile.available() && isSending) {
        String line = dataFile.readStringUntil('\n');
        line.trim();
        bytesTotal += line.length() + 2; 
        progress = (bytesTotal * 100) / fileSize;

        if (line.startsWith("DELAY")) {
          int waitTime = line.substring(6).toInt();
          delay(waitTime);
        } 
        else if (line == "INTERRUPT") {
          isPaused = true;
          while (digitalRead(btnSelect) == HIGH) { delay(10); }
          delay(400);
          isPaused = false;
        } 
        else if (line.length() > 0) {
          Serial.println(line);
          Serial1.println(line);
          delay(100);
        }
      }
      dataFile.close();
      progress = 100;
      fileToProcess = "";
    }
  }
  delay(100);
}
