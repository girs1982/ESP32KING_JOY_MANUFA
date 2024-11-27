#include "star_kee.h"
#include <stdint.h>
Preferences prefs;
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
uint32_t hop = 0;
uint32_t hop_to_sender = 0;
uint64_t CrazyMonkey=0;
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
byte starline_codePAK2[9]={0};
byte starline_codePAK1[9]={0};
byte webpak2[9];
byte webpak1[9];
byte buttonopen=0x42;
byte buttonclose=0x46;

int starline_state = 0;
int zoro=1;
/////////////////////////////////////////////////VVODNIE
uint32_t decryptedData=0;
uint8_t btnm = 0;
uint8_t fixm = 0;
uint16_t cntm = 0;
volatile unsigned long prevtime;
volatile unsigned int lolen, hilen, state;
volatile int cameCounter = 0;    // count of bits stored
volatile static long cameCode = 0;       // code itself
volatile static long lastCode = 0;


volatile byte level = 255;
volatile unsigned long last;
volatile unsigned long len;
byte p_level;
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
};



// Функция для шифрования и дешифрования с использованием XOR
uint64_t simpleEncryptDecrypt(uint64_t data, uint64_t key) {
    return data ^ key; // Применяем XOR для шифрования и дешифрования
}

// Массив с зашифрованными ключами
DeviceData Manafacture[] = {
    {0x0000000000000000, "Tomohawk"},
    {0x0000000000000000, "Tomohawk9010"},
    {0x0000000000000000, "Tomohawk9020"},
    {0x0000000000000000, "StarlineB6"},
    {0x0000000000000000, "StarlineA2"},
    {0x0000000000000000, "StarlineA8"},
    {0x0000000000000000, "Lacosta"},
    {0x0000000000000000, "Aligofrend"},
    {0x0000000000000000, "Centurion"},
    {0x0000000000000000, "Obama"},
    {0x0000000000000000, "Sherif"},
    {0x0000000000000000, "Obama2"},
    {0x0000000000000000, "Starline25"},
    {0x0000000000000000, "Starline89"},
    {0x0000000000000000, "Tomohawk68"},
    {0x0000000000000000, "Starline12"},
    {0x0000000000000000, "Starline74"},
    {0x0000000000000000, "APS"},
    {0x0000000000000000, "DoorHanchik"},
    {0x0000000000000000, "DoorHanchik1"},
    {0x0000000000000000, "Door1"},
    {0x0000000000000000, "Door2"}
};

DeviceData ManafactureNS[] = {
    {0x0000000000000000, "Tomohawk"},
    {0x0000000000000000, "Tomohawk9010"},
    {0x0000000000000000, "Tomohawk9020"},
    {0x0000000000000000, "StarlineB6"},
    {0x0000000000000000, "StarlineA2"},
    {0x0000000000000000, "StarlineA8"},
    {0x0000000000000000, "Lacosta"},
    {0x0000000000000000, "Aligofrend"},
    {0x0000000000000000, "Centurion"},
    {0x0000000000000000, "Obama"},
    {0x0000000000000000, "Sherif"},
    {0x0000000000000000, "Obama2"},
    {0x0000000000000000, "Starline25"},
    {0x0000000000000000, "Starline89"},
    {0x0000000000000000, "Tomohawk68"},
    {0x0000000000000000, "Starline12"},
    {0x0000000000000000, "Starline74"},
    {0x0000000000000000, "APS"},
    {0x0000000000000000, "DoorHanchik"},
    {0x0000000000000000, "DoorHanchik1"},
    {0x0000000000000000, "Door1"},
    {0x0000000000000000, "Door2"}
};

const int deviceCount = sizeof(Manafacture) / sizeof(Manafacture[0]);
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
void saveUniqueCode(const char* packageName, uint8_t* newCode, size_t codeLength) {
    prefs.begin(packageName, false);  // Начинаем работу с Preferences, открываем указанное пространство

    // Получаем текущий индекс записи для данного пакета, по умолчанию 0
    int currentIndex = prefs.getInt("index", 0);

    // Проверяем, есть ли уже этот код в памяти
    for (int i = 0; i < 30; ++i) {
        String key = String(i);  // Индексный ключ, уникальный для каждого кода в пакете
        uint8_t existingCode[codeLength];
        if (prefs.getBytes(key.c_str(), existingCode, codeLength) == codeLength) {
            if (memcmp(newCode, existingCode, codeLength) == 0) {
                Serial.println("Код уже сохранен в текущем пакете.");
                prefs.end();
                return;
            }
        }
    }

    // Проверка на переполнение
    if (currentIndex >= 30) {
        Serial.println("Достигнут предел записей. Код не сохранён.");
        prefs.end();
        return;
    }

    // Генерируем ключ для текущего индекса
    String key = String(currentIndex);

    // Очистка старых данных (на случай записи кода меньшей длины)
    uint8_t emptySlot[codeLength] = {0};
    prefs.putBytes(key.c_str(), emptySlot, codeLength);

    // Сохраняем новый код в текущий слот
    size_t bytesWritten = prefs.putBytes(key.c_str(), newCode, codeLength);
    if (bytesWritten != codeLength) {
        Serial.println("Ошибка записи кода!");
        prefs.end();
        return;
    }

    Serial.print("Код сохранен в ");
    Serial.print(packageName);
    Serial.print(" под ключом ");
    Serial.println(key);

    // Обновляем индекс для следующей записи
    currentIndex = (currentIndex + 1) % 30;  // Кольцевой переход после 30
    prefs.putInt("index", currentIndex);

    prefs.end();
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
    else if(starline_state==1){// получаем биты
      if(difTime2 > 350 && difTime2 < 650 && lastRxValue == 1){// получили 1
        if(decodeMethod==0){
          starline_code[lround(starlineZCounter/8)] = (starline_code[lround(starlineZCounter/8)]>>1)|B10000000;
        }
        else{
          starline_code[lround(starlineZCounter/8)] = (starline_code[lround(starlineZCounter/8)]<<1)|B00000000;
        }
        bValidPacket = true;
      }
      else if(difTime2 > 150 && difTime2 < 350 && lastRxValue == 1){
        if(decodeMethod==0){
          starline_code[lround(starlineZCounter/8)] = (starline_code[lround(starlineZCounter/8)]>>1)|B00000000;
        }
        else{
          starline_code[lround(starlineZCounter/8)] = (starline_code[lround(starlineZCounter/8)]<<1)|B00000001;
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
byte* inverted_star = invertArray(starline_code, 7);
    // Выводим инвертированный массив
    Serial.println("Inverted array:");
    for (int i = 0; i < 7; i++) {
        Serial.print(inverted_star[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
// Перенос значений starline_code[0], [1], [2] и [3] в переменную hop
hop = (static_cast<uint32_t>(inverted_star[3]) << 24) | 
      (static_cast<uint32_t>(inverted_star[4]) << 16) | 
      (static_cast<uint32_t>(inverted_star[5]) << 8) | 
      (static_cast<uint32_t>(inverted_star[6]));

for (int i = 0; i < sizeof(Manafacture) / sizeof(Manafacture[0]); i++) {
    // Дешифруем данные с помощью текущего ключа
    uint64_t encryptionKey = 0x0000000000; // Задаем ключ шифрования
    uint64_t decryptedKey =simpleEncryptDecrypt(Manafacture[i].key, encryptionKey);//  Manafacture[i].key;//Manafacture[i].key; 
    
    uint64_t decryptedData = subghz_protocol_keeloq_common_decrypt(hop, decryptedKey);


    // Извлекаем старшие 8 бит (btnm)
    uint8_t btnm = (decryptedData >> 24) & 0xFF;
    // Извлекаем следующие 8 бит (fixm)
    uint8_t fixm = (decryptedData >> 16) & 0xFF;
    // Извлекаем младшие 16 бит (cntm)
    uint16_t cntm = decryptedData & 0xFFFF;


    // Проверяем, совпадают ли btnm и fixm с соответствующими значениями из starline_code
    if (btnm == starline_code[7] && fixm == starline_code[4]) {
        Serial.println("Manafactured!!!");
        Serial.print("Count (cntm): 0x");
        Serial.println(cntm, HEX);
        foundProtocol=String(Manafacture[i].name);
        // Получаем hop_to_sender
        hop_to_sender = subghz_protocol_keeloq_common_encrypt(decryptedData, Manafacture[i].key);
        Serial.print("hop_to_sender: 0x");
        Serial.println(hop_to_sender, HEX);

        // Сохраняем расшифрованные данные в Preferences
        prefs.begin("decryptedData");
        prefs.putUInt("decryptedData", decryptedData);
        prefs.end();

        // Сохраняем starline_codeM в Preferences
        prefs.begin("sta0M");
        prefs.putBytes("sta0M", starline_codeM, 9);
        prefs.end();
        
        CrazyMonkey=decryptedKey;

   prefs.begin("staMonkey", false); // Открываем пространство имен
    // Сохранение значения
    prefs.putULong64("monky",  CrazyMonkey);    // Получение значения  
    prefs.end(); // Закрываем пространство имен

        manafak = true; // Устанавливаем флаг успешного выполнения
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
prefs.begin("sta0");
prefs.putBytes("sta0",starline_code,9);
prefs.end();
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
saveUniqueCode("sta0",starline_code, sizeof(starline_code));
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
Serial.print("btn");Serial.println(keeloq_codeM[0]>>4,HEX);
Serial.print("SN:");Serial.println(keeloq_codeM[3],HEX);
for (int i = 0; i < sizeof(Manafacture) / sizeof(Manafacture[0]); i++) {
    // Дешифруем данные с помощью текущего ключа
    uint64_t encryptionKey = 0x000000000000; // Задаем ключ шифрования
    uint64_t decryptedKey = simpleEncryptDecrypt(Manafacture[i].key, encryptionKey);//  Manafacture[i].key;//Manafacture[i].key; 
    uint64_t decryptedData = subghz_protocol_keeloq_common_decrypt(hop, decryptedKey);

Serial.print("DECRYPTED:");Serial.println(decryptedData,HEX);
    // Извлекаем старшие 8 бит (btnm)
    uint8_t btnm = (decryptedData >> 28) & 0xFF;
    // Извлекаем следующие 8 бит (fixm)
    uint8_t fixm = (decryptedData >> 16) & 0xFF;
    // Извлекаем младшие 16 бит (cntm)
    uint16_t cntm = decryptedData & 0xFFFF;

    // Проверяем, совпадают ли btnm и fixm с соответствующими значениями из keloq
    if (btnm == keeloq_codeM[0]>>4&& fixm == keeloq_codeM[3]) {
        Serial.println("Manafactured!!!");
        Serial.print("Count (cntm): 0x");
        Serial.println(cntm, HEX);
        foundProtocol=String(Manafacture[i].name);
        // Получаем hop_to_sender
        hop_to_sender = subghz_protocol_keeloq_common_encrypt(decryptedData, Manafacture[i].key);
        Serial.print("hop_to_sender: 0x");
        Serial.println(hop_to_sender, HEX);

        // Сохраняем расшифрованные данные в Preferences
        prefs.begin("decryptedData");
        prefs.putUInt("decryptedData", decryptedData);
        prefs.end();

        // Сохраняем starline_codeM в Preferences
        prefs.begin("kee0M");
        prefs.putBytes("kee0M", keeloq_codeM, 9);
        prefs.end();
        
        CrazyMonkey=decryptedKey;

   prefs.begin("keeMonkey", false); // Открываем пространство имен
    // Сохранение значения
    prefs.putULong64("monky",  CrazyMonkey);    // Получение значения  
    prefs.end(); // Закрываем пространство имен

        manafak = true; // Устанавливаем флаг успешного выполнения
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
Serial.println(codan);
 if(k<2){
// prefs.begin("kee0");
// prefs.putBytes("kee0",inverted_kee,9);
// prefs.end();
//k++;
 }
// Определите длину явно
size_t keelog_code_length = sizeof(keelog_code);  // Вернёт 9, так как массив известен на этапе компиляции
saveUniqueCode("kee0", inverted_kee, keelog_code_length);

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

void starline_send(byte* starline_code){
  
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
prefs.begin("kee0", false);
prefs.getBytes("kee0",keelog_codePAK1,9);
prefs.end();
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
prefs.begin("kee0", false);
//EEPROM.begin(3512);
// keelog_codePAK1[0]=EEPROM.read(0);
//keelog_codePAK1[0]=prefs.getUChar("kee0", 0);
prefs.getBytes("kee0",keelog_codePAK2,9);
prefs.end();
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
prefs.begin("decryptedData");
decryptedData=prefs.getUInt("decryptedData", 0); 
prefs.end();
}
if(CrazyMonkey==0){
   prefs.begin("staMonkey", false); // Открываем пространство имен  
    // Получение значения
    CrazyMonkey = prefs.getULong64("monky", 0); // Получаем значение, по умолчанию 0
    prefs.end(); // Закрываем пространство имен
}
 prefs.begin("sta0M", false);
 prefs.getBytes("sta0M",starline_codePAK1,9);
 prefs.end();
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
}
void posilstarlinemanaOpen(){ 
if(decryptedData==0){
prefs.begin("decryptedData");
decryptedData=prefs.getUInt("decryptedData", 0); 
prefs.end();
}

if(CrazyMonkey==0){
     prefs.begin("staMonkey", false); // Открываем пространство имен
    // Получение значения
    uint64_t savedKey = prefs.getULong64("monky", 0); // Получаем значение, по умолчанию 0
    prefs.end(); // Закрываем пространство имен
}
//hop_to_sender== (hop_to_sender & 0xFF00FFFF) | (0x42000000);
 prefs.begin("sta0M", false);
 prefs.getBytes("sta0M",starline_codePAK1,9);
 prefs.end();
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


void posilstarline1(){

 
prefs.begin("sta0", false);
prefs.getBytes("sta0",starline_codePAK1,9);
prefs.end();
   sending="code star_line:"+String(   starline_codePAK1[0],HEX);
sending=sending+String(   starline_codePAK1[1],HEX);
sending=sending+String(   starline_codePAK1[2],HEX);
sending=sending+String(   starline_codePAK1[3],HEX);
sending=sending+String(  starline_codePAK1[4],HEX);
sending=sending+String(   starline_codePAK1[5],HEX);
sending=sending+String(  starline_codePAK1[6],HEX);
sending=sending+String(   starline_codePAK1[7],HEX);
sending=sending+String(   starline_codePAK1[8],HEX);

starline_send(starline_codePAK1);
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
prefs.begin("sta0", false);
prefs.getBytes("sta0",starline_codePAK2,9);
prefs.end();
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
    prefs.begin("sta0", false);
   prefs.getBytes("shlak",potco,24);  
    prefs.end(); 
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
 

   prefs.begin("sta0", false);
   prefs.putBytes("shlak",sambo,24);  
    prefs.end();   
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

// Функция для получения сохранённых кодов
void getSavedCodes(const char* packageName, String& jsonResponse, uint8_t expectedCodeLength) {
    prefs.begin(packageName, false);  // Открываем пространство памяти с указанным именем пакета
    Serial.printf("Чтение данных из пакета: %s\n", packageName);

    jsonResponse = "[";  // Начинаем JSON-массив

    for (int i = 0; i < 30; ++i) {
        String key = String(i);  // Используем ключ на основе индекса
        uint8_t code[expectedCodeLength];  // Буфер для чтения данных
        size_t bytesRead = prefs.getBytes(key.c_str(), code, expectedCodeLength);  // Читаем данные
        
        Serial.printf("Ключ: %s, Прочитано байт: %d\n", key.c_str(), bytesRead);

        if (bytesRead > 0) {  // Если данные существуют
            if (jsonResponse.length() > 1) {
                jsonResponse += ",";  // Добавляем запятую между элементами массива
            }
            jsonResponse += "[";  // Начинаем новый массив
            for (size_t j = 0; j < bytesRead; ++j) {
                jsonResponse += "\"" + String(code[j], HEX) + "\"";  // Конвертируем байт в HEX-строку
                if (j < bytesRead - 1) {
                    jsonResponse += ",";
                }
            }
            jsonResponse += "]";  // Закрываем массив
        }
    }

    jsonResponse += "]";  // Закрываем JSON-массив
    prefs.end();  // Закрываем пространство памяти
    Serial.println("Чтение завершено");
    Serial.println(jsonResponse);  // Печатаем результат
}
