#ifndef GANSTA_TRANSMIT_ESP32_H
#define GANSTA_TRANSMIT_ESP32_H
#include <Arduino.h>
// Максимальное количество действий в очереди
#define MAX_ACTIONS 2000
class GanstaTransmitEsp32 {
public:
    // Конструктор класса с одним параметром - пин
    GanstaTransmitEsp32(int inputPin);
    // Инициализация библиотеки
    void begin();
    // Отправка пакета данных с указанием количества повторов
    void sendPacketStarline(byte* data, int length, int repeats);
    void sendPacketKeeloq(byte* data, int length, int repeats);
    void sendPacketNice(long Code, int repeats);
     void sendPacketCame(long Code, int repeats);
    // Проверка занятости передатчика
    bool isBusy();

private:
    // Пин, к которому подключен выход
    int _inputPin;
    
    // Количество повторов
    int _repeatCount;

    // Таймер
    hw_timer_t* _timer;

    // Структура для описания действия
    struct EncoderAction {
        bool state;
        uint32_t duration;
    };

    // Массив действий
    EncoderAction _encoder[MAX_ACTIONS];

    // Индексы для текущего и общего числа действий
    int _currentAction;
    int _totalActions;

    // Статус занятости
    bool _busy;

    // Настройка таймера
    void setupTimer();

    // Добавление действия в очередь
    void addEncoderAction(bool state, uint32_t duration);

    // Основная логика таймера
    void IRAM_ATTR onTimer();

    // Статическая версия обработчика таймера
    static void IRAM_ATTR onTimerStatic();
};

#endif // GANSTA_TRANSMIT_ESP32_H
