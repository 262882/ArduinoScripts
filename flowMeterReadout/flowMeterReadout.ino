/*
  Reads an analog input pin in and maps the result to a flow for Omron D6F 50A6
*/

// Assign pins
const int analogInPin = A0;  // Analog input pin that the sensor is attached to

// Initialise vars
int rawInput = 0;  // value read from the flowmeter in a digital form
float sensorVout = 0;  // value read converted to an analog 5v range form
float flowValue = 0;  // value read converted to a flow rate in normal l/min

void setup() {
  Serial.begin(9600);  // initialize serial communications at 9600 bps:
}

void loop() {
  // read the analog in value:
  rawInput = analogRead(analogInPin);
  // map it to the range of 5v:
  sensorVout = rawInput*(5/1023.0);  // ADC 10-bit
  // map it to voltage in l/min
  if (sensorVout>1) {
    flowValue = 2.685*pow(sensorVout, 2) - 4.225*sensorVout + 2.216; 
  } else {
    flowValue=0;
  }
  // print the results to the Serial Monitor:
  Serial.print("The flow rate in normal l/min = ");
  Serial.println(flowValue);

  // Allow ADC to settle
  delay(100);
}
