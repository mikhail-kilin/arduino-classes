int encoder_pin_1 = 2; 
int encoder_pin_2 = 3; // пин энкодера
int rpm_1, rpm_2, pwm_1; // количество оборотов в минуту
volatile unsigned int pulses_1; // количество импульсов
volatile unsigned int pulses_2;
unsigned long timeold;// количество импульсов на оборот
#define HOLES_DISC 20

//моторы 
#define enA 10
#define enB 11
#define motorA1 6 //IN1
#define motorA2 7 //IN2
#define motorB1 8 //IN3 
#define motorB2 9 //IN4

const float k_p = 0.5;
const float k_i = 0.07;
const float k_d = 0.04;

float err, u;

int sum1 = 0;
int sum2 = 0;

int speed1 = 100;

int prev_error1;
long prev_time1;

void setup()
{
   Serial.begin(9600);
   pinMode(encoder_pin_1, INPUT); //Прерывание 0 на цифровой линии 2
   pinMode(encoder_pin_2, INPUT); //Прерывание 0 на цифровой линии 2

   prev_time1 = millis();
   prev_error1 = 0;

   pinMode(motorA1, OUTPUT);
   pinMode(motorA2, OUTPUT);
   pinMode(motorB1, OUTPUT);
   pinMode(motorB2, OUTPUT);
   analogWrite(enA, 100); //скорость мотора A
   analogWrite(enB, 100); //скорость мотора B 

  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
   
}

int PID(int err) {
  long curr_time = millis();
  long dt = (curr_time - prev_time1)/1000;

  sum1 += err * dt;
  int u = k_p * err + k_i *sum1 + k_d * (err - prev_error1)/dt;

  prev_error1 = err;
  prev_time1 = curr_time;

  return u;
}

void loop()
{
  if(Serial.available() > 0) {
    speed1 = speed1 + PID(Serial.parseInt());

    analogWrite(enA, speed1); //скорость мотора A
  }
}
