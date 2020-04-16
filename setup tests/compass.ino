/*
Test code to see if your compass is correctly connected to the Adafruit compass. Moves the motor the amount you turn the compass
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

const float Pi = 3.14159;

//Change the pins to match the pins you use
const int stepPin = 5; 
const int dirPin = 22; 
const int enPin = 28;

float rotation = 180.0;
float heading = 0.0;

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void)
{
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);
  Serial.println("Magnetometer Test"); Serial.println("");

  /* Enable auto-gain */
  mag.enableAutoRange(true);

  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  /* Display some basic information on this sensor */
  displaySensorDetails();

  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
}

void getHeading() {
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);

  heading = (atan2((event.magnetic.y -= 11.5), event.magnetic.x) * 180) / Pi;

  if (heading < 0) {
    heading = 360 + heading;
  }
}

void checkRotation() {
  if (rotation > 360) {
    rotation -= 360;
  }
  if (rotation < 0) {
    rotation += 360;
  }
}

void displayInfo() {
  Serial.print("Rotation: ");
  Serial.print(rotation);
  Serial.print("  Heading: ");
  Serial.println(heading);
}

void loop(void)
{
  getHeading();

  if (!(rotation - heading < 2 && rotation - heading > -2)) {
    if (rotation - heading < -2) {
      digitalWrite(dirPin,HIGH);
      while (rotation - heading < -2) {
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(500); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(500);
        rotation += 1.8;
        checkRotation();
        getHeading();
        displayInfo();
      }
    } else {
      digitalWrite(dirPin,LOW);
      while (rotation - heading > 2) {
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(500); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(500);
        rotation -= 1.8;
        checkRotation();
        getHeading();
        displayInfo();
      }
    }
  }
}
