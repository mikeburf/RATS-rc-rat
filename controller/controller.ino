#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[5] = {0x12, 0x34, 0x56, 0x78, 0x9A};

uint32_t counter = 0;

void setup() {
  Serial.begin(115200);

  while (!radio.begin()) {
    Serial.println(F("Radio initialization failed; retrying..."));
    delay(1000);
  }
  while (!radio.isChipConnected()) {
    Serial.println(F("Radio chip not connected; retrying..."));
    delay(1000);
  }

  radio.setChannel(76);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setCRCLength(RF24_CRC_16);
  radio.setAutoAck(true);
  radio.setRetries(5, 15);
  radio.setPayloadSize(sizeof(counter));
  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println(F("Controller ready."));
}

void loop() {
  ++counter;

  if (radio.write(&counter, sizeof(counter))) {
    Serial.print(F("Sent: "));
  } else {
    Serial.print(F("Send failed: "));
  }
  Serial.println(counter);

  delay(1000);
}
