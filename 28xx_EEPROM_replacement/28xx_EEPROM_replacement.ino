#include <EEPROM.h>

int x = 0;
int y = 0;
char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}
int htoInt(char hexValue[]) {
  return (int) strtol(hexValue, 0, 16);
}
void setup() {
  Serial.begin(9600);
  DDRD = (DDRD & 0b00000011) | (0b00000000);
  DDRB = 0b11111100;
  DDRC = (DDRC & 0b00001111) | 0b11110000;
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // remove any leading/trailing whitespace
    char opp = input.charAt(0);
    int addr = htoInt(string2char(input.substring(2, 4)));
    int val = htoInt(string2char(input.substring(5)));
    Serial.println(input.substring(2, 4));
    Serial.print("Opp code: ");
    Serial.println(opp);
    if (opp == 'W') {
      EEPROM.write(addr, val);
      Serial.print("Written ");
      Serial.print("0x");
      if(val< 0x10) Serial.print("0");
      Serial.print(val, HEX);
      Serial.print(" to address ");
      Serial.print("0x");
      if(addr < 0x10) Serial.print("0");
      Serial.println(addr, HEX);
    } 
    else if (opp == 'r') {
      int read_val = EEPROM.read(addr);
      Serial.print("0x");
      if(addr < 0x10) Serial.print("0");
      Serial.print(addr, HEX);
      Serial.print(": ");
      Serial.print("0x");
      if(read_val < 0x10) Serial.print("0");
      Serial.println(read_val, HEX);
    }
    else if (opp == 'R') {
      int i;
      int line_count = 0;
      Serial.print("0x");
      if(addr < 0x10) Serial.print("0");
      Serial.print(addr, HEX);
      Serial.print(": ");
      for (i = addr; i <= addr + 255; i++) {
        int val = EEPROM.read(i);
        if(val < 0x10) Serial.print("0");
        Serial.print(val, HEX);
        Serial.print(" ");
        line_count++;
        if (line_count == 8) {
          Serial.print("\t");
        }
        if (line_count == 16) {
          line_count = 0;
          if(i != addr + 255) {
            Serial.println();
            Serial.print("0x");
            if(addr + (i - addr + 1) < 0x10) Serial.print("0");
            Serial.print(addr + (i - addr + 1), HEX);
            Serial.print(": ");
          }
        }
      }
      Serial.println();
    }else if(opp == 'e') {
       Serial.println("Erasing EEPROM...");
       for(int i = 0; i <= 255; i++) EEPROM.write(i, addr);
       Serial.println("Done erasing...");
       Serial.print("Erase byte: ");
       Serial.println(addr);
    } else if(opp == 't') {
      for(int i = 0; i <= 255; i+=2) EEPROM.write(i, 0xAA);
      for(int i = 1; i <= 255; i+=2) EEPROM.write(i, 0x55);
    }
  }
  
  int x = 0;
  x |= (PIND & 0b00001100) >> 2; // reads the value of pins 2 and 3 and sets the 0th and 1st bits of x
  x |= (PIND & 0b00110000) >> 2; // reads the value of pins 4 and 5, shifts the result by 2, and sets the 2nd and 3rd bits of x
  x |= (PIND & 0b11000000) >> 2; // reads the value of pins 6 and 7, shifts the result by 4, and sets the 4th and 5th bits of x
  x |= (PINB & 0b00000011) << 6; // reads the value of pins 8 and 9, shifts the result by 6, and sets the 6th and 7th bits of x
  y = EEPROM.read(x);
  PORTB = (PORTB & B11000000) | ((y & B00001111) << 2);
  PORTC = (PORTC & B11110000) | ((y >> 4) & B00001111);
}
