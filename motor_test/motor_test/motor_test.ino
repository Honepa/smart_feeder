#define motor1_a 5
#define motor1_b 6
#define knok     4

const int degree = 360; //Количество градусов для поворота мешалки

int motor_speed = 170; // Скорость поворота



int count_knok = 0;
int knoks = 0;

void motor_stop()
{
  analogWrite(motor1_a, 0);
  analogWrite(motor1_b, 0);
}

void motor_run()
{
  analogWrite(motor1_a, 0);
  analogWrite(motor1_b, motor_speed);
}

int in_knok()
{
  int is_knok = 0;
  if (!digitalRead(knok))
  {
    is_knok = 1;
  }
  else
  {
    is_knok = 0;
  }
  return is_knok;
}

int is_knok_0, is_knok = 1;

int counter_knok()
{
  if (is_knok_0 > is_knok)
  {
    count_knok++;
  }
  is_knok_0 = is_knok;
  is_knok = !digitalRead(knok);
  return count_knok;
}

int how_knok(int degreeds)
{
  return degreeds / 90;
}

void setup()
{
  Serial.begin(9600);
  pinMode(knok,     INPUT_PULLUP);
  pinMode(motor1_a, OUTPUT);
  pinMode(motor1_b, OUTPUT);
  knoks = how_knok(degree);
}

void loop()
{
  //int is_knok = in_knok();
  count_knok = counter_knok();
  Serial.print(in_knok());
  Serial.print("\t");
  Serial.print(count_knok);
  Serial.print("\n");
  if(count_knok <= knoks)
  {
    motor_run();
  }
  else
  {
    motor_stop();
  }
  
}
