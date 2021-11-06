// HS2_SexRobotControllerFirmware v1.0
// Honey Select 2 Sex Robot Controller Firmware for the ESP32
// The open source sex robot is a variant (the micro-OSR2 model as some call it) of the OSR2+ by TempestVR @ https://www.patreon.com/tempestvr
// by hs2robotics - 11/2021

// ESP32 pin assignments for servos
#define leftServoPin 4
#define rightServoPin 13

// Servo zeros
// Change these to adjust arm positions
// (1500 = center)
#define leftServoZero 1450
#define rightServoZero 1575

// Servo frequency
#define servoFrequency 50

// Servo PWMs
#define leftServoPWM 0
#define rightServoPWM 2

// Servo Min/Max ledc offset values
#define ledcMinOffset -250
#define ledcMaxOffset 250

// Servo interval
#define servoInterval 1000000/servoFrequency

// Setup globals
long position = 0;
long positionOffset = 0;
String inputString = "";

// Setup function, runs once
void setup()
{
  // Start serial connection
  Serial.begin(115200);

  // Setup Servo PWM channels  
  // Left Servo
  ledcSetup(leftServoPWM, servoFrequency, 16);
  ledcAttachPin(leftServoPin, leftServoPWM);  
  // Right Servo
  ledcSetup(rightServoPWM, servoFrequency, 16);
  ledcAttachPin(rightServoPin, rightServoPWM);

  // Setup done
  Serial.println("Setup...");
}

// Loop runs continuously
void loop()
{  
  // Read in serial and parse input value
  // Input values should be from 0 to 100 and followed by a '\n'
  while (Serial.available() > 0)
  {
    byte inputByte = Serial.read();
    if (inputByte == '\n')
    {
      position = inputString.toInt();

      inputString = "";
    }
    else if (isDigit(inputByte))
    {
      inputString += (char)inputByte;
    }
  }

  // Constrain position values to the 0 to 100 range
  if (position < 0)
  {
    position = 0;
  }
  else if (position > 100)
  {
    position = 100;
  }

  // Map the current position to the min and max ledc offset values
  positionOffset = map(position, 0, 100, ledcMinOffset, ledcMaxOffset);

  // Move the left and right servos to the desired position
  ledcWrite(leftServoPWM, map(leftServoZero + positionOffset, 0, servoInterval, 0, 65535));
  ledcWrite(rightServoPWM, map(rightServoZero - positionOffset, 0, servoInterval, 0, 65535));
}
