#include "ESP8266WiFi.h"
   
const char* ssid = "KNEZEVIC"; //Unesite naziv vaše wifi mreže
const char* password = "034250637"; //Unesite vašu lozinku
   
int ledPin = 2; //Pin na kojem se LED dioda nalazi

String outputState = "iskljucena";

WiFiServer server(80); 
   
void setup() {
  Serial.begin(115200);
  delay(10);
   
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
   
  //Spajanje na WiFi mrežu
  Serial.print("Spajanje na ");
  Serial.println(ssid); 
  WiFi.begin(ssid, password);

  //Ispisivanje "." znaka sve dok se ne spoji na WiFi mrežu
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi spojen");
   
  //Pokreni server
  server.begin();
  Serial.println("Server pokrenut");
   
  //Ispiši IP adresu
  Serial.print("IP adresa: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
   
void loop() {
  // Proveri je li klijent spojen
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Čekaj dok klijent ne pošalje neke podatke
  Serial.println("Nov zahtev klijenta");
  while(!client.available()){
    delay(1);
  }
   
  // Pročitaj prvu liniju zahteva
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("");
  client.println("");

  // turns the GPIOs on and off  
  int value = LOW;
  if (request.indexOf("GET /2/on") != -1)  {
    Serial.println("GPIO 2 on");
    outputState = "ukljucena";
    digitalWrite(ledPin, HIGH);
  }
  if (request.indexOf("GET /2/off") != -1)  {
    Serial.println("GPIO 2 off");
    outputState = "iskljucena";
    digitalWrite(ledPin, LOW);
  }
   
  // Prikaz HTML web strane
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  // Stilizovanje dugmeta 
  client.println("<style>html { font-family: Helvetica; font-size: 22px; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println("body {  background-image: linear-gradient(to right, #739d8c, #729899, #7c929b, #878c94, #898989);}")
  client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; border-radius:12px;}");
  client.println(".button2 {background-color: #77878A;}</style></head>");
  
  client.println("<body><br><br><br><h1>ESP8266 Web Server</h1>");
  
  // Prikaz trenutnog stanja na web strani i prikaz ON/OFF dugmeta za GPIO 2  
  client.println("<p>LED dioda je sada: " + outputState + "</p>");       
  if (outputState=="iskljucena") {
    client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
  }

  delay(1);
  Serial.println("Kraj zahteva");
  Serial.println("");
}
