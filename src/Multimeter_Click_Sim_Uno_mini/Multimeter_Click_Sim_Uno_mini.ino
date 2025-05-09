// Multimeter Click Simulator for Arduino Uno Mini
// Simulates MCP3204 behavior using UART on main Serial pins
#define LED 13

bool adcJitter = true;

void setup() {
  randomSeed(analogRead(A0));
  Serial.begin(115200);
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
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("CMD")) {
      int channel = command.substring(4).toInt();
      uint16_t adcValue = simulateADCValue(channel);

      Serial.print("VAL ");
      Serial.println(adcValue);
    }
  }
} 
