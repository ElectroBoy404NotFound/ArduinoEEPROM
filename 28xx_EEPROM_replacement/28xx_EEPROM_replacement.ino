#include <EEPROM.h>

int x = 0;
int y = 0;
volatile boolean writeFlag = false;
volatile boolean OEFlag = false;
int readBinaryInput() {
  int x = 0;
  bitWrite(x, 0, digitalRead(2));
  bitWrite(x, 1, digitalRead(3));
  bitWrite(x, 2, digitalRead(4));
  bitWrite(x, 3, digitalRead(5));
  bitWrite(x, 4, digitalRead(6));
  bitWrite(x, 5, digitalRead(7));
  bitWrite(x, 6, digitalRead(8));
  bitWrite(x, 7, digitalRead(9));
  return x;
}

int getYValueFromInputs() {
  int y = 0;
  bitWrite(y, 0, digitalRead(10));
  bitWrite(y, 1, digitalRead(11));
  bitWrite(y, 2, digitalRead(12));
  bitWrite(y, 3, digitalRead(13));
  bitWrite(y, 4, digitalRead(A0));
  bitWrite(y, 5, digitalRead(A1));
  bitWrite(y, 6, digitalRead(A2));
  bitWrite(y, 7, digitalRead(A3));
  return y;
}

void outputY() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  digitalWrite(10, bitRead(y, 0));
  digitalWrite(11, bitRead(y, 1));
  digitalWrite(12, bitRead(y, 2));
  digitalWrite(13, bitRead(y, 3));
  digitalWrite(A0, bitRead(y, 4));
  digitalWrite(A1, bitRead(y, 5));
  digitalWrite(A2, bitRead(y, 6));
  digitalWrite(A3, bitRead(y, 7));
}

void setOutputMode() {
  if (digitalRead(A5) == LOW) {
    outputY();
    OEFlag = true;
  } else {
    OEFlag = false;
    pinMode(10, INPUT);
    pinMode(11, INPUT);
    pinMode(12, INPUT);
    pinMode(13, INPUT);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
  }
}

void writeYToEEPROM() {
  writeFlag = true;
}
void setup() {
  Serial.begin(9600);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(A4), writeYToEEPROM, FALLING);
  attachInterrupt(digitalPinToInterrupt(A5), setOutputMode, CHANGE);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // remove any leading/trailing whitespace
    char opp = input.charAt(0);
    int addr = input.substring(2, 5).toInt();
    int val = input.substring(6).toInt();
    Serial.print("Opp code: ");
    Serial.println(opp);
    if (opp == 'W') {
      EEPROM.write(addr, val);
      Serial.print("Written ");
      Serial.print(val);
      Serial.print(" to address ");
      Serial.println(addr);
    } 
    else if (opp == 'r') {
      int read_val = EEPROM.read(addr);
      Serial.print(addr, HEX);
      Serial.print(": ");
      Serial.println(read_val);
    }
    else if (opp == 'R') {
      int i;
      int line_count = 0;
      Serial.print(addr, HEX);
      Serial.print(": ");
      for (i = addr; i < addr + 255; i++) {
        int val = EEPROM.read(i);
        Serial.print(val);
        Serial.print(" ");
        line_count++;
        if (line_count == 8) {
          Serial.print("\t");
        }
        if (line_count == 16) {
          Serial.println();
          line_count = 0;
          Serial.print(addr + (i - addr + 1), HEX);
          Serial.print(": ");
        }
      }
      Serial.println();
    }
  }
  
  x = readBinaryInput();
  if (writeFlag) {
    writeFlag = false;
    EEPROM.write(x, getYValueFromInputs());
  } else {
    if(OEFlag) {
      y = EEPROM.read(x);
      outputY();
    }
  }
}
