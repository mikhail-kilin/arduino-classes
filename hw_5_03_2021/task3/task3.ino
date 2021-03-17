#include <Wire.h>
#include <MPU6050.h>

const int in1 = 9;
const int in2 = 8;
const int in3 = 7;
const int in4 = 6;
const int e1 = 11;
const int e2 = 10;
float timeStep = 0.01;
long prev_time;
long prev_error;
float setpoint = 10;
const float k_p = 0.9;
const float k_i = 0.1;
const float k_d = 0.1;
float sum = 0;
const int trigPin = 12;
const int echoPin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

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

  delay(100);
}

void loop() {
  int distance = getDistance();
  float data = PID(distance);
  Serial.println(data);
  if (abs(distance - 10) > 0.5) {
    if (distance > 10) {
      forward(abs(data) + 40);
    } else {
      back(abs(data) + 40);
    }
  } else {
    stop();
  }
}

void forward(double data) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(e1, data);
  analogWrite(e2, data);
}

void back(double data) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
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

double getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  double duration = pulseIn(echoPin, HIGH);
  double distance = duration * 0.034 / 2;
  return distance;
}
