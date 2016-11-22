/**
* Альтернативная прошивка для управления модулем SONOFF LED
* https://www.itead.cc/sonoff-led.html
* Версия 1.0
* 
При недоступности WiFi подключения старт в режиме точки дступа
Авторизованный доступ к странице настроек. Пароль к WEB по умолчанию "admin". Настройки сохраняются в EEPROM
Обновление прошивки через WEB
Установка уровня яркости по обоим каналам (теплый и холодный свет) 0-100% с шагом 1%
Сохранение настроек сети и диммера в энергонезависимую память
Управление по HTTP GET для интеграции с устройствами умного дома http://<адрес>/on?auth=<pass> и http://<адрес>/off?auth=<pass>
Пароль для GET HTTP устанавливается в настройках. По умолчанию - "12345"
Пароли сохраняются в EEPROM в зашифрованном виде SHA256
* 
* Copyright (C) 2016 Алексей Шихарбеев
* http://samopal.pro
*/
#include <arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ESP8266HTTPUpdateServer.h>

#include "WC_EEPROM.h"
#include "WC_HTTP.h"
#include "SHA256.h"

// Перегружаться, если в точке доступа
#define AP_REBOOT_TM 600

bool setup_debug = true;

uint32_t ms, ms1 = 0,ms2 = 0, ms_ok = 0;

bool led_stat = true;
bool led_stat_save = true;
bool led_go  = false;

uint8_t PIN_WHITE  = 12;
uint8_t PIN_YELLOW = 14;
uint8_t PIN_RED    =  5;
uint8_t PIN_GREEN  =  4;
uint8_t PIN_BLUE   = 15;


void setup() {
// Последовательный порт для отладки
   DebugSerial.begin(115200);
   DebugSerial.printf("ESP8266 Controller. %s\n",NetConfig.ESP_NAME);
// Инициализация EEPROM
   EC_begin();  
   EC_read();
   EC_led_read();
//   analogWriteFreq(10000);

   
   uint32_t v1 = LedConfig.VAL_W*1023/100;
   uint32_t v2 = LedConfig.VAL_Y*1023/100;
// Плавное загорание светодиодов
/*
   int n = LedConfig.TM_ON*100;
   for( int i=0; i<n; i++){
      uint32_t vv1 =  v1*i/n;
      uint32_t vv2 =  v2*i/n;
      analogWrite(PIN_WHITE,vv1);
      analogWrite(PIN_YELLOW,vv2);
      delay(10);
   }  
*/   
   analogWrite(PIN_WHITE,v1);
   analogWrite(PIN_YELLOW,v2);


// Подключаемся к WiFi  
   WiFi_begin();
   delay(2000);
// Старт внутреннего WEB-сервера  
   HTTP_begin(); 
    
     
}


int lev1=0;
int lev2=0;

void loop() {
   ms = millis();
   if( isAP && ms > AP_REBOOT_TM*1000 ){
       DebugSerial.printf("TIMEOUT. REBOOT. \n");
       ESP.reset();
   }
   HTTP_loop();

}

