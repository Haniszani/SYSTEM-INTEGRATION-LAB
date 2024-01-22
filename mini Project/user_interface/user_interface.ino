#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#include <SPI.h>
#include <MFRC522.h>
#include "pitches.h"
#define BUZZER_PIN 2


#define SS_PIN 21  // MOSI of RFID
#define RST_PIN 5  // RST of RFID
#define btn_Up 17
#define btn_OK 16
#define btn_DOWN 4

const int WASH_LED_PIN = 27;
const int RINSE_LED_PIN = 14;
const int SPIN_LED_PIN = 12;

bool status_up = false;
bool status_ok = false;
bool status_down = false;

bool last_status_up = false;
bool last_status_ok = false;
bool last_status_down = false;

bool UP = false;
bool OKAY = false;
bool DOWN = false;

int page = 0;
int item = 1;

// Variables to store settings values
int waterTemperature = 30;  // Default value
int washDuration = 5;       // Default value
int spinSpeed = 800;        // Default value

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.



// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//song
int melody[] = {
  NOTE_C4, NOTE_C4,
  NOTE_D4, NOTE_C4, NOTE_F4,
  NOTE_E4, NOTE_C4, NOTE_C4,
  NOTE_D4, NOTE_C4, NOTE_G4,
  NOTE_F4, NOTE_C4, NOTE_C4,

  NOTE_C5, NOTE_A4, NOTE_F4,
  NOTE_E4, NOTE_D4, NOTE_AS4, NOTE_AS4,
  NOTE_A4, NOTE_F4, NOTE_G4,
  NOTE_F4
};

int durations[] = {
  4, 8,
  4, 4, 4,
  2, 4, 8,
  4, 4, 4,
  2, 4, 8,

  4, 4, 4,
  4, 4, 4, 8,
  4, 4, 4,
  2
};

//if RFID is correct then:
void authorizedMode() {

  while (true) {
    loopWashingMachine();
  }
}

//display message on Oled
void message(String text) {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print(text);
  display.display();
  //delay(1000);
}

void loopWashingMachine() {
  lcdSettings();

  status_up = digitalRead(btn_Up);
  status_ok = digitalRead(btn_OK);
  status_down = digitalRead(btn_DOWN);

  btnUpPressed();
  btnOkPressed();
  btnDownPressed();

  // FOR button up
  if (UP && page == 0) {
    UP = false;
    item--;
    if (item < 1) item = 4;
  }
  // FOR button down
  if (DOWN && page == 0) {
    DOWN = false;
    item++;
    if (item > 4) item = 1;
  }

  // FOR button ok
  if (OKAY) {
    OKAY = false;
    if (page == 0 && item == 1) {
      page = 1;
    } else if (page == 0 && item == 2) {
      page = 2;
    } else if (page == 0 && item == 3) {
      page = 3;
    } else if (page == 0 && item == 4) {
      page = 4;
    } else if (page == 1 && item == 1) {
      page = 0;
    } else if (page == 2 && item == 1) {
      page = 0;
    } else if (page == 3 && item == 1) {
      page = 0;
    } else if (page == 4 && item == 1) {
      page = 0;
    }
  }

  // Adjust values on settings pages
  if (page == 1) {
    adjustSetting(waterTemperature, 10, 90, "Water Temperature");
  } else if (page == 2) {
    adjustSetting(washDuration, 1, 10, "Wash Duration");
  } else if (page == 3) {
    adjustSetting(spinSpeed, 400, 1200, "Spin Speed");
  } else if (page == 4) {
    // Start Washing
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.print("Washing Started");
    display.display();

    while (true) {
      status_ok = digitalRead(btn_OK);

      if (status_ok == 0) {
        OKAY = true;
        delay(50);
      }

      if (OKAY) {
        OKAY = false;
        Serial.println("S");
        //delay(10000);
        startWash();

        page = 0;
        return;
      }

      delay(100);
    }
  }

  Serial.println(item);
  delay(100);
}

void btnUpPressed() {
  if (status_up != last_status_up) {
    if (status_up == 0) {
      UP = true;
    }
    delay(50);
  }
  last_status_up = status_up;
}

void btnOkPressed() {
  if (status_ok != last_status_ok) {
    if (status_ok == 0) {
      OKAY = true;
    }
    delay(50);
  }
  last_status_ok = status_ok;
}

void btnDownPressed() {
  if (status_down != last_status_down) {
    if (status_down == 0) {
      DOWN = true;
    }
    delay(50);
  }
  last_status_down = status_down;
}

void lcdSettings() {
  if (page == 0) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(30, 0);
    display.print("Washing Machine Settings");

    if (item == 1) {
      display.setCursor(5, 17);
      display.setTextColor(WHITE);
      display.print("> Water Temperature");
    } else {
      display.setCursor(5, 17);
      display.setTextColor(WHITE);
      display.print("  Water Temperature");
    }

    if (item == 2) {
      display.setCursor(5, 27);
      display.setTextColor(WHITE);
      display.print("> Wash Duration");
    } else {
      display.setCursor(5, 27);
      display.setTextColor(WHITE);
      display.print("  Wash Duration");
    }

    if (item == 3) {
      display.setCursor(5, 37);
      display.setTextColor(WHITE);
      display.print("> Spin Speed");
    } else {
      display.setCursor(5, 37);
      display.setTextColor(WHITE);
      display.print("  Spin Speed");
    }

    if (item == 4) {
      display.setCursor(5, 47);
      display.setTextColor(WHITE);
      display.print("> Start Washing");

    } else {
      display.setCursor(5, 47);
      display.setTextColor(WHITE);
      display.print("  Start Washing");
    }
  } else if (page == 1) {
    // Water Temperature Settings
    adjustSettingScreen(waterTemperature, 10, 90, "Water Temperature");
  } else if (page == 2) {
    // Wash Duration Settings
    adjustSettingScreen(washDuration, 1, 10, "Wash Duration");
  } else if (page == 3) {
    // Spin Speed Settings
    adjustSettingScreen(spinSpeed, 400, 1200, "Spin Speed");
  } else if (page == 4) {
    // Start Washing
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.print("Start Washing");
    display.setCursor(10, 30);
    display.print("Press OKAY to start");
  }

  display.display();
}

void adjustSetting(int &setting, int minValue, int maxValue, const char *settingName) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.print(settingName);
  display.setCursor(10, 30);
  display.print("Current: ");
  display.print(setting);
  display.setCursor(10, 50);
  display.print("Press UP/DOWN to adjust");
  display.display();
  delay(100);

  while (true) {
    status_up = digitalRead(btn_Up);
    status_down = digitalRead(btn_DOWN);
    status_ok = digitalRead(btn_OK);

    if (status_up == 0) {
      UP = true;
      delay(50);
    } else if (status_down == 0) {
      DOWN = true;
      delay(50);
    } else if (status_ok == 0) {
      OKAY = true;
      delay(50);
    }

    if (UP) {
      UP = false;
      setting++;
      if (setting > maxValue) setting = minValue;
    }
    if (DOWN) {
      DOWN = false;
      setting--;
      if (setting < minValue) setting = maxValue;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.print(settingName);
    display.setCursor(10, 30);
    display.print("Current: ");
    display.print(setting);
    display.setCursor(10, 50);
    display.print("OKAY to confirm");

    display.display();

    if (OKAY) {
      OKAY = false;
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(10, 10);
      display.print(settingName);
      display.setCursor(10, 30);
      display.print("Set to ");
      display.print(setting);
      display.setCursor(10, 50);
      display.print("OKAY to continue");
      display.display();
      while (true) {
        status_ok = digitalRead(btn_OK);
        if (status_ok == 0) {
          OKAY = true;
          delay(50);
        }
        if (OKAY) {
          OKAY = false;
          page = 0;
          return;
        }
      }
    }
    delay(100);
  }
}

void adjustSettingScreen(int &setting, int minValue, int maxValue, const char *settingName) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.print(settingName);
  display.setCursor(10, 30);
  display.print("Current: ");
  display.print(setting);
  display.setCursor(10, 50);
  display.print("Press UP/DOWN to adjust");
  display.display();

  while (true) {
    status_up = digitalRead(btn_Up);
    status_down = digitalRead(btn_DOWN);
    status_ok = digitalRead(btn_OK);

    if (status_up == 0) {
      UP = true;
      delay(50);
    } else if (status_down == 0) {
      DOWN = true;
      delay(50);
    }

    if (UP) {
      UP = false;
      setting++;
      if (setting > maxValue) setting = minValue;
    }
    if (DOWN) {
      DOWN = false;
      setting--;
      if (setting < minValue) setting = maxValue;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.print(settingName);
    display.setCursor(10, 30);
    display.print("Current: ");
    display.print(setting);
    display.setCursor(10, 50);
    display.print("Press OKAY to confirm");

    display.display();

    if (status_ok == 0) {
      OKAY = true;
      delay(50);
    }

    if (OKAY) {
      OKAY = false;
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(10, 10);
      display.print(settingName);
      display.setCursor(10, 30);
      display.print("Set to ");
      display.print(setting);
      display.setCursor(10, 50);
      display.print("Press OKAY to continue");
      display.display();

      while (status_ok == 0) {
        status_ok = digitalRead(btn_OK);
        delay(50);
      }

      page = 0;
      return;
    }

    delay(100);
  }
}
void startWash() {
  Serial.println("S");
  wash();
  rinse();
  spin();
  complete();
}
void wash() {
  Serial.println("S");
  display.clearDisplay();
  display.setCursor(10, 30);
  display.print("Washing...");
  display.display();
  statusLed(1);
  delay(10000);
}
void rinse() {
  Serial.println("S");
  display.clearDisplay();
  display.setCursor(10, 30);
  display.print("Rinsing...");
  display.display();
  statusLed(2);
  delay(10000);
}
void spin() {
  Serial.println("S");
  display.clearDisplay();
  display.setCursor(10, 30);
  display.print("Spinning...");
  display.display();
  statusLed(3);
  delay(10000);
}
void complete() {
  Serial.println("S");
  display.clearDisplay();
  display.setCursor(10, 30);
  display.print("Washing complete");
  display.display();
  statusLed(4);
  delay(10000);
  statusLed(0);
  playSong();
  delay(10000);
}
void statusLed(int mode) {
  switch (mode) {
    case 0:
      digitalWrite(WASH_LED_PIN, LOW);
      digitalWrite(RINSE_LED_PIN, LOW);
      digitalWrite(SPIN_LED_PIN, LOW);
      Serial.println('P');  // Washing
      break;
    case 1:
      digitalWrite(WASH_LED_PIN, HIGH);
      digitalWrite(RINSE_LED_PIN, LOW);
      digitalWrite(SPIN_LED_PIN, LOW);
      Serial.println('W');  // Washing
      break;
    case 2:
      digitalWrite(WASH_LED_PIN, LOW);
      digitalWrite(RINSE_LED_PIN, HIGH);
      digitalWrite(SPIN_LED_PIN, LOW);
      Serial.println('R');  // Washing
      break;
    case 3:
      digitalWrite(WASH_LED_PIN, LOW);
      digitalWrite(RINSE_LED_PIN, LOW);
      digitalWrite(SPIN_LED_PIN, HIGH);
      Serial.println('S');  // Washing
      break;
    case 4:
      digitalWrite(WASH_LED_PIN, HIGH);
      digitalWrite(RINSE_LED_PIN, HIGH);
      digitalWrite(SPIN_LED_PIN, HIGH);
      Serial.println('C');  // Washing
      break;
  }
}

void playSong() {
  int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / durations[note];
    tone(BUZZER_PIN, melody[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    //stop the tone playing:
    noTone(BUZZER_PIN);
  }
}

void setup() {

  Serial.begin(115200);
  Wire.begin(25, 26);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  pinMode(btn_Up, INPUT_PULLUP);
  pinMode(btn_OK, INPUT_PULLUP);
  pinMode(btn_DOWN, INPUT_PULLUP);
  pinMode(WASH_LED_PIN, OUTPUT);
  pinMode(RINSE_LED_PIN, OUTPUT);
  pinMode(SPIN_LED_PIN, OUTPUT);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  message("welcome");

  SPI.begin();         // Initiate SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522
  Serial.println("Put your card to the reader...");
  //Serial.println();
  delay(3000);
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // Show UID on the serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    //display.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //display.print(mfrc522.uid.uidByte[i], HEX );
    //message(String(HEX));
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "83 B2 1A 11") {
    Serial.println("Authorized access");
    //Serial.println();
    message("Wash Away~");
    authorizedMode();
  } else {
    //Serial.println("Access denied");
    message("Access denied");
    delay(1000);
    message("Play a game for free wash");
    delay(5000);
    Serial.println('G');
    while (1) {
      if (Serial.available() > 0) {
        char receivedChar = Serial.read();
        if (receivedChar == 'F') {
          authorizedMode();
        }
      }
    }
  }
}

//functions
