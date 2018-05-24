
#include <SoftwareSerial.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <LiquidCrystal_I2C.h>

Adafruit_24bargraph bar = Adafruit_24bargraph();
SoftwareSerial mySerial(0, 1); // RX, TX
Servo myservo;
LiquidCrystal_I2C lcd(0x27, 20, 4);

char buf[128] = {};

//double battery;
double soc;
double kms;
double speed_;



void setup() {
  Serial.begin(9600);
  Serial.println("Attente réception !");
  Serial.println("HT16K33 Bi-Color Bargraph test");

  bar.begin(0x70);  // pass in the address
  mySerial.begin(9600);
  myservo.attach(9);
  myservo.write(0);
  lcd.init(); // initialisation de l'afficheur

}


void loop() {


  char recv = '\0';
  int i = 0;
  if (Serial.available()) {
    do {
      while ((recv = Serial.read()) != -1) {
        buf[i++] = recv;
      }
      buf[i] = '\0';
      if (i == 0) {
        break;
      }
    } while (buf[i - 1] != '\n' && buf[i - 1] != '\r');

    Serial.println("valeur de buf : ");
    Serial.println(buf);
    //}


    //uint8_t newbyte = mySerial.read();
    //Serial.write(newbyte);

    char str[128] = {0};
    strcpy(str, buf);

    Serial.println("valeur de str : ");

    Serial.println(str);

    char * pch;
    pch = strtok (str, "=");
    char * first = pch;
    pch = strtok (NULL, "\n");
    Serial.println("premier pch :");
    Serial.println(pch);
    Serial.println("seconde partie : ");
    Serial.println(first);



    if (strcmp(first , "speed") == 0)
    {
      speed_ = atof(pch);
      affichage_Speed();
    }
    else if (strcmp(first , "kms") == 0)
    {
      kms = atof(pch);
      affichage_LCD();

    }
    else if (strcmp(first , "soc") == 0)
    {
      soc = atof(pch);
      switchOffLED();
      affichage_Baragraph ();
      affichage_LCD();
    }
    else
    {
      printf ("Error : any values ");
    }


  }

}


/* the function affichage_Baragraph permit to display the leds according to the value of "battery"*/

void affichage_Baragraph ()
{
  double value_battery;
  value_battery = soc;
  if (value_battery == 100)
  {
    for (uint8_t b = 0; b < 24; b++ ) {
      bar.setBar(b, LED_GREEN);
      bar.writeDisplay();
    }
  }

  else if (value_battery < 100 && value_battery > 87.5)
  {
    for (uint8_t b = 0; b < 21; b++ ) {
      bar.setBar(b, LED_GREEN);
      bar.writeDisplay();
    }
  }

  else if (value_battery < 87.51 && value_battery > 75)
  {
    for (uint8_t b = 0; b < 21; b++ ) {
      bar.setBar(b, LED_GREEN);
      bar.writeDisplay();
    }
  }

  else if (value_battery < 75.1 && value_battery > 62.5)
  {
    for (uint8_t b = 0; b < 18; b++ ) {
      bar.setBar(b, LED_GREEN);
      bar.writeDisplay();
    }
  }

  else if (value_battery < 62.51 && value_battery > 50)
  {
    for (uint8_t b = 0; b < 15; b++ ) {
      bar.setBar(b, LED_GREEN);
      bar.writeDisplay();
    }
  }

  else if (value_battery < 50.1 && value_battery > 37.50)
  {
    for (uint8_t b = 0; b < 12; b++ ) {
      bar.setBar(b, LED_GREEN);
      bar.writeDisplay();
    }
  }

  else if (value_battery < 37.51 && value_battery > 25)
  {
    for (uint8_t b = 0; b < 9; b++ ) {
      bar.setBar(b, LED_GREEN);
      bar.writeDisplay();
    }
  }

  else if (value_battery < 26 && value_battery > 12.5)
  {
    for (uint8_t b = 0; b < 6; b++ ) {
      bar.setBar(b, LED_GREEN);
      bar.writeDisplay();
    }
  }

  else if (value_battery < 12.6  && value_battery > 0)
  {
    for (uint8_t b = 0; b < 3; b++ ) {
      bar.setBar(b, LED_RED);
      bar.writeDisplay();
    }
  }

  else if (value_battery == 0)
  {
    for (uint8_t b = 0; b < 1; b++ ) {
      bar.setBar(b, LED_OFF);
      bar.writeDisplay();
    }
  }

}


/* the function affichage_LCD allows to display the kilometer value on the screen of  the servo motor according to the value of "speed_"*/

void affichage_LCD()
{
  double value_kms = kms;
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Kilometrage");
  lcd.setCursor(0, 1);
  lcd.print(value_kms, 4);

}


/* the function affichage_Speed allows to give the angle of the speed on the servo motor according to the value of "speed_"*/

void affichage_Speed()
{

  double val = map(speed_, 0, 200, 0, 180);
  myservo.write(val);
  delay(10);
}

void switchOffLED()
{
  for (uint8_t b = 0; b < 24; b++ ) {
    bar.setBar(b, LED_OFF);
    bar.writeDisplay();
  }
}


