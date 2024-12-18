#include "GanstaTransmitEsp32.h"

// Статическая переменная для хранения указателя на объект
static GanstaTransmitEsp32* _instance = nullptr;
// Конструктор класса
GanstaTransmitEsp32::GanstaTransmitEsp32(int inputPin)
    : _inputPin(inputPin), _repeatCount(0), _busy(false), _currentAction(0), _totalActions(0) {
    _timer = NULL;
}
// Инициализация библиотеки
void GanstaTransmitEsp32::begin() {
    pinMode(_inputPin, OUTPUT);
    digitalWrite(_inputPin, LOW);
    setupTimer();
}
// Настройка таймера
void GanstaTransmitEsp32::setupTimer() {
    _instance = this;  // Сохраняем указатель на текущий объект
    _timer = timerBegin(3, 80, true);  // Таймер 1, делитель 80 (1 мкс)
    timerAttachInterrupt(_timer, &onTimerStatic, true);  // Указываем функцию обработчик
    timerAlarmWrite(_timer, 1000, true);  // 1 мс
}



// Статическая функция обработчик прерывания
void IRAM_ATTR GanstaTransmitEsp32::onTimerStatic() {
    if (_instance != nullptr) {
        _instance->onTimer();  // Вызов нестатического метода onTimer
    }
}
// Основная логика таймера
void IRAM_ATTR GanstaTransmitEsp32::onTimer() {
    if (_currentAction < _totalActions) {
        _busy = true;
        digitalWrite(_inputPin, _encoder[_currentAction].state);
        timerAlarmWrite(_timer, _encoder[_currentAction].duration, true);
        _currentAction++;
    } else {
        if (_repeatCount > 0) {
            _currentAction = 0;
            _repeatCount--;
        } else {
            timerAlarmDisable(_timer);
            _busy = false;
            digitalWrite(_inputPin, LOW);
        }
    }
}

// Добавление действия в очередь
void GanstaTransmitEsp32::addEncoderAction(bool state, uint32_t duration) {
    if (_totalActions < MAX_ACTIONS) {
        _encoder[_totalActions].state = state;
        _encoder[_totalActions].duration = duration;
        _totalActions++;
    }
}
// Отправка пакета данных с указанием количества повторов
void GanstaTransmitEsp32::sendPacketStarline(byte* data, int length, int repeats) {
    if (_busy) {
        // Если передача идет, пропускаем выполнение
        return;
    }
    _repeatCount = repeats;
    _totalActions = 0;
    _currentAction = 0;
    // Заполняем массив с действиями для преамбулы
    for (int i = 6; i > 0; i--) {  // Преамбула
        addEncoderAction(true, 1000);
        addEncoderAction(false, 1000);
    }
    // Преобразование данных в последовательность действий
    for (int i = 0; i < length; i++) {
        byte currentByte = data[i];
        for (int j = 7; j >= 0; j--) {  // От 7 до 0 битов
            bool bitState = currentByte & (1 << j);
            if (!bitState) {
                addEncoderAction(true, 250);
                addEncoderAction(false, 250);
            } else {
                addEncoderAction(true, 500);
                addEncoderAction(false, 500);
            }
        }
    }
    // Добавляем в конце посылки дополнительные импульсы HIGH и LOW
    addEncoderAction(true, 1000);  // Пин в HIGH, задержка 1000 мкс
    addEncoderAction(false, 1000); // Пин в LOW, задержка 1000 мкс

    // Запускаем таймер с первым действием
    if (_totalActions > 0) {
        _busy = true;  // Устанавливаем флаг busy
        timerAlarmWrite(_timer, _encoder[0].duration, true);
        timerAlarmEnable(_timer);  // Включаем таймер
    }
}
// Проверка занятости передатчика
bool GanstaTransmitEsp32::isBusy() {
    return _busy;
}
// Отправка пакета данных с указанием количества повторов
void GanstaTransmitEsp32::sendPacketKeeloq(byte* data, int length, int repeats) {
    if (_busy) {
        // Если передача идет, пропускаем выполнение
        return;
    }
    _repeatCount = repeats;
    _totalActions = 0;
    _currentAction = 0;
    // Заполняем массив с действиями для преамбулы
    for (int i = 11; i > 0; i--) {  // Преамбула
        addEncoderAction(true, 400);
        addEncoderAction(false, 400);
    }
      addEncoderAction(true, 400);
      addEncoderAction(false, 400*10);
    // Преобразование данных в последовательность действий
    for (int i = 0; i < length; i++) {
        byte currentByte = data[i];
        for (int j = 7; j >= 0; j--) {  // От 7 до 0 битов
            bool bitState = currentByte & (1 << j);
            if (bitState) {
                addEncoderAction(true, 400);
                addEncoderAction(false, 2*400);
            } else {
                addEncoderAction(true, 2*400);
                addEncoderAction(false, 400);
            }
        }
    }
    // Добавляем в конце посылки дополнительные импульсы HIGH и LOW
    addEncoderAction(true, 400);  // Пин в HIGH, задержка 1000 мкс
    addEncoderAction(false, 2*400); // Пин в LOW, задержка 1000 мкс

    addEncoderAction(true, 400);  // Пин в HIGH, задержка 1000 мкс
    addEncoderAction(false, 40*400); // Пин в LOW, задержка 1000 мкс

    // Запускаем таймер с первым действием
    if (_totalActions > 0) {
        _busy = true;  // Устанавливаем флаг busy
        timerAlarmWrite(_timer, _encoder[0].duration, true);
        timerAlarmEnable(_timer);  // Включаем таймер
    }
}
// Отправка пакета данных с указанием количества повторов
void GanstaTransmitEsp32::sendPacketNice(long Code,  int repeats) {
    if (_busy) {
        // Если передача идет, пропускаем выполнение
        return;
    }
    _repeatCount = repeats;
    _totalActions = 0;
    _currentAction = 0;
    ///стартовый импульс     
        addEncoderAction(true, 700);
        addEncoderAction(false, 0);    
    // Преобразование данных в последовательность действий
   for (int i = 24; i > 0; i--) {
      byte b = bitRead(Code, i - 1); // побитово перебираем и посылаем код
      if (b) {
         addEncoderAction(false, 1400);    
          addEncoderAction(true, 700);  
      }
      else {
     addEncoderAction(false, 700);    
      addEncoderAction(true, 1400);  
      }
   }

     addEncoderAction(false, 1850); 
    
    // Запускаем таймер с первым действием
    if (_totalActions > 0) {
        _busy = true;  // Устанавливаем флаг busy
        timerAlarmWrite(_timer, _encoder[0].duration, true);
        timerAlarmEnable(_timer);  // Включаем таймер
    }
}

void GanstaTransmitEsp32::sendPacketCame(long Code,  int repeats) {
    if (_busy) {
        // Если передача идет, пропускаем выполнение
        return;
    }
    _repeatCount = repeats;
    _totalActions = 0;
    _currentAction = 0;
    ///стартовый импульс     
        addEncoderAction(true, 320);
        addEncoderAction(false, 0);    
    // Преобразование данных в последовательность действий
   for (int i = 12; i > 0; i--) {
      byte b = bitRead(Code, i - 1); // побитово перебираем и посылаем код
      if (b) {
         addEncoderAction(false, 640);    
          addEncoderAction(true, 320);  
      }
      else {
     addEncoderAction(false, 320);    
      addEncoderAction(true, 640);  
      }
   }
     addEncoderAction(false, 11520); 
    
    // Запускаем таймер с первым действием
    if (_totalActions > 0) {
        _busy = true;  // Устанавливаем флаг busy
        timerAlarmWrite(_timer, _encoder[0].duration, true);
        timerAlarmEnable(_timer);  // Включаем таймер
    }
}