// CT-B-GONE, by Alexander Israelov

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
int motorPin = 26;

//8.5 left => -11 right
//we want 5 => -1.9
void setup(void) {
  Serial.begin(115200);

  pinMode(motorPin, OUTPUT);
  
  //while (!Serial)
  //  delay(10); // will pause Zero, Leonardo, etc until serial console opens

  //Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

double currTime = 0;
double typingStartTime = -1;
int pollingRate = 500;
int msToVibrate = minsToMs(15); //15 minutes

void loop() {
  currTime = currTime + pollingRate; //who needs a proper timer?

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  double rotationValue = a.acceleration.x;

  //If we don't have a timer set
  if(typingStartTime == -1)
  {
    //If we're starting to type
    if(isTypingPos(rotationValue))
    {
       typingStartTime = currTime;
    }
  }
  //if we DO have a timer
  else
  {
    if(currTime - typingStartTime > msToVibrate)
    {
       digitalWrite(motorPin, HIGH);
    }

    //If we're no longer typing, reset
    if(!isTypingPos(rotationValue))
    {
      typingStartTime = -1;
      digitalWrite(motorPin, 0);
    }
  }
  delay(pollingRate);
}

int minsToMs(int mins)
{
  return mins * 60000;
}

bool isTypingPos(double pos)
{
  return (pos > -6 && pos < 1);
}
