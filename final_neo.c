#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#define X_PULSE 6
#define Y_PULSE 9 
#define X_DIR 10
#define Y_DIR 12
#define Z_DIR 12
#define NEO 8

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, NEO, NEO_GRB + NEO_KHZ800);

  int color_no=0;


/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");
  strip.begin();
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  pinMode(6, OUTPUT); // X pulse
  pinMode(9, OUTPUT);  // Y Pulse
  pinMode(10, OUTPUT);  // X direction
  pinMode(12, OUTPUT);  // Y Direction

}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  delay(500);
  //D6 and D9 used as PWM output as Pulses for X and Y Stepper motor
  //D10 and D12 used as Stepper motor diretion for X and Y
  color_no=0x0000;
  if(event.acceleration.x>0)
  { 
    digitalWrite(X_DIR, HIGH); //Forward DIRECTION
    Serial.print("\nStepper Motor X in Forward Direction\n");
    //strip.setPixelColor(0,strip.Color(0,255,0));//red,green,blue));
    //strip.show();
    color_no=0x04;
  }
  else
  {
    digitalWrite(X_DIR, LOW); // Reverse Direction
    Serial.print("Stepper Motor X in Reverse Direction\n");
    //strip.setPixelColor(0,strip.Color(255,0,0));//red,green,blue));
    //strip.show();
     color_no=0x00;
  }
  //Serial.print("x color added\n"); Serial.print(color_no);Serial.print("  ");
  if(event.acceleration.y>0)
  { 
    digitalWrite(Y_DIR, HIGH); //Forward DIRECTION
    Serial.print("Stepper Motor Y in Forward Direction\n");
    //strip.setPixelColor(0,strip.Color(0,255,255));//red,green,blue));
    //strip.show();
     color_no=color_no|0x02;
  }
  else
  {
    digitalWrite(Y_DIR, LOW); // Reverse Direction
    Serial.print("Stepper Motor Y in Reverse Direction\n");
    color_no=color_no|0x00;
  }
 // Serial.print("y color added\n"); Serial.print(color_no);Serial.print("  ");
  if(event.acceleration.z>0)
  { 
    digitalWrite(Z_DIR, HIGH); //Forward DIRECTION
    Serial.print("Stepper Motor Z in Forward Direction\n");
    color_no=color_no|0x01;
  }
  else
  {
    digitalWrite(Z_DIR, LOW); // Reverse Direction
    Serial.print("Stepper Motor Z in Reverse Direction\n");
    color_no=color_no|0x00;
  }
 Serial.print("Color Code No:\n"); Serial.print(color_no);Serial.print("  ");
  switch(color_no)
  {
  case 0:  strip.setPixelColor(0,strip.Color(255,0,0)); break;  //red  // X,Y,Z Reverse 
  case 1:  strip.setPixelColor(0,strip.Color(255,125,0)); break;  //Orange // X,Y Rev Z forward
  case 2:  strip.setPixelColor(0,strip.Color(255,255,0)); break;  //Yellow  //X,Z Rev Y Forward
  case 3:  strip.setPixelColor(0,strip.Color(0,255,0)); break; //Green  //X Rev Y,Z Forward
  case 4:  strip.setPixelColor(0,strip.Color(0,255,255)); break; //cyan  //X Forward Y,Z Reverse
  case 5:  strip.setPixelColor(0,strip.Color(0,0,255)); break; //Blue   //X,Z Forward Y Reverse
  case 6:  strip.setPixelColor(0,strip.Color(255,0,255)); break; //Magenta  //X,Y Forward Z Rev
  case 7:  strip.setPixelColor(0,strip.Color(255,255,255)); break; //white  //X,Y,Z Forward
  default: strip.setPixelColor(0,strip.Color(0,0,0)); break; //black
  }
  strip.show();
  delay(1000);
}