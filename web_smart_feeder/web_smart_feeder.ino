#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define motor1_a D5
#define motor1_b D3
#define knok     D4

const int degree = 360; //Количество градусов для поворота мешалки

int motor_speed = 170; // Скорость поворота

int count_knok = 0;
int knoks = 0;

const char* ssid = "robocenter";
const char* password = "retnecobor";

String page = "<meta charset='utf-8'><p>Покормить кота</p> <a href=\"run\"><button>Кормить</button></a>";
ESP8266WebServer server(80);
MDNSResponder mdns;

void motor_stop()
{
  //analogWrite(motor1_a, 0);
  //analogWrite(motor1_b, 0);
  digitalWrite(motor1_b, 0);
}

void motor_run()
{
  //analogWrite(motor1_a, 0);
  //analogWrite(motor1_b, motor_speed);
  digitalWrite(motor1_b, 1);
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

void go()
{
  
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
  }
}

void setup()
{
  Serial.begin(115200);

  //pinMode(motor1_a, OUTPUT);
  pinMode(motor1_b, OUTPUT);
  pinMode(knok, INPUT_PULLUP);
  
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

  server.on("/", []()
  {
    server.send(200, "text/html", page);
  });

  server.on("/run", []()
  {
    //digitalWrite(D5, 1);
    go();
    yield();
    server.send(200, "text/html", page);
    
  });

  Serial.println("HTTP server started");
  MDNS.addService("http", "tcp", 80);
  server.begin();

}

void loop()
{
  MDNS.update();
  server.handleClient();
}
