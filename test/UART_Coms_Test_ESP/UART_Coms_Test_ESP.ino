// ESP32 UART Coms Test for OPOSSUM
// Adapted for ESP32-2432S028R using UART on pins 22 (RX) and 27 (TX)

#define UART_RX 22
#define UART_TX 27

uint8_t channel = 0;
float current = 0;
float voltage = 0;
float resistance = 0;
float capacitance_uF = 0;

HardwareSerial SensorSerial(1);

void setup() {
  Serial.begin(115200);
  delay(1500);
  
  // Configure UART1 on custom pins
  SensorSerial.begin(115200, SERIAL_8N1, UART_RX, UART_TX);

  Serial.println("ESP32 UART Reader Ready");
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

void calculateValue(uint16_t adcValue) {    // Convert the raw ADC value to the appropriate value and display

  Serial.print("Raw ADC Value: ");
  Serial.println(adcValue);
  voltage = (adcValue * 2.048) / 4095.0;
  capacitance_uF = map(adcValue, 0, 4095, 0, 2200) / 1000.0;

  switch (channel) {
    case 0:   // Current reading
      current = voltage / 0.1;
      Serial.print("Approximate Current: ");
      Serial.print(current, 3);
      Serial.println(" A");
      break;
    case 1:   // Voltage reading
      Serial.print("Approximate Voltage: ");
      Serial.print(voltage, 3);
      Serial.println(" V");
      break;
    case 2:   // Resistance reading
      resistance = voltage / 1.584 * 10000;
      Serial.print("Approximate Resistance: ");
      Serial.print(resistance, 3);
      Serial.println(" ohms");
      break;
    case 3:   // Capacitance reading
      Serial.print("Approximate Capacitance: ");
      Serial.print(capacitance_uF, 3);
      Serial.println(" uF");
      break;
  }
  Serial.println();
}

void loop() {
  sendCommand(channel); // Send a command via serial

  while (!SensorSerial.available()) {
    // wait for response
  }

  uint16_t adcValue = readResponse();
  calculateValue(adcValue);

  if (channel == 3) {   // Reset the channel counter after reading the final channel
    channel = 0;
  } else {
    channel += 1;   // Move to the next channel
  }

  delay(3000);
}
