/**
 * @author Priyank Patel
 * @desc Arduino code for SmartCart
 */
// Include the Libraries
#include <Wire.h>
#include <SPI.h>

// Requires third party library found at: https://github.com/adafruit/Adafruit-PN532
#include <Adafruit_PN532.h>

// Pins connected to the IRQ and reset lines
#define PN532_IRQ    (2)
#define PN532_RESET  (3)

// Breakout with a I2C connection
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

/**
 * Setup method for initialization
 * @param  {void}  void empty void
 * @return {}      none
 */
void setup(void) {
  // Begin serial at 115220 baud rate
  Serial.begin(115200);
  Serial.println("[DEBUG]  : Initializing Arduino!");

  // Begin the PN532 board
  nfc.begin(); 

  // Get the Version information
  uint32_t versiondata = nfc.getFirmwareVersion();

  // Throw error if Board is not found
  if(!versiondata) {
    Serial.println("[ERROR]  : PN532x Board not Found!");

    // Halt forever
    while (1);
  }

  // Print Board Information
  Serial.print("[SUCCESS]: Found chip PN5"); 
  Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("[DEBUG]  : Firmware ver. "); 
  Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); 
  Serial.println((versiondata>>8) & 0xFF, DEC);

  // Set the max number of attempts to read from a card
  // Prevents from waiting forever
  nfc.setPassiveActivationRetries(0xFF);

  // Configure the board to read RFID tags
  nfc.SAMConfig();

  Serial.println("[DEBUG]  : Waiting for RFID Scan...");
}

void loop(void) {
  // Init boolean success
  boolean success;

  // Contains UID of card after scan
  uint8_t uid[] = { 
    0, 0, 0, 0, 0, 0, 0   };

  // Contains UID length
  uint8_t uidLength;

  // Read the card when scanned
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  // If card is read then print its information and validate
  if(success) {
    Serial.println("[SUCCESS]: Card Found!");
    // Print the RFID card information
    printCardInfo(uid, uidLength);

    delay(2000);
  } 
  else {
    Serial.println("[ERROR]: Timed out waiting for a card"); 
    while(1);
  }
}

/**
 * Print UID Information
 * @param  {uint8_t} uint8_t uid[]        Array of UID numbers
 * @param  {uint8_t} uint8_t uidLength    length of UID
 * @return {none}                         none
 */   
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
