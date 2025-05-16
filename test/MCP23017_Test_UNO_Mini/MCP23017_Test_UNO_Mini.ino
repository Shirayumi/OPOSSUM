#include <Wire.h>
#include <MCP23017.h>

#define MCP23017_ADDR 0x27
MCP23017 mcp = MCP23017(MCP23017_ADDR);

void setup() {
  Wire.begin();
  Serial.begin(115200);

  mcp.init();
  mcp.pinMode(0, 1); // Pin A1 as output
}


void loop() {
  mcp.digitalWrite(0, 1);
  delay(200);
  mcp.digitalWrite(0, 0);
  delay(200);
}