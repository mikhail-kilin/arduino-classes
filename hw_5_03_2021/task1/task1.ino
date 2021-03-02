#include <Wire.h>
#include <MPU6050.h>

const int in1 = 9;
const int in2 = 8;
const int in3 = 7;
const int in4 = 6;
const int e1 = 11;
const int e2 = 10;
float timeStep = 0.01;
double yaw;

MPU6050 mpu;

void setup() {
  Serial.begin(9600);

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  mpu.calibrateGyro();
  mpu.setThreshold(3);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(e1, 80);
  analogWrite(e2, 80);
  moveLeft(180);
  delay(1000);
  moveRight(90);
  delay(1000);
  moveLeft(45);
}

void loop() {
  
}

void moveLeft(double x) {
  yaw = 0;
  long timer = millis();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  while (abs(x-abs(yaw)) > 10) {
    Vector norm = mpu.readNormalizeGyro();
  
    yaw = yaw + norm.ZAxis * timeStep;

    Serial.println(yaw);
    
    delay(1000*timeStep - (millis() - timer));
    timer = millis();
  }
  stop();
}

void moveRight(double x) {
  yaw = 0;
  long timer = millis();
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  while (abs(x-abs(yaw)) > 10) {
    Vector norm = mpu.readNormalizeGyro();
  
    yaw = yaw + norm.ZAxis * timeStep;

    Serial.println(yaw);
    
    delay(1000*timeStep - (millis() - timer));
    timer = millis();
  }
  stop();
}

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
