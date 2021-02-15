#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

const char* ssid = "robocenter";
const char* password = "retnecobor";

#define  ledPin  5
WiFiServer server(80);
MDNSResponder mdns;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

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
  //MDNS.update();
}

void loop() {
  MDNS.update();
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int value = LOW;
  if (request.indexOf("/ON") != -1) {
    digitalWrite(ledPin, request.indexOf("/ON") != -1);
    value = HIGH;
  }
  if (request.indexOf("/OFF") != -1) {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }



  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Led pin is now: ");

  if (value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 5 ON<br>");
  client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 5 OFF<br>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");

}
