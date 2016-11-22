/**
* Сохранение настроек в EEPROM
* 
* Copyright (C) 2016 Алексей Шихарбеев
* http://samopal.pro
*/

#ifndef WC_EEPROM_h
#define WC_EEPROM_h
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "WC_HTTP.h"
#include <EEPROM.h>
#include "SHA256.h"


struct WC_NET_CONFIG{
// Наименование в режиме точки доступа  
   char ESP_NAME[32];
   char ESP_PASS[33];
// Параметры подключения в режиме клиента
   char AP_SSID[32];
   char AP_PASS[65];
   IPAddress IP;
   IPAddress MASK;
   IPAddress GW;
// Пароль на доступ к WEB в SHA256
   char WEB_PASS[65];
   uint16_t SRC;   
};

struct WC_LED_CONFIG{
   char    GET_PASS[65];
   uint8_t VAL_W;
   uint8_t VAL_Y;
   uint8_t VAL_R;
   uint8_t VAL_G;
   uint8_t VAL_B;
   uint8_t TM_ON;
   uint8_t TM_OFF;
   uint16_t SRC;   
};

extern struct WC_NET_CONFIG NetConfig;
extern struct WC_LED_CONFIG LedConfig;
extern bool setup_debug;

void     EC_begin(void);
void     EC_read(void);
void     EC_save(void);
uint16_t EC_SRC(void);
void     EC_default(void);
void     EC_led_read(void);
void     EC_led_save(void);
uint16_t EC_led_SRC(void);
void     EC_led_default(void);





#endif
