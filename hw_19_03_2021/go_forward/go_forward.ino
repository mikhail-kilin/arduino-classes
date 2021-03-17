int encoder_pin_1 = 2; 
int encoder_pin_2 = 3; // пин энкодера
int rpm_1, rpm_2, pwm_1; // количество оборотов в минуту
volatile unsigned int pulses_1; // количество импульсов
volatile unsigned int pulses_2;
unsigned long timeold;// количество импульсов на оборот
double speed = 200;
#define HOLES_DISC 20

//моторы 
#define enA 10
#define enB 11
#define motorA1 6 //IN1
#define motorA2 7 //IN2
#define motorB1 8 //IN3 
#define motorB2 9 //IN4

const float k_p = 0.4;
const float k_i = 0;
const float k_d = 0;

float err, u;
float rpm_setpoint = 200;
float pwm_min = 100;
float pwm_max = 255;

int sum1 = 0;
int sum2 = 0;

int speed1 = speed;
int speed2 = speed;

int prev_error1;
int prev_error2;
long prev_time1;
long prev_time2;
  
void counter_1(){
   pulses_1++; //обновление счета импульсов. Т.е каждую 1 сек
}

void counter_2(){
   pulses_2++; //обновление счета импульсов. Т.е каждую 1 сек
}

void readEncoder(){
   if (millis() - timeold >= 1000) {
      detachInterrupt(digitalPinToInterrupt(encoder_pin_1)); //Не обрабатывать прерывания во время счёта
      detachInterrupt(digitalPinToInterrupt(encoder_pin_2)); //Не обрабатывать прерывания во время счёта
      rpm_1 = (pulses_1 * 60)/ (HOLES_DISC);
      rpm_2 = (pulses_2 * 60)/ (HOLES_DISC);
      timeold = millis();
      pulses_1 = 0;
      pulses_2 = 0;

      speed1 = speed1 + PID1(rpm_1);
      speed2 = speed2 + PID2(rpm_2);

      analogWrite(enA, speed1); //скорость мотора A
      analogWrite(enB, speed2); //скорость мотора A

//      Serial.print(speed1);
//      Serial.print(" ");
//      Serial.print(speed2);
//      Serial.println(" "); 
 
      Serial.print(rpm_1);
      Serial.print(" ");
      Serial.print(rpm_2);
      Serial.println(" "); 
      attachInterrupt(digitalPinToInterrupt(encoder_pin_1), counter_1, FALLING); //Перезагрузка процесса обработки прерываний
      attachInterrupt(digitalPinToInterrupt(encoder_pin_2), counter_2, FALLING); //Перезагрузка процесса обработки прерываний
   }
}

void setup()
{
   Serial.begin(9600);
   pinMode(encoder_pin_1, INPUT); //Прерывание 0 на цифровой линии 2
   pinMode(encoder_pin_2, INPUT); //Прерывание 0 на цифровой линии 2
   
   attachInterrupt(0, counter_1, FALLING); //Срабатывание триггера по спаду сигнала
   attachInterrupt(1, counter_2, FALLING); //Срабатывание триггера по спаду сигнала
   prev_time1 = millis();
   prev_error1 = 0;
   prev_time2 = millis();
   prev_error2 = 0;
   // Инициализация
   pulses_1 = 0;
   pulses_2 = 0;
   rpm_1 = 0;
   rpm_2 = 0;
   timeold = 0;

   pinMode(motorA1, OUTPUT);
   pinMode(motorA2, OUTPUT);
   pinMode(motorB1, OUTPUT);
   pinMode(motorB2, OUTPUT);
   analogWrite(enA, speed); //скорость мотора A
   analogWrite(enB, speed); //скорость мотора B 

  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
   
}

int PID1(int input) {
  long curr_time = millis();
  long dt = curr_time - prev_time1;
  int err = speed - input;
  sum1 += err * dt;
  int u = k_p * err + k_i *sum1 + k_d * (err - prev_error1)/dt;

  prev_error1 = err;
  prev_time1 = curr_time;

  return u;
}

int PID2(int input) {
  long curr_time = millis();
  long dt = curr_time - prev_time2;
  int err = speed - input;
  sum2 += err * dt;
  int u = k_p * err + k_i *sum2 + k_d * (err - prev_error2)/dt;

  prev_error1 = err;
  prev_time1 = curr_time;

  return u;
}

void loop()
{
  readEncoder();
   
}
