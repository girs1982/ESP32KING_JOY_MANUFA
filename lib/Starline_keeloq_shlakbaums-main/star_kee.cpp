#include "star_kee.h"
#include <stdint.h>
// Определения макросов для получения битов и функции g5
int keloq_difftime[2]={400,800};
bool disponto=false;
bool disponto2=false;
int SMALL = 700;
int LONG =1400;  
int ing = 0;
int starlineCounter = 0;
int starlinePreambula = 0;
bool manafak=false;
char totable;
uint32_t hop = 0;
uint32_t hop_to_sender = 0;
uint64_t CrazyMonkey=0;
uint8_t btnm = 0;
float freq_m=433.92;
static long StarLineCode1 = 0; // first part
static long StarLineCode2 = 0; // last part
static long invertStarLineCode1 = 0; // first part
static long invertStarLineCode2 = 0; // last part
int k=0;//schet keeloq  
int kk  =0; //shet star line
String code = "";
String codan ="Scanning begin";
String priem ="0";
String sending ="nothing no send";
String camo_codan="";
String camo_codanI="";
String getun="";
String foundProtocol="";
int starkk = 0;
///bool bryt_g1 = false;
bool jam_g=false;
boolean bValidPacket=false;
int decodeMethod = 0;
int lastRxValue = 0;
int tempRxValue = 0;
unsigned long lastRxTime = 0;
unsigned long tempTime = 0; 
//keeloq
int keeloqHeader=0;
int keeloqCounter = 0;
int starlineZCounter = 0;
int starlineZpreambulaCounter = 0;
int keeloqPreambula = 0;
int stralineHeader=0;
static long keeloqCode1 = 0; // first part
static long keeloqCode2 = 0; // last part
static long invertkeeloqCode1 = 0; // first part
static long invertkeeloqCode2 = 0; // last part
String keecode = "";
boolean keebValidPacket=false;
int keelastRxValue = 0;
int keetempRxValue = 0;
unsigned long keelastRxTime = 0;
unsigned long keetempTime = 0;
//пробуем выброс
//keelog start
unsigned long difTime = 0;
unsigned long difTime2 = 0;
int keelog_state = 0;
volatile int keelogCounter = 0;
byte keelog_code[9]={0};
byte keelog_codePAK2[9]={0};
byte keelog_codePAK1[9]={0};
byte hugaz[9];
byte hugazk[9];
byte hugazi[9];
byte starline_code[9]={0}; 
byte code_fara[9]={0};
byte starline_codePAK2[9]={0};
byte starline_codePAK1[9]={0};
byte* siga_code = nullptr;  // Указатель на массив байтов
uint16_t siga_code_length=0;
byte webpak2[9];
byte webpak1[9];
byte buttonopen=0x42;
byte buttonclose=0x46;
int starline_state = 0;
int zoro=1;
/////////////////////////////////////////////////VVODNIE
uint32_t decryptedData=0;
uint8_t fixm = 0;
uint16_t cntm = 0;
// Глобальная переменная для отслеживания состояния таска
TaskHandle_t sendTaskHandle = NULL;
volatile unsigned long prevtime;
volatile unsigned int lolen, hilen, state;
volatile int cameCounter = 0;    // count of bits stored
volatile static long cameCode = 0;       // code itself
volatile static long lastCode = 0;
volatile byte level = 255;
volatile unsigned long last;
volatile unsigned long len;
byte p_level;
GanstaTransmitEsp32 transmitterGansta(12);
unsigned long p_len;
unsigned long p_len_prev;
struct
{
    uint8_t state;
    uint8_t data[3], dat_bit;
} came;

// Структура для хранения данных устройства
struct DeviceData {
    uint64_t key;   // 64-битный ключ производителя
    const char* name;  // Название устройства
    int method;    
};
// Параметры для передачи в таск
struct SendTaskParams {
    uint8_t* protocol;       // Указатель на массив данных
    size_t protocolLength;   // Длина массива данных
    uint32_t shortDelta;     // Короткая длительность (мкс)
    uint32_t longDelta;      // Длинная длительность (мкс)
    bool isInverted;         // Инверсия сигнала
    uint8_t repeats;         // Количество повторов
    uint8_t txPin;           // Пин для передачи данных
    int meanderTime;          // Длительность меандра (мкс)
    int meanderRepeats;       // Количество повторов меандра
};

// Функция Busy Wait для задержек в микросекундах
void busyWaitMicroseconds(uint32_t us) {
    uint32_t start = micros();
    while ((micros() - start) < us) {
        // Пустой цикл для ожидания
    }
}
// Функция для выполнения таска
void sendDataTask(void* parameters) {
    SendTaskParams* params = (SendTaskParams*)parameters;
    // Повторяем весь цикл (меандр + пакет) заданное количество раз
    for (uint8_t repeat = 0; repeat < params->repeats; repeat++) {
        
        // Отправка меандра перед отправкой пакета
        for (int meanderRepeat = 0; meanderRepeat < params->meanderRepeats; meanderRepeat++) {
            digitalWrite(params->txPin, HIGH);
           vTaskDelay(params->meanderTime / 1000);  // Высокий уровень меандра
            digitalWrite(params->txPin, LOW);
          vTaskDelay(params->meanderTime / 1000);  // Высокий уровень меандра
        }

        // Отправка основного пакета
        for (size_t byteIndex = 0; byteIndex < params->protocolLength; byteIndex++) {
            uint8_t currentByte = params->protocol[byteIndex];
            // Проходим по каждому биту в текущем байте
            for (int bitIndex = 7; bitIndex > 0; bitIndex--) {
                bool bit = bitRead(currentByte, bitIndex);  // Получаем бит
              //  bit = params->isInverted ? !bit : bit;      // Инвертируем, если нужно
                // Передача бита
                if (bit) {
                     digitalWrite(params->txPin, HIGH);
                     vTaskDelay(params->shortDelta / portTICK_PERIOD_MS / 1000);   
                     digitalWrite(params->txPin, LOW);
                     vTaskDelay(params->shortDelta / portTICK_PERIOD_MS / 1000);   
                } else {
                    digitalWrite(params->txPin, HIGH);
                  vTaskDelay(params->longDelta / portTICK_PERIOD_MS / 1000);
                    digitalWrite(params->txPin, LOW);
                    vTaskDelay(params->longDelta / portTICK_PERIOD_MS / 1000);
                }
            }
        }

        // Задержка между повторами
     //   Serial.println("Cycle completed, waiting before next repeat...");
  
      // vTaskDelay(100 / 1000);
    }
    // Завершение таска
    Serial.println("Task completed, deleting...");
     ELECHOUSE_cc1101.SetRx(freq_m); 
    // Сброс хэндла таска после его завершения
    sendTaskHandle = NULL;
    vTaskDelete(NULL);
}
// Функция для создания таска
void startSendTask(
    uint8_t* protocol, size_t protocolLength, uint32_t shortDelta, uint32_t longDelta,
    bool isInverted, uint8_t repeats, uint8_t txPin,int meanderTime,int meanderRepeats
) {
      // Проверяем, выполняется ли уже таск
    if (sendTaskHandle != NULL) {
        // Если таск еще выполняется, не создаём новый
        Serial.println("Task is already running. Task will not be created.");
        return;
    }
    // Убедимся, что пин настроен на выход
    pinMode(txPin, OUTPUT);
    digitalWrite(txPin, LOW);
    // Настраиваем параметры для таска
    SendTaskParams* params = new SendTaskParams{
        protocol, protocolLength, shortDelta, longDelta, isInverted, repeats, txPin,meanderTime, meanderRepeats
    };

    // Создаем таск
    xTaskCreate(
        sendDataTask,   // Функция таска
        "SendDataTask", // Имя таска
        2048,           // Размер стека
        params,         // Параметры таска
        configMAX_PRIORITIES - 1,              // Приоритет таска
        &sendTaskHandle // Указатель на хэндл таска
    );
}

// Функция для шифрования и дешифрования с использованием XOR
uint64_t simpleEncryptDecrypt(uint64_t data, uint64_t key) {
    return data ^ key; // Применяем XOR для шифрования и дешифрования
}

// Массив с зашифрованными ключами

// Массив данных устройств без шифрования
// Массив данных устройств без шифрования
DeviceData ManafactureNS[] = {
   {0x0000000000000000, "Tomohawk", 1},
    {0x0000000000000000, "Tomohawk9010", 1},
    {0x0000000000000000, "Tomohawk9020", 1},
    {0x0000000000000000, "StarlineB6", 1},
    {0x0000000000000000, "StarlineA2", 1},
    {0x0000000000000000, "StarlineA8", 1},
    {0x0000000000000000, "Lacosta", 1},
    {0x0000000000000000, "Aligofrend", 1},
    {0x0000000000000000, "Centurion", 1},
    {0x0000000000000000, "Obama", 1},
    {0x0000000000000000, "Sherif", 1},
    {0x0000000000000000, "Obama2", 1},
    {0x0000000000000000, "Starline25", 1},
    {0x0000000000000000, "Starline89", 1},
    {0x0000000000000000, "Tomohawk68", 1},
    {0x0000000000000000, "Starline12", 1},
    {0x0000000000000000, "Starline74", 1},
    {0x0000000000000000, "APS", 1},
    {0x0000000000000000, "DoorHanchik", 1},
    {0x0000000000000000, "DoorHanchik1", 1},
    {0x0000000000000000, "Door1", 1},
    {0x0000000000000000, "Door2", 1},
    {0x0000000000000000, "DoorHan", 1},
    {0x0000000000000000, "Alligator", 1},
    {0x0000000000000000, "Stilmatic", 2},
    {0x0000000000000000, "Mongoose", 2},
    {0x0000000000000000, "SL_A6-A9/Tomahawk_9010", 1},
    {0x0000000000000000, "Pantera", 1},
    {0x0000000000000000, "SL_A2-A4", 1},
    {0x0000000000000000, "Cenmax_St-5", 1},
    {0x0000000000000000, "SL_B6,B9_dop", 1},
    {0x0000000000000000, "Harpoon", 1},
    {0x0000000000000000, "Tomohawk_TZ-9030", 1},
    {0x0000000000000000, "Tomohawk_Z,X_3-5", 1},
    {0x0000000000000000, "Cenmax_St-7", 1},
    {0x0000000000000000, "Sheriff", 1},
    {0x0000000000000000, "Pantera_CLK", 1},
    {0x0000000000000000, "Cenmax", 1},
    {0x0000000000000000, "Alligator_S-275", 1},
    {0x0000000000000000, "Guard_RF-311A", 2},
    {0x0000000000000000, "Partisan_RX", 1},
    {0x0000000000000000, "APS-1100_APS-2550", 1},
    {0x0000000000000000, "Pantera_XS/Jaguar", 1},
    {0x0000000000000000, "NICE_Smilo", 1},
    {0x0000000000000000, "NICE_MHOUSE", 1},
    {0x0000000000000000, "Dea_Mio", 1},
    {0x0000000000000000, "Genius_Bravo", 2},
    {0x0000000000000000, "FAAC_RC,XT", 2},
    {0x0000000000000000, "FAAC_SLH", 5},
    {0x0000000000000000, "BFT", 3},
    {0x0000000000000000, "Came_Space", 1},
    {0x0000000000000000, "DTM_Neo", 1},
    {0x0000000000000000, "GSN", 2},
    {0x0000000000000000, "Beninca", 4},
    {0x0000000000000000, "Elmes_Poland", 2},
    {0x0000000000000000, "IronLogic", 1},
    {0x0000000000000000, "Comunello", 2},
    {0x0000000000000000, "Sommer(fsk476)", 2},
    {0x0000000000000000, "Normstahl", 1},
    {0x0000000000000000, "KEY", 1},
    {0x0000000000000000, "JCM_Tech", 1},
    {0x0000000000000000, "Novoferm", 1},
    {0x0000000000000000, "EcoStar", 2},
    {0x0000000000000000, "Gibidi", 1},
    {0x0000000000000000, "Aprimatic", 1},
    {0x0000000000000000, "Kingates_Stylo4k", 1},
    {0x0000000000000000, "Centurion", 2},
    {0x0000000000000000, "Monarch", 2},
    {0x0000000000000000, "KGB/Subaru", 6},
    {0x0000000000000000, "Magic_1", 7},
    {0x0000000000000000, "Magic_2", 7},
    {0x0000000000000000, "Magic_3", 8},
    {0x0000000000000000, "Magic_4", 8},
    {0x0000000000000000, "Teco", 0},
    {0x0000000000000000, "Mutanco_Mutancode", 0},
    {0x0000000000000000, "Leopard", 0},
    {0x0000000000000000, "Faraon", 0},
    {0x0000000000000000, "Reff", 0},
    {0x0000000000000000, "ZX-730-750-1055", 0}
};

const int deviceCount = sizeof(ManafactureNS) / sizeof(ManafactureNS[0]);
/**
 * Простая функция дешифровки для протокола Keeloq
 * @param data - зашифрованные данные
 * @param key - ключ производителя (64 бита)
 * @return 0xBSSSCCCC, где B (4 бита) - ключ, S (10 бит) - serial & 0x3FF, C (16 бит) - счётчик
 */
inline uint32_t subghz_protocol_keeloq_common_decrypt(const uint32_t data, const uint64_t key) {
    uint32_t x = data, r;
    for(r = 0; r < 528; r++) {
        x = (x << 1) ^ bit(x, 31) ^ bit(x, 15) ^ (uint32_t)bit(key, (15 - r) & 63) ^
            bit(KEELOQ_NLF, g5(x, 0, 8, 19, 25, 30));  // KEELOQ_NLF 
    }
    return x;
}

/** Simple Learning Encrypt
 * @param data - 0xBSSSCCCC, B(4bit) key, S(10bit) serial&0x3FF, C(16bit) counter
 * @param key - manufacture (64bit)
 * @return keeloq encrypt data
 */
inline uint32_t subghz_protocol_keeloq_common_encrypt(const uint32_t data, const uint64_t key) {
    uint32_t x = data, r;
    for(r = 0; r < 528; r++)
        x = (x >> 1) ^ ((bit(x, 0) ^ bit(x, 16) ^ (uint32_t)bit(key, r & 63) ^
                         bit(KEELOQ_NLF, g5(x, 1, 9, 20, 26, 31)))
                        << 31);
    return x;
}

/** Normal Learning
 * @param data - serial number (28bit)
 * @param key - manufacture (64bit)
 * @return manufacture for this serial number (64bit)
 */
inline uint64_t subghz_protocol_keeloq_common_normal_learning(uint32_t data, const uint64_t key) {  //vod fix+mana
    uint32_t k1, k2;

    data &= 0x0FFFFFFF;
    data |= 0x20000000;
    k1 = subghz_protocol_keeloq_common_decrypt(data, key);

    data &= 0x0FFFFFFF;
    data |= 0x60000000;
    k2 = subghz_protocol_keeloq_common_decrypt(data, key);

    return ((uint64_t)k2 << 32) | k1; // key - shifrovanoya
}


byte* invertArray(const byte* array, size_t length) {
    byte* inverted = new byte[length]; // Создаём новый массив
    for (size_t i = 0; i < length; i++) {
        inverted[i] = array[length - 1 - i]; // Заполняем инвертированный массив
    }
    return inverted; // Возвращаем указатель на инвертированный массив
}

void reverseBitsInBytes(byte* input, byte* output, int length) {
    for (int i = 0; i < length; i++) {
        byte reversedByte = 0;
        byte currentByte = input[i];
        
        // Переворачиваем биты
        for (int j = 0; j < 8; j++) {
            if (currentByte & (1 << j)) {
                reversedByte |= (1 << (7 - j));
            }
        }
        
   

        output[i] = reversedByte;
    }
    byte temp;
    for (int i = 0; i < length / 2; i++) {
        temp = output[i];  // Сохраняем текущий элемент
        output[i] = output[length - i - 1];  // Меняем местами с симметричным элементом
        output[length - i - 1] = temp;  // Возвращаем сохранённый элемент
    }

}
// Функция для сохранения кода, если он уникален
void saveUniqueCode(const char* packageName, uint8_t* newCode, size_t codeLength, const String& manufacturerName) {
    // Инициализация SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("Ошибка инициализации SPIFFS");
        return;
    }

    // Имя файла для хранения кодов
    String fileName = "/" + String(packageName) + ".txt";

    // Проверяем, существует ли файл, если нет — создаём новый
    if (!SPIFFS.exists(fileName)) {
        File newFile = SPIFFS.open(fileName, FILE_WRITE);
        if (!newFile) {
            Serial.println("Ошибка создания файла.");
            return;
        }
        newFile.close();
    }

    // Открываем файл для чтения
    File file = SPIFFS.open(fileName, FILE_READ);
    if (!file) {
        Serial.println("Ошибка открытия файла для чтения.");
        return;
    }

    // Считываем содержимое файла для проверки на уникальность
    String existingContent = "";
    while (file.available()) {
        existingContent += (char)file.read();  // Считываем весь файл как строку
    }
    file.close();  // Закрываем файл после чтения

    // Проверяем, есть ли код в файле
    String newCodeHex = "";
    for (size_t i = 0; i < codeLength; ++i) {
        newCodeHex += String(newCode[i], HEX);  // Преобразуем код в строку HEX
    }

    // Если код уже существует, ничего не делаем
    if (existingContent.indexOf(newCodeHex) != -1) {
        Serial.println("Код уже существует в файле.");
        return;
    }

    // Открываем файл для добавления нового кода
    File writeFile = SPIFFS.open(fileName, FILE_APPEND);
    if (!writeFile) {
        Serial.println("Ошибка открытия файла для записи.");
        return;
    }

    // Записываем новый код в файл (в шестнадцатеричном формате) с именем производителя
    writeFile.print("CODE: ");
    for (size_t i = 0; i < codeLength; ++i) {
        writeFile.print(String(newCode[i], HEX) + " ");  // Записываем код в HEX-формате
    }
    writeFile.print(" Manafacture: ");
    writeFile.println(manufacturerName);  // Записываем имя производителя
    writeFile.close();  // Закрываем файл после записи

    Serial.println("Код успешно сохранён в файл.");
}
void init_kepsta(){
pinMode(TX,OUTPUT);
pinMode(rxPin,INPUT);
lastRxValue = digitalRead(rxPin);
lastRxTime = micros();
//kee
keelastRxValue = digitalRead(rxPin);
keelastRxTime = micros();
}

void SPI_SendBit(byte Bit){
  digitalWrite(spiMosi, Bit);
  delayMicroseconds(1);
  digitalWrite(spiSclk, HIGH);
  delayMicroseconds(1);
  digitalWrite(spiSclk, LOW);
}
void send_meander(int time)//pra meandr
{
  digitalWrite(TX, HIGH);
  delayMicroseconds(time);
  digitalWrite(TX, LOW);
  delayMicroseconds(time);
}

void startgrabber(){
tempRxValue = digitalRead(rxPin);
if(tempRxValue != lastRxValue){
tempTime = micros();
difTime = tempTime - lastRxTime;
difTime2 = tempTime - lastRxTime;
///Serial.println(String(difTime));
keelog_get(); 
starline_get(); 
//grab(tempTime - lastRxTime, lastRxValue);
lastRxTime = tempTime;
lastRxValue = tempRxValue;
}
}

void starline_get(){
    bValidPacket = false;
    if(starline_state==0){//ждем преамбулу и хедер
      if(difTime2 > 900 && difTime2 < 1100 && lastRxValue ==1 ){
        starlineZpreambulaCounter ++;
      } 
      if(starlineZpreambulaCounter==6){
            starline_state=1;         
        }
       //  else{       
     //  starlineZCounter = 0;
     // }
    }
    /////////////////////////////////////////////////////PREAMBULA STARA///////////////////////////////////////////////////////////////////////////
else if (starline_state == 1) { // получаем биты
    if (difTime2 > 350 && difTime2 < 650 && lastRxValue == 1) { // получили 1
        int byteIndex = starlineZCounter / 8;          // Определяем индекс байта
        int bitPosition = 7 - (starlineZCounter % 8);  // Определяем позицию бита (старший бит = 7)
        
        if (decodeMethod == 0) {
            // Устанавливаем бит в 1
            starline_code[byteIndex] |= (1 << bitPosition);
        } else {
            // Устанавливаем бит в 1 (альтернативная логика)
            starline_code[byteIndex] |= (1 << (starlineZCounter % 8));
        }
        
        bValidPacket = true;
    } else if (difTime2 > 150 && difTime2 < 350 && lastRxValue == 1) { // получили 0
        int byteIndex = starlineZCounter / 8;          // Определяем индекс байта
        int bitPosition = 7 - (starlineZCounter % 8);  // Определяем позицию бита (старший бит = 7)
        
        if (decodeMethod == 0) {
            // Очищаем бит (устанавливаем в 0)
            starline_code[byteIndex] &= ~(1 << bitPosition);
        } else {
            // Очищаем бит (альтернативная логика)
            starline_code[byteIndex] &= ~(1 << (starlineZCounter % 8));
        }     

        bValidPacket = true;
      }
      else if(lastRxValue == 0){
      }
      else{
        starline_state=1;
        starlineZCounter = 0;
      }      
      if(bValidPacket){
        starlineZCounter++;
        if(starlineZCounter==64){           //64ili66
          starline_vardump();
          starlineZCounter = 0;
          starline_state = 0;
          starlineZpreambulaCounter = 0; /////////novshetch
          starkk++;                   /////////shetchik
        }
      }
    }
}

// Функция для шифрования с использованием XOR
uint64_t simpleEncrypt(uint64_t data, uint64_t key) {
    return data ^ key; // Применяем XOR для шифрования
}
void starline_vardump(){
  if(decodeMethod == 0){
    Serial.println(" - starline origin - ");
  }
  else{
    Serial.println(" - starline invert - ");
  } 
  byte starline_codeM[9];
  memcpy(starline_codeM, starline_code, sizeof(starline_code));
  Serial.print(starline_code[0], HEX);
  Serial.print(starline_code[1], HEX);
  Serial.print(starline_code[2], HEX);
  Serial.print(starline_code[3], HEX);
  Serial.println("-hop");
  Serial.print(starline_code[4], HEX);
  Serial.print(starline_code[5], HEX);
  Serial.print(starline_code[6], HEX);
  Serial.println("-fix");
  Serial.print(starline_code[7], HEX);
  Serial.println("-btn");
  Serial.print(starline_code[8], HEX);
  Serial.println("-dop");
  Serial.println();
reverseBitsInBytes(starline_code,starline_codeM, 8);
byte* inverted_star = invertArray(starline_codeM, 8);
    // Выводим инвертированный массив
    Serial.println("rev array:");
    for (int i = 0; i < 8; i++) {
        Serial.print(starline_codeM[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    Serial.println("inv array:");
    for (int i = 0; i < 8; i++) {
        Serial.print(inverted_star[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
// Перенос значений starline_code[0], [1], [2] и [3] в переменную hop
// Перенос значений starline_code[0], [1], [2] и [3] в переменную hop
hop = (static_cast<uint32_t>(starline_codeM[4]) << 24) | 
      (static_cast<uint32_t>(starline_codeM[5]) << 16) | 
      (static_cast<uint32_t>(starline_codeM[6]) << 8) | 
      (static_cast<uint32_t>(starline_codeM[7]));  

 Serial.print("hop to decrypt:");Serial.println(hop,HEX);
for (int i = 0; i < sizeof(ManafactureNS) / sizeof(ManafactureNS[0]); i++) {
    // Дешифруем данные с помощью текущего ключа
    uint64_t encryptionKey = 0x1234567890ABCDEF; // Задаем ключ шифрования
    uint64_t decryptedKey =ManafactureNS[i].key;//simpleEncryptDecrypt(Manafacture[i].key, encryptionKey);//  Manafacture[i].key;//Manafacture[i].key;     
    uint64_t decryptedData = subghz_protocol_keeloq_common_decrypt(hop, decryptedKey);
    // Извлекаем старшие 8 бит (btnm)
    btnm = (decryptedData >> 24) & 0xFF;
    // Извлекаем следующие 8 бит (fixm)
    uint8_t fixm = (decryptedData >> 16) & 0xFF;
    // Извлекаем младшие 16 бит (cntm)
    cntm = decryptedData & 0xFFFF;
    // Проверяем, совпадают ли btnm и fixm с соответствующими значениями из starline_code
    if (btnm == inverted_star[7] && fixm == inverted_star[4]) {
        Serial.println("Manafactured!!!");
        Serial.print("Count (cntm): 0x");
        Serial.println(cntm, HEX);
        foundProtocol=String(ManafactureNS[i].name);
        // Получаем hop_to_sender
        hop_to_sender = subghz_protocol_keeloq_common_encrypt(decryptedData, ManafactureNS[i].key);
        Serial.print("hop_to_sender: 0x");
        Serial.println(hop_to_sender, HEX);        
        CrazyMonkey=decryptedKey;
        manafak = true; // Устанавливаем флаг успешного выполнения
        saveUniqueCode("sta0",starline_code, sizeof(starline_code),ManafactureNS[i].name);  
        foundProtocol=String(ManafactureNS[i].name);
             
        break; // Выходим из цикла, если нашли совпадение
    }
}

// Если не нашли ни одного совпадения
if (!manafak) {
    Serial.println("No matching device found.");
    foundProtocol="";
}


///proda vibrosaaaaaaaaaaaaaaaa
if(kk<2){
//prefs.begin("sta0");
//prefs.putBytes("sta0",starline_code,9);
//prefs.end();
}
priem="333";
///proda vibrosaaaaaaaaaaaaaaaa
codan="code starline:"+String(starline_code[0],HEX);
codan=codan+String(starline_code[1],HEX);
codan=codan+String(starline_code[2],HEX);
codan=codan+String(starline_code[3],HEX);
codan=codan+"-hop-";
codan=codan+String(starline_code[4],HEX);
codan=codan+String(starline_code[5],HEX);
codan=codan+String(starline_code[6],HEX);
codan=codan+"-fix-";
codan=codan+String(starline_code[7],HEX);
codan=codan+"-btn-";
codan=codan+String(starline_code[8],HEX);
if(manafak){codan=codan+String(":M:")+foundProtocol;manafak=false;}
else{saveUniqueCode("sta0",starline_code, sizeof(starline_code),"unknown");}
Serial.println(codan);
disponto=true;
}
void keelog_get(){
    bValidPacket = false;
    if(keelog_state==0){//ждем преамбулу и хедер
      if(difTime > 280 && difTime < 620 && lastRxValue != tempRxValue){
        keelogCounter ++;
      }
      else{
        if(keelogCounter==23){
          if(difTime>4000 && difTime<6200 && lastRxValue == 0){
            keelog_state=1;
          }
        }
       keelogCounter = 0;
      }
    }
    else if(keelog_state==1){// получаем биты
      if(difTime > 560 && difTime < 1240 && lastRxValue == 1){// получили 1
        if(decodeMethod==1){
          keelog_code[lround(keelogCounter/8)] = (keelog_code[lround(keelogCounter/8)]>>1)|B10000000;
        }
        else{
          keelog_code[lround(keelogCounter/8)] = (keelog_code[lround(keelogCounter/8)]<<1)|B00000000;
        }
        bValidPacket = true;
      }
      else if(difTime > 280 && difTime < 620 && lastRxValue == 1){
        if(decodeMethod==1){
          keelog_code[lround(keelogCounter/8)] = (keelog_code[lround(keelogCounter/8)]>>1)|B00000000;
        }
        else{
          keelog_code[lround(keelogCounter/8)] = (keelog_code[lround(keelogCounter/8)]<<1)|B00000001;
        }
        bValidPacket = true;
      }
      else if(lastRxValue == 0){
      }
      else{
        keelog_state=1;
        keelogCounter = 0;
      }
      
      if(bValidPacket){
        keelogCounter++;
        if(keelogCounter==66){
          keelog_vardump();
          keelogCounter = 0;
          keelog_state = 0;
         ///////  glushilka();
        }
      }
    }
}


void keelog_vardump(){
  if(decodeMethod == 0){
    Serial.println(" - keelog origin - ");
  }
  else{
    Serial.println(" - keelog invert - ");
  }
  Serial.print(keelog_code[0], HEX);
  Serial.print(keelog_code[1], HEX);
  Serial.print(keelog_code[2], HEX);
  Serial.print(keelog_code[3], HEX);
  Serial.println("-hop");
  Serial.print(keelog_code[4], HEX);
  Serial.print(keelog_code[5], HEX);
  Serial.print(keelog_code[6], HEX);
  Serial.println("-fix");
  Serial.print(keelog_code[7], HEX);
  Serial.println("-btn");
  Serial.print(keelog_code[8], HEX);
  Serial.println("-dop");
  byte keeloq_codeM[9];
memcpy(keeloq_codeM, keelog_code, sizeof(keelog_code));
  byte* inverted_kee = keelog_code; //invertArray(keelog_code, 9);
    // Выводим инвертированный массив
    Serial.println("Inverted array:");
    for (int i = 0; i < 8; i++) {
        Serial.print(keelog_code[i], HEX);
        Serial.print(" ");
    }
   reverseBitsInBytes(keelog_code,keeloq_codeM, 8);
    Serial.println("M array:");
    for (int i = 0; i < 8; i++) {
        Serial.print(keeloq_codeM[i], HEX);
        Serial.print(" -");
    }

//byte* inverted_kee = invertArray(keeloq_codeM, 9);
// hop = (static_cast<uint32_t>(keeloq_codeM[3]) << 24) | 
//       (static_cast<uint32_t>(keeloq_codeM[4]) << 16) | 
//       (static_cast<uint32_t>(keeloq_codeM[5]) << 8) | 
//       (static_cast<uint32_t>(keeloq_codeM[6]));
uint32_t hop = (keeloq_codeM[4] << 24) | (keeloq_codeM[5] << 16) | (keeloq_codeM[6] << 8) | keeloq_codeM[7];
Serial.println();
Serial.print("HOP:");Serial.println(hop,HEX);
Serial.print("btn:");Serial.println(keeloq_codeM[0]>>4,HEX);
Serial.print("SN:");Serial.println(keeloq_codeM[3],HEX);
for (int i = 0; i < sizeof(ManafactureNS) / sizeof(ManafactureNS[0]); i++) {
    // Дешифруем данные с помощью текущего ключа
    uint64_t encryptionKey = 0x1234567890ABCDEF; // Задаем ключ шифрования
    uint64_t decryptedKey =  ManafactureNS[i].key;//Manafacture[i].key;  simpleEncryptDecrypt(Manafacture[i].key, encryptionKey);// 
    uint64_t decryptedData = subghz_protocol_keeloq_common_decrypt(hop, decryptedKey);

Serial.print("DECRYPTED:");Serial.println(decryptedData,HEX);
    // Извлекаем старшие 8 бит (btnm)
     btnm = (decryptedData >> 28) & 0xFF;
    // Извлекаем следующие 8 бит (fixm)
    uint8_t fixm = (decryptedData >> 16) & 0xFF;
    // Извлекаем младшие 16 бит (cntm)
     cntm = decryptedData & 0xFFFF;
Serial.print("btnCr:");Serial.println(btnm);
Serial.print("SNCr:");Serial.println(fixm,HEX);
Serial.print("Name:");Serial.println(ManafactureNS[i].name);
    // Проверяем, совпадают ли btnm и fixm с соответствующими значениями из keloq
    if (btnm == keeloq_codeM[0]>>4&& fixm == keeloq_codeM[3]) {
        Serial.println("Manafactured!!!");
        Serial.print("Count (cntm): 0x");
        Serial.println(cntm, HEX);
        foundProtocol=String(ManafactureNS[i].name);
       
        // Получаем hop_to_sender
        hop_to_sender = subghz_protocol_keeloq_common_encrypt(decryptedData, ManafactureNS[i].key);
        Serial.print("hop_to_sender: 0x");
        Serial.println(hop_to_sender, HEX);

        // Сохраняем расшифрованные данные в Preferences
     //   prefs.begin("decryptedData");
      //  prefs.putUInt("decryptedData", decryptedData);
    //    prefs.end();

        // Сохраняем starline_codeM в Preferences
    //    prefs.begin("kee0M");
    //    prefs.putBytes("kee0M", keeloq_codeM, 9);
     //   prefs.end();
        
        CrazyMonkey=decryptedKey;

  // prefs.begin("keeMonkey", false); // Открываем пространство имен
    // Сохранение значения
  //  prefs.putULong64("monky",  CrazyMonkey);    // Получение значения  
  //  prefs.end(); // Закрываем пространство имен

        manafak = true; // Устанавливаем флаг успешного выполнения
        // Определите длину явно
if(manafak){size_t keelog_code_length = sizeof(keelog_code);  // Вернёт 9, так как массив известен на этапе компиляции
saveUniqueCode("kee0", inverted_kee, 9,ManafactureNS[i].name);}
        break; // Выходим из цикла, если нашли совпадение
    }
}
// Если не нашли ни одного совпадения
if (!manafak) {
    Serial.println("No matching device found.");
    foundProtocol="";
}
priem="333";
codan="code keqloq:"+String(inverted_kee[0],HEX);
codan=codan+String(inverted_kee[1],HEX);
codan=codan+String(inverted_kee[2],HEX);
codan=codan+String(inverted_kee[3],HEX);
codan=codan+"-hop-";
codan=codan+String(inverted_kee[4],HEX);
codan=codan+String(inverted_kee[5],HEX);
codan=codan+String(inverted_kee[6],HEX);
codan=codan+"-fix-";
codan=codan+String(inverted_kee[7],HEX);
codan=codan+"-btn-";
codan=codan+String(inverted_kee[8],HEX);
if(manafak){codan=codan+String(":M:")+foundProtocol;manafak=false;}
else{// Определите длину явно
size_t keelog_code_length = sizeof(keelog_code);  // Вернёт 9, так как массив известен на этапе компиляции
saveUniqueCode("kee0", inverted_kee, keelog_code_length,"unknown");}
Serial.println(codan);
 if(k<2){
// prefs.begin("kee0");
// prefs.putBytes("kee0",inverted_kee,9);
// prefs.end();
//k++;
 }


disponto=true;
}
void keelog_send(byte* keelog_code){
  Serial.println("- sending keelog -");
  for(int i = 0; i<9; i++){
    Serial.print(keelog_code[i], HEX);
    Serial.print(" - ");
  }
  for(int i = 0; i<11; i++){//посылаем преамблу
    send_meander(400);
  }
 digitalWrite(TX, HIGH);
 delayMicroseconds(400);
 digitalWrite(TX, LOW);
  delayMicroseconds(4000);//посылаем хедер
  for( int i = 0; i<9; i++){
    if(decodeMethod==0){
      for(int i2 = 7;i2>=0;i2--){
        if(bitRead(keelog_code[i], i2)){///1 согласно
          digitalWrite(TX, HIGH);
          delayMicroseconds(400);
          digitalWrite(TX, LOW);
          delayMicroseconds(2*400);
        }
        else{
          digitalWrite(TX, HIGH);///0 согласно
          delayMicroseconds(2*400);
          digitalWrite(TX, LOW);
          delayMicroseconds(400);
        }
      }
    }
    else{
        for(int i2 = 7;i2>=0;i2--){
          if(!bitRead(keelog_code[i], i2)){
            digitalWrite(TX, HIGH);
            delayMicroseconds(400);
            digitalWrite(TX, LOW);
            delayMicroseconds(400*2);
          }
          else{
            digitalWrite(TX, HIGH);
            delayMicroseconds(400*2);
            digitalWrite(TX, LOW);
            delayMicroseconds(400);
          }
       } 
    }
    
  }
  //  digitalWrite(TX, LOW);
  //  delayMicroseconds(1000);
}

void starline_send(byte* starline_code,int invert){
  
  Serial.println("- sending staray -");
  for(int i = 0; i<9; i++){
    Serial.print(starline_code[i], HEX);
    Serial.print(" - ");
  }

  for(int i = 0; i<13; i++){//посылаем преамблу//13 srab
    send_meander(1000);
  }
  //digitalWrite(TX, HIGH);
 // delayMicroseconds(1000);
 // digitalWrite(TX, LOW);
  //delayMicroseconds(4000);//посылаем хедер
  for( int i = 0; i<8; i++){
    if(invert==1){
      for(int i2 = 8;i2>=0;i2--){
        if(bitRead(starline_code[i], i2)){
          digitalWrite(TX, HIGH);
          delayMicroseconds(250);
          digitalWrite(TX, LOW);
          delayMicroseconds(250);
        }
        else{
          digitalWrite(TX, HIGH);
          delayMicroseconds(500);
          digitalWrite(TX, LOW);
          delayMicroseconds(500);
        }
      }
    }
    else{
        for(int i2 = 0;i2<8;i2++){
          if(!bitRead(starline_code[i], i2)){
            digitalWrite(TX, HIGH);
            delayMicroseconds(250);
            digitalWrite(TX, LOW);
            delayMicroseconds(250);
          }
          else{
            digitalWrite(TX, HIGH);
            delayMicroseconds(500);
            digitalWrite(TX, LOW);
            delayMicroseconds(500);
          }
       } 
    }
    
  }
  
}


bool manufan(){return manafak;}
void starline_sendMan(byte* starline_code){
 
  Serial.println("- sending staray -");
  for(int i = 0; i<9; i++){
    Serial.print(starline_code[i], HEX);
    Serial.print(" - ");
  }

  for(int i = 0; i<13; i++){//посылаем преамблу//13 srab
    send_meander(1000);
  }
  //digitalWrite(TX, HIGH);
 // delayMicroseconds(1000);
 // digitalWrite(TX, LOW);
  //delayMicroseconds(4000);//посылаем хедер
  for( int i = 0; i<8; i++){
    if(decodeMethod==1){
      for(int i2 = 8;i2>=0;i2--){
        if(bitRead(starline_code[i], i2)){
          digitalWrite(TX, HIGH);
          delayMicroseconds(250);
          digitalWrite(TX, LOW);
          delayMicroseconds(250);
        }
        else{
          digitalWrite(TX, HIGH);
          delayMicroseconds(500);
          digitalWrite(TX, LOW);
          delayMicroseconds(500);
        }
      }
    }
    else{
        for(int i2 = 0;i2<8;i2++){
          if(!bitRead(starline_code[i], i2)){
            digitalWrite(TX, HIGH);
            delayMicroseconds(250);
            digitalWrite(TX, LOW);
            delayMicroseconds(250);
          }
          else{
            digitalWrite(TX, HIGH);
            delayMicroseconds(500);
            digitalWrite(TX, LOW);
            delayMicroseconds(500);
          }
       } 
    }
    
  }
  
}

static void ganstaDelaymic(unsigned long ms)
{
  unsigned long start = micros();
  do
  {yield();
  } while (micros() - start < ms);
}
void glushilko_gansta(int tim){yield();
while (jam_g){if(!jam_g){break;}yield();
digitalWrite(TX,!digitalRead(TX)); 
ganstaDelaymic(tim);}
}

void SendCame4(long Code) {
   Serial.println("snding shlako:"+String(Code));
   sending=String("snding shlako:")+String(Code);
    disponto2=true;
  for (int j = 0; j < 7; j++) { // посылку посылаем 4 раза подряд.
    // время стартового импульса 
    digitalWrite(TX, HIGH);
    delayMicroseconds(320);
    digitalWrite(TX, LOW);
    for (int i = 12; i > 0; i--) {
      byte b = bitRead(Code, i - 1); // побитово перебираем и посылаем код
      if (b) {
        digitalWrite(TX, LOW); // 1
        delayMicroseconds(640);
        digitalWrite(TX, HIGH);
        delayMicroseconds(320);
      }
      else {
        digitalWrite(TX, LOW); // 0
        delayMicroseconds(320);
        digitalWrite(TX, HIGH);
        delayMicroseconds(640);
      }
    }
    digitalWrite(TX, LOW);
    delayMicroseconds(11520);
  }
}



void posilkeeloq1(){
//prefs.begin("kee0", false);
//prefs.getBytes("kee0",keelog_codePAK1,9);
//prefs.end();
for(int i=0;i<30;i++){keelog_send(keelog_codePAK1);}
//digitalWrite(TX, HIGH);
//delay(100);
//digitalWrite(TX, LOW);
sending="code keqloq:"+String( keelog_codePAK1[0],HEX);
sending=sending+String( keelog_codePAK1[1],HEX);
sending=sending+String( keelog_codePAK1[2],HEX);
sending=sending+String( keelog_codePAK1[3],HEX);
sending=sending+String( keelog_codePAK1[4],HEX);
sending=sending+String( keelog_codePAK1[5],HEX);
sending=sending+String( keelog_codePAK1[6],HEX);
sending=sending+String( keelog_codePAK1[7],HEX);
sending=sending+String( keelog_codePAK1[8],HEX);
 disponto2=true;   
  keelog_state = 0;
/// for(int i = 0; i<9; i++){
///    keelog_code[i]=0;}

//Serial.println("srabotalo-keeloq");
/// for(int i = 0; i<9; i++){
///    keelog_code[i]=0;}
//Serial.println("srabotalo-keeloq");
}
void posilkeeloq2(){
//prefs.begin("kee0", false);
//EEPROM.begin(3512);
// keelog_codePAK1[0]=EEPROM.read(0);
//keelog_codePAK1[0]=prefs.getUChar("kee0", 0);
//prefs.getBytes("kee0",keelog_codePAK2,9);
//prefs.end();
sending="code keqloq:"+String( keelog_codePAK2[0],HEX);
sending=sending+String(keelog_codePAK2[1],HEX);
sending=sending+String(keelog_codePAK2[2],HEX);
sending=sending+String(keelog_codePAK2[3],HEX);
sending=sending+String(keelog_codePAK2[4],HEX);
sending=sending+String(keelog_codePAK2[5],HEX);
sending=sending+String(keelog_codePAK2[6],HEX);
sending=sending+String(keelog_codePAK2[7],HEX);
sending=sending+String(keelog_codePAK2[8],HEX);
for(int i=0;i<8;i++){ keelog_send(keelog_codePAK2);}
 digitalWrite(TX, HIGH);
  delay(100);
  digitalWrite(TX, LOW);
//vibros na displey   
  keelog_state = 0;
  disponto2=true;
/// for(int i = 0; i<9; i++){
///    keelog_codePAK2[i]=0;}
/// for(int i = 0; i<9; i++){
///    keelog_codePAK2[i]=0;}
}

void posilstarlinemana(){ 
if(decryptedData ==0){
//prefs.begin("decryptedData");
//decryptedData=prefs.getUInt("decryptedData", 0); 
//prefs.end();
}
if(CrazyMonkey==0){
 //  prefs.begin("staMonkey", false); // Открываем пространство имен  
    // Получение значения
   // CrazyMonkey = prefs.getULong64("monky", 0); // Получаем значение, по умолчанию 0
 //   prefs.end(); // Закрываем пространство имен
}
 //prefs.begin("sta0M", false);
 //prefs.getBytes("sta0M",starline_codePAK1,9);
// prefs.end();
for(int i=0;i<7;i++){
cntm++;
 if(cntm>255){cntm=0;}
// Обновляем `decryptedData` с новым значением `cntm`
decryptedData = (decryptedData & 0xFFFF0000) | cntm;
//decryptedData = (decryptedData & 0x00FFFFFF) | (0x46000000);
decryptedData = (decryptedData & 0x00FFFFFF) | (buttonclose << 24);
Serial.println(cntm,HEX);  
hop_to_sender=subghz_protocol_keeloq_common_encrypt(decryptedData,CrazyMonkey);
Serial.print("hopsender:");
Serial.println(hop_to_sender,HEX);
// Разворачиваем `hop_to_sender` побайтово и сохраняем в `starline_codePAK1`
starline_codePAK1[0] = (hop_to_sender >> 0) & 0xFF;   // Младший байт (0xF2)
starline_codePAK1[1] = (hop_to_sender >> 8) & 0xFF;   // Следующий байт (0x3D)
starline_codePAK1[2] = (hop_to_sender >> 16) & 0xFF;  // Следующий байт (0x4E)
starline_codePAK1[3] = (hop_to_sender >> 24) & 0xFF;  // Старший байт (0x49)
starline_codePAK1[7]=buttonclose;
sending="code star_line:"+String(   starline_codePAK1[0],HEX);
sending=sending+String(   starline_codePAK1[1],HEX);
sending=sending+String(   starline_codePAK1[2],HEX);
sending=sending+String(   starline_codePAK1[3],HEX);
sending=sending+String(  starline_codePAK1[4],HEX);
sending=sending+String(   starline_codePAK1[5],HEX);
sending=sending+String(  starline_codePAK1[6],HEX);
sending=sending+String(   starline_codePAK1[7],HEX);
sending=sending+String(   starline_codePAK1[8],HEX);
  starline_sendMan(starline_codePAK1);}
   disponto2=true;
  digitalWrite(TX, HIGH);
  delay(10);
  digitalWrite(TX, LOW); 
 
  starline_state = 0;
// for(int i = 0; i<9; i++){
 //   starline_code[i]=0;
//  }
 
///  Serial.println("srabotalo");
}
void convertStringsToBytes(const String &str1, const String &str2) {
    // Преобразуем каждую строку в число и записываем в байтовые переменные
    buttonopen = (byte) strtol(str1.c_str(), NULL, 16); // Интерпретация строки как hex числа
    buttonclose = (byte) strtol(str2.c_str(), NULL, 16);
    btnm=buttonopen;
}
void posilstarlinemanaOpen(){ 
if(decryptedData==0){
//prefs.begin("decryptedData");
//decryptedData=prefs.getUInt("decryptedData", 0); 
//prefs.end();
}

if(CrazyMonkey==0){
    // prefs.begin("staMonkey", false); // Открываем пространство имен
    // Получение значения
  //  uint64_t savedKey = prefs.getULong64("monky", 0); // Получаем значение, по умолчанию 0
  //  prefs.end(); // Закрываем пространство имен
}
//hop_to_sender== (hop_to_sender & 0xFF00FFFF) | (0x42000000);
// prefs.begin("sta0M", false);
 //prefs.getBytes("sta0M",starline_codePAK1,9);
 //prefs.end();
for(int i=0;i<7;i++){
 cntm++;
 if(cntm>255){cntm=0;}
// Обновляем `decryptedData` с новым значением `cntm`
decryptedData = (decryptedData & 0xFFFF0000) | cntm;
//decryptedData = (decryptedData & 0x00FFFFFF) | (0x42000000);
decryptedData = (decryptedData & 0x00FFFFFF) | (buttonopen << 24);
//decryptedData = (decryptedData & 0xFF00FFFF) | (0x42000000);
Serial.println(decryptedData,HEX);
Serial.println(cntm,HEX);  
hop_to_sender=subghz_protocol_keeloq_common_encrypt(decryptedData,CrazyMonkey);
Serial.print("hopsender:");
Serial.println(hop_to_sender,HEX);
// Разворачиваем `hop_to_sender` побайтово и сохраняем в `starline_codePAK1`
starline_codePAK1[0] = (hop_to_sender >> 0) & 0xFF;   // Младший байт (0xF2)
starline_codePAK1[1] = (hop_to_sender >> 8) & 0xFF;   // Следующий байт (0x3D)
starline_codePAK1[2] = (hop_to_sender >> 16) & 0xFF;  // Следующий байт (0x4E)
starline_codePAK1[3] = (hop_to_sender >> 24) & 0xFF;  // Старший байт (0x49)
starline_codePAK1[7] = buttonopen;
sending="code star_line:"+String(   starline_codePAK1[0],HEX);
sending=sending+String(   starline_codePAK1[1],HEX);
sending=sending+String(   starline_codePAK1[2],HEX);
sending=sending+String(   starline_codePAK1[3],HEX);
sending=sending+String(  starline_codePAK1[4],HEX);
sending=sending+String(   starline_codePAK1[5],HEX);
sending=sending+String(  starline_codePAK1[6],HEX);
sending=sending+String(   starline_codePAK1[7],HEX);
sending=sending+String(   starline_codePAK1[8],HEX);
starline_sendMan(starline_codePAK1);}
disponto2=true;
digitalWrite(TX, HIGH);
delay(10);
digitalWrite(TX, LOW); 
 
  starline_state = 0;
// for(int i = 0; i<9; i++){
 //   starline_code[i]=0;
//  }
 
///  Serial.println("srabotalo");
}


void posilkeloqmanaOpen(){ 
if(decryptedData==0){
//prefs.begin("decryptedData");
//decryptedData=prefs.getUInt("decryptedData", 0); 
//prefs.end();
}

if(CrazyMonkey==0){
  //   prefs.begin("keeMonkey", false); // Открываем пространство имен
    // Получение значения
 //   uint64_t savedKey = prefs.getULong64("monky", 0); // Получаем значение, по умолчанию 0
 //   prefs.end(); // Закрываем пространство имен
}
//hop_to_sender== (hop_to_sender & 0xFF00FFFF) | (0x42000000);
 //prefs.begin("kee0M", false);
 //prefs.getBytes("kee0M",keelog_codePAK1,9);
 //prefs.end();
for(int i=0;i<7;i++){
 cntm++;
 if(cntm>255){cntm=0;}
// Обновляем `decryptedData` с новым значением `cntm`
decryptedData = (decryptedData & 0xFFFF0000) | cntm;
//decryptedData = (decryptedData & 0x00FFFFFF) | (0x42000000);
decryptedData = (decryptedData & 0x00FFFFFF) | (buttonopen << 28);
//decryptedData = (decryptedData & 0xFF00FFFF) | (0x42000000);
Serial.println(decryptedData,HEX);
Serial.println(cntm,HEX);  
hop_to_sender=subghz_protocol_keeloq_common_encrypt(decryptedData,CrazyMonkey);
Serial.print("hopsender:");
Serial.println(hop_to_sender,HEX);
// Разворачиваем `hop_to_sender` побайтово и сохраняем в `starline_codePAK1`
keelog_codePAK1[0] = (hop_to_sender >> 0) & 0xFF;   // Младший байт (0xF2)
keelog_codePAK1[1] = (hop_to_sender >> 8) & 0xFF;   // Следующий байт (0x3D)
keelog_codePAK1[2] = (hop_to_sender >> 16) & 0xFF;  // Следующий байт (0x4E)
keelog_codePAK1[3] = (hop_to_sender >> 24) & 0xFF;  // Старший байт (0x49)
keelog_codePAK1[7] = buttonopen;
sending="code star_line:"+String(   keelog_codePAK1[0],HEX);
sending=sending+String(  keelog_codePAK1[1],HEX);
sending=sending+String(   keelog_codePAK1[2],HEX);
sending=sending+String(  keelog_codePAK1[3],HEX);
sending=sending+String(  keelog_codePAK1[4],HEX);
sending=sending+String(   keelog_codePAK1[5],HEX);
sending=sending+String(  keelog_codePAK1[6],HEX);
sending=sending+String(   keelog_codePAK1[7],HEX);
sending=sending+String(  keelog_codePAK1[8],HEX);
keelog_send(keelog_codePAK1);}
disponto2=true;
digitalWrite(TX, HIGH);
delay(10);
digitalWrite(TX, LOW);  
starline_state = 0;
// for(int i = 0; i<9; i++){
 //   starline_code[i]=0;
//  }
 
///  Serial.println("srabotalo");
}


void posilstarline1(){

 
//prefs.begin("sta0", false);
//prefs.getBytes("sta0",starline_codePAK1,9);
//prefs.end();
   sending="code star_line:"+String(   starline_codePAK1[0],HEX);
sending=sending+String(   starline_codePAK1[1],HEX);
sending=sending+String(   starline_codePAK1[2],HEX);
sending=sending+String(   starline_codePAK1[3],HEX);
sending=sending+String(  starline_codePAK1[4],HEX);
sending=sending+String(   starline_codePAK1[5],HEX);
sending=sending+String(  starline_codePAK1[6],HEX);
sending=sending+String(   starline_codePAK1[7],HEX);
sending=sending+String(   starline_codePAK1[8],HEX);

starline_send(starline_codePAK1,0);
 disponto2=true;
  digitalWrite(TX, HIGH);
  delay(100);
  digitalWrite(TX, LOW); 
 
  starline_state = 0;
// for(int i = 0; i<9; i++){
 //   starline_code[i]=0;
//  }
 
///  Serial.println("srabotalo");
}
void posilstarline2(){ 
//prefs.begin("sta0", false);
//prefs.getBytes("sta0",starline_codePAK2,9);
//prefs.end();
sending="code star_line:"+String(   starline_codePAK2[0],HEX);
sending=sending+String(   starline_codePAK2[1],HEX);
sending=sending+String(   starline_codePAK2[2],HEX);
sending=sending+String(   starline_codePAK2[3],HEX);
sending=sending+String(  starline_codePAK2[4],HEX);
sending=sending+String(   starline_codePAK2[5],HEX);
sending=sending+String(  starline_codePAK2[6],HEX);
sending=sending+String(   starline_codePAK2[7],HEX);
sending=sending+String(   starline_codePAK2[8],HEX);
 starline_sendPAK2(starline_codePAK2); 
 disponto2=true;
  digitalWrite(TX, HIGH);
  delay(100);
  digitalWrite(TX, LOW);
   
  starline_state = 0;
// for(int i = 0; i<9; i++){
//    starline_code[i]=0;
 // }
 
 }

//pak2
void keelog_sendPAK2(byte* keelog_codePAK2){
  Serial.println("- sending keelog -ss-");
  for(int i = 0; i<9; i++){
    Serial.print(keelog_codePAK2[i], HEX);
    Serial.print(" -zz ");
  }
  for(int i = 0; i<11; i++){//посылаем преамблу
    send_meander(400);
  }
  digitalWrite(TX, HIGH);
  delayMicroseconds(400);
  digitalWrite(TX, LOW);
  delayMicroseconds(4000);//посылаем хедер
  for( int i = 0; i<9; i++){
    if(decodeMethod==1){
      for(int i2 = 7;i2>=0;i2--){
        if(bitRead(keelog_codePAK2[i], i2)){
          digitalWrite(TX, HIGH);
          delayMicroseconds(keloq_difftime[0]);
          digitalWrite(TX, LOW);
          delayMicroseconds(2*keloq_difftime[0]);
        }
        else{
          digitalWrite(TX, HIGH);
          delayMicroseconds(2*keloq_difftime[1]);
          digitalWrite(TX, LOW);
          delayMicroseconds(keloq_difftime[1]);
        }
      }
    }
    else{
        for(int i2 = 0;i2<8;i2++){
          if(!bitRead(keelog_codePAK2[i], i2)){
            digitalWrite(TX, HIGH);
            delayMicroseconds(keloq_difftime[1]);
            digitalWrite(TX, LOW);
            delayMicroseconds(2*keloq_difftime[1]);
          }
          else{
            digitalWrite(TX, HIGH);
            delayMicroseconds(2*keloq_difftime[0]);
            digitalWrite(TX, LOW);
            delayMicroseconds(keloq_difftime[0]);
          }
       } 
    }
    
  }
}
void starline_sendPAK2(byte* starline_codePAK2){
  Serial.println("- sending staraylayn -ss-");
  for(int i = 0; i<9; i++){
    Serial.print(starline_codePAK2[i], HEX);
    Serial.print(" -zz ");
  }
  for(int i = 0; i<5; i++){//посылаем преамблу
    send_meander(1000);
  }
//  digitalWrite(TX, HIGH);
 // delayMicroseconds(400);
//  digitalWrite(TX, LOW);
//  delayMicroseconds(4000);//посылаем хедер
  
  for( int i = 0; i<8; i++){
    if(decodeMethod==1){
      for(int i2 = 7;i2>=0;i2--){
        if(bitRead(starline_codePAK2[i], i2)){
          digitalWrite(TX, HIGH);
          delayMicroseconds(250);
          digitalWrite(TX, LOW);
          delayMicroseconds(250);
        }
        else{
          digitalWrite(TX, HIGH);
          delayMicroseconds(500);
          digitalWrite(TX, LOW);
          delayMicroseconds(500);
        }
      }
    }
    else{
        for(int i2 = 0;i2<8;i2++){
          if(!bitRead(starline_codePAK2[i], i2)){
            digitalWrite(TX, HIGH);
            delayMicroseconds(250);
            digitalWrite(TX, LOW);
            delayMicroseconds(250);
          }
          else{
            digitalWrite(TX, HIGH);
            delayMicroseconds(500);
            digitalWrite(TX, LOW);
            delayMicroseconds(500);
          }
       } 
    }
    
  }
}

void SendNice(long Code) {
  for (int j = 0; j < 7; j++) { // посылку посылаем 4 раза подряд.
    // время стартового импульса 
    digitalWrite(TX, HIGH);
    delayMicroseconds(SMALL);
    digitalWrite(TX, LOW);
     ////delayMicroseconds(PREAMBUL);
    for (int i = 24; i > 0; i--) {
      byte b = bitRead(Code, i - 1); // побитово перебираем и посылаем код
      if (b) {
        digitalWrite(TX, LOW); // 1
        delayMicroseconds(LONG);
        digitalWrite(TX, HIGH);
        delayMicroseconds(SMALL);
      }
      else {
        digitalWrite(TX, LOW); // 0
        delayMicroseconds(SMALL);
        digitalWrite(TX, HIGH);
        delayMicroseconds(LONG);
      }
    }
    digitalWrite(TX, LOW);
    delayMicroseconds(18500);
  }
}

String return_codan(){
codan=codan;
return codan;
}
String return_sending(){
sending=sending;
return sending;
}
String set_sending(String set){
sending=set;
return sending;
}
bool return_disponto(){disponto=disponto;return disponto; }
void falldisponto(){disponto=false;}
bool return_disponto2(){disponto2=disponto2;return disponto2; }
void falldisponto2(){disponto2=false;}
void jamok(bool jam){
jam_g=jam;
}
boolean CheckValue(unsigned int base, unsigned int value)
{
  return ((value == base) || ((value > base) && ((value - base) < MAX_DELTA_1)) || ((value < base) && ((base - value) < MAX_DELTA_1)));
}




void grabshlak() {
   state = digitalRead(rxPin);
  if (state == HIGH)
    lolen = micros() - prevtime;
  else
    hilen = micros() - prevtime;
  prevtime = micros(); 
  if (state == LOW)
  {
    // последовательность закончилась
    if (CheckValue(320, hilen) && CheckValue(640, lolen)) // valid 1
    {
      cameCode = (cameCode << 1) | 1;
      cameCounter++;
    }
    else if (CheckValue(640, hilen) && CheckValue(320, lolen)) // valid 0
    {
      cameCode = (cameCode << 1) | 0;
      cameCounter++;
    }
    else cameCounter = 0;
  } else 
      if (lolen > 1000 &&
           (cameCounter == 12 || cameCounter == 13) &&
           ((cameCode & 0xfff) != 0xfff))
  {
    lastCode = cameCode & 0xfff;
    codan="shlak-"+String(lastCode);
    Serial.println("Gaboslako"+String(lastCode));
    disponto=true;
    cameCounter = 0;
    cameCode = 0;     
  }
}



void grabshlak2023() {  
tempRxValue = digitalRead(rxPin);
if(tempRxValue != lastRxValue){
tempTime = micros();
if (tempRxValue == HIGH){lolen = tempTime - lastRxTime;}
if (tempRxValue == LOW){hilen = tempTime - lastRxTime;}
lastRxTime = tempTime;
lastRxValue = tempRxValue;
}  
//if(tempRxValue==LOW){
  int sibur640_1=640+MAX_DELTA_1;
  int sibur640_2=640-MAX_DELTA_1;
  int sibur320_1=320+MAX_DELTA_1;
  int sibur320_2=320-MAX_DELTA_1;
    // последовательность закончилась
    if (lolen>sibur640_2&&lolen<sibur640_1&&hilen>sibur320_2&&hilen<sibur320_1 ) // valid 1   ///lolen 640 hilen 320
    {
     /// cameCodeByte[cameCounter]=1;
      cameCode = (cameCode << 1) | 1;
      cameCounter++;
    ///Serial.println("1");
    }
    else if (hilen>sibur640_2&&hilen<sibur640_1&&lolen>sibur320_2&&lolen<sibur320_1 )// valid 0
    {
     /// Serial.println("0");
      cameCode = (cameCode << 1) | 0;
    ///  cameCodeByte[cameCounter]=0;
      cameCounter++;
      Serial.println(cameCounter);
    }
    else {cameCounter = 0;}
///}

//else 
      if (cameCounter>=13 )
  {
    lastCode = cameCode & 0xfff;
    Serial.println("Codemo"+String(lastCode));
 
    cameCounter = 0;
    cameCode = 0;
    }    
     
  //   lastCode = cameCode & 0xfff;
  //   codan="shlak-"+String(lastCode);
  //   Serial.println("Gaboslako"+String(lastCode));
  //   disponto=true;
  //  prefs.begin("esp32king", false);
  //  prefs.putULong("shlak0",lastCode);  
  //   prefs.end();
  //   cameCounter = 0;
  //   cameCode = 0;     
  
}



void grabshlack_mymod(){

  pinRX_int();
    RfReceive();

}

void sendshlackfrom_mem(){ 
///unsigned long zaduo;
byte potco[24];
   // prefs.begin("sta0", false);
  // prefs.getBytes("shlak",potco,24);  
  //  prefs.end(); 
    String code;
   code+=potco[0];
  code+="0";
   code+=potco[1];
   for(int i=0;i<2;i++){ Serial.print(potco[i],HEX);}
    Serial.println();
   /// lastCode=0;
   /// lastCode +=potco[0]<<2;
   //  lastCode +=potco[1];
   ////   getun=String(zaduo);
  Serial.println(code);
  lastCode=code.toInt();
  /// lastCode= potco;   
    SendCame4(lastCode);
}

void RfReceive()
{
    byte sambo[24];
    if (level != 255)
    {    
      
        p_level = level;
        p_len = len;
        len = 0;
        level = 255;     
        process_came();
        p_len_prev = p_len;
    }
    if (came.state == 100)
    {
        for (int i = 0; i < sizeof(came.data) - (came.dat_bit == CM_BITS12 ? 1 : 0); i++) {
            if (i > 0) {
               /// Serial.print("0");
                camo_codan+="0";
                camo_codanI+="0";
              ///  sambo[i]=0;
            }
            /// Serial.print(String(came.data[i],HEX));
             camo_codan+=String(came.data[i],HEX);
             camo_codanI+=String(came.data[i]);
             sambo[i]=came.data[i];
           // Serial.print(stringWithPrefix(String(came.data[i], HEX), came.dat_bit == CM_BITS12 ? 6 : 8, '0'));
        }

        codan=camo_codan;
       Serial.println(codan);
       Serial.println(camo_codanI);
   lastCode=camo_codanI.toInt();
 

  // prefs.begin("sta0", false);
   //prefs.putBytes("shlak",sambo,24);  
   // prefs.end();   
    delay(10);
        disponto=true;
        camo_codan="";
        camo_codanI="";
        came.state = 0;
        Serial.println();
    }
}


void pinRX_int()
{
  tempRxValue = digitalRead(rxPin);
  if(tempRxValue != lastRxValue){
    if (level != 255) return;
    len = micros() - last;
    last = micros();
    if (digitalRead(rxPin) == HIGH) level = 0;
    else level = 1;
    lastRxValue = tempRxValue;}
}

void process_came()
{
    unsigned char b;

    switch (came.state)
    {
    case 0:
        if (p_level) break;
        came.state = 1;
        break;
    case 1: //start
        if (!p_level) break;

        else if (p_len >= CM_MIN_TE && p_len <= CM_MAX_TE)
        {
            came.state = 2;
            came.dat_bit = 0;

            for (int i = 0; i < sizeof(came.data); i++) {
                came.data[i] = 0x00;
            }

        }
        else came.state = 0;
    case 2: //dat
        if (p_level)
        {
            if (came.dat_bit == CM_BITS24)
            {
                came.state = 0;
                break;
            }

            if (p_len_prev <= CM_MAX_TE && p_len_prev >= CM_MIN_TE &&
                    p_len <= CM_MAX_TE * 2 && p_len >= CM_MIN_TE * 2) b = 0;
            else if (p_len_prev <= CM_MAX_TE * 2 && p_len_prev >= CM_MIN_TE * 2 &&
                     p_len <= CM_MAX_TE && p_len >= CM_MIN_TE) b = 1;
            else
            {
                came.state = 0;
                break;
            }

            if (b) set_bit(came.data, came.dat_bit);
            came.dat_bit++;
            break;
        }
        else
        {
            if ((p_len > 5000) && (came.dat_bit == CM_BITS12 || came.dat_bit == CM_BITS24)) came.state = 100;
        }
        break;
    }
}


void set_bit(uint8_t *data, uint8_t n)
{
    data[n / 8] |= 1 << (n % 8);
}

String stringWithPrefix(String line, int len, char prefix)
{
    String addon = "";
    int n = len - line.length();
    for (int i = 0; i < n; i++) {
        addon += prefix;
    }
    return addon + line;
}

// Функция для получения сохранённых кодов с добавлением имени производителя
void getSavedCodes(const char* fileName, String& jsonResponse, uint8_t expectedCodeLength) {
    // Убедимся, что SPIFFS смонтирована
    if (!SPIFFS.begin(true)) {
        Serial.println("Ошибка монтирования SPIFFS.");
        jsonResponse = "[]";
        return;
    }

    // Открываем файл для чтения
    File file = SPIFFS.open(fileName, "r");
    if (!file) {
        Serial.println("Файл не найден, возвращаем пустой массив.");
        jsonResponse = "[]";
        return;
    }

    // Читаем содержимое файла
    size_t fileSize = file.size();
    if (fileSize == 0) {
        Serial.println("Файл пуст, возвращаем пустой массив.");
        jsonResponse = "[]";
        file.close();
        return;
    }

    // Создаем буфер для хранения данных
    std::unique_ptr<char[]> fileContent(new char[fileSize + 1]);
    file.readBytes(fileContent.get(), fileSize);
    fileContent[fileSize] = '\0';
    file.close();

    // Парсим содержимое файла как JSON
    DynamicJsonDocument doc(2048);  // Настройте размер под ваши данные
    DeserializationError error = deserializeJson(doc, fileContent.get());
    if (error) {
        Serial.println("Ошибка разбора JSON: " + String(error.c_str()));
        jsonResponse = "[]";
        return;
    }

    // Формируем JSON-ответ
    JsonArray codesArray = doc.as<JsonArray>();
    jsonResponse = "[";
    for (JsonObject codeObj : codesArray) {
        if (jsonResponse.length() > 1) {
            jsonResponse += ",";
        }

        // Начинаем объект
        jsonResponse += "{";

        // Добавляем код
        JsonArray codeArray = codeObj["code"];
        jsonResponse += "\"code\":[";
        for (size_t i = 0; i < codeArray.size(); ++i) {
            jsonResponse += "\"" + String((uint8_t)codeArray[i], HEX) + "\"";
            if (i < codeArray.size() - 1) {
                jsonResponse += ",";
            }
        }
        jsonResponse += "]";

        // Добавляем производителя
        const char* manufacturer = codeObj["manufacturer"] | "Неизвестно";
        jsonResponse += ",\"manufacturer\":\"" + String(manufacturer) + "\"";

        // Закрываем объект
        jsonResponse += "}";
    }
    jsonResponse += "]";
    Serial.println("Данные успешно прочитаны из SPIFFS:");
    Serial.println(jsonResponse);
}
// Функция обработки и отправки данных
void handleSpecialCode(const String &manufacturer, const String &code, const String &tableName,uint16_t lineNumber) {
    // Поиск ключа для указанного производителя
    uint64_t manufacturerKey = 0;
    bool keyFound = false;
    String filename="/sta0.txt";
    if(tableName=="keeCodeTable"){filename="/kee0.txt";}
    for (size_t i = 0; i < sizeof(ManafactureNS) / sizeof(ManafactureNS[0]); i++) {
        if (manufacturer.equalsIgnoreCase(ManafactureNS[i].name)) {
            manufacturerKey = ManafactureNS[i].key;           
            keyFound = true;
            break;
        }
    }
    if (!keyFound) {
        Serial.println("Ошибка: Производитель не найден!");
        return;
    }
    // Преобразование строки кода в массив байтов
    byte dataToSend[9] = {0};  // Массив для отправки
    size_t dataLen = 0;  // Количество байтов
    // Разделяем строку по запятой и обрабатываем каждый байт
    int startIndex = 0;
    while (startIndex < code.length()) {
        // Ищем позицию запятой
        int commaIndex = code.indexOf(',', startIndex);
        if (commaIndex == -1) {
            commaIndex = code.length();
        }
        // Извлекаем байт из строки и преобразуем его в число
        String byteStr = code.substring(startIndex, commaIndex);
        dataToSend[dataLen] = strtoul(byteStr.c_str(), nullptr, 16);
        dataLen++;
        // Обновляем индекс для следующего байта
        startIndex = commaIndex + 1;
        // Прерываем, если превышен размер массива
        if (dataLen >= sizeof(dataToSend)) {
            break;
        }
    }
    Serial.print("Таблица: ");
    Serial.println(tableName);
    Serial.print("Код: ");
    Serial.println(code);
    Serial.print("Производитель: ");
    Serial.println(manufacturer);
//    Serial.print("Производитель key: ");
//    Serial.println(manufacturerKey,HEX);
    cntm++;
    Serial.print("CNT: ");
    Serial.println(cntm,HEX);  
    ////////////ok 
  if(cntm>255){cntm=0;}
  byte starline_codeM[9];
  reverseBitsInBytes(dataToSend,starline_codeM, 8);
  byte* inverted_star = invertArray(starline_codeM, 8);  
    // Выводим инвертированный массив
    Serial.println("rev array:");
    for (int i = 0; i < 8; i++) {
        Serial.print(starline_codeM[i], HEX);
        Serial.print(" ");
    }
   uint32_t fix = (static_cast<uint32_t>(starline_codeM[0]) << 24) | 
        (static_cast<uint32_t>(starline_codeM[1]) << 16) | 
        (static_cast<uint32_t>(starline_codeM[2]) << 8) | 
        (static_cast<uint32_t>(starline_codeM[3]));   
    ///fix ok    
    Serial.print("fix:"); Serial.println(fix,HEX);   
    Serial.println();
    Serial.println("inv array:");
    for (int i = 0; i < 8; i++) {
        Serial.print(inverted_star[i], HEX);
        Serial.print(" ");
    }
  hop = (static_cast<uint32_t>(starline_codeM[4]) << 24) | 
        (static_cast<uint32_t>(starline_codeM[5]) << 16) | 
        (static_cast<uint32_t>(starline_codeM[6]) << 8) | 
        (static_cast<uint32_t>(starline_codeM[7])); 
    Serial.print("BTN: ");
    Serial.println(btnm,HEX);
    Serial.print("HOP: ");
    Serial.println(hop,HEX);        
	  uint64_t decryptedData = subghz_protocol_keeloq_common_decrypt(hop, manufacturerKey);
    btnm = (decryptedData >> 24) & 0xFF;
    decryptedData = (decryptedData & 0xFFFF0000) | cntm;
     //decryptedData = (decryptedData & 0x00FFFFFF) | (0x46000000);
    decryptedData = (decryptedData & 0x00FFFFFF) | (buttonclose << 24);
    uint8_t fixm = (decryptedData >> 16) & 0xFF;
    if (btnm == inverted_star[7] && fixm == inverted_star[4]) {Serial.print("DECRYPT DATA: ");Serial.print(decryptedData,HEX);Serial.print(" DECRYPT OKKK: ");}
    hop_to_sender=subghz_protocol_keeloq_common_encrypt(decryptedData,manufacturerKey);      
    Serial.print("hopsender:");
    Serial.println(hop_to_sender,HEX);
        inverted_star[0] = (hop_to_sender >> 0) & 0xFF;   // Младший байт (0xF2)
        inverted_star[1] = (hop_to_sender >> 8) & 0xFF;   // Следующий байт (0x3D)
        inverted_star[2] = (hop_to_sender >> 16) & 0xFF;  // Следующий байт (0x4E)
        inverted_star[3] = (hop_to_sender >> 24) & 0xFF;  // Старший байт (0x49)
        inverted_star[7]=buttonclose; 
   byte starline_codeM2[9];
   reverseBitsInBytes(inverted_star,starline_codeM2, 8);
   byte* inverted_star2 = invertArray(starline_codeM2, 8);  
   Serial.println("data to send:");
     for (int i = 0; i < 8; i++) {
         Serial.print(inverted_star2[i], HEX);
         Serial.print(" ");
     }   
   // if(!transmitterGansta.isBusy()){transmitterGansta.sendPacketStarline(inverted_star2,8,30);} 
    switch (tableName[0]) {
        case 'k':  // Если tableName начинается с 'k' (например, keeCodeTable)
            Serial.println("Протокол: KeeCodeTable");
            // Логика обработки для keeCodeTable
            sending=String("sending kee:")+bytesToString(inverted_star2,  sizeof(inverted_star2))+String(manufacturer);
            if(!transmitterGansta.isBusy()){transmitterGansta.sendPacketKeeloq(inverted_star2,8,30);} 
            break;

        case 's':  // Если tableName начинается с 's' (например, staCodeTable)
            Serial.println("Протокол: StaCodeTable");
            // Логика обработки для staCodeTable
            sending=String("sending sta:")+bytesToString(inverted_star2,  sizeof(inverted_star2))+String(manufacturer);
             if(!transmitterGansta.isBusy()){transmitterGansta.sendPacketStarline(inverted_star2,8,30);} 
            break;

        default:  // Для других значений tableName
            Serial.println("Протокол: Неизвестный");
            // Логика по умолчанию
            for (size_t i = 0; i < dataLen; i++) {
                dataToSend[i] = 0x00;  // Обнуление данных
            }
            break;
    }
   
    memcpy(code_fara,dataToSend, sizeof(dataToSend));     
    // Управление состоянием TX (пример) 
    // Вывод информации в консоль для отладки
    Serial.println("Данные отправлены:");
    // Открываем файл для чтения
    File file = SPIFFS.open(filename, FILE_READ);
    if (!file) {
        Serial.println("Не удалось открыть файл для чтения!");
        return;
    }
    String fileContent = "";
    while (file.available()) {
        fileContent += file.readString();
    }
    file.close();
    // Разбиваем файл на строки
    int lineCount = 0;
    String newFileContent = "";
    bool lineFound = false;
    // Проходим по строкам и заменяем нужную
    while (fileContent.length() > 0) {
        int newlinePos = fileContent.indexOf('\n');
        if (newlinePos == -1) {
            newlinePos = fileContent.length();
        }
        String currentLine = fileContent.substring(0, newlinePos);
        fileContent = fileContent.substring(newlinePos + 1);
        lineCount++;
        // Если это нужная строка, заменяем её
        if (lineCount == lineNumber+1) {
            // Формируем строку в нужном формате
            String newLine = "CODE: ";
            for (size_t i = 0; i < sizeof(dataToSend); i++) {
                newLine += String(dataToSend[i], HEX);
                if (i < sizeof(dataToSend) - 1) newLine += " ";
            }
            newLine += " Manafacture: " + manufacturer;

            newFileContent += newLine + "\n";
            lineFound = true;
        } else {
            // Иначе просто добавляем текущую строку
            newFileContent += currentLine + "\n";
        }
    }
    // Если нужная строка найдена, перезаписываем файл
    if (lineFound) {
        file = SPIFFS.open(filename, FILE_WRITE);
        if (!file) {
            Serial.println("Не удалось открыть файл для записи!");
            return;
        }

        file.print(newFileContent);
        file.close();

        Serial.println("Строка успешно обновлена в SPIFFS.");
    } else {
        Serial.println("Строка с указанным номером не найдена.");
    }



    // Сброс состояния
 //   starline_state = 0;
      disponto2 = true;

}

void initializeTransmitter() {
    transmitterGansta.begin();  // Вызываем метод инициализации
}
bool itxbusy(){
  return transmitterGansta.isBusy();
}


void handleSpecialCodeNmf(const String &manufacturer, const String &code, const String &tableName,uint16_t lineNumber) {
    // Поиск ключа для указанного производителя
    uint64_t manufacturerKey = 0;
    bool keyFound = false;
    String filename="/sta0.txt";
    if(tableName=="keeCodeTable"){filename="/kee0.txt";}
    for (size_t i = 0; i < sizeof(ManafactureNS) / sizeof(ManafactureNS[0]); i++) {
        if (manufacturer.equalsIgnoreCase(ManafactureNS[i].name)) {
            manufacturerKey = ManafactureNS[i].key;           
            keyFound = true;
            break;
        }
    }
    if (!keyFound) {
        Serial.println("Ошибка: Производитель не найден!");
      //  return;
    }
    // Преобразование строки кода в массив байтов
    byte dataToSend[9] = {0};  // Массив для отправки
    size_t dataLen = 0;  // Количество байтов
    // Разделяем строку по запятой и обрабатываем каждый байт
    int startIndex = 0;
    while (startIndex < code.length()) {
        // Ищем позицию запятой
        int commaIndex = code.indexOf(',', startIndex);
        if (commaIndex == -1) {
            commaIndex = code.length();
        }
        // Извлекаем байт из строки и преобразуем его в число
        String byteStr = code.substring(startIndex, commaIndex);
        dataToSend[dataLen] = strtoul(byteStr.c_str(), nullptr, 16);
        dataLen++;
        // Обновляем индекс для следующего байта
        startIndex = commaIndex + 1;
        // Прерываем, если превышен размер массива
        if (dataLen >= sizeof(dataToSend)) {
            break;
        }
    }
    Serial.print("Таблица: ");
    Serial.println(tableName);
    Serial.print("Код: ");
    Serial.println(code);
    Serial.print("Производитель: ");
    Serial.println(manufacturer);    
//    Serial.print("Производитель key: ");
//    Serial.println(manufacturerKey,HEX);
    cntm++;
    Serial.print("CNT: ");
    Serial.println(cntm,HEX);  
    ////////////ok 
  if(cntm>255){cntm=0;}
 
   // if(!transmitterGansta.isBusy()){transmitterGansta.sendPacketStarline(inverted_star2,8,30);} 
    switch (tableName[0]) {
        case 'k':  // Если tableName начинается с 'k' (например, keeCodeTable)
            Serial.println("Протокол: KeeCodeTable");
            // Логика обработки для keeCodeTable
            sending=String("sending kee:")+String(code);
            if(!transmitterGansta.isBusy()){transmitterGansta.sendPacketKeeloq(dataToSend,8,30);} 
            break;

        case 's':  // Если tableName начинается с 's' (например, staCodeTable)
            Serial.println("Протокол: StaCodeTable");
            // Логика обработки для staCodeTable
            sending=String("sending sta:")+String(code);
             if(!transmitterGansta.isBusy()){transmitterGansta.sendPacketStarline(dataToSend,8,30);} 
            break;

        default:  // Для других значений tableName
            Serial.println("Протокол: Неизвестный");
            // Логика по умолчанию
            for (size_t i = 0; i < dataLen; i++) {
                dataToSend[i] = 0x00;  // Обнуление данных
            }
            break;
    }
   
    
    Serial.println("Данные отправлены:");
 

    // Сброс состояния
 //   starline_state = 0;
    disponto2 = true; //запуск таска вывода на дисплей
}
String bytesToString(const byte* byteArray, size_t length) {
    String result = ""; // Инициализация пустой строки

    for (size_t i = 0; i < length; ++i) {
        if (byteArray[i] < 0x10) {
            result += "0"; // Добавляем ведущий ноль для байтов меньше 0x10
        }
        result += String(byteArray[i], HEX); // Преобразуем байт в HEX
    }

    result.toUpperCase(); // Приводим к верхнему регистру (если нужно)
    return result;
}