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

char* ssid = "ZON-5420";
char* password = "e9879478b141";

int screenWidth = 16;
int screenHeight = 2;
//variáveis de colocação de caracteres
int stringStart, stringStop = 0;
int scrollCursor = screenWidth;

String line1 = webServer.arg("fname");
String line2 = "   LEVITANEEC   ";

byte bola[8] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B01110,
  B00100,
  B00000,
};

String mainHTML = "<html lang=\"pt\"><head><meta charset=\"utf-8\"/><title>Test</title><link rel=\"preconnect\" href=\"https://fonts.gstatic.com\"><link href=\"https://fonts.googleapis.com/css2?family=Roboto:wght@400;500&display=swap\" rel=\"stylesheet\"><link rel=\"stylesheet\" href=\"style.css\"><style>body{ padding: 0; margin: 0; font-family: 'Roboto', sans-serif;}.LCD{ background-color:#2d3035; padding: 25px 0; color: #fff;}.LCD h2{ font-size: 30px; font-weight: 500; margin-top: 40px; margin-bottom: 20px; line-height: 1.1; padding-bottom: 9px;}h2, h3{ font-weight: 500; line-height: 1.1; margin-bottom: 10px;}h3{ font-size: 22px;}.LCDInsertText{ padding-top: 10px; padding-bottom: 10px; padding-left: 20px; padding-right: 20px; text-align: center; font-size: 20px; margin-bottom: 10px;}.LCDSubmit{ margin-top: 10px; margin-bottom: 30px; width: 200px; padding: 10px; background: #009de0; border-radius: 3px; font-size: 14px; font-weight: 600; color: #fff;}hr.solid { border-top: 3px solid #2d3035; }.Controlador{ margin-top: 40px; margin-bottom: 20px;}.Arrows{ margin-top: 30px;}.ArrowVert{ font-size: 1.3rem; text-align: center; color: #009de0; padding-top: 0; padding-bottom: 3px;border-style: solid; border-radius: 3px; border-color: #929292;}.ArrowHor{ font-size: 1.3rem; text-align: center; color: #009de0; padding-top: 1; padding-bottom: 4px;border-style: solid; border-radius: 3px; border-color: #929292;}.ArrowVert:focus{ background-color: #009de0; color: #fff;}.ArrowHor:focus{ background-color: #009de0; color: #fff;}</style></head><body><div class=\"LCD\"><h2 style=\"text-align:center\">LCD</h2></div><form style=\"text-align:center\" action=\"/print\" method=\"post\"><h3><label for=\"fname\">Text:</label></h3><br><input class=\"LCDInsertText\" type=\"text\" placeholder=\"Insert text here\" id=\"fname\" name=\"fname\"><br><input class=\"LCDSubmit\" type=\"submit\" value=\"Submit\"></form><hr class=\"solid\"><h3 class=\"Controlador\" style=\"text-align:center\">Controlador</h3><div class=\"Arrows\"><div style=\"text-align:center;\"><form action=\"/moveup\" method=\"POST\" style=\"display: inline-block; margin-bottom: 7px;\"><input class=\"ArrowVert\" type=\"submit\" style=\"transform: rotate(-90deg);\" value=\"➜\"></form></div><div style=\"text-align:center\"><form action=\"/moveleft\" method=\"POST\" style=\"display: inline-block;\"><input class=\"ArrowHor\" type=\"submit\" style=\"transform: rotate(180deg);\" value=\"➜\"></form><form action=\"/movedown\" method=\"POST\" style=\"display: inline-block;\"><input class=\"ArrowVert\" type=\"submit\" style=\"transform: rotate(90deg);\" value=\"➜\"></form><form action=\"/moveright\" method=\"POST\" style=\"display: inline-block;\"><input class=\"ArrowHor\" type=\"submit\" value=\"➜\"></form></div></div></body></html>";

String popup_mainHTML  = "<html lang=\"pt\"><head><meta charset=\"utf-8\"/><title>Test</title><link rel=\"preconnect\" href=\"https://fonts.gstatic.com\"><link href=\"https://fonts.googleapis.com/css2?family=Roboto:wght@400;500&display=swap\" rel=\"stylesheet\"><link rel=\"stylesheet\" href=\"style.css\"><style>body{ padding: 0; margin: 0; font-family: 'Roboto', sans-serif;}.LCD{ background-color:#2d3035; padding: 25px 0; color: #fff;}.LCD h2{ font-size: 30px; font-weight: 500; margin-top: 40px; margin-bottom: 20px; line-height: 1.1; padding-bottom: 9px;}h2, h3{ font-weight: 500; line-height: 1.1; margin-bottom: 10px;}h3{ font-size: 22px;}.LCDInsertText{ padding-top: 10px; padding-bottom: 10px; padding-left: 20px; padding-right: 20px; text-align: center; font-size: 20px; margin-bottom: 10px;}.LCDSubmit{ margin-top: 10px; margin-bottom: 30px; width: 200px; padding: 10px; background: #009de0; border-radius: 3px; font-size: 14px; font-weight: 600; color: #fff;}hr.solid { border-top: 3px solid #2d3035; }.Controlador{ margin-top: 40px; margin-bottom: 20px;}.Arrows{ margin-top: 30px;}.ArrowVert{ font-size: 1.3rem; text-align: center; color: #009de0; padding-top: 0; padding-bottom: 3px;border-style: solid; border-radius: 3px; border-color: #929292;}.ArrowHor{ font-size: 1.3rem; text-align: center; color: #009de0; padding-top: 1; padding-bottom: 4px;border-style: solid; border-radius: 3px; border-color: #929292;}.ArrowVert:focus{ background-color: #009de0; color: #fff;}.ArrowHor:focus{ background-color: #009de0; color: #fff;}</style></head><body onload=\"myFunction()\"><div class=\"LCD\"><h2 style=\"text-align:center\">LCD</h2></div><form style=\"text-align:center\" action=\"/print\" method=\"post\"><h3><label for=\"fname\">Text:</label></h3><br><input class=\"LCDInsertText\" type=\"text\" placeholder=\"Insert text here\" id=\"fname\" name=\"fname\"><br><input class=\"LCDSubmit\" type=\"submit\" value=\"Submit\"></form><hr class=\"solid\"><h3 class=\"Controlador\" style=\"text-align:center\">Controlador</h3><div class=\"Arrows\"><div style=\"text-align:center;\"><form action=\"/moveup\" method=\"POST\" style=\"display: inline-block; margin-bottom: 7px;\"><input class=\"ArrowVert\" type=\"submit\" style=\"transform: rotate(-90deg);\" value=\"➜\"></form></div><div style=\"text-align:center\"><form action=\"/moveleft\" method=\"POST\" style=\"display: inline-block;\"><input class=\"ArrowHor\" type=\"submit\" style=\"transform: rotate(180deg);\" value=\"➜\"></form><form action=\"/movedown\" method=\"POST\" style=\"display: inline-block;\"><input class=\"ArrowVert\" type=\"submit\" style=\"transform: rotate(90deg);\" value=\"➜\"></form><form action=\"/moveright\" method=\"POST\" style=\"display: inline-block;\"><input class=\"ArrowHor\" type=\"submit\" value=\"➜\"></form></div></div><script>function myFunction() { alert(\"Message changed\");}</script></body></html>";

void handleRoot() {
  webServer.send(200, "text/html", mainHTML);
}

void amIOnline() {
  Serial.print("Entered");
  webServer.send(200, "text/html", "Online and Ready");
}

void setup() {
  lcd.createChar(0, bola);

  /*WiFi.mode(WIFI_AP);*/  
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("insert LevitaNEEC text");
  WiFi.begin(ssid, password);
  

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   }
  
  Serial.println("");
  Serial.println("WiFi connected");
   
   
 // Print the IP address ---------------------------------------------------
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());


  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  webServer.on("/online",HTTP_GET, amIOnline);
  //webServer.on("/",HTTP_GET, handleRoot);
  webServer.on("/print", HTTP_POST, changeLCD);
  webServer.on("/moveup", HTTP_POST, moveUP);
  /*webServer.on("/moveright", HTTP_POST, moveRIGHT);
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
void moveUP(){
  Serial.println("Going upsies");
}
/*
void moveRIGHT(){
}
void moveDOWN(){
}
void moveLEFT(){
}*/
