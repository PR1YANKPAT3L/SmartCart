#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ    (2)
#define PN532_RESET  (3)

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup(void) {
  Serial.begin(115200);
  Serial.println("[DEBUG]  : Initializing Arduino!");

  nfc.begin(); 

  uint32_t versiondata = nfc.getFirmwareVersion();
  if(!versiondata) {
    Serial.println("[ERROR]  : PN532x Board not Found!");
    while (1);
  }

  Serial.print("[SUCCESS]: Found chip PN5"); 
  Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("[DEBUG]  : Firmware ver. "); 
  Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); 
  Serial.println((versiondata>>8) & 0xFF, DEC);

  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();

  Serial.println("[DEBUG]  : Waiting for RFID Scan...");
}

void loop(void) {
  boolean success;
  uint8_t uid[] = { 
    0, 0, 0, 0, 0, 0, 0   };
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if(success) {
    Serial.println("[SUCCESS]: Card Found!");
    printCardInfo(uid, uidLength);
    delay(2000);
  } 
  else {
    Serial.println("[ERROR]: Timed out waiting for a card"); 
    while(1);
  }
}

void printCardInfo(uint8_t uid[], uint8_t uidLength) {
  Serial.print("[DEBUG]  : UID Length = ");
  Serial.print(uidLength, DEC);
  Serial.print(" bytes");
  Serial.print("\n[DEBUG]  : UID Values = ");
  for(uint8_t i = 0; i < uidLength; i++) {
    Serial.print(" 0x");
    Serial.print(uid[i], HEX); 
  }
  Serial.println(""); 
}
