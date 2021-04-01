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

const float k_p = 0.13;
const float k_i = 0.01;
const float k_d = 0.023;

int last_error;

float err, u;

int sum1 = 0;
int sum2 = 0;

int speed1 = 70;
int speed2 = 70;

int prev_error1;
long prev_time1;

int fix;

void setup()
{
   Serial.begin(9600);
   pinMode(encoder_pin_1, INPUT); //Прерывание 0 на цифровой линии 2
   pinMode(encoder_pin_2, INPUT); //Прерывание 0 на цифровой линии 2

   prev_time1 = millis();
   prev_error1 = 0;
   last_error = 0;
   fix = 0;

   pinMode(motorA1, OUTPUT);
   pinMode(motorA2, OUTPUT);
   pinMode(motorB1, OUTPUT);
   pinMode(motorB2, OUTPUT);
   analogWrite(enA, 0); //скорость мотора A
   analogWrite(enB, 0); //скорость мотора B 

  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
   
}

int PID(int err) {
  long curr_time = millis();
  long dt = 1;

  sum1 += err * dt;
  if (sum1 > 1000) {
    sum1 = 0;
    }
  int u = k_p * err + k_i * sum1 + k_d * (err - prev_error1)/dt;

  prev_error1 = err;
  prev_time1 = curr_time;

  return u;
}

void go(int input) {
  analogWrite(enA, 73 + input); //скорость мотора A
  analogWrite(enB, 73 - input); //скорость мотора B
}

void loop()
{
  if(Serial.available() > 0) {
    last_error = Serial.parseInt();
  }
  int k = PID(last_error);
  go(k);
}
