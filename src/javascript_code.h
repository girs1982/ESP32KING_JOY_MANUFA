// javascript_code.h
#ifndef JAVASCRIPT_CODE_H
#define JAVASCRIPT_CODE_H

const char* javaScriptCode =  R"(
<SCRIPT>
let isCodesFetched = false; // Флаг для отслеживания, были ли коды загружены
xmlHttp = createXmlHttpObject();
function createXmlHttpObject(){
  if (window.XMLHttpRequest) {
    xmlHttp = new XMLHttpRequest();
  } else {
    xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
  }
  return xmlHttp;
}

function process() {
//openTab(event, 'Tab1');
  if (xmlHttp.readyState == 0 || xmlHttp.readyState == 4) {
    xmlHttp.onreadystatechange = function() {
      if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
        xmlDoc = xmlHttp.responseXML;
        xmlmsg = xmlDoc.getElementsByTagName('millistime')[0].firstChild.nodeValue;
        var pri = xmlDoc.getElementsByTagName('Priem')[0].firstChild.nodeValue;
        if (pri > 332) {
          var audio = new Audio('/gun0.mp3'); 
          audio.play(); 
          pri = 0;
        }
        document.getElementById('runtime').innerHTML = xmlmsg;

        xmlmsg = xmlDoc.getElementsByTagName('Sila')[0].firstChild.nodeValue;
        document.getElementById('Sila').innerHTML = xmlmsg;

        xmlmsg = xmlDoc.getElementsByTagName('Batareya')[0].firstChild.nodeValue;
        document.getElementById('Batareya').innerHTML = xmlmsg;

        xmlmsg = xmlDoc.getElementsByTagName('sending')[0].firstChild.nodeValue;
        document.getElementById('sending').innerHTML = xmlmsg;

        xmlmsg = xmlDoc.getElementsByTagName('Paket')[0].firstChild.nodeValue;
        document.getElementById('Paket').innerHTML = xmlmsg;

        xmlmsg = xmlDoc.getElementsByTagName('Paket1')[0].firstChild.nodeValue;
        document.getElementById('Paket1').innerHTML = xmlmsg;

        document.getElementById('button').onclick = function() { 
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/socket2On', true);
          xmlhttp.send();
        };

        document.getElementById('buz').onclick = function() {
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/socket2Off', true);
          xmlhttp.send();
        };

        document.getElementById('kee1').onclick = function() {
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/socket1On', true);
          xmlhttp.send();
        };

        document.getElementById('kee2').onclick = function() {
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/socket1Off', true);
          xmlhttp.send();
        };

        document.getElementById('brut_g').onclick = function() {
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/brut_g', true);
          xmlhttp.send();
        };

        document.getElementById('prion').onclick = function() {
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/socket3On', true);
          xmlhttp.send();
        };

        document.getElementById('prioff').onclick = function() {
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/socket3Off', true);
          xmlhttp.send();
        };
        document.getElementById('setbuttonvaues').onclick = function() {
          xmlhttp = new XMLHttpRequest();
          let buttonopen = document.getElementById('buttonval_open').value;
          let buttonclose = document.getElementById('buttonval_close').value;
          xmlhttp.open('GET', '/setbuttonvaues?open='+buttonopen+'&close='+buttonclose, true);
          xmlhttp.send();
        };

        document.getElementById('mana').onclick = function() {
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/mana', true);
          xmlhttp.send();
        };

        document.getElementById('manaopen').onclick = function() {
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/manaopen', true);
          xmlhttp.send();
        };

        document.getElementById('sendshlak').onclick = function() {
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/ShlakSend', true);
          xmlhttp.send();
        };

        document.getElementById('shlakoff').onclick = function() {
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/ShlakSendOff', true);
          xmlhttp.send();
        };

        document.getElementById('but_glushilka').onclick = function() {
          let vglush = document.getElementById('glushilk').value;
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/GETGLUSH?name=' + vglush, true);
          xmlhttp.send();
        };

        document.getElementById('but_Freq').onclick = function() {
          let freq = document.getElementById('freq_in').value;
          xmlhttp = new XMLHttpRequest();
          xmlhttp.open('GET', '/FREQ?name=' + freq, true);
          xmlhttp.send();
        };
      }
    }
    xmlHttp.open('PUT', 'xml', true);
    xmlHttp.send(null);
  }
  setTimeout('process()', 100);
}

      // Функция для переключения вкладок
   function openTab(event, tabName) {
            var i, tabcontent, tablinks;
            tabcontent = document.getElementsByClassName('tabcontent');
            tablinks = document.getElementsByClassName('tab');

            // Скрываем все вкладки
            for (i = 0; i < tabcontent.length; i++) {
                tabcontent[i].style.display = 'none';
            }

            // Убираем фон с всех вкладок
            for (i = 0; i < tablinks.length; i++) {
                tablinks[i].style.backgroundColor = '#d5e0d3';
            }

            // Показываем выбранную вкладку
            var selectedTab = document.getElementById(tabName);
            if (selectedTab) {
                selectedTab.style.display = 'block';
            }

            // Устанавливаем фон для активной вкладки
            if (event.currentTarget) {
                event.currentTarget.style.backgroundColor = '#c3d1c0';
            }

            // Загружаем коды только при переходе на вкладку 3, если коды еще не загружены
            if (tabName === 'tab3' && !isCodesFetched) {
                  fetchCodes('/getSavedCodes', 'staCodeTable', 'staCodeList');
                 fetchCodes('/getSavedCodesKee', 'keeCodeTable', 'keeCodeList');
               // isCodesFetched = true; // Устанавливаем флаг, чтобы не загружать коды снова
            }
        }

function fetchCodes(url, tableId, listId) {
    fetch(url)
        .then(response => response.json()) // Парсим JSON
        .then(data => {
            const codeList = document.getElementById(listId);
            codeList.innerHTML = ''; // Очищаем текущий список

            // Добавляем данные в таблицу
            data.forEach((code, index) => {
                const row = document.createElement('tr');
                row.dataset.index = index; // Устанавливаем индекс строки

                const cellNumber = document.createElement('td');
                cellNumber.textContent = index + 1; // Номер строки

                const cellCode = document.createElement('td');
                cellCode.textContent = code.code.map(byte => byte.toUpperCase()).join(', '); // Код в верхнем регистре

                const cellManufacturer = document.createElement('td');
                cellManufacturer.textContent = code.manufacturer || "Unknown"; // Производитель, если есть

                // Создаем колонку для кнопки
                const cellAction = document.createElement('td');
                const sendButton = document.createElement('button');
                sendButton.textContent = 'Send';
                sendButton.style.display = 'none'; // Сначала скрываем кнопку

                const specialButton = document.createElement('button');
                specialButton.textContent = 'Special Action';
                specialButton.style.display = 'none'; // Сначала скрываем кнопку
                specialButton.style.backgroundColor = 'red';
                specialButton.style.color = 'white';

                // При выделении строки показываем кнопки
                row.addEventListener('click', () => {
                    // Убираем выделение с других строк
                    document.querySelectorAll(`#${listId} .selected`).forEach(selectedRow => {
                        selectedRow.classList.remove('selected');
                    });

                    // Выделяем текущую строку
                    row.classList.add('selected');

                    // Показываем кнопку "Send"
                    sendButton.style.display = 'inline-block';

                    // Проверяем, если производитель не "Unknown", показываем красную кнопку
                    if (code.manufacturer && code.manufacturer.toLowerCase() !== "unknown") {
                        specialButton.style.display = 'inline-block';
                    } else {
                        specialButton.style.display = 'none';
                    }

                    // При нажатии на кнопку отправляем GET запрос
                    sendButton.onclick = () => {
                        const selectedCode = code.code.map(byte => byte.toUpperCase()).join(', ');

                        // Получаем название таблицы, из которой был выбран код
                        const tableName = row.closest('table').id;

                        // Создаем URL для GET запроса
                        const requestUrl = `/send_codeoutTable?table=${tableName}&code=${encodeURIComponent(selectedCode)}`;

                        // Отправляем GET запрос
                        fetch(requestUrl)
                            .then(response => response.text())  // Ожидаем текстовый ответ от сервера
                            .then(data => {
                                console.log('Ответ от сервера:', data);

                                // Меняем цвет кнопки после успешной отправки
                                sendButton.style.backgroundColor = 'green';  // Успешно отправлено
                                sendButton.style.color = 'white';  // Меняем цвет текста на белый
                                sendButton.textContent = 'Sent';  // Изменяем текст на 'Sent'

                                // Добавляем alert об успешной отправке
                              //  alert(`Отправлен код: ${selectedCode} из таблицы: ${tableName}`);
                            })
                            .catch(error => {
                                console.error('Ошибка при отправке запроса:', error);
                                alert('Ошибка при отправке кода');
                            });
                    };

                    // При нажатии на специальную кнопку отправляем GET запрос для производителя
                    specialButton.onclick = () => {
                        const selectedCode = code.code.map(byte => byte.toUpperCase()).join(', ');

                        // Получаем название таблицы, из которой был выбран код
                        const tableName = row.closest('table').id;

                        // Создаем URL для GET запроса
                        const requestUrl = `/send_specialCodeoutTable?table=${tableName}&code=${encodeURIComponent(selectedCode)}&manufacturer=${encodeURIComponent(code.manufacturer)}`;

                        // Отправляем GET запрос для специального действия
                        fetch(requestUrl)
                            .then(response => response.text())  // Ожидаем текстовый ответ от сервера
                            .then(data => {
                                console.log('Ответ от сервера:', data);

                                // Меняем цвет кнопки после успешной отправки
                                specialButton.style.backgroundColor = 'green';  // Успешно отправлено
                                specialButton.style.color = 'white';  // Меняем цвет текста на белый
                                specialButton.textContent = 'Sent';  // Изменяем текст на 'Sent'

                                // Добавляем alert об успешной отправке
                              //  alert(`Отправлен код с производителя: ${code.manufacturer} из таблицы: ${tableName}`);
                            })
                            .catch(error => {
                                console.error('Ошибка при отправке запроса:', error);
                                alert('Ошибка при отправке кода');
                            });
                    };
                });

                // Добавляем кнопку в ячейку действия
                cellAction.appendChild(sendButton);
                cellAction.appendChild(specialButton);  // Добавляем специальную кнопку

                // Добавляем ячейки в строку
                row.appendChild(cellNumber);
                row.appendChild(cellCode);
                row.appendChild(cellManufacturer);  // Добавляем колонку для производителя
                row.appendChild(cellAction);

                // Добавляем строку в таблицу
                codeList.appendChild(row);
            });
        })
        .catch(error => {
            console.error(`Ошибка при получении кодов с ${url}:`, error);
            alert('Ошибка при загрузке данных с сервера');
        });
}

function PutXML() {}
function writetofile() {}
</SCRIPT>
)";
#endif // JAVASCRIPT_H