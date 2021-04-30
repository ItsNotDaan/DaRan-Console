// All radio listen on pipe 0
// All radio speak on pipe 1
// Hub has one address, and all game controlers share the same address.

// Devices are differentiated by an ID
// 0 means central Hub. use anything else for game controllers
const byte deviceID = 2;

// ****************   WIRING  ****************
// nRF24L01+    CE    CSN   MISO    MOSI    SCK
// UNO           7      8     12      11     13
// MEGA          7      8     50      51     52
// ESP32       D13    D27    D19     D23    D18       (ESP32 dev module)

#include <RF24.h> // http://tmrh20.github.io/RF24/
const byte CEPin = 9;
const byte CSNPin = 8;
RF24 radio(CEPin, CSNPin);

uint8_t mainAddress[] = {0x00, 0xCE, 0xCC, 0xCE, 0xCC};
uint8_t gameAddress[] = {0x01, 0xCE, 0xCC, 0xCE, 0xCC};

struct __attribute__ ((packed)) t_message {
  uint8_t UID;
  char description;
} message;

void sendMessage(t_message &msg) //De waarde van (message) wordt bekenen in t_message en dan in msg gestopt.
{
  radio.stopListening();
  if (!radio.write( &msg, sizeof(t_message) )) Serial.println(F("Error sending message"));
  else Serial.println(F("Message sent"));
  radio.startListening();
}

bool getMessage(t_message &msg)
{
  if (! radio.available()) return false;
  radio.read( &msg, sizeof(t_message) );
  return true;
}

void setup() {
  Serial.begin(115200);
  if (deviceID == 0) Serial.println(F("Configuring Central Hub"));
  else Serial.println(F("Configuring game controller"));

  if (!radio.begin()) {
    Serial.println(F("Could not start radio"));
    while (true) ;
  }

  if (!radio.isChipConnected()) {
    Serial.println(F("No NRF24L01 chip found"));
    while (true) ;
  }

  radio.setPALevel(RF24_PA_LOW);            // RF24_PA_MAX
  radio.setPayloadSize(sizeof(t_message));
  if (deviceID == 0) {
    radio.openWritingPipe(gameAddress);
    radio.openReadingPipe(1, mainAddress);
  } else {
    radio.openWritingPipe(mainAddress);
    radio.openReadingPipe(1, gameAddress);
  }
  radio.startListening();
  if (deviceID == 0) {
    Serial.println(F("Central Hub Ready"));
  } else {
    Serial.print(F("game controller #"));
    Serial.print(deviceID);
    Serial.println(F(" Ready!"));
  }
}

void loop() {
  int c = Serial.read();
  if ((c != -1) && (!isspace((char) c))) {
    message.UID = deviceID;
    message.description = (char) c ;
    sendMessage(message);
  }

  if (getMessage(message)) {
    Serial.print(F("Device with ID #")); Serial.print(message.UID);
    Serial.print(F(" sent command ")); Serial.println(message.description);
  }
}
