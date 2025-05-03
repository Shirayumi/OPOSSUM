// ESP32-2432S028R screen Test for OPOSSUM

#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <TFT_eSPI.h>
#include <floatToString.h>


// Custom UART pins
#define UART_RX 22
#define UART_TX 27

// ESP32-2432S028R touchscreen pins
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

uint8_t channel = 0;
const char* channelLabels[] = {"Current", "Voltage", "Resistance", "Capacitance"};
const char* valueLabels[] = {"A", "V", "Ohms", "uF"};

int prevChannel = 0;
bool touchFlag = false;

HardwareSerial SensorSerial(1);

SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  
  // Configure UART1 on custom pins
  SensorSerial.begin(115200, SERIAL_8N1, UART_RX, UART_TX);

  // Start the SPI for the touch screen and init the TS library
  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(mySpi);
  ts.setRotation(1);

  // Start the tft display and set it to black
  tft.init();
  tft.setRotation(1); //This is the display in landscape

  // Clear the screen before writing to it
  tft.fillScreen(TFT_BLACK);

  int x = 320 / 2; // center of display
  int y = 90;
  int fontSize = 4;
  tft.drawCentreString("OPOSSUM Ready", x, y, fontSize);
  fontSize -= 2;
  y += 20;
  tft.drawCentreString("Touch Screen to Start", x, y, fontSize);

  Serial.println("OPOSSUM Ready");
  delay(1000);
  tft.fillScreen(TFT_BLACK);
}

void sendCommand(uint8_t command) {
  SensorSerial.print("CMD ");
  SensorSerial.println(command);
}

uint16_t readResponse() {
  String response = SensorSerial.readStringUntil('\n');
  response.trim();

  if (response.startsWith("VAL")) {
    return response.substring(4).toInt();
  } else {
    return 0; // Error or invalid response
  }
}

void printValue(float value, uint16_t adcValue) {
  // Write output to Serial
  Serial.print("Raw ADC Value: ");
  Serial.println(adcValue);

  Serial.print("Approximate ");
  Serial.print(channelLabels[channel]);
  Serial.print(" : ");
  Serial.print(value, 3);
  Serial.print(" ");
  Serial.println(valueLabels[channel]);
  Serial.println();

  
  // Write output to screen

  if (channel != prevChannel) {   // clear the screen before printing new channel info
    tft.fillScreen(TFT_BLACK);
  }

  // Draw measurement type at top of screen
  tft.drawCentreString(channelLabels[channel], 160, 50, 4);
  // Draw calculated value with units
  char valueString[10];
  floatToString(value, valueString, 10, 3);
  tft.drawRightString(valueString, 175, 100, 4);
  tft.drawString(valueLabels[channel], 185, 100, 4);
  // Draw raw ADC data at bottom of screen
  char adcString[5]; 
  floatToString(adcValue, adcString, 5, 0);
  tft.drawString("Raw ADC Value:", 10, 210, 4);
  tft.drawString(adcString, 200, 210, 4);

  prevChannel = channel;
}

float calculateValue(uint16_t adcValue) {    // Convert the raw ADC value to the appropriate value and display
  
  float voltage = (adcValue * 2.048) / 4095.0;
  float capacitance_uF = map(adcValue, 0, 4095, 0, 2200) / 1000.0;
  float current = 0;
  float resistance = 0;

  switch (channel) {
    case 0:   // Current reading
      current = voltage / 0.1;
    return current;

    case 1:   // Voltage reading
    return voltage;

    case 2:   // Resistance reading
      resistance = voltage / 1.584 * 10000;
    return resistance;

    case 3:   // Capacitance reading
    return capacitance_uF;
  }
}

void loop() {
  sendCommand(channel); // Send a command via serial

  while (!SensorSerial.available()) {
    // wait for response
  }

  uint16_t adcValue = readResponse();
  printValue(calculateValue(adcValue), adcValue);

  if (ts.tirqTouched() && ts.touched() && !touchFlag) {
    channel += 1;
    touchFlag = true;
  }

  if (channel > 3) {   // Reset the channel counter
    channel = 0;
  }

  delay(500);


  if (!ts.tirqTouched() && !ts.touched()) {
    touchFlag = false;
  }
}
