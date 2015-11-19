/**
 * @author Priyank Patel
 * @desc Arduino code for SmartCart
 */
// Include the Libraries
#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>

// Requires third party library found at: https://github.com/adafruit/Adafruit-PN532
#include <Adafruit_PN532.h>

// Pins connected to the IRQ and reset lines
#define PN532_IRQ    (2)
#define PN532_RESET  (3)

// Breakout with a I2C connection
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

// MAC Address of the Ethernet shield
byte mac[] = {
  0xBE, 0xEF, 0xBA, 0x7C, 0x5F, 0xDD
};

// Arduino IP Address, depending on network configuration
IPAddress ip(192, 168, 2, 35);

// Create the Ethernet client
EthernetClient client;

// Server Hostname
char serverName[] = "sandbox.priyankpatel.io";

boolean isConnected = false;

int readyState = 9;
int connectedState = 8;
int processState = 6;
int errorState = 5;

int button = 7;

boolean initiated = false;


/**
 * Setup method for initialization
 * @param  {void}  void empty void
 * @return {}      none
 */
void setup() {
  pinMode(button, INPUT);
  pinMode(readyState, OUTPUT);
  pinMode(processState, OUTPUT);
  pinMode(connectedState, OUTPUT);
  pinMode(errorState, OUTPUT);
  
  digitalWrite(processState, LOW);

  // Start the Ethernet Connection
  Ethernet.begin(mac, ip);

  // Begin serial at 115220 baud rate
  Serial.begin(115200);

  waitForInit();

  Serial.println("[DEBUG]  : Initializing Arduino!");

  isConnected = connectToServer(client, serverName);
  if(!isConnected) {
    while(1); 
  }

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

  digitalWrite(readyState, HIGH);

  Serial.println("[DEBUG]  : Waiting for RFID Scan...");
}

void loop() {

  // Init boolean success
  boolean success;

  // Contains UID of card after scan
  uint8_t uid[] = { 
    0, 0, 0, 0, 0, 0, 0                   };

  // Contains UID length
  uint8_t uidLength;

  // Read the card when scanned
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  digitalWrite(processState, HIGH);
  digitalWrite(readyState, LOW);
  delay(1000);

  // If card is read then print its information and validate
  if(success) {
    Serial.println("[SUCCESS]: Card Found!");
    // Print the RFID card information
    printCardInfo(uid, uidLength);
    digitalWrite(processState, LOW);
    digitalWrite(readyState, HIGH);

    delay(2000);
  } 
  else {
    Serial.println("[ERROR]: Timed out waiting for a card"); 
    digitalWrite(processState, LOW);
    digitalWrite(errorState, HIGH);
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

boolean connectToServer(EthernetClient client, char serverName[]) {
  if(client.connect(serverName, 1337)) {
    Serial.println("[DEBUG]  : Connection to server Successful!");
    digitalWrite(connectedState, HIGH);
    return true;
  } 
  else {
    Serial.println("[ERROR]  : Failed to connect to the server"); 
    digitalWrite(errorState, HIGH);
    return false;
  }
  return false;
}

void waitForInit() {
  int sensorValue = digitalRead(7);

  do {
    sensorValue = digitalRead(7);
  }
  while(!sensorValue); 
}




