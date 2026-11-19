#include <SPI.h>
#include "RF24.h"
#include <printf.h>

RF24 radio(9, 10);

const byte ADDR[] = {0x5E, 0x5B, 0xBE, 0x8F, 0x8B};

uint32_t counter = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Hello!");

  if (!radio.begin()) {
    Serial.println("Radio init failed!");
    while (true);
  }
  while (!radio.isChipConnected()) {
    Serial.println("Radio not connected...");
    delay(1000);
  }
  radio.setChannel(76);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.setCRCLength(RF24_CRC_16);
  radio.setAutoAck(true);
  radio.setRetries(5, 15);
  Serial.println("Radio connected!");

  radio.setStatusFlags(RF24_IRQ_NONE);

  radio.openWritingPipe(ADDR);
  radio.stopListening();

  printf_begin();
  radio.printDetails();
}

void loop() {
  counter++;
  Serial.print("Send ");
  Serial.print(counter);
  if (radio.write(&counter, sizeof(counter))) {
    Serial.println(" successful!");
  }
  else {
    Serial.println(" failed!");
  }
  radio.printDetails();
  delay(1000);
}
