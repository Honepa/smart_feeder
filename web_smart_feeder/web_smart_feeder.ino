#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#define motor1_a 3
#define motor1_b 5
#define knok     4

const int degree = 360; //Количество градусов для поворота мешалки

int motor_speed = 170; // Скорость поворота

int count_knok = 0;
int knoks = 0;

const char* ssid = "robocenter";
const char* password = "retnecobor";

String page = "<meta charset='utf-8'><p>Покормить кота</p> <a href=\"run\"><button>Кормить</button></a>";
WiFiServer server(80);
MDNSResponder mdns;

void motor_stop()
{
  //analogWrite(motor1_a, 0);
  //analogWrite(motor1_b, 0);
  digitalWrite(14, 0);
}

void motor_run()
{
  //analogWrite(motor1_a, 0);
  //analogWrite(motor1_b, motor_speed);
  digitalWrite(14, 1);
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
  yield();
  delay(0);
  if (is_knok_0 > is_knok)
  {
    count_knok++;
  }
  is_knok_0 = is_knok;
  is_knok = !digitalRead(2);
  return count_knok;
}

int how_knok(int degreeds)
{
  return degreeds / 90;
}

int go()
{
  yield();
  count_knok = counter_knok();
  Serial.println(digitalRead(knok));
  if (count_knok <= knoks)
  {
    motor_run();
    go();
  }
  else
  {
    motor_stop();
    return 1;
  }
}

void setup()
{
  Serial.begin(115200);
  
  //pinMode(motor1_a, OUTPUT);
  pinMode(motor1_b, OUTPUT);
  pinMode(knok, INPUT_PULLUP);
  motor_stop();
  knoks = how_knok(degree);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //server.begin();
  if (MDNS.begin("esp8266", WiFi.localIP()))
  {
    Serial.println("MDNS responder started");
  }

  Serial.println("HTTP server started");
  MDNS.addService("http", "tcp", 80);
  server.begin();

}



void loop()
{
  MDNS.update();
  motor_stop();
  WiFiClient client = server.available();
  if (client)
  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<meta charset='utf-8'>");
    client.println("<p>Покормить кота</p>");
    client.println("<a href=\"run\"><button>Кормить</button></a>");
    String request = client.readStringUntil('\r');
    if(request.indexOf("/run") != -1)
    {
      yield();
      while(counter_knok() <= knoks * 2)
      {
        yield();
        delay(0);
        motor_run();
        Serial.println(counter_knok());
      }
      motor_stop();
      count_knok = 1;
      Serial.println("all right");
      client.println("<script>window.location.href = 'http://esp8266.local';</script>");
    }
  }

}
