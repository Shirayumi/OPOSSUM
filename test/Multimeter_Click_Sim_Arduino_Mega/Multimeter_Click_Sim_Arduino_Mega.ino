// Multimeter Click Simulator for Arduino Mega
// Simulates MCP3204 behavior using UART on Serial1 (TX1 = pin 18, RX1 = pin 19)

bool adcJitter = true;

void setup() {
  randomSeed(analogRead(A0));
  Serial.begin(9600); // For debug messages on Serial0 (USB)
  Serial1.begin(115200); // Communication on Serial1
  delay(1500);
  Serial.println("Simulated Multimeter Click Ready (UART on Serial1)");
}

uint16_t simulateADCValue(uint8_t channel) {
  int baseValue = 0;
  int value = 0;

  switch (channel) {
    case 0: baseValue = 120; break;    // Current
    case 1: baseValue = 2000; break;   // Voltage
    case 2: baseValue = 1000; break;   // Resistance
    case 3: baseValue = 3000; break;   // Capacitance
    default: baseValue = 0; break;
  }

  if (adcJitter) {
    int jitter = random(-20, 21); // jitter between -10 and +10
    value = baseValue + jitter;
  }
  return constrain(value, 0, 4095); // ensure valid 12-bit range
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
