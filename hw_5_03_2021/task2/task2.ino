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
long prev_time;
long prev_error;
float setpoint = 0;
const float k_p = 0.9;
const float k_i = 0.3;
const float k_d = 0;
float sum = 0;

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

  prev_time = millis();
  prev_error = 0;
}

void loop() {
  long timer = millis();
  Vector norm = mpu.readNormalizeGyro();
  yaw = yaw + norm.ZAxis * timeStep;
  
  float data = PID(yaw);
  Serial.println(data);
  if (abs(yaw) > 3) {
    if (yaw > 0) {
      right(abs(data)+60);
    } else {
      left(abs(data)+60);
    }
  } else {
    stop();
  }

  delay(1000*timeStep - (millis() - timer));
}

void left(double data) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(e1, data);
  analogWrite(e2, data);
}

void right(double data) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(e1, data);
  analogWrite(e2, data);
}

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

float PID(float input) {
  long curr_time = millis();
  long dt = curr_time - prev_time;
  float err = setpoint - input;
  sum += err * dt;
  float u = k_p * err + k_i *sum + k_d * (err - prev_error)/dt;

  prev_error = err;
  prev_time = curr_time;

  return u;
}
