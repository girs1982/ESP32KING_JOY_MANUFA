#include <Arduino.h>
#include "SPIFFS.h"
#include <Wire.h>
#include "WiFi.h"
#include "webs.h"
#include "IotWebConf.h"
#include "IotWebConfUsing.h"
#include <Preferences.h>
#include <ArduinoJson.h>
#include <favicon.h>
#include "soc/rtc_wdt.h"
#include "webpage.h"
#include "javascript_code.h"
#include <nvs_flash.h>
// #define rxPin 17   ////pin reciver
// #define TX 16     //// pin transiver
const char thingName[] = "Kingraberr";
const char wifiInitialApPassword[] = "123412345";
#define spiMosi 23
#define spiSclk 18
#include <star_kee.h>
int kiop3 = 0;
int tim_glushilka_task=1;
bool sendulo=false;
// #include <U8g2lib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define joy_x_pin 39
#define joy_y_pin 36
#define joy_sw_pin 14
#define STRING_LEN 128
#define NUMBER_LEN 32
int schel=0;
int SW=0;
int switch_freq=0;
volatile static long send_code_brut=1000;
bool sw=false;
volatile unsigned long sw_time;
bool swy1=false;
volatile unsigned long sw_timey1;
bool swy2=false;
volatile unsigned long sw_timey2;
volatile unsigned long menu_time;
String menu_select;
int menuitem = 0;
int page = 1;
#define DBG_OUTPUT_PORT Serial
bool bryt_g1 = false;
bool jam_g_m=false;
bool sisa=false;
int joy_x=0;
int joy_y=0;
int cnt_joy=0;
DNSServer dnsServer;
WebServer server(80);
IotWebConf iotWebConf(thingName, &dnsServer, &server, wifiInitialApPassword);
char iotssid[STRING_LEN];
char iotssidpass[STRING_LEN];
IotWebConfTextParameter ssid_io = IotWebConfTextParameter("HomeSsid", "ssid", iotssid, STRING_LEN);
IotWebConfTextParameter ssid_pass_io = IotWebConfTextParameter("Homepass", "ssidpass", iotssidpass, STRING_LEN);
bool iotWeb=false;
float freqncy=433.92;
String  codan_m="wait";
String webSite,javaScript,XML,Bat, SilaW,sending_m,sending_m1;
unsigned long wait000=0UL,wait001=1000UL,wait003=1000UL;
int Sila, SilaLow = -43; 
String codan2 ="not code";
String codan1 ="not code";
///#include "shlak.h" //shlakbaums 
volatile static long codsh;
int stoppriem =1;
int shlakoffpriem=1;
int stopsigi =1;
bool shlack=false;
///String sending ="nothing no send";
String button ="";
String gasa="zagasa";
// U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const char* ssid = "Gansta-ParadiseKin.com";
const char* password = "123412345";
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif
volatile int zalu=0;
TaskHandle_t  gansta;  ///держатель таска
TaskHandle_t  gansta2;
TaskHandle_t  gansta3;
// define two tasks for Blink & AnalogRead
 void drawMenu();
 void grabmenu();
 void buttonSW();
void handleRoot();

void TaskAnalogReadA3( void *pvParameters );
void TaskGrab( void *pvParameters );
void TaskGlushilka( void *pvParameters );
void TaskPosil( void *pvParameters );
void TaskGrabshlak( void *pvParameters );
void TaskGlushilkaM( void *pvParameters );
void create_task_posil();
void create_task_grabshlack();
void create_task_glushika();
static void ganstaDelaymic2(unsigned long ms)
{
  unsigned long start = micros();
  do
  {yield();
  } while (micros() - start < ms);
}
void glushilko_gansta22(int tim){
while (jam_g_m){if(!jam_g_m){break;}
rtc_wdt_feed();
digitalWrite(TX,!digitalRead(TX)); 
ganstaDelaymic2(tim);}
}
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void codanz(){
//codan;  
}
// Функция для отправки сохранённых кодов на сервер
void GetSavedCodes(const char* packageName) {
    String jsonResponse;
    getSavedCodes(packageName,jsonResponse,10);  // Получаем коды в формате JSON
    // Отправляем полученный JSON на сервер
    server.send(200, "application/json", jsonResponse);  // Отправляем JSON с кодами
}

// Функция для отображения всех файлов
void handleListFiles() {
    if (!SPIFFS.begin(true)) {
        server.send(500, "text/plain", "Error SPIFFS.");
        return;
    }

    String html = "<html><body>";
    html += "<h1>Files in SPIFFS</h1>";
    html += "<ul>";

    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    while (file) {
        String fileName = file.name();
        size_t fileSize = file.size();
        html += "<li><a href=\"/files/read?file=" + fileName + "\">" + fileName + "</a> (" + String(fileSize) + " bytes)</li>";
        file = root.openNextFile();
    }

    html += "</ul>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handleReadFile() {
    String fileName = server.arg("file");  // Получаем имя файла из параметров URL

    if (fileName.length() == 0) {
        server.send(400, "text/plain", "Bad Request: file name is missing.");
        return;
    }

    File file = SPIFFS.open("/" + fileName, "r");  // Открываем файл на чтение
    if (!file) {
        server.send(404, "text/plain", "File not found.");
        return;
    }

    String html = "<html><body>";
    html += "<h1>Contents of file: " + fileName + "</h1>";
    html += "<pre>";

    // Читаем файл и выводим его содержимое
    while (file.available()) {
        html += (char)file.read();
    }

    html += "</pre>";
    html += "</body></html>";
    file.close();  // Закрываем файл
    server.send(200, "text/html", html);  // Отправляем содержимое файла как HTML
}

void handleViewFile() {
    if (!server.hasArg("file")) {
        server.send(400, "text/plain", "Не указан файл для просмотра.");
        return;
    }

    String fileName = server.arg("file");
    if (!fileName.startsWith("/")) {
        fileName = "/" + fileName;  // Добавляем '/' в начало, если его нет
    }

    Serial.printf("Просмотр содержимого файла: %s\n", fileName.c_str());

    if (!SPIFFS.exists(fileName)) {
        Serial.println("Файл не найден!");
        server.send(404, "text/plain", "Файл не найден.");
        return;
    }

    File file = SPIFFS.open(fileName, "r");
    if (!file) {
        Serial.println("Ошибка открытия файла!");
        server.send(500, "text/plain", "Ошибка открытия файла.");
        return;
    }

    String content = file.readString();  // Читаем содержимое файла как строку
    file.close();

    // Возвращаем содержимое файла с MIME-типом text/plain для .txt файлов
    String htmlResponse = "<html><body><h2>Содержимое файла: " + fileName + "</h2>";
    htmlResponse += "<pre>" + content + "</pre>";  // Помещаем содержимое файла в тег <pre> для сохранения форматирования
    htmlResponse += "</body></html>";

    server.send(200, "text/html", htmlResponse);
}

void handleClearFile() {
    // Получаем имя файла из параметра запроса
    String fileName = server.arg("file");

    // Проверяем, что имя файла передано
    if (fileName.length() == 0) {
        server.send(400, "text/plain", "Имя файла не передано.");
        return;
    }

    // Инициализация SPIFFS
    if (!SPIFFS.begin(true)) {
        server.send(500, "text/plain", "Ошибка инициализации SPIFFS.");
        return;
    }

    // Проверяем, существует ли файл
    if (!SPIFFS.exists(fileName)) {
        server.send(404, "text/plain", "Файл не найден.");
        return;
    }

    // Открываем файл для очистки
    File file = SPIFFS.open(fileName, FILE_WRITE);
    if (!file) {
        server.send(500, "text/plain", "Ошибка открытия файла для записи.");
        return;
    }

    // Очищаем содержимое файла (закрытие файла очищает его содержимое)
    file.close();

    server.send(200, "text/plain", "Файл очищен.");
}

void brutshlak2(){
if(send_code_brut<4097){
  codan_m = String(send_code_brut);
  Serial.println(send_code_brut);
   SendNice(send_code_brut);
sending_m ="Shlak-Brutting:"+String(send_code_brut);
 send_code_brut++;
 }
}
void brutshlak(){
if(send_code_brut<4097){
  codan_m = String(send_code_brut);
  Serial.println(send_code_brut);
   SendCame4(send_code_brut);
sending_m ="Shlak-Brutting:"+String(send_code_brut);
 send_code_brut++;
  }
}

void brutshlakgansta(){
///ESP.wdtDisable();
if(bryt_g1){
 for (int send_code = 0; send_code < 4096; send_code++) // этот цикл после того как код определен необходимо убрать
  {if(!bryt_g1){break;}
    for (int j = 0; j <7; j++) // достаточно 4-х, но из-за помех поставил 7
    {
      digitalWrite(TX, HIGH); // стартовый импульс
      delayMicroseconds(700);
      digitalWrite(TX, LOW);
      for (int i = 12; i > 0; i--)
      {
        boolean bit_code = bitRead(send_code, i - 1);
        if (bit_code)
        {
          digitalWrite(TX, LOW); // единица
          delayMicroseconds(1400);
          digitalWrite(TX, HIGH);
          delayMicroseconds(700);
        }
        else
        {
          digitalWrite(TX, LOW); // ноль
          delayMicroseconds(700);
          digitalWrite(TX, HIGH);
          delayMicroseconds(1400);
        }
      }
      digitalWrite(TX, LOW); // пилотный период
      delayMicroseconds(25200);
    }

  codan_m = String(send_code);
  Serial.println(send_code);
  }
}
}
void Batare(){
 /// Batareya = 333666;         // Меряем напряжение батареи 
}
void SilaWifi(){
  Sila = WiFi.RSSI();  // Меряем силу сигнала WiFi
  if (Sila < SilaLow){
    SilaW = "          Сигнал слабый";
  }
  else{
    SilaW = "";
  }
}

void buildWebsite(){    // Создаём страницу с элементами управления
 // buildJavascript();
  webSite=webSiteContent; 
  webSite+=javaScriptCode;
}
String millis2time(){ // преобразование милисекунд в вид ч/м/с
  String Time="";
  unsigned long ss;
  byte mm,hh;
  ss=millis()/1000;
  hh=ss/3600;
  mm=(ss-hh*3600)/60;
  ss=(ss-hh*3600)-mm*60;
  if(hh<10)Time+="0";
  Time+=(String)hh+":";
  if(mm<10)Time+="0";
  Time+=(String)mm+":";
  if(ss<10)Time+="0";
  Time+=(String)ss;
  return Time;
}
void buildXML(){
  XML="<?xml version='1.0'?>";
  XML+="<xml>";
  XML+="<millistime>";
  XML+=millis2time();
  XML+="</millistime>";   // Добавляем наши данные
  SilaWifi();             // Здесь измеряем силу сигнала
  XML+="<Sila>";          // Сила сигнала WiFi
  XML+=String(Sila)+SilaW;// Сила сигнала WiFi
  XML+="</Sila>";         // Сила сигнала WiFi
   codanz();
  /// startgrabber(); ////code
  XML+="<Batareya>";       
 ////XML+="<br>";
  XML+=codan_m;
  XML+="</Batareya>"; 
  XML+="<Paket>";       
  XML+=String(codan2);
  XML+="</Paket>";   
  XML+="<Paket1>";       
  XML+=String(codan1);
  XML+="</Paket1>";   
  XML+="<Priem>";       
  XML+=String("priem");
  XML+="</Priem>";  
  XML+="<button>";          
  XML+=String(button)+"button send";
  XML+="</button>";        
  XML+="<button2>";          
  XML+=String(button)+"button send";
  XML+="</button2>"; 
  XML+="<sending>";         
  XML+=sending_m;
  XML+="</sending>";  
  XML+="<senk>";         
  XML+=String("seva");
  XML+="</senk>";  
  XML+="</xml>";
}
void handleWebsite(){
  buildWebsite();
  server.send(200,"text/html",webSite);
}
void handleXML(){
  if(sendulo){sending_m=return_sending();}
  codan_m=return_codan();
  buildXML();
  server.send(200,"text/xml",XML);
}
void handleESPval(){
// int sliderCNT=server.arg("Batareya");
  String button3243=server.arg("codan");
  Serial.println(button3243);
  buildXML();
  server.send(200,"text/xml",XML);
}
void setup() {
   //   nvs_flash_erase();      // erase the NVS partition and...
 //   nvs_flash_init();       // initialize the NVS partition.
  Serial.begin(115200);
  Serial.println(F("KG"));  
  initializeTransmitter();
  sending_m="Wait to send";
   if (ELECHOUSE_cc1101.getCC1101()){       // Check the CC1101 Spi connection.
  Serial.println("CC1101 Connection OK");
  }else{
    sisa=true;
    codan2 += "CC1101_ERROR";
  Serial.println("CC1101 Connection Error");
  }
//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101! 
  ELECHOUSE_cc1101.setPA(12);       // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12)   Default is max!
  //// ELECHOUSE_cc1101.setSyncMode(4);  
        ELECHOUSE_cc1101.setChsp(405.45);  
   ///     ELECHOUSE_cc1101.setGDO0(TX);   
//ELECHOUSE_cc1101.setRxBW(812.50);  // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
ELECHOUSE_cc1101.setMHZ(freqncy); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
 ELECHOUSE_cc1101.SetRx(433.92);
    iotWebConf.addSystemParameter(&ssid_io);
  iotWebConf.addSystemParameter(&ssid_pass_io);
  iotWebConf.init();
//    u8g2.begin();
//   u8g2.setFont(u8g2_font_6x10_tr);
//   Serial.println("Test");
//   u8g2.firstPage();
// do {
// draw1(42,0,44,64);
// } while( u8g2.nextPage() );
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));

  }
   display.display();
  delay(2000); // Pause for 2 seconds
  if(sisa){display.clearDisplay();
   display.setCursor(15, 50);
   display.print("CC1101ERROR");
   display.display();
   delay(3000);
  }  
 // Принудительное форматирование SPIFFS
    if (!SPIFFS.begin(true)) {  // true включает автоматическое форматирование
        Serial.println("Ошибка монтирования SPIFFS, попытка форматирования...");
        if (SPIFFS.format()) {
            Serial.println("SPIFFS успешно отформатирована!");
        } else {
            Serial.println("Не удалось отформатировать SPIFFS!");
        }
    } else {
        Serial.println("SPIFFS успешно смонтирована!");
    }
WiFi.mode(WIFI_STA);
WiFi.begin(iotssid, iotssidpass); // Создаём точку доступа
  while (WiFi.status() != WL_CONNECTED&&schel<30) {
    delay(1000);   
    schel++;
    Serial.println("Connecting to WiFi.."+String(schel));

  }
if(schel>=30){WiFi.disconnect();delay(100); WiFi.mode(WIFI_AP_STA);iotWeb=true;}// WiFi.mode(WIFI_AP_STA);delay(100); WiFi.softAP("KGrab", "123412345");}
//  while(WiFi.status()!=WL_CONNECTED)delay(500);
//  WiFi.mode(WIFI_STA);
if(!iotWeb){
  Serial.println(WiFi.localIP());

      xTaskCreatePinnedToCore(
    TaskGrab
    ,  "TaskGrab"   // A name just for humans
    ,  10000 // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  &gansta 
    ,  0);///core running
//u8g2.begin();
init_kepsta(); 
    // Настраиваем маршруты
    // Настраиваем маршруты
    server.on("/files", handleListFiles);      // Список файлов
    server.on("/files/read", handleReadFile);  // Чтение файла
    server.on("/files/view", HTTP_GET, handleViewFile);
    server.on("/files/clear", HTTP_GET, handleClearFile);
server.on("/",handleWebsite);
server.on("/xml",handleXML);
  server.on("/socket1On", [](){
  server.send(200, "text/html", webSite);
  Serial.println("\n\nKeloqpack1 shlremm ...");
  ELECHOUSE_cc1101.SetTx(freqncy); 
  posilkeeloq1();
  ELECHOUSE_cc1101.SetRx(freqncy); 
   });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", webSite);
Serial.println("\n\nSend KeeloqPack2...");
ELECHOUSE_cc1101.SetTx(freqncy); 
posilkeeloq2();
ELECHOUSE_cc1101.SetRx(freqncy); 
 });
 server.on("/socket2On", [](){
ELECHOUSE_cc1101.SetTx(freqncy);
 server.send(200, "text/html", webSite);
Serial.println("star paket1...");
posilstarline1();
ELECHOUSE_cc1101.SetRx(freqncy); 
});
server.on("/socket2Off", [](){
ELECHOUSE_cc1101.SetTx(freqncy);
server.send(200, "text/html", webSite);
Serial.println(" star paket2...");
posilstarline2();
});
server.on("/socket3On", [](){
ELECHOUSE_cc1101.SetRx(freqncy); 
server.send(200, "text/html", webSite);//scan on
Serial.println("priem on");
stoppriem = 1;
sending_m="priem on";
display.clearDisplay();
display.setCursor(4, 0);
 display.setTextSize(1);   
    display.print("Priem-On");
   display.display(); 
});
server.on("/socket3Off", [](){
server.send(200, "text/html", webSite);
Serial.println(" priem off");
  ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(812.50);  // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
ELECHOUSE_cc1101.setPA(12);  
ELECHOUSE_cc1101.SetTx(freqncy); 
stoppriem = 0;
sending_m="priem off";
});
server.on("/mana", [](){
server.send(200, "text/html", webSite);
Serial.println("Manafacturing");
ELECHOUSE_cc1101.SetTx(freqncy);
Serial.println("Send Star Mana");
posilstarlinemana();
 // ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(812.50);  // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
//ELECHOUSE_cc1101.setPA(12);  
//ELECHOUSE_cc1101.SetTx(freqncy); 
ELECHOUSE_cc1101.SetRx(freqncy); 
stoppriem = 0;
sending_m="manafakturing";
});
server.on("/manaopen", [](){
server.send(200, "text/html", webSite);
Serial.println("ManafacturingOpen");
ELECHOUSE_cc1101.SetTx(freqncy);
posilstarlinemanaOpen();
 // ELECHOUSE_cc1101.Init();            // must be set to initialize the cc1101!
//ELECHOUSE_cc1101.setRxBW(812.50);  // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
//ELECHOUSE_cc1101.setPA(12);  
//ELECHOUSE_cc1101.SetTx(freqncy); 
ELECHOUSE_cc1101.SetRx(freqncy); 
stoppriem = 0;
sending_m="manafakturing";
});
server.on("/brut_g", [](){
server.send(200, "text/html", webSite);
Serial.println("\n\nBryt_g ...");
bryt_g1=!bryt_g1;
brutshlakgansta();
});
server.on("/StopSigi", [](){
server.send(200, "text/html", webSite);
Serial.println("priem on");
sending_m ="Sigi-Scaning-OFF";
stopsigi = 0;
});
server.on("/StatSigi", [](){
server.send(200, "text/html", webSite);
Serial.println("priem on");
sending_m ="Sigi-Scaning-ONN";
stopsigi = 1;
});
////////////////////////////////////////////////////////////////////
server.on("/getSavedCodes", []() {  
    GetSavedCodes("sta0");  // Отправка кодов на сервер
});
server.on("/getSavedCodesKee", []() {
    GetSavedCodes("kee0");  // Отправка кодов на сервер
});
server.on("/send_codeoutTable", []() { //////send_specialCodeoutTable
  // Получаем параметры из GET запроса
  String tableName = server.arg("table"); // Параметр table
  String selectedCode = server.arg("code"); // Параметр code
  String Manafactur = server.arg("manufacturer"); // Параметр code
  String line = server.arg("line"); // Параметр code
  // Выводим полученные параметры в консоль
  Serial.println("Получены данные:");
  Serial.print("Таблица: ");
  Serial.println(tableName);
  Serial.print("Код: ");  
  Serial.println(selectedCode);
  Serial.print("Манафактура: ");
  Serial.println(Manafactur);
  Serial.print("line: ");
  Serial.println(line);
  ELECHOUSE_cc1101.SetTx(freqncy);
  handleSpecialCodeNmf(Manafactur, selectedCode, tableName,line.toInt());
  server.send(200, "text/html", "Данные успешно получены");
});

server.on("/send_specialCodeoutTable", []() { //////send_specialCodeoutTable
  // Получаем параметры из GET запроса
  String tableName = server.arg("table"); // Параметр table
  String selectedCode = server.arg("code"); // Параметр code
  String Manafactur = server.arg("manufacturer"); // Параметр code
  String line = server.arg("line"); // Параметр code
  // Выводим полученные параметры в консоль
  Serial.println("Получены данные:");
  Serial.print("Таблица: ");
  Serial.println(tableName);
  Serial.print("Код: ");
  Serial.println(selectedCode);
  Serial.print("Манафактура: ");
  Serial.println(Manafactur);
  Serial.print("line: ");
  Serial.println(line);
  ELECHOUSE_cc1101.SetTx(freqncy);
  handleSpecialCode(Manafactur, selectedCode, tableName,line.toInt());
  // Отправляем ответ пользователю
 /// ELECHOUSE_cc1101.SetRx(freqncy); 
  server.send(200, "text/html", "Данные успешно получены");
});


server.on("/GETS", [](){
  ELECHOUSE_cc1101.SetTx(freqncy);
Serial.println("SEND SHLAK CODE");
String message = "";
message +=server.arg(0);
////for (int i = 0; i < server.args(); i++) {
////message += "Arg nº" + (String)i + " –> ";
////message += server.argName(i) + ": ";
////message += server.arg(i) + "\n";
///Serial.println(message);
////} 

Serial.println(message);
SendCame4(message.toInt());
sending_m ="Shlak-send:"+message;
ELECHOUSE_cc1101.SetRx(freqncy); 
server.send(200, "text/html", webSite);
});
///////////////////NICE///////////////////////
server.on("/GETSN", [](){
  ELECHOUSE_cc1101.SetTx(freqncy);
Serial.println("SEND SHLAK CODE NICE");
String message = "";
message +=server.arg(0);
////for (int i = 0; i < server.args(); i++) {
///message += "Arg nº" + (String)i + " –> ";
////message += server.argName(i) + ": ";
////message += server.arg(i) + "\n";
///Serial.println(message);
////} 

Serial.println(message);
SendNice(message.toInt());
sending_m ="Shlak-nice-send:"+message;
ELECHOUSE_cc1101.SetRx(freqncy); 
server.send(200, "text/html", webSite);
});
server.on("/GETSNGS", [](){///
ELECHOUSE_cc1101.SetTx(freqncy);
Serial.println("Send Star");
String message = "";
message +=server.arg(0);
Serial.println(message);
//char sen[9];
//message.toCharArray(sen,10);
String dfo[10];
for(int i=0;i<10;i++){dfo[i]=getValue(message,';',i);}
byte sen2[9];
for(int i=0;i<9;i++){sen2[i]=dfo[i].toInt();}///sen[i]-'0';}
starline_send(sen2,0);
///Serial.println((char*)sen);
///glushilko_gansta(message.toInt());
sending_m ="SendStarlineByte:"+message;
ELECHOUSE_cc1101.SetRx(freqncy); 
server.send(200, "text/html", webSite);
});
server.on("/setbuttonvaues", []() {
    // Установка частоты передачи
    ELECHOUSE_cc1101.SetTx(freqncy);
    Serial.println("ButtonCodes_IS:");

    // Получаем значения параметров из запроса
    String buttonOpen = server.arg("open");  // Значение параметра open
    String buttonClose = server.arg("close");  // Значение параметра close
    convertStringsToBytes(buttonOpen, buttonClose) ;
    // Отладочный вывод значений
    Serial.println("Button Open: " + buttonOpen);
    Serial.println("Button Close: " + buttonClose);
    // Формируем сообщение из двух значений, разделенных символом ';'
    String message = buttonOpen + ";" + buttonClose;
 
    // Формируем и отправляем ответ клиенту
    sending_m = "ButtonCodes_IS:" + message;
    ELECHOUSE_cc1101.SetRx(freqncy);
    server.send(200, "text/html", webSite);
});

server.on("/GETGLUSH", [](){
Serial.println("Send Glushilko");
jam_g_m=!jam_g_m;
String message = "";
///jamok(jam_g_m);
message +=server.arg(0);
Serial.println(message);
if(jam_g_m){ELECHOUSE_cc1101.SetTx(freqncy); sending_m ="SendGlushilko:"+message;create_task_glushika();}
if(!jam_g_m){ELECHOUSE_cc1101.SetTx(freqncy); ELECHOUSE_cc1101.SetRx(freqncy); sending_m ="Glushilko:OFF";Serial.println("Send Glushilko Off");}///ESP.restart();
tim_glushilka_task=message.toInt();
server.send(200, "text/html", webSite);
});


server.on("/FREQ", [](){
Serial.println("ChangeFreq ");
String message = "";
message +=server.arg(0);
freqncy=message.toFloat();
Serial.println(freqncy);  
ELECHOUSE_cc1101.setMHZ(freqncy); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
 // cc1101 set Transmit on
ELECHOUSE_cc1101.SetTx();
ELECHOUSE_cc1101.SetRx(freqncy); 
server.send(200, "text/html", webSite);
});

/////////////shlaprisend
server.on("/ShlakSend", [](){
  ELECHOUSE_cc1101.SetTx(freqncy);

SendCame4(codsh);
/////sending ="Shlak-send:"+String(lastCode);
byte zur [4] ;
long codsh1=codsh;
display.clearDisplay();
for (int i = 0; i < 4; i++) {
zur[i] = codsh1 % 10;
codsh1 /= 10;
Serial.println(zur[i],HEX);

///int huypo = bitRead(zur[i], i);
//// Serial.println(huypo);
}
sending_m ="Shlak-send:"+String(codsh);
Serial.println(" Send Shlak code:" );
Serial.println(codsh,HEX);
Serial.println(codsh,BIN);
Serial.println(zur[0],HEX);
Serial.println(zur[1],HEX);
Serial.println(zur[2],HEX);
Serial.println(zur[3],HEX);
ELECHOUSE_cc1101.SetRx(freqncy); 
server.send(200, "text/html", webSite);
});
//////////////////////////////
///////////brutshlak
server.on("/ShlakOn", [](){
sending_m ="Shlak-Brut-Long-Wait";
Serial.println(" Send Shlak code");
server.send(200, "text/html", webSite);
brutshlak();

////BrutSh2
});
server.on("/BrutSh2", [](){
  ELECHOUSE_cc1101.SetTx(freqncy);
sending_m ="Shlak-Brut-Long-Wait";
Serial.println(" Send Shlak code");
server.send(200, "text/html", webSite);
brutshlak2();
/////BrutSh2
});
server.on("/Reboot", [](){
sending_m ="Reboot";
Serial.println(" Send Shlak code");
server.send(200, "text/html", webSite);
ESP.restart();
});
server.on("/OTA", [](){
sending_m ="Ota";

Serial.println(" Send Shlak code");
server.send(200, "text/html", webSite);
});

//////////////////
/////////ShlakSendOff
server.on("/ShlakSendOff", [](){
shlakoffpriem=~shlakoffpriem;
if(shlakoffpriem==1){
sending_m ="Shlak-Scaning-ONN";
}
else{
Serial.println(" Shlak-Scaning-OFF");
sending_m ="Shlak-Scaning-OFF";}
server.send(200, "text/html", webSite);
});
 server.on("/favicon", [](){  
 server.send_P(200, "image/x-icon", favicon,sizeof(favicon));
});
 server.begin();}

 if(iotWeb){
display.clearDisplay(); 	
display.setTextSize(1.5); // Draw 2X-scale text
display.setTextColor(WHITE);
display.setCursor(10, 0);
display.println(F("Achtung!!!")); 
display.setCursor(10, 20);
display.println(F("Setup Wifi NET")); 
display.display();      // Show initial text
server.on("/", handleRoot);
server.on("/config", []{ iotWebConf.handleConfig(); });
server.onNotFound([](){ iotWebConf.handleNotFound(); });
iotWebConf.skipApStartup();
///iotWebConf.setWifiConnectionCallback();
}
// server.begin();
}

void loop()
{
if(!iotWeb){if(stoppriem==1&&!shlack){startgrabber();}if(stoppriem==1&&shlack){grabshlack_mymod();}}
if(iotWeb){iotWebConf.doLoop();if(digitalRead(joy_sw_pin)==0){ESP.restart();}}
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskGlushilka(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;) // A Task shall never return or exit.
  {
    if(!jam_g_m){vTaskDelete(NULL);ELECHOUSE_cc1101.SetRx(freqncy);}
 digitalWrite(TX,!digitalRead(TX)); 
 vTaskDelay(tim_glushilka_task);  
  }
}

void TaskPosil(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;) // A Task shall never return or exit.
  {
posilstarline1();
 vTaskDelay(100);
vTaskDelete(NULL);
  }
}


void TaskGrabshlak(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;) // A Task shall never return or exit.
  {
grabshlak();
if(!shlack){vTaskDelete(NULL);}
  }
}

void TaskGlushilkaM(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    if(!jam_g_m){vTaskDelete(NULL);ELECHOUSE_cc1101.SetRx(freqncy);}
 digitalWrite(TX,!digitalRead(TX)); 
 vTaskDelay(tim_glushilka_task);  
  }
}

void TaskAnalogReadA3(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;)
  { 

    vTaskDelay(1000);  // one tick delay (15ms) in between reads for stability
    if(zalu>9){zalu++;}
    if(zalu>30){zalu=0;  }
  }
}


void TaskGrab(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;)
  { 
if(return_disponto2()){
sending_m=return_sending();
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(WHITE, BLACK);
    display.setCursor(15, 0);
    display.print(sending_m);
    //display.drawFastHLine(0,10,83,BLACK);
    display.display();  
    falldisponto2();
  //  falldisponto(); 
    vTaskDelay(1000);
}
   if(return_disponto()){codan_m=return_codan();
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(WHITE, BLACK);
    display.setCursor(15, 0);
    display.print(codan_m);
    //display.drawFastHLine(0,10,83,BLACK);
    display.display();  
 //    falldisponto2();
    falldisponto();
    vTaskDelay(1000);
   }
    server.handleClient();
 //startgrabber();
 /// Serial.println("wwww");
if(millis()>wait000){
    buildXML();
    wait000=millis()+1000UL;
  }
  if(millis()>wait001){
    wait001=millis()+300;           //Обновляем значения раз в 300 милисекунд
 }
if(stoppriem ==0){grabmenu();}
  buttonSW();
   vTaskDelay(10);

  }
}


void drawMenu()
  {  
////page main
  if (page==1)
  {   
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("KING GRAB");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(0, 15);
    kiop3 = 0;   
    if (menuitem==1)
    {
      display.setTextColor(WHITE, BLACK);
      if(SW==0){display.clearDisplay();menu_select="Grabber";}
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">KODE GRABBER");
    display.setCursor(0, 25);
  ///4 punkt
     if (menuitem==2)
    {
      display.setTextColor(WHITE, BLACK);
      /// if(SW==0){display.clearDisplay();menu_select="Grabber";}
        if(SW==0){display.clearDisplay();menu_select="GLUSHILKA";page=3;menu_time=millis();}
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">GLUSHILKA");
    display.setCursor(0, 35);
   /////////////////   
    if (menuitem==3)
    {
      display.setTextColor(WHITE, BLACK);
     
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.setCursor(0, 36);
    display.print(">FREQ");   
    display.display();   
   
    if (menuitem==3)
    {
      display.setTextColor(WHITE, BLACK);
      if(SW==0){display.clearDisplay();menu_select="FREQ";page=2;menu_time=millis();}
    }
   else
    {
      display.setTextColor(BLACK, WHITE);
    }
   display.setCursor(15, 50);
   display.print("GANSTA");
   display.display();
  }


///  scan mode
if(menu_select=="Grabber"&&page==1){
  stoppriem=1;
  menu_select="";
  menuitem=0;
  display.clearDisplay();
///iotWebConf.ad(&ssid2) ;
  ///Serial.println(teso);
 display.setTextSize(1.5); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("King-grabber")); 
    display.setCursor(10, 20);
  display.println(F("Scanning Mode...")); 
  display.display();      // Show initial text

}
////page freq
 if (page==2&&millis()-menu_time>1000)
  {   
    ///ganstaDelaymic(3000000);
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("FREQS");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(0, 15);
    kiop3 = 0;   
    if (menuitem==1)
    {
      display.setTextColor(WHITE, BLACK);
     if(SW==0){display.clearDisplay();menu_select="433";switch_freq=1; }
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">433");
    display.setCursor(0, 25);
  ///4 punkt
     if (menuitem==2)
    {
      display.setTextColor(WHITE, BLACK);
       if(SW==0){display.clearDisplay();menu_select="868";switch_freq=2;}
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">868");
    display.setCursor(0, 35);
   /////////////////   
    if (menuitem==3)
    {
      display.setTextColor(WHITE, BLACK);
     if(SW==0){display.clearDisplay();menu_select="868";switch_freq=3;}
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.setCursor(0, 36);
    display.print(">915");   
    display.display();   
   
  //   if (menuitem==3)
  //   {
  //     display.setTextColor(WHITE, BLACK);
  //    if(SW==0){display.clearDisplay();menu_select="915";switch_freq=3;}
  //   }
  //  else
  //   {
  //     display.setTextColor(BLACK, WHITE);
  //   }
   display.setCursor(15, 50);
   display.print("GANSTA");
   display.display();
  switch (switch_freq)
  {
  case 1:
    freqncy=433.92;
    Serial.println(freqncy);  
    ELECHOUSE_cc1101.setMHZ(freqncy); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    // cc1101 set Transmit on
    ELECHOUSE_cc1101.SetTx();    
    ELECHOUSE_cc1101.SetRx();
    sending_m =String(freqncy);
    display.clearDisplay();
    display.setCursor(4, 0);
    display.setTextSize(2);   
    display.print(sending_m);
    display.display(); 
    vTaskDelay(3000);
   
    page=1;
    switch_freq=0;
    break;
   case 2:

    freqncy=868.92;
    Serial.println(freqncy);  
    ELECHOUSE_cc1101.setMHZ(freqncy); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    // cc1101 set Transmit on
    ELECHOUSE_cc1101.SetTx();   
    ELECHOUSE_cc1101.SetRx(); 
    sending_m =String(freqncy);
    display.clearDisplay();
    display.setCursor(4, 0);
    display.setTextSize(2);   
    display.print(sending_m);
    display.display(); 
     vTaskDelay(3000);
   
    page=1;
    switch_freq=0;
    break; 
    case 3:
    freqncy=915.92;
    Serial.println(freqncy);  
    ELECHOUSE_cc1101.setMHZ(freqncy); // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    // cc1101 set Transmit on
    ELECHOUSE_cc1101.SetTx();    
    ELECHOUSE_cc1101.SetRx();
    sending_m =String(freqncy);
    display.clearDisplay();
    display.setCursor(4, 0);
    display.setTextSize(2);   
    display.print(sending_m);
    display.display(); 
     vTaskDelay(3000);
    switch_freq=0;
    page=1;
    break;
  
  ///default:
  ///  break;
  }  }  
  
  
  ////page glushilka
  if (page==3&&millis()-menu_time>1000)
  {   
    ///ganstaDelaymic(3000000);
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("Glushilka");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(0, 15);
    kiop3 = 0;   
    if (menuitem==1)
    {
      display.setTextColor(WHITE, BLACK);
     if(SW==0&&page==3){ELECHOUSE_cc1101.SetTx(freqncy); Serial.println("onglushara"); jam_g_m=true;vTaskDelay(1000);create_task_glushika();display.clearDisplay();menu_select="Glushilka on";
 display.setTextSize(1.5); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("GLUSHILKA ON")); 
    display.setCursor(10, 20);
  display.println(String(freqncy)); 
  display.display();      // Show initial text
   vTaskDelay(3000);  
     }
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Glushilka on");
    display.setCursor(0, 25);  
     if (menuitem==2)
    {
      display.setTextColor(WHITE, BLACK);
      if(SW==0){display.clearDisplay();menu_select="Glushilka off";jam_g_m=false;ELECHOUSE_cc1101.SetRx();
       display.setTextSize(1.5); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("GLUSHILKA OFF")); 
    display.setCursor(10, 20);
  display.println(String(freqncy)); 
  display.display();      // Show initial text
   vTaskDelay(3000);  
      }
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Glushilka off");
    display.setCursor(0, 35);  
    if (menuitem==3)
    {
      display.setTextColor(WHITE, BLACK);
     if(SW==0){display.clearDisplay();menu_select="Glushilka Exit";jam_g_m=false;page=1;}
    }
    else
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.setCursor(0, 36);
    display.print(">EXIT");   
    display.display();   
  }
  //   if (menuitem==3)
  //   {
  //     display.setTextColor(WHITE, BLACK);
  //    if(SW==0){display.clearDisplay();menu_select="915";switch_freq=3;}
  //   }
  //  else
  //   {
  //     display.setTextColor(BLACK, WHITE);
  //   }
  //  display.setCursor(15, 50);
  //  display.print("GANSTA");
  //  display.display();
  
  
  }

void grabmenu(){
  drawMenu();
  if(joy_x==0){menuitem--;if(menuitem<1){menuitem=3;}}
  if(joy_x>3095){menuitem++;cnt_joy++; if(menuitem>3){menuitem=1;}}
 if(joy_y==0&&!swy1){swy1=true;sw_timey1=millis();}
 if(joy_y>3095&&!swy2){swy2=true;sw_timey2=millis();}
 if(millis()-sw_timey1>3000&&joy_y==0&&!shlack){ELECHOUSE_cc1101.SetTx();if(manufan){posilstarlinemanaOpen();}else{swy1=false;sw_timey1=millis();posilstarline1();}ELECHOUSE_cc1101.SetRx();}
  if(millis()-sw_timey2>3000&&joy_y>3095&&!shlack){ELECHOUSE_cc1101.SetTx();if(manufan){posilstarlinemana();}else{swy2=false;sw_timey2=millis(); posilkeeloq1();}ELECHOUSE_cc1101.SetRx();}
   if(millis()-sw_timey1>3000&&joy_y==0&&shlack){swy1=false;sw_timey1=millis(); ELECHOUSE_cc1101.SetTx();sendshlackfrom_mem();ELECHOUSE_cc1101.SetRx();}
  if(millis()-sw_timey2>3000&&joy_y>3095&&shlack){swy2=false;sw_timey2=millis(); ELECHOUSE_cc1101.SetTx();sendshlackfrom_mem();ELECHOUSE_cc1101.SetRx();}
 if(joy_y<1800&&joy_y>1700){swy2=false;swy1=false;}
///Serial.println(joy_y);

}
void buttonSW(){
 joy_x=analogRead(joy_x_pin);
 joy_y=analogRead(joy_y_pin);
if(joy_y==0&&!swy1){swy1=true;sw_timey1=millis();}
if(joy_y>3095&&!swy2){swy2=true;sw_timey2=millis();}
if(joy_y<1800&&joy_y>1700){swy2=false;swy1=false;}
if(millis()-sw_timey1>3000&&joy_y==0&&stoppriem==1){swy1=false;sw_timey1=millis(); shlack=!shlack;
display.clearDisplay();
if(shlack){ display.setTextSize(1.5); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("ShlackMode ON")); 
    display.setCursor(10, 20);
  display.println(String(freqncy)); 
  display.display();      // Show initial text
   vTaskDelay(1000); } 
if(!shlack){ display.setTextSize(1.5); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("ShlackMode OFF")); 
    display.setCursor(10, 20);
  display.println(String(freqncy)); 
  display.display();      // Show initial text
   vTaskDelay(1000); } 
}

SW=digitalRead(joy_sw_pin);
if(SW==0&&!sw){sw=true;sw_time=millis();}
if(SW==1){sw=false;}
if(millis()-sw_time>3000&&SW==0){sw_time=millis();Serial.println("Press");if(stoppriem==1){stoppriem=0;}else{stoppriem=1;menu_select="";page=1;}
  display.clearDisplay();
///iotWebConf.ad(&ssid2) ;
  ///Serial.println(teso);
 display.setTextSize(1.5); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("King-grabber")); 
    display.setCursor(10, 20);
  display.println(F("Scanning Mode...")); 
  display.display();      // Show initial text
}///button joy hold
}

void create_task_glushika(){
xTaskCreatePinnedToCore(TaskGlushilka,"TaskGlushilka",  1024 ,  NULL,  2,&gansta,0);
}
void create_task_posil(){
xTaskCreatePinnedToCore(TaskPosil,"TaskPosil",  10024 ,  NULL,  2,&gansta,0);
}
void create_task_grabshlack(){
xTaskCreatePinnedToCore(TaskGrabshlak,"TaskGrabshlak",  10024 ,  NULL,  1,&gansta,1);
}

void handleRoot()
{
  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal())
  {
    // -- Captive portal request were already served.
    return;
  }
  String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
  s += "<title>IotWebConf 01 Minimal</title></head><body>";
  s += "Go to <a href='config'>configure page</a> to change settings.";
  s += "</body></html>\n";

  server.send(200, "text/html", s);
}
