// Sistema de Monitoreo de Calidad del Agua
// Desarrollado para ESP32-C3 Luatos

// Incluimos las bibliotecas necesarias
#include <Arduino.h>      // Biblioteca principal de Arduino
#include <U8g2lib.h>      // Para controlar la pantalla OLED
#include <EEPROM.h>       // Para almacenar datos en la memoria no volátil
#include "GravityTDS.h"   // Para el sensor TDS (Total de Sólidos Disueltos)
#include <OneWire.h>      // Para comunicación con sensores OneWire
#include <DallasTemperature.h> // Para el sensor de temperatura DS18B20

// Definimos los pines que vamos a utilizar
#define OLED_SDA 4        // Pin de datos para la pantalla OLED
#define OLED_SCL 5        // Pin de reloj para la pantalla OLED
#define OLED_RST -1       // Pin de reset para la pantalla OLED (no usado)
#define TdsSensorPin 1    // Pin para el sensor TDS
#define ONE_WIRE_BUS 2    // Pin para el sensor de temperatura DS18B20

// Pines para los LEDs indicadores
#define LED_GREEN 6       // LED verde para agua de buena calidad
#define LED_YELLOW 10     // LED amarillo para agua de calidad media
#define LED_RED 3         // LED rojo para agua de baja calidad

#define EEPROM_SIZE 512   // Tamaño de la memoria EEPROM que vamos a usar

// Configuración de la pantalla OLED
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_RST, OLED_SCL, OLED_SDA);

// Objetos para los sensores
GravityTDS gravityTds;    // Objeto para el sensor TDS
OneWire oneWire(ONE_WIRE_BUS); // Configuración del bus OneWire
DallasTemperature sensors(&oneWire); // Objeto para el sensor de temperatura

// Variables globales
float temperature = 25, tdsValue = 0;
int calidad_buena = 250, calidad_media = 350;

void setup() {
  // Inicializamos la comunicación serial
  Serial.begin(115200);
  
  // Inicializamos la EEPROM
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("Fallo al inicializar EEPROM");
    Serial.println("Reiniciando...");
    delay(1000);
    ESP.restart();
  }

  // Configuramos los pines
  pinMode(TdsSensorPin, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Inicializamos la pantalla OLED
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB10_tr);

  // Configuramos el sensor TDS
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(3.3);  // ESP32 usa 3.3V como voltaje de referencia
  gravityTds.setAdcRange(4096);  // ESP32 tiene un ADC de 12 bits
  gravityTds.begin();

  // Inicializamos el sensor de temperatura
  sensors.begin();

  Serial.println("Configuración completa");
}

// Función para actualizar los LEDs según la calidad del agua
void updateLEDs(float tds) {
  // Apagamos todos los LEDs
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);

  // Encendemos el LED correspondiente según el valor TDS
  if (tds < calidad_buena) {
    digitalWrite(LED_GREEN, HIGH);  // Agua de buena calidad
  } else if (tds < calidad_media) {
    digitalWrite(LED_YELLOW, HIGH); // Agua de calidad media
  } else {
    digitalWrite(LED_RED, HIGH);    // Agua de baja calidad
  }
}

void loop() {
  // Leemos la temperatura
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  // Leemos el valor TDS usando la temperatura actual
  gravityTds.setTemperature(temperature);
  gravityTds.update();
  tdsValue = gravityTds.getTdsValue();

  // Actualizamos los LEDs
  updateLEDs(tdsValue);

  // Actualizamos la pantalla OLED
  u8g2.clearBuffer();
  
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(0, 20, "Temp:");
  u8g2.drawStr(0, 45, "TDS:");
  
  char tempStr[10];
  char tdsStr[10];
  dtostrf(temperature, 4, 1, tempStr);
  dtostrf(tdsValue, 4, 1, tdsStr);
  
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(50, 20, tempStr);
  u8g2.drawStr(50, 45, tdsStr);
  
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(105, 20, "C");
  u8g2.drawStr(105, 45, "ppm");
  
  u8g2.sendBuffer();

  // Imprimimos los datos en el monitor serial
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" C, Valor TDS: ");
  Serial.print(tdsValue, 1);
  Serial.print(" ppm, LED: ");
  if (tdsValue < 250) {
    Serial.println("Verde");
  } else if (tdsValue < 350) {
    Serial.println("Amarillo");
  } else {
    Serial.println("Rojo");
  }

  // Esperamos un segundo antes de la siguiente lectura
  delay(1000);
}