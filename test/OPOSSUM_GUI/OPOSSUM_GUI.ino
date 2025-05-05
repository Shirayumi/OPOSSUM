// ESP32-2432S028R screen Test for OPOSSUM

#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <floatToString.h>

// ESP32-2432S028R RGB LED pins
#define RED 4
#define GREEN 16
#define BLUE 17

// Custom UART pins on CN1 connector
#define UART_RX 22
#define UART_TX 27

// ESP32-2432S028R touchscreen pins
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

// Screen settings
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// font size settings
#define btn_font_size 3
#define reading_font_size 4
#define sub_font_size 2

// Button frame position and size
#define btn_frame_X 0
#define btn_frame_Y 180
#define btn_frame_W 320
#define btn_frame_H 60

// Current button zone size
#define I_btn_X btn_frame_X
#define I_btn_Y btn_frame_Y
#define I_btn_W (btn_frame_W / 4)
#define I_btn_H btn_frame_H

// Voltage button zone size
#define V_btn_X (btn_frame_W / 4)
#define V_btn_Y btn_frame_Y
#define V_btn_W (btn_frame_W / 4)
#define V_btn_H btn_frame_H

// Resistance button zone size
#define R_btn_X (btn_frame_W / 4 * 2)
#define R_btn_Y btn_frame_Y
#define R_btn_W (btn_frame_W / 4)
#define R_btn_H btn_frame_H

// Capacitance button zone size
#define C_btn_X (btn_frame_W / 4 * 3)
#define C_btn_Y btn_frame_Y
#define C_btn_W (btn_frame_W / 4)
#define C_btn_H btn_frame_H

TFT_eSPI tft = TFT_eSPI();
SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
HardwareSerial SensorSerial(1);

const char* channelLabels[] = {"Current", "Voltage", "Resistance", "Capacitance"};
const char* valueLabels[] = {"A", "V", "Ohms", "uF"};

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

uint8_t channel = 0;
bool touchFlag = false;


// BUTTON FUNCTIONS ---------------------------------------------------------------------------

// Draw all butttons
void drawButtons() {
  for (int i = 0; i < SCREEN_WIDTH; i += 80) {
    tft.fillRect(i + 1, btn_frame_Y + 1, (btn_frame_W / 4 - 2), (btn_frame_H - 2), TFT_WHITE);
  }
  
  tft.setTextColor(TFT_BLACK);

  tft.drawCentreString("I", I_btn_X + (I_btn_W / 2), I_btn_Y + (I_btn_H / 2), 4);
  tft.drawCentreString("V", V_btn_X + (V_btn_W / 2), V_btn_Y + (V_btn_H / 2), 4);
  tft.drawCentreString("R", R_btn_X + (R_btn_W / 2), R_btn_Y + (R_btn_H / 2), 4);
  tft.drawCentreString("C", C_btn_X + (C_btn_W / 2), C_btn_Y + (C_btn_H / 2), 4);
}

// Draw Current (I) button as green and select channel 0
void I_btn_selected() {
  drawButtons();
  tft.setTextColor(TFT_BLACK);

  tft.fillRect(I_btn_X + 1, I_btn_Y + 1, (I_btn_W - 2), (I_btn_H - 2), TFT_GREEN);
  tft.drawCentreString("I", I_btn_X + (I_btn_W / 2), I_btn_Y + (I_btn_H / 2), 4);
  channel = 0;
}

// Draw voltage button button as green and select channel 1
void V_btn_selected() {
  drawButtons();
  tft.setTextColor(TFT_BLACK);

  tft.fillRect(V_btn_X + 1, V_btn_Y + 1, (V_btn_W - 2), (V_btn_H - 2), TFT_GREEN);
  tft.drawCentreString("V", V_btn_X + (V_btn_W / 2), V_btn_Y + (V_btn_H / 2), 4);
  channel = 1;
}

// Draw Resistance button button as green and select channel 2
void R_btn_selected() {
  drawButtons();
  tft.setTextColor(TFT_BLACK);

  tft.fillRect(R_btn_X + 1, R_btn_Y + 1, (R_btn_W - 2), (R_btn_H - 2), TFT_GREEN);
  tft.drawCentreString("R", R_btn_X + (R_btn_W / 2), R_btn_Y + (R_btn_H / 2), 4);
  channel = 2;
}

// Draw Capacitance button button as green and select channel 3
void C_btn_selected() {
  drawButtons();
  tft.setTextColor(TFT_BLACK);

  tft.fillRect(C_btn_X + 1, C_btn_Y + 1, (C_btn_W - 2), (C_btn_H - 2), TFT_GREEN);
  tft.drawCentreString("C", C_btn_X + (C_btn_W / 2), C_btn_Y + (C_btn_H / 2), 4);
  channel = 3;
}
// END BUTTON FUNCTIONS


// SERIAL1 FUNCTIONS ---------------------------------------------------------------------------

// Send UART command
void sendCommand(uint8_t command) {
  SensorSerial.print("CMD ");
  SensorSerial.println(command);
}

// Receive UART response
uint16_t readResponse() {
  String response = SensorSerial.readStringUntil('\n');
  response.trim();

  if (response.startsWith("VAL")) {
    return response.substring(4).toInt();
  } else {
    return 0; // Error or invalid response
  }
}
// END SERIAL FUNCTIONS


// METER FUNCTIONS ---------------------------------------------------------------------------

// Write output to screen
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

  // clear the top of the screen before printing new channel info
  tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
  // Draw measurement type at top of screen
  tft.drawCentreString(channelLabels[channel], 160, 20, 4);

  // Draw calculated value with units
  char valueString[10];
  floatToString(value, valueString, 10, 3);
  tft.drawRightString(valueString, 175, 70, 4);
  tft.drawString(valueLabels[channel], 185, 70, 4);

  // Draw raw ADC data at bottom of screen above buttons
  char adcString[5]; 
  floatToString(adcValue, adcString, 5, 0);
  tft.drawString("Raw ADC Value:", 10, 150, 4);
  tft.drawString(adcString, 200, 150, 4);
}

// Convert the raw ADC value to the appropriate value based on channel
float calculateValue(uint16_t adcValue) {
  
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
// END METER FUNCTIONS


// SETUP FUNCTION ---------------------------------------------------------------------------
void setup() {

  // Turn off the LEDs
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, 1);
  digitalWrite(GREEN, 1);
  digitalWrite(BLUE, 1);

  Serial.begin(115200);

  // Configure UART1 on custom pins
  SensorSerial.begin(115200, SERIAL_8N1, UART_RX, UART_TX);

  // Start the SPI for the touchscreen and init the touchscreen
  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(mySpi);
  // Set the Touchscreen rotation in landscape mode
  ts.setRotation(1);

  // Start the tft display
  tft.init();
  // Set the TFT display rotation in landscape mode
  tft.setRotation(1);

  // Clear the screen before writing to it
  tft.fillScreen(TFT_BLACK);

  tft.drawCentreString("OPOSSUM Ready", 162, 100, 2);

  Serial.println("OPOSSUM Ready");
  delay(1000);
  tft.fillScreen(TFT_BLACK);

  // Draw buttons with channel 0 (Current) selected
  I_btn_selected();

}
// END SETUP FUNCTION


// MAIN LOOP ---------------------------------------------------------------------------
void loop() {

  sendCommand(channel); // Send a command via serial

  while (!SensorSerial.available()) {

  }


  uint16_t adcValue = readResponse();
  printValue(calculateValue(adcValue), adcValue);

  // Checks if a measurement button was touched and changes channel if required
  if (ts.tirqTouched() && ts.touched()  && !touchFlag) {
    // Prevents screen from flickering if the touch is held
    touchFlag = true;
    // Get Touchscreen points
    TS_Point p = ts.getPoint();
    // Calibrate Touchscreen points with map function to the correct width and height
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;


    if ((x > C_btn_X) && (x < SCREEN_WIDTH)) {
        if ((y > (C_btn_Y)) && (y <= (C_btn_Y + C_btn_H))) {
          Serial.println("C");
          tft.fillScreen(TFT_BLACK);
          C_btn_selected();
        }
    } else if ((x > R_btn_X) && (x < (R_btn_X + C_btn_Y))) {
        if ((y > (R_btn_Y)) && (y <= (R_btn_Y + R_btn_H))) {
          Serial.println("R");
          tft.fillScreen(TFT_BLACK);
          R_btn_selected();
        }
    } else if ((x > V_btn_X) && (x < (V_btn_X + R_btn_Y))) {
        if ((y > (V_btn_Y)) && (y <= (V_btn_Y + V_btn_H))) {
          Serial.println("V");
          tft.fillScreen(TFT_BLACK);
          V_btn_selected();
        }
    } else if ((x > I_btn_X) && (x < (I_btn_X + V_btn_Y))) {
        if ((y > (I_btn_Y)) && (y <= (I_btn_Y + I_btn_H))) {
          Serial.println("I");
          tft.fillScreen(TFT_BLACK);
          I_btn_selected();
        }
    }
  }

  delay(200);

  if (!ts.tirqTouched() && !ts.touched()) {
    touchFlag = false;
  }
}
