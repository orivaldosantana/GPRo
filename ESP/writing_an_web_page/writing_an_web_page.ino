/*
 * Baseado no tutorial do site instructables  
 * 
 * http://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/
 * 
 */

#include <ESP8266WiFi.h>

#define REFLECTANCE_SENSOR D8
#define TAM_READS 3


const char* ssid = "WIFI";
const char* password = "SENHA";

unsigned long readTimes[TAM_READS], startTime; 
unsigned long currentTime; 
int timePos = 0; 

bool previusSensorState = false; 
bool sensorState = false; 
 
int ledPin = 13; // GPIO13
WiFiServer server(80);
 
void setup() {
  startTime = 0; 

  for (int i = 0; i < TAM_READS; i++) {
    readTimes[i] = 0;
  }
  
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  /////////////////////////////////////////////////////////////////////////
  // código para registra o tempo (milisegundos) que o sensor foi ativado 
  if ( digitalRead(REFLECTANCE_SENSOR) == 0 ){
    currentTime = millis(); 
    sensorState = true;  
  }
  else {
    sensorState = false;
  } 

  // só registra quando o sensor passa do estado desativado para ativado  
  if ( ! previusSensorState && sensorState ) {
     // não permite gravar em posições maiores que o tamanho do vetor 
     if (timePos >= TAM_READS) {
        timePos = 0;
     }    
     readTimes[timePos] = currentTime; 
     Serial.println("... reading the sensor ... \n Value (ms): ");
     Serial.println(currentTime);
     timePos++;  
  }
  previusSensorState = sensorState;
  /////////////////////////////////////////////////////////////////////////
  
  
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
  if (request.indexOf("/START") != -1)  {
     startTime = millis(); 
  }
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.println("Sensor readings (milliseconds): <br>");

  for (int i = 0; i < TAM_READS; i++) {
    client.print(i);
    client.print(": ");
    client.println(readTimes[i]);
    client.println("<br>");
  }

  client.println("<br><br>");
  client.println("<a href=\"/START\"\"><button>Start experiment</button></a>");
  client.println("<a href=\"/UPDADE\"\"><button>Update informations</button></a>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
 
