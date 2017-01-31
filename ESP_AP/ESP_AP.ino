
#include <ESP8266WiFi.h>
#include "FS.h"
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiAPPSK[] = "manchester";
const char* host = "evolution";


#define MOTOR1_A 16
#define MOTOR1_B 14

#define MOTOR2_A 12
#define MOTOR2_B 13

#define MOTOR3_A 1
#define MOTOR3_B 3

#define MOTOR4_A 14
#define MOTOR4_B 13

/////////////////////
// Pin Definitions //
/////////////////////

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
uint8_t last_socket_num;
int Wheel_FL,Wheel_FR,Wheel_BL,Wheel_BR; //Front Left, Front Right, Back Left, Back Right
void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();
  bool s_result=SPIFFS.begin();
  //Serial.println("SPIFFS opened: " + s_result);
  Dir dir = SPIFFS.openDir("/");
////Serial.println("Hello world");
//while (dir.next()) {
    //Serial.print(dir.fileName());
  //  File f = dir.openFile("r");
    //Serial.println(f.size());
//}
webSocket.begin();
//Serial.println("Websocket created ");
webSocket.onEvent(webSocketEvent);

//pinMode(MOTOR1_A,OUTPUT);
//pinMode(MOTOR1_B,OUTPUT);

//pinMode(MOTOR2_A,OUTPUT);
//pinMode(MOTOR2_B,OUTPUT);

pinMode(MOTOR3_A,OUTPUT); //motor 3 is OK
pinMode(MOTOR3_B,OUTPUT);

pinMode(MOTOR4_A,OUTPUT); //motor 4 is OK
pinMode(MOTOR4_B,OUTPUT);

digitalWrite(MOTOR1_A,LOW);
digitalWrite(MOTOR2_A,LOW);
digitalWrite(MOTOR3_A,LOW);
digitalWrite(MOTOR4_A,LOW);

digitalWrite(MOTOR1_B,LOW);
digitalWrite(MOTOR2_B,LOW);
digitalWrite(MOTOR3_B,LOW);
digitalWrite(MOTOR4_B,LOW);


}
//format bytes
String formatBytes(size_t bytes){
  if (bytes < 1024){
    return String(bytes)+"B";
  } else if(bytes < (1024 * 1024)){
    return String(bytes/1024.0)+"KB";
  } else if(bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0)+"MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0)+"GB";
  }
}

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path){
  //Serial.println("handleFileRead: " + path);
  if(path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}


void handleFileList() {
  if(!server.hasArg("dir")) {server.send(500, "text/plain", "BAD ARGS"); return;}
  
  String path = server.arg("dir");
  //Serial.println("handleFileList: " + path);
  Dir dir = SPIFFS.openDir(path);
  path = String();

  String output = "[";
  while(dir.next()){
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir)?"dir":"file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }
  
  output += "]";
  server.send(200, "text/json", output);
}

  
//#define USE_////Serial ////Serial

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
int c_index;
String command;
int speed1;
last_socket_num=num;

 ESP.wdtFeed();
    switch(type) {
        case WStype_DISCONNECTED:
            //Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                //Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
        // send message to client
        webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            //Serial.printf("[%u] get Text: %s\n", num, payload);
            command=String((char*)payload);

            //First, get speed
            c_index=command.indexOf(",");
            
            if (c_index>0)
            {
              speed1=command.substring(c_index+1).toInt();
              ////Serial.print("Got speed ");
              ////Serial.println(speed1);
            }
              Wheel_FL=0;
              Wheel_FR=0;
              Wheel_BL=0;
              Wheel_BR=0;
            if (command.indexOf("up")>0)
            {
              
              Wheel_FL=1000;
              Wheel_FR=1000;
              Wheel_BL=1000;
              Wheel_BR=1000;
            }
            else if (command.indexOf("down")>0)
            {
              Wheel_FL=-1000;
              Wheel_FR=-1000;
              Wheel_BL=-1000;
              Wheel_BR=-1000;            
            }
            else if (command.indexOf("right")>0)
            {
              Wheel_FL=500;
              Wheel_FR=-500;
              Wheel_BL=500;
              Wheel_BR=-500;
            }
            else if (command.indexOf("left")>0)
            {
              Wheel_FL=-500;
              Wheel_FR=500;
              Wheel_BL=-500;
              Wheel_BR=500;  
            }

 if (command.indexOf("0000")>=0)
 {
  Wheel_FL=0;
  Wheel_FR=0;
  Wheel_BL=0;
  Wheel_BR=0;
  }
/*////Serial.print("Wheel_FL ");
////Serial.println(Wheel_FL);
////Serial.print("Wheel_FR ");
////Serial.println(Wheel_FR);
////Serial.print("Wheel_BL ");
////Serial.println(Wheel_BL);
////Serial.print("Wheel_BR ");
////Serial.println(Wheel_BR);*/
            
SetMotors(Wheel_FL, Wheel_FR, Wheel_BL,  Wheel_BR);

              

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        case WStype_BIN:
            //Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
            hexdump(payload, lenght);

            // send message to client
            // webSocket.sendBIN(num, payload, lenght);
            break;
    }

}




void SetMotors(int FL, int FR, int BL, int BR)
{
  ESP.wdtFeed();
  if (FL==0)
  {
digitalWrite(MOTOR1_A,LOW);
digitalWrite(MOTOR1_B,LOW);
 }
 else if (FL>0)
 {
digitalWrite(MOTOR1_B,LOW);
digitalWrite(MOTOR1_A,HIGH);
  }
  else if (FL<0)
  {
digitalWrite(MOTOR1_A,LOW);
digitalWrite(MOTOR1_B,HIGH);
  }

//======FRONT RIGHT===========

  if (FR==0)
  {
digitalWrite(MOTOR2_A,LOW);
digitalWrite(MOTOR2_B,LOW);
 }
 else if (FR>0)
 {
  digitalWrite(MOTOR2_B,LOW);
digitalWrite(MOTOR2_A,HIGH);
  }
  else if (FR<0)
  {
digitalWrite(MOTOR2_A,LOW);
digitalWrite(MOTOR2_B,HIGH);
  }
//======BACK LEFT===========
    if (BL==0)
  {
digitalWrite(MOTOR4_A,LOW);
digitalWrite(MOTOR4_B,LOW);
 }
 else if (BL>0)
 {

 ////Serial.println("BL FWD");
digitalWrite(MOTOR4_B,LOW);
digitalWrite(MOTOR4_A,HIGH);

  }
  else if (BL<0)
  {
////Serial.println("BL BACK");
digitalWrite(MOTOR4_A,LOW);
digitalWrite(MOTOR4_B,HIGH);
  }

//======BACK RIGHT===========

    if (BR==0)
  {
   ////Serial.println("BR STOP");
digitalWrite(MOTOR3_A,LOW);
digitalWrite(MOTOR3_B,LOW);
 }
 else if (BR>0)
 {

 ////Serial.println("BR FWD");
digitalWrite(MOTOR3_B,LOW);
digitalWrite(MOTOR3_A,HIGH);
  }
  else if (BR<0)
  {
//Serial.println("BR BACK");
digitalWrite(MOTOR3_A,LOW);
digitalWrite(MOTOR3_B,HIGH);
  }
  
  
  
}

void loop() 
{
 webSocket.loop();
 server.handleClient();
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "EvoAllTerrain " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  IPAddress Ip(192, 168, 1, 1);
  IPAddress NMask(255, 255, 255, 0);


  WiFi.softAPConfig(Ip, Ip, NMask);
 if (!WiFi.softAP( AP_NameChar, WiFiAPPSK))
  {
   //Serial.println("WiFi.softAP failed.(Password too short?)");
   return;
  }
  IPAddress myIP = WiFi.softAPIP();
  //Serial.println();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);
  
  MDNS.begin(host);
  //Serial.print("Open http://");
  //Serial.print(host);
 //Serial.println(".local/edit to see the file browser");

   //SERVER INIT
  //list directory
  server.on("/list", HTTP_GET, handleFileList);
  //load editor
  server.on("/edit", HTTP_GET, [](){
    if(!handleFileRead("/edit.htm")) server.send(404, "text/plain", "FileNotFound");
  });
  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  //get heap status, analog input value and all GPIO statuses in one json call
  server.on("/all", HTTP_GET, [](){
    String json = "{";
    json += "\"heap\":"+String(ESP.getFreeHeap());
    json += ", \"analog\":"+String(analogRead(A0));
    json += ", \"gpio\":"+String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });
  server.begin();
  //Serial.println("HTTP server started");
}

void initHardware()
{
//Serial.begin(115200);
}
