
#include <ShiftedLCD.h>
#include <Wire.h>
#include <DS3231.h>

DS3231 clock;
RTCDateTime dt;
LiquidCrystal lcd(10);

// 3 - blue, 5, 6 - white

int light_len = 3;
int light_config[][6] = {

  {
    3,
    18 * 60,
    2 * 60,
    22 * 60,
    2 * 60 - 1,
    10
  },
  {
    5,
    6 * 60,
    2 * 60,
    19 * 60,
    2 * 60 - 1,
    50
  },
  {
    6,
    6 * 60,
    2 * 60,
    20 * 60,
    2 * 60 - 1,
    50
  }
};



void setup() {
  lcd.begin(16, 2);
  delay(500);
  Serial.begin(9600);
  clock.begin();
  //    clock.setDateTime(__DATE__, __TIME__);

  for (int i = 0; i < light_len; i++) {
    pinMode(light_config[i][0], OUTPUT);
  }
}

void set_light() {
  int t = dt.hour * 60 + dt.minute;
  for (int i = 0; i < light_len; i++) {

    int pin = light_config[i][0];
    int brightening_start = light_config[i][1];
    int brightening_end = brightening_start + light_config[i][2];
    int darkening_start = light_config[i][3];
    int darkening_end = darkening_start + light_config[i][4];
    int max_power = light_config[i][5];
    int power = 0;

    if (t > brightening_start && t <= brightening_end) {
      // calculate
      power = map(t, brightening_start, brightening_end, 0, max_power);
    } else if (t > brightening_end && t <= darkening_start) {
      power = max_power;
    } else if (t > darkening_start && t <= darkening_end) {
      power = map(t, darkening_start, darkening_end, max_power, 0);
    }

    analogWrite(pin, power);
  }
}

void loop()
{

  lcd.clear();
  lcd.setCursor(0, 0);

 // dt = clock.getDateTime();
  //set_light();

  clock.forceConversion();
 
  Serial.print("Temperature: ");
  Serial.println(clock.readTemperature());
  
  //  Serial.print(dt.year);   Serial.print("-");
  //  Serial.print(dt.month);  Serial.print("-");
  //  Serial.print(dt.day);    Serial.print(" ");
  //  Serial.print(dt.hour);   Serial.print(":");
  //  Serial.print(dt.minute); Serial.print(":");
  //  Serial.print(dt.second); Serial.println("");
  
  delay(1000);
}

