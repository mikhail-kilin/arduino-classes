int encoder_pin_1 = 2; 
int encoder_pin_2 = 3; // пин энкодера
int rpm_1, rpm_2, pwm_1; // количество оборотов в минуту
volatile double pulses_1; // количество импульсов
volatile double pulses_2;
unsigned long timeold;// количество импульсов на оборот
double n_1 = 0;
double n_2 = 0;
double x = 0;
double y = 0;
double Q = 0;
double L = 12;
double r = 3.25;
double sum = 0;

const double pi = 3.1415;

#define HOLES_DISC 20

//моторы 
#define enA 10
#define enB 11
#define motorA1 6 //IN1
#define motorA2 7 //IN2
#define motorB1 8 //IN3 
#define motorB2 9 //IN4

float k_p = 0.7;
float err, u;
float rpm_setpoint = 200;
float pwm_min = 100;
float pwm_max = 255;
  
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
      double n_1 = pulses_1 / HOLES_DISC ;
      double n_2 = pulses_2 / HOLES_DISC;
      timeold = millis();
      pulses_1 = 0;
      pulses_2 = 0;

      double S_1 = n_1*pi*2*r;
      double S_2 = n_2*pi*2*r;
      double S = (S_1 + S_2)/2;
      Q = Q + (S_2 - S_1)/L;
      sum += S;
      x = x + S*cos(Q);
      y = y + S*sin(Q);

      Serial.print(sum);
      Serial.print(" ");
      Serial.print(Q/pi*180);
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
   analogWrite(enA, 255); //скорость мотора A
   analogWrite(enB, 255); //скорость мотора B 

  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
   
}
void loop()
{
  readEncoder();
}
