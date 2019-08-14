#include <Arduino_FreeRTOS.h>
#include <Servo.h>

void uzaklik(void *pvParameters);
void timer(void *pvParameters);

int32_t echoGiris = 12;
int32_t trigGiris = 11;
int32_t echoCikis = 10;
int32_t trigCikis = 9;
Servo servo;

int32_t salise = 0, saniye = 0, dakika = 0, sayac = 0, aradakiUzaklikGiris, aradakiUzaklikCikis;
unsigned long sureGiris, sureCikis;
double toplamYolGiris, toplamYolCikis;
float toplamSure = 0;

void setup() {
  xTaskCreate(uzaklik, (const portCHAR*)"Uzaklık", 100, NULL, 1, NULL);
  xTaskCreate(timer, (const portCHAR*)"Timer", 100, NULL, 1, NULL);  
  servo.attach(13);
}

void loop() {
}

void uzaklik(void *pvParameters) {
  (void) pvParameters;
  const char *pcTaskName = "Uzaklık";

  pinMode(echoGiris, INPUT);
  pinMode(trigGiris, OUTPUT);
  pinMode(echoCikis, INPUT);
  pinMode(trigCikis, OUTPUT);
  Serial.begin(9600);

  for(;;) {
    digitalWrite(trigGiris, HIGH);
    delayMicroseconds(2);
    digitalWrite(trigGiris, LOW);
    sureGiris = pulseIn(echoGiris, HIGH);
    toplamYolGiris = (double)sureGiris * 0.034;
    aradakiUzaklikGiris = toplamYolGiris / 2;
    if(aradakiUzaklikGiris > 200) {
      aradakiUzaklikGiris = 200;
    }
    if(aradakiUzaklikGiris < 200) {
      Serial.print("Sensör1 :");
      Serial.print(aradakiUzaklikGiris);
      Serial.println("cm.\n");
    }
    delay(100);

    digitalWrite(trigCikis, HIGH);
    delayMicroseconds(2);
    digitalWrite(trigCikis, LOW);
    sureCikis= pulseIn(echoCikis, HIGH);
    toplamYolCikis = (double)sureCikis * 0.034;
    aradakiUzaklikCikis = toplamYolCikis / 2;
    if(aradakiUzaklikCikis > 200) {
      aradakiUzaklikCikis = 200;
    }
    if(aradakiUzaklikCikis < 200) {
      Serial.print("Sensör2 :");
      Serial.print(aradakiUzaklikGiris);
      Serial.println("cm.\n");
    }
    delay(100);
  }
  Serial.println(pcTaskName);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void timer(void *pvParameters) {
  (void) pvParameters;
  Serial.begin(9600);
  const char *pcTaskName = "Kronometre";  
  servo.write(0);
  
  for(;;) {
    if(aradakiUzaklikGiris > 0 && aradakiUzaklikGiris < 30) {
      sayac = 1;
    }
    if(sayac == 1) {
      salise = salise + 1;
      if(salise > 59) {
        salise = 0;
        saniye = saniye + 1;
        if(saniye > 59) {
          saniye = 0;
          dakika = dakika + 1;
        }
      }
      Serial.print(dakika);
      Serial.print(":");
      Serial.print(saniye);
      Serial.print(":");
      Serial.print(salise);
      Serial.print("\n");
      delay(10);
    }
    if(aradakiUzaklikCikis > 0 && aradakiUzaklikCikis <30) {
      sayac = 0;
      Serial.println("Çıkış Yapıldı");
      toplamSure = (float)salise * 0.0167 + saniye;
      Serial.print("Toplam Süre :");
      Serial.print(toplamSure);
      Serial.print("\n");
      if(toplamSure > 0 && toplamSure < 5) {
        servo.write(180); //kapı açılıyor
        delay(4000);   
        servo.write(0); //kapı kapanıyor   
      }
      else if(toplamSure > 6 && toplamSure < 10) {
        delay(1000);
        servo.write(180); //kapı açılıyor
        delay(5000);   
        servo.write(0); //kapı kapanıyor   
      }
      else if(toplamSure > 11 && toplamSure < 15) {
        delay(2000);
        servo.write(180); //kapı açılıyor
        delay(5000);   
        servo.write(0); //kapı kapanıyor   
      }
      else if(toplamSure > 16 && toplamSure < 20) {
        delay(3000);
        servo.write(180); //kapı açılıyor
        delay(5000);   
        servo.write(0); //kapı kapanıyor   
      }
      else if(toplamSure > 21 && toplamSure < 25) {
        delay(4000);
        servo.write(180); //kapı açılıyor
        delay(5000);   
        servo.write(0); //kapı kapanıyor   
      }
      else {
        delay(2000);
        servo.write(180); //kapı açılıyor
        delay(7000);   
        servo.write(0); //kapı kapanıyor
      }
      salise = 0;
      saniye = 0;
      dakika = 0;
      delay(1000);
    }
  }
  Serial.println(pcTaskName);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
