#include <ESP8266WiFi.h>


#define motor1_a 3
#define motor1_b 0
#define knok     4


#define motor1_a_gpio 3
#define motor1_b_gpio 0
#define knok_gpio     2

const int degree = 90; //Количество градусов для поворота мешалки

String frontend = "<head><style> body{ background-size: 50px 50px; background-color: #ddd;background-image: -webkit-linear-gradient(transparent 50%, rgba(136, 136, 136, .2) 50%, rgba(136, 136, 136, .2)),-webkit-linear-gradient(0deg, transparent 50%, rgba(136, 136, 136, .2) 50%, rgba(136, 136, 136, .2));background-image: -moz-linear-gradient(transparent 50%, rgba(136, 136, 136, .2) 50%, rgba(136, 136, 136, .2)),-moz-linear-gradient(0deg, transparent 50%, rgba(136, 136, 136, .2) 50%, rgba(136, 136, 136, .2));background-image: linear-gradient(transparent 50%, rgba(136, 136, 136, .2) 50%, rgba(136, 136, 136, .2)),linear-gradient(90deg, transparent 50%, rgba(136, 136, 136, .2) 50%, rgba(136, 136, 136, .2));}</style></head>";

volatile int count_knok = 0;
unsigned long t = 0;
int knoks = 0;

const char* ssid = "robocenter";
const char* password = "retnecobor";
void ICACHE_RAM_ATTR knoking();

String page = "<meta charset='utf-8'><p>Покормить кота</p> <a href=\"run\"><button>Кормить</button></a>";

IPAddress ip(192, 168, 1, 200);
IPAddress geteway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80);

void motor_stop()
{
  digitalWrite(motor1_a_gpio, 0);
  digitalWrite(motor1_b_gpio, 0);
}

void motor_run()
{
  digitalWrite(motor1_a_gpio, 0);
  digitalWrite(motor1_b_gpio, 1);
}


int is_knok_0, is_knok = 1;

int how_knok(int degreeds)
{
  return degreeds / 90;
}

void knoking()
{
  if (millis() - t >= 1000)
  {
    count_knok++;
    t = millis();
  }
}

String redirect = "<script>window.location.href = 'http://";

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") + \
         String(ipAddress[1]) + String(".") + \
         String(ipAddress[2]) + String(".") + \
         String(ipAddress[3])  ;
}

void setup()
{
  Serial.begin(115200);

  pinMode(motor1_a, OUTPUT);
  pinMode(motor1_b, OUTPUT);
  pinMode(knok, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), knoking, FALLING);
  t = millis();
  motor_stop();
  knoks = how_knok(degree);

  WiFi.begin(ssid, password);
  WiFi.config(ip, geteway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //server.begin();
  String myIP = IpAddress2String(WiFi.localIP());
  redirect = redirect + myIP;
  redirect = redirect + "';</script>";
  Serial.println(WiFi.localIP());
  Serial.println("HTTP server started");

  server.begin();

}



void loop()
{

  motor_stop();
  count_knok != 0 ? count_knok = 0 : count_knok = 0;
  WiFiClient client = server.available();
  if (client)
  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    String request = client.readStringUntil('\r');
    if (request.indexOf("/run") != -1)
    {
      client.println("State: RUN");
      client.println("");
      client.println("<meta charset='utf-8'>");
      client.println(frontend);
      client.println("<center><h1>Покормить кота</h1></center>");
      //client.println("<a href=\"run\" style='text-decoration: none;'><button style='width:40%; height:30%; display:block; margin:auto; background-color: coral;'>Кормить</button></a>");
      yield();
      knoks = how_knok(degree);
      if (!digitalRead(2))
      {
        knoks++;
      }
      while ((count_knok < knoks) and client)
      {
        yield();
        delay(0);
        motor_run();

        Serial.print(count_knok);
        Serial.print(" ");
        Serial.print(digitalRead(knok_gpio));
        Serial.print('\n');
      }
      Serial.println("not yet");
      motor_stop();
      count_knok = 0;
      Serial.println("all right");
      client.println(redirect);
    }
    else if(request.indexOf("/") != -1)
    {
      client.println("State: WAIT");
      client.println("");
      client.println("<meta charset='utf-8'>");
      client.println(frontend);
      client.println("<center><h1>Покормить кота</h1></center>");
      client.println("<a href=\"run\" style='text-decoration: none;'><button style='width:40%; height:30%; display:block; margin:auto; background-color: coral;'>Кормить</button></a>");
    }

  }
}

}
