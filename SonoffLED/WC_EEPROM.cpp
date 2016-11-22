/**
* Сохранение настроек в EEPROM
* 
* Copyright (C) 2016 Алексей Шихарбеев
* http://samopal.pro
*/

#include "WC_EEPROM.h"


struct WC_NET_CONFIG NetConfig;
struct WC_LED_CONFIG LedConfig;




/**
 * Инициализация EEPROM
 */
void EC_begin(void){
/*
 * Если есть еще секции, записываемые в EEPROM, то размер EEPROM.begin() должен бть общим
 */ 
   size_t sz1  = sizeof(NetConfig); //Конфигурация сети
   size_t sz2  = sizeof(LedConfig); //Конфигурация сети
   EEPROM.begin(sz1+sz2);   
   if( setup_debug )DebugSerial.printf("EEPROM init. Size = %d\n",(int)sz1);

}

/**
 * Читаем конфигурацию из EEPROM в память
 */
void EC_read(void){
   size_t sz1 = sizeof(NetConfig);
   for( int i=0; i<sz1; i++ ){
       uint8_t c = EEPROM.read(i);
       *((uint8_t*)&NetConfig + i) = c; 
    }  
    uint16_t src = EC_SRC();
    if( NetConfig.SRC == src ){
       if( setup_debug )DebugSerial.printf("EEPROM Config is correct\n");
    }
    else {
       if( setup_debug )DebugSerial.printf("EEPROM SRC is not valid: %d %d\n",src,NetConfig.SRC);
       EC_default();
       EC_save();
    }        
}

/**
 * Устанавливаем значения конфигурации по-умолчанию
 */
void EC_default(void){
   size_t sz1 = sizeof(NetConfig);
   memset( &NetConfig, '\0',sz1);
//   for( int i=0, byte *p = (byte *)&NetConfig; i<sz1; i++, p++) 
//       *p = 0;   
     
   strcpy(NetConfig.ESP_NAME,"SAMOPAL_SERVER_1");
   strcpy(NetConfig.ESP_PASS,"admin");
   strcpy(NetConfig.AP_SSID, "none");
   strcpy(NetConfig.AP_PASS, "");  
   NetConfig.IP[0]      = 192;   
   NetConfig.IP[1]      = 168;   
   NetConfig.IP[2]      = 1;     
   NetConfig.IP[3]      = 4;
   NetConfig.MASK[0]    = 255; 
   NetConfig.MASK[1]    = 255; 
   NetConfig.MASK[2]    = 255; 
   NetConfig.MASK[3]    = 0;
   NetConfig.GW[0]      = 192;   
   NetConfig.GW[1]      = 168;   
   NetConfig.GW[2]      = 1;     
   NetConfig.GW[3]      = 1;
// Пароль на доступк к WEB в зашифрованном виде
   Sha256.init();
   Sha256.add("admin");
   strcpy(NetConfig.WEB_PASS,Sha256.char_result()); 
}

/**
 * Сохраняем значение конфигурации в EEPROM
 */
void EC_save(void){
   NetConfig.SRC = EC_SRC();
   size_t sz1 = sizeof(NetConfig);
   for( int i=0; i<sz1; i++)
      EEPROM.write(i, *((uint8_t*)&NetConfig + i));
   EEPROM.commit();     
   if( setup_debug )DebugSerial.printf("Save Config to EEPROM. SCR=%d\n",NetConfig.SRC);   
}

/**
 * Рвсчет контрольной суммы
 */
uint16_t EC_SRC(void){
   uint16_t src = 0;
   size_t sz1 = sizeof(NetConfig);
   uint16_t src_save = NetConfig.SRC;
   NetConfig.SRC = 0;
   for( int i=0; i<sz1; i++)src +=*((uint8_t*)&NetConfig + i);
   if( setup_debug )DebugSerial.printf("SCR=%d\n",src); 
   NetConfig.SRC = src_save;
   return src;  
}

/**
 * Читаем состояние LED из EEPROM в память
 */
void EC_led_read(void){
   size_t offset1 = sizeof(NetConfig);
   size_t sz1 = sizeof(LedConfig);
   for( int i=0; i<sz1; i++ ){
       uint8_t c = EEPROM.read(offset1+i);
       *((uint8_t*)&LedConfig + i) = c; 
    }  
    uint16_t src = EC_led_SRC();
    if( LedConfig.SRC == src ){
       if( setup_debug )DebugSerial.printf("EEPROM LED state is correct\n");
    }
    else {
       if( setup_debug )DebugSerial.printf("EEPROM LED state SRC is not valid: %d %d\n",src,LedConfig.SRC);
       EC_led_default();
       EC_led_save();
    }        
}

/**
 * Устанавливаем значения конфигурации по-умолчанию
 */
void EC_led_default(void){
   size_t sz1 = sizeof(LedConfig);
   memset( &LedConfig, '\0',sz1);
//   for( int i=0, byte *p = (byte *)&NetConfig; i<sz1; i++, p++) 
//       *p = 0;   
     
   Sha256.init();
   Sha256.add("12345");
   strcpy(LedConfig.GET_PASS,Sha256.char_result()); 
   LedConfig.VAL_W  = 100;
   LedConfig.VAL_Y  = 100;
   LedConfig.VAL_R  = 0;
   LedConfig.VAL_G  = 0;
   LedConfig.VAL_B  = 0;
   LedConfig.TM_ON  = 0;
   LedConfig.TM_OFF = 0;
   
}

/**
 * Сохраняем значение конфигурации в EEPROM
 */
void EC_led_save(void){
   size_t offset1 = sizeof(NetConfig);
   size_t sz1 = sizeof(LedConfig);
   LedConfig.SRC = EC_led_SRC();
   for( int i=0; i<sz1; i++)
      EEPROM.write(i+offset1, *((uint8_t*)&LedConfig + i));
   EEPROM.commit();     
   if( setup_debug )DebugSerial.printf("Save Config to EEPROM. SCR=%d\n",NetConfig.SRC);   
}

/**
 * Рвсчет контрольной суммы
 */
uint16_t EC_led_SRC(void){
   uint16_t src = 0;
   size_t sz1 = sizeof(LedConfig);
   uint16_t src_save = LedConfig.SRC;
   LedConfig.SRC = 0;
   for( int i=0; i<sz1; i++)src +=*((uint8_t*)&LedConfig + i);
   if( setup_debug )DebugSerial.printf("SCR=%d\n",src); 
   LedConfig.SRC = src_save;
   return src;  
}




