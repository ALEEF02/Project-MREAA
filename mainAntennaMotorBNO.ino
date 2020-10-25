// Main code to interact with Orbitron and move the motors into position. Uses BNO005

#include <SoftwareSerial.h>
#include <StringSplitter.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <WString.h>
#include "quaternionFilters.h"

// Setup secondary serial through a second Arduino for Orbitron communication
SoftwareSerial secondSer(53, 52);

uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

const float Pi = 3.14159;

//Change the pins to match the pins you use. A = Azumith, E = Elevation
const int stepPinA = 5; 
const int dirPinA = 23; 
const int enPinA = 25;
const int stepPinE = 4; 
const int dirPinE = 22; 
const int enPinE = 24;

/* For LED
const int DI = 12;
const int RW = 11;
const int DB[] = {3, 4, 5, 6, 7, 8, 9, 10};// use array to select pin for bus
const int Enable = 2;
*/

double Mag_x, Mag_y, Mag_z, Mag_pitch, Mag_roll;      

double az = 0.0;
double el = 0.0; // Initial elevation and azumith
double heading = 0.0;
double elevation = 8.0;

// Toggle Motor Usage
boolean on = true;

/* LCD display
 *  
void LcdCommandWrite(int value) {
  // define all pins
  int i = 0;
  for (i=DB[0]; i <= DI; i++) // assign value for bus
    {
       digitalWrite(i,value & 01);// for 1602 LCD, it uses D7-D0( not D0-D7) for signal identification; here, it’s used for signal inversion. 
       value >>= 1;
    }
  digitalWrite(Enable,LOW);
  delayMicroseconds(1);
  digitalWrite(Enable,HIGH);
  delayMicroseconds(1);  // wait for 1ms
  digitalWrite(Enable,LOW);
  delayMicroseconds(1);  // wait for 1ms
}

void LcdDataWrite(int value) {
  // initialize all pins
  int i = 0;
  digitalWrite(DI, HIGH);
  digitalWrite(RW, LOW);
  for (i=DB[0]; i <= DB[7]; i++) {
       digitalWrite(i,value & 01);
       value >>= 1;
  }
  digitalWrite(Enable,LOW);
  delayMicroseconds(1);
  digitalWrite(Enable,HIGH);
  delayMicroseconds(1);
  digitalWrite(Enable,LOW);
  delayMicroseconds(1);  // wait for 1ms
}

void writeToLCD(String input) {
  LcdCommandWrite(0x01);  // clear the screen, cursor position returns to 0  
  delay(10); 
  LcdCommandWrite(0x80); 
  delay(10);
  int inLength = input.length();

  char charArray[inLength];
  strcpy(charArray, input.c_str());
  int i = 0;
  for (i = 0; i < inLength; i++) {
    if (i == 15) {
      delay(10);
      LcdCommandWrite(0xc0);  // set cursor position at second line, second position
      delay(10); 
    }
    LcdDataWrite(charArray[i]);
  }
}
*/

void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
 
  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }
 
  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.println(mag, DEC);
}

void checkGoodCal(void) {
  adafruit_bno055_offsets_t offsets;
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  if (system == 3 && gyro == 3 && mag == 3) {
    bno.getSensorOffsets(offsets);
    Serial.print("Good Calibration: ");
    Serial.print(offsets.mag_offset_x);
    Serial.print("\t");
    Serial.print(offsets.mag_offset_y);
    Serial.print("\t");
    Serial.println(offsets.mag_offset_z);
  }
}

void getHeading() {
  sensors_event_t orientationData, magnetometerData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  Mag_x = magnetometerData.magnetic.x;
  Mag_y = magnetometerData.magnetic.y;
  Mag_z = magnetometerData.magnetic.z;
  Mag_pitch = orientationData.orientation.y * DEG_TO_RAD;
  Mag_roll = orientationData.orientation.z * DEG_TO_RAD;
  Mag_x = Mag_x * cos(Mag_pitch) + Mag_y * sin(Mag_roll) * sin(Mag_pitch) - Mag_z * cos(Mag_roll) * sin(Mag_pitch);
  Mag_y = Mag_y * cos(Mag_roll) + Mag_z * sin(Mag_roll);
  heading = (atan2(Mag_y, Mag_x) * 180) / Pi;
  heading += 11.6;
  heading += 180;
  if (heading >= 360) {
    heading -= 360;
  }
}

void getElevation() {
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  elevation = double(-orientationData.orientation.y);
}

void displayInfo() {
  Serial.print("Sat AZ: ");
  Serial.print(az);
  Serial.print("  El: ");
  Serial.println(el);
  Serial.print("Ant AZ: ");
  Serial.print(heading);
  Serial.print("  El: ");
  Serial.println(elevation);
  Serial.println();
  
  /* LCD Display
  
  String displayText = "sAZ:";
  displayText += String(az);
  displayText += "EL:";
  displayText += String(el);
  writeToLCD(displayText);*/
}

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  Serial.begin(115200);
  while(!Serial){};
  delay(100);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Starting Project MREAA...");
  Serial.println();

  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  delay(1000);

  // Start connection to Arduino UNO board for Orbitron communitcation
  secondSer.begin(9600);
  secondSer.setTimeout(50);

  // Sets the two pins as Outputs
  pinMode(stepPinA,OUTPUT); 
  pinMode(dirPinA,OUTPUT);
  pinMode(enPinA,OUTPUT);
  pinMode(stepPinE,OUTPUT); 
  pinMode(dirPinE,OUTPUT);
  pinMode(enPinE,OUTPUT);
  digitalWrite(enPinA,LOW);
  digitalWrite(enPinE,LOW);
  
  getHeading();
  getElevation();

  //while(secondSer.available() <= 0);

  /* For LCD
  int i = 0;
  for (i=Enable; i <= DI; i++) {
     pinMode(i,OUTPUT);
  }
  delay(100);  // initialize LCD after a brief pause for LCD control
  LcdCommandWrite(0x38);  // select as 8-bit interface, 2-line display, 5x7 character size 
  delay(64);                      
  LcdCommandWrite(0x38);  // select as 8-bit interface, 2-line display, 5x7 character size 
  delay(50);                      
  LcdCommandWrite(0x38);  // select as 8-bit interface, 2-line display, 5x7 character size             
  delay(20);                      
  LcdCommandWrite(0x06);  // set input mode, auto-increment, no display of shifting
  delay(20);                      
  LcdCommandWrite(0x0E);  // display setup,turn on the monitor, cursor on, no flickering
  delay(20);                      
  LcdCommandWrite(0x01);  // clear the screen, cursor position returns to 0
  delay(100);                      
  LcdCommandWrite(0x80);  //  display setup, turn on the monitor, cursor on, no flickering
  
  delay(20);
  writeToLCD("SerialAvalible");*/
}

/*void processIncomingByte (const byte inByte)
{
  static unsigned int input_pos = 0;

  switch (inByte) {
    case '\n':   // end of text
      incString [input_pos] = 0;  // terminating null byte
       // reset buffer for next time
      input_pos = 0;  
      break;
    case '\r':   // discard carriage return
      break;
    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (11))
        incString [input_pos++] = inByte;
      break;
   }
}*/


char incString[11];
int byteNum = 0;
char terminate = '\n';
char item[6];

void loop(void)
{
  secondSer.listen();
  if (secondSer.available() > 6) {   
    // Use a C String to prevent the fragmentation of the heap
    memset(incString, 0, sizeof incString);
    memset(item, 0, sizeof item);
    byteNum = 0;
    
    // Read the incoming bytes
    Serial.println("Second serial avalible");
    while(secondSer.available() > 0) {
      char inC = secondSer.read();
      if (inC != terminate) {
        incString[byteNum] = inC;
      } else {
        incString[byteNum] = '\0';
        break;
      }
      byteNum++;
    }
    
    Serial.print("Got Message: ");
    Serial.println(incString);

    StringSplitter *splitter = new StringSplitter(incString, ';', 2);
    
    for(int i = 0; i < 2; i++){
      item[5] = '\0';
      strcpy(item, splitter->getItemAtIndex(i).c_str());
      if (i == 0) {
        az = atof(item);
      } else {
        el = atof(item);
      }
    }

    //Free the splitter pointer. Fixes a large memory leak. I am aware there is also another memory leak and I am looking into it
    free(splitter);
    getHeading();
    getElevation();
    displayInfo();
    delay(10);
  } else {
    getHeading();
    getElevation();
    displayInfo();
  }
  
  if (on == true) {
    
    // Check and move to elevation
    if (!(elevation - el < 5 && elevation - el > -5) && (el > 5 && el < 70)) {
      int spinCT = 0;
      int speedMotor = 10000;
      if (elevation - el < -5) {
        digitalWrite(dirPinE,HIGH);
        Serial.println("Moving up...");
        while (elevation - el < -5 && el < 70) {
          Serial.println("Elevation: " + String(elevation) + ", Dest: " + String(el));
          for (int i = 0; i < 200; i++) {
            digitalWrite(stepPinE,HIGH); 
            delayMicroseconds(speedMotor); 
            digitalWrite(stepPinE,LOW); 
            delayMicroseconds(speedMotor);
          }
          spinCT++;
          Serial.println("Getting elevation...");
          getElevation();
          displayInfo();
          
          if (spinCT == 2) {
            Serial.println("Speeding up...");
            speedMotor = 5000;
          }
          if (spinCT == 4) {
            Serial.println("Speeding up again...");
            speedMotor = 2500;
          }
        }
      } else {
        digitalWrite(dirPinE,LOW);
        Serial.println("Moving down...");
        while (elevation - el > 5 && el > 5) {
          Serial.println("Elevation: " + String(elevation) + ", Dest: " + String(el));
          for (int i = 0; i < 200; i++) {
            digitalWrite(stepPinE,HIGH); 
            delayMicroseconds(speedMotor); 
            digitalWrite(stepPinE,LOW); 
            delayMicroseconds(speedMotor);
          }
          spinCT++;
          Serial.println("Getting elevation...");
          getElevation();
          displayInfo();
          
          if (spinCT == 2) {
            Serial.println("Speeding up...");
            speedMotor = 5000;
          }
          if (spinCT == 4) {
            Serial.println("Speeding up again...");
            speedMotor = 2500;
          }
        }
      }
    }
  
  
    // Check and move to azumith
    if (!((heading - az < 5 && heading - az > -5) || (heading - az > 355 || heading - az < -355)) && el > 5) {
      int spinCT = 0;
      int speedMotor = 80000;
      if ((heading - az < -5 && heading - az > -180) || (heading - 360 - az > -180)) {
        digitalWrite(dirPinA,HIGH); // CLOCKWISE!
        Serial.println("Moving clockwise...");
        while (heading - az < -5 || (heading - az < 355 && heading - az > 180)) {
          Serial.println("Heading: " + String(heading) + ", Dest: " + String(az));
          for (int i = 0; i < 14; i++) {
            digitalWrite(stepPinA,HIGH); 
            delayMicroseconds(speedMotor); 
            digitalWrite(stepPinA,LOW); 
            delayMicroseconds(speedMotor);
          }
          spinCT++;
          Serial.println("Getting heading...");
          getHeading();
          displayInfo();
          if (spinCT == 6) {
            Serial.println("Speeding up...");
            speedMotor = 40000;
          }
        }
      } else {
        digitalWrite(dirPinA,LOW); // COUNTER-CLOCKWISE!
        Serial.println("Moving counterclockwise...");
        while (heading - az > 5  || (heading - az > -355 && heading - az < -180)) {
          Serial.println("Heading: " + String(heading) + ", Dest: " + String(az));
          for (int i = 0; i < 14; i++) {
            digitalWrite(stepPinA,HIGH); 
            delayMicroseconds(speedMotor); 
            digitalWrite(stepPinA,LOW); 
            delayMicroseconds(speedMotor);
          }
          spinCT++;
          Serial.println("Getting heading...");
          getHeading();
          displayInfo();
          if (spinCT == 6) {
            Serial.println("Speeding up...");
            speedMotor = 40000;
          }
        }
      }
    }

  }

  delay(1000);
}
