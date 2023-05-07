#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // remove any leading/trailing whitespace
    char opp = input.charAt(0);
    int addr = input.substring(2, 5).toInt();
    int val = input.substring(6).toInt();
    
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
}
