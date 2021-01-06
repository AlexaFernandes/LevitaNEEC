#include <ESP8266WiFi.h>
#include "./DNSServer.h"
#include <ESP8266WebServer.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int screenWidth = 16;
int screenHeight = 2;
//variáveis de colocação de caracteres
int stringStart, stringStop = 0;
int scrollCursor = screenWidth;

String line1 = webServer.arg("fname");
String line2 = "   LEVITANEEC   ";

String mainHTML = "<html lang=\"pt\"> <head> <meta charset=\"utf-8\"/> <title>Test</title> </head> <body> <h1 style=\"text-align:center\">LCD</h1> <form style=\"text-align:center\" action=\"/print\" method=\"post\"> <h2><label for=\"fname\">Text:</label></h2><br> <input type=\"text\" placeholder=\"Insert text here\" id=\"fname\" name=\"fname\"><br> <input type=\"submit\" value=\"Submit\"> </form> <h2 style=\"text-align:center\">Controlador</h2> <div style=\"text-align:center\"> <form action=\"/moveup\" method=\"POST\" style=\"display: inline-block;\"><input type=\"submit\" style=\"font-size: 1.3rem; transform: rotate(-90deg);\" value=\"➜\"></form> </div> <div style=\"text-align:center\"> <form action=\"/moveleft\" method=\"POST\" style=\"display: inline-block;\"><input type=\"submit\" style=\"font-size: 1.3rem; transform: rotate(180deg);\" value=\"➜\"></form> <form action=\"/movedown\" method=\"POST\" style=\"display: inline-block;\"><input type=\"submit\" style=\"font-size: 1.3rem; transform: rotate(90deg);\" value=\"➜\"></form> <form action=\"/moveright\" method=\"POST\" style=\"display: inline-block;\"><input type=\"submit\" style=\"font-size: 1.3rem;;\" value=\"➜\"></form> </div> </body></html>";

String popup_mainHTML  = "<html lang=\"pt\"><head> <meta charset=\"utf-8\" /> <title>Test</title></head><body onload=\"myFunction()\"> <h1 style=\"text-align:center\">LCD</h1> <form style=\"text-align:center\" action=\"/print\" method=\"post\"> <h2><label for=\"fname\">Text:</label></h2><br> <input type=\"text\" placeholder=\"Insert text here\" id=\"fname\" name=\"fname\"><br> <input type=\"submit\" value=\"Submit\"> </form> <h2 style=\"text-align:center\">Controlador</h2> <div style=\"text-align:center;padding-bottom: 1vh;\"> <form action=\"/moveup\" method=\"POST\" style=\"display: inline-block;\"><input type=\"submit\" style=\"font-size: 1.3rem; transform: rotate(-90deg);\" value=\"➜\"></form> </div> <div style=\"text-align:center\"> <form action=\"/moveleft\" method=\"POST\" style=\"display: inline-block;\"><input type=\"submit\" style=\"font-size: 1.3rem; transform: rotate(180deg);\" value=\"➜\"></form> <form action=\"/movedown\" method=\"POST\" style=\"display: inline-block;\"><input type=\"submit\" style=\"font-size: 1.3rem; transform: rotate(90deg);\" value=\"➜\"></form> <form action=\"/moveright\" method=\"POST\" style=\"display: inline-block;\"><input type=\"submit\" style=\"font-size: 1.3rem;\" value=\"➜\"></form> </div> <script>function myFunction() { alert(\"Message changed\");}</script></body></html>";

void handleRoot() {
  webServer.send(200, "text/html", mainHTML);
}

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("insert LevitaNEEC text");
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  
  //webServer.on("/",HTTP_GET, handleRoot);
  webServer.on("/print", HTTP_POST, changeLCD);
  /*webServer.on("/moveup", HTTP_POST, moveUP);
  webServer.on("/moveright", HTTP_POST, moveRIGHT);
  webServer.on("/movedown", HTTP_POST, moveDOWN);
  webServer.on("/moveleft", HTTP_POST, moveLEFT);*/
  
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", mainHTML);
  });

  webServer.begin();
}

void loop() {
  //scroll code
  if((webServer.hasArg("fname") && webServer.arg("fname") != NULL)){
    line1=webServer.arg("fname");
    //reset 
    stringStart=0;
    stringStop=0;
    scrollCursor=screenWidth;
  }
  Serial.print(line1);
  if(line1 != NULL){
    if(line1.length()>16){
        lcd.setCursor(scrollCursor, 1);
        lcd.print(line1.substring(stringStart,stringStop));
        lcd.setCursor(0, 0);
        lcd.print(line2);
        delay(300);
        lcd.clear();
        if(stringStart == 0 && scrollCursor > 0){
          scrollCursor--;
          stringStop++;
        } else if (stringStart == stringStop){
          stringStart = stringStop = 0;
          scrollCursor = screenWidth;
        } else if (stringStop == line1.length() && scrollCursor == 0) {
          stringStart++;
        } else {
          stringStart++;
          stringStop++;
        }
    }
    else{
      lcd.setCursor(0, 0);
      lcd.print(line2);
      lcd.setCursor(0, 1);
      lcd.print(line1);
    }
  }
  else{
    lcd.setCursor(0, 0);
    lcd.print(line2);
  }
  dnsServer.processNextRequest();
  webServer.handleClient();
  
}

void changeLCD(){
  if( !webServer.hasArg("fname") || webServer.arg("fname") == NULL) {       // If the POST request doesn't have any text data
    webServer.send(400, "text/html", "<h1>400: Invalid Request</h1><form action=\"/\" method=\"POST\" style=\"display: inline-block;\"><input type=\"submit\" style=\"font-size: 1.3rem;\" value=\"Voltar para trás\"></form>");         // The request is invalid, so send HTTP status 400
    return;
  }
  /*if(webServer.arg("fname").length() > 16){
    webServer.send(200, "text/html", popup_mainHTML);
  }else{
    webServer.send(200, "text/html", "<h1>Message changed</h1>");
  }*/
  webServer.send(200, "text/html", popup_mainHTML);
}
/*void moveUP(){
  delay(3000);
}
void moveRIGHT(){
}
void moveDOWN(){
}
void moveLEFT(){
}*/
