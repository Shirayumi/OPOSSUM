// Multimeter Click Simulator for Arduino Mega (Slave Device) - UART Version
// Simulates MCP3204 behavior using UART on Serial1 (TX1 = pin 18, RX1 = pin 19)


void setup() {
  Serial.begin(9600); // For debug messages on Serial0 (USB)
  Serial1.begin(115200); // Communication on Serial1
  delay(1500);
  Serial.println("Simulated Multimeter Click Ready (UART Slave on Serial1)");
}

uint16_t simulateADCValue(uint8_t channel) {
  switch (channel) {
    case 0: return 125;    // Simulated Current (I)
    case 1: return 2000;   // Simulated Voltage (U)
    case 2: return 1000;   // Simulated Resistance (R)
    case 3: return 3000;   // Simulated Capacitance (CU)
    default: return 0;
  }
}

void loop() {
  if (Serial1.available()) {
    String command = Serial1.readStringUntil('\n');
    command.trim();

    if (command.startsWith("CMD")) {
      int channel = command.substring(4).toInt();
      uint16_t adcValue = simulateADCValue(channel);

      Serial1.print("VAL ");
      Serial1.println(adcValue);

      Serial.print("Simulating channel ");
      Serial.print(channel);
      Serial.print(" with value ");
      Serial.println(adcValue);
    }
  }
} 
