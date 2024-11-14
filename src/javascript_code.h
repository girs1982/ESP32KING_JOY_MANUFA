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
                fetchCodes();  // Вызываем функцию загрузки кодов
               // isCodesFetched = true; // Устанавливаем флаг, чтобы не загружать коды снова
            }
        }

function fetchCodes() {
    fetch('/getSavedCodes')
        .then(response => response.json())  // Парсим JSON
        .then(data => {
            const codeList = document.getElementById('codeList');
            codeList.innerHTML = '';  // Очищаем текущий список

            // Добавляем данные в таблицу
            data.forEach((code, index) => {
                const row = document.createElement('tr');
                row.dataset.index = index;  // Устанавливаем индекс строки

                const cellNumber = document.createElement('td');
                cellNumber.textContent = index + 1;  // Номер строки

                const cellCode = document.createElement('td');
                cellCode.textContent = code.map(byte => byte.toUpperCase()).join(', ');  // Код в верхнем регистре

                // Обработчик для выделения строки
                row.addEventListener('click', () => {
                    row.classList.toggle('selected');
                });

                // Добавляем ячейки в строку
                row.appendChild(cellNumber);
                row.appendChild(cellCode);
                // Добавляем строку в таблицу
                codeList.appendChild(row);
            });
        })
        .catch(error => console.error('Ошибка при получении кодов:', error));
}

function PutXML() {}
function writetofile() {}
</SCRIPT>
)";
#endif // JAVASCRIPT_H