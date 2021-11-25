#include <NewPing.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <EEPROM.h>

VL53L0X sensor;

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

long long scramble_length = 16;

String scramble() {
  int count = 0;
  int i = 0;
  String ans = "                                ";
  char c, h = ' ';
  int a = -1, aold = -2, aold2 = -3;
  while (count < scramble_length)
  {
    a = random(0, 6);
    while (a == aold)
      a = random(0, 6);
    if (aold2 == a && abs(aold - a) == 3)
    {
      while (aold2 == a || aold == a)
        a = random(0, 6);
    }
    if (a == 0)
      c = 'R';
    if (a == 1)
      c = 'U';
    if (a == 2)
      c = 'F';
    if (a == 3)
      c = 'L';
    if (a == 4)
      c = 'D';
    if (a == 5)
      c = 'B';
    aold2 = aold;
    aold = a;
    a = random(0, 3);
    if (a == 1)
      h = char(39);
    if (a == 2)
      h = char(50);
    ans[i] = c;
    if (a > 0)
      ans[i + 1] = h;
    i++;
    if (a > 0)
      i++;
    h = ' ';
    count++;
  }
  return ans;
}

String s = scramble();
String old;
bool leave1 = false, leave2 = false;
long long old_distance;
long long d;
bool AVGmode = false;
long long m[12];
int it = 0;
long long ch[12];
int place;
int distance;
int wait_time;
bool lights;
bool print_scramble;
bool resetWithVL;
int AVGset;
int soundTime;
bool button = false;

void setup() {
  if (EEPROM.read(0) == 255)
    distance = 250;
  else
    distance = EEPROM.read(0);
  if (distance == 0)
    distance = 150;
  if (distance == 1)
    distance = 200;
  if (distance == 2)
    distance = 250;
  if (distance == 3)
    distance = 300;
  if (distance == 4)
    distance = 350;
  if (EEPROM.read(1) == 255)
    wait_time = 750;
  else
    wait_time = EEPROM.read(1);
  if (wait_time == 0)
    wait_time = 500;
  if (wait_time == 1)
    wait_time = 750;
  if (wait_time == 2)
    wait_time = 1000;
  if (wait_time == 3)
    wait_time = 1250;
  if (wait_time == 4)
    wait_time = 1500;
  if (EEPROM.read(2) == 255)
    lights = true;
  else
    lights = EEPROM.read(2);
  if (EEPROM.read(3) == 255)
    print_scramble = true;
  else
    print_scramble = EEPROM.read(3);
  if (EEPROM.read(4) == 255)
    resetWithVL = false;
  else
    resetWithVL = EEPROM.read(4);
  if (EEPROM.read(5) == 255)
    AVGset = 5;
  else
    AVGset = EEPROM.read(5);
  if (EEPROM.read(6) == 255)
    soundTime = -1;
  else
    soundTime = EEPROM.read(6);
  if (EEPROM.read(7) == 255)
    button = false;
  else {
    if (EEPROM.read(7) == 1)
      button = true;
    else
      button = false;
  }
  if (soundTime == 0)
    soundTime = -1;
  if (soundTime == 1)
    soundTime = 10000;
  if (soundTime == 2)
    soundTime = 20000;
  if (soundTime == 3)
    soundTime = 30000;
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("   Timer v4.1");
  delay(2000);
  Wire.begin();
  sensor.init();
  sensor.setTimeout(500);
  Serial.print(1);
  while (1) {
    while (1) { //вечный цикл
      digitalWrite(11, LOW);
      digitalWrite(10, LOW);
      lcd.clear();
      bool leave1 = false, leave2 = false;
      long long temp_time;
      long long temp_time2;
      randomSeed(analogRead(0));
      if (print_scramble) {
        printScramble(s);
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     0:00.00");
      }
      while (1) { //ждём нажатия или когда дальномер что-то запалит
        if (leave1)
          break;
        if (leave1)
          break;
        if (resetBut()) { //нажали кнопку сброса
          s = scramble();
          temp_time = millis();
          while (resetBut()) {
            if (millis() - temp_time >= 1000 && millis() - temp_time <= 1050) { //если была нажата в течение секунды
              lcd.clear();
              lcd.setCursor(0, 0);
              if (AVGmode)
                lcd.print("  AVGmode: ON");
              else
                lcd.print("  AVGmode: OFF");
            }
          }
          if (millis() - temp_time >= 1000) { //если была нажата в течение секунды
            for (int i = 9; ; ) {
              if (resetBut()) { //настройки
                temp_time2 = millis();
                while (resetBut()) {
                  if (millis() - temp_time2 >= 1000) {
                    leave1 = true;
                    if (distance == 150)
                      EEPROM.write(0, 0);
                    if (distance == 200)
                      EEPROM.write(0, 1);
                    if (distance == 250)
                      EEPROM.write(0, 2);
                    if (distance == 300)
                      EEPROM.write(0, 3);
                    if (distance == 350)
                      EEPROM.write(0, 4);
                    if (wait_time == 500)
                      EEPROM.write(1, 0);
                    if (wait_time == 750)
                      EEPROM.write(1, 1);
                    if (wait_time == 1000)
                      EEPROM.write(1, 2);
                    if (wait_time == 1250)
                      EEPROM.write(1, 3);
                    if (wait_time == 1500)
                      EEPROM.write(1, 4);
                    EEPROM.write(2, lights);
                    EEPROM.write(3, print_scramble);
                    EEPROM.write(4, resetWithVL);
                    EEPROM.write(5, AVGset);
                    if (soundTime == -1)
                      EEPROM.write(6, 0);
                    if (soundTime == 10000)
                      EEPROM.write(6, 1);
                    if (soundTime == 20000)
                      EEPROM.write(6, 2);
                    if (soundTime == 30000)
                      EEPROM.write(6, 3);
                    if (button)
                      EEPROM.write(7, 1);
                    else
                      EEPROM.write(7, 0);
                    break;
                  }
                }
                if (leave1)
                  break;
                i++;
                if (i == 10)
                  i = 0;
                if (i == 0) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Wait time: ");
                  lcd.print(double(wait_time) / 1000.0);
                  lcd.setCursor(0, 1);
                  lcd.print("seconds");
                }
                if (i == 1) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Distance: ");
                  lcd.print(int(distance) / 10);
                  lcd.print(" cm");
                }
                if (i == 2) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Scramble length:");
                  lcd.setCursor(0, 1);
                  lcd.print(int(scramble_length));
                }
                if (i == 3) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Lights: ");
                  if (lights)
                    lcd.print("On");
                  else
                    lcd.print("Off");
                }
                if (i == 4) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Print scramble:");
                  lcd.setCursor(0, 1);
                  if (print_scramble)
                    lcd.print("On");
                  else
                    lcd.print("Off");
                }
                if (i == 5) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Reset with");
                  lcd.setCursor(0, 1);
                  lcd.print("rangefinder: ");
                  if (resetWithVL)
                    lcd.print("Yes");
                  else
                    lcd.print("No");
                }
                if (i == 6) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Calculate ");
                  if (AVGset == 5)
                    lcd.print("AVG5");
                  if (AVGset == 12)
                    lcd.print("AVG12");
                }
                if (i == 7) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Beep");
                  lcd.setCursor(0, 1);
                  if (soundTime == -1)
                    lcd.print("never");
                  if (soundTime == 10000)
                    lcd.print("every 10 seconds");
                  if (soundTime == 20000)
                    lcd.print("every 20 seconds");
                  if (soundTime == 30000)
                    lcd.print("every 30 seconds");
                }
                if (i == 8) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Button: ");
                  if (button)
                    lcd.print("ON");
                  else
                    lcd.print("OFF");
                }
                if (i == 9) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  if (AVGmode)
                    lcd.print("  AVGmode: ON");
                  else
                    lcd.print("  AVGmode: OFF");
                }
              }
              if (i == 0) {
                if (setBut()) {
                  wait_time += 250;
                  if (wait_time >= 1750)
                    wait_time = 500;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Wait time: ");
                  lcd.print(double(wait_time) / 1000.0);
                  lcd.setCursor(0, 1);
                  lcd.print("seconds");
                  while (setBut())
                    ;
                }
              }
              if (i == 1) {
                if (setBut()) {
                  distance += 50;
                  if (distance >= 400)
                    distance = 150;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Distance: ");
                  lcd.print(int(distance) / 10);
                  lcd.print(" cm");
                  while (setBut())
                    ;
                }
              }
              if (i == 2) {
                if (setBut()) {
                  scramble_length += 2;
                  if (scramble_length >= 18)
                    scramble_length = 8;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Scramble length:");
                  lcd.setCursor(0, 1);
                  lcd.print(int(scramble_length));
                  s = scramble();
                  while (setBut())
                    ;
                }
              }
              if (i == 3) {
                if (setBut()) {
                  lights = !lights;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Lights: ");
                  if (lights)
                    lcd.print("On");
                  else
                    lcd.print("Off");
                  while (setBut())
                    ;
                }
              }
              if (i == 4) {
                if (setBut()) {
                  print_scramble = !print_scramble;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Print scramble:");
                  lcd.setCursor(0, 1);
                  if (print_scramble)
                    lcd.print("On");
                  else
                    lcd.print("Off");
                  while (setBut())
                    ;
                }
              }
              if (i == 5) {
                if (setBut()) {
                  resetWithVL = !resetWithVL;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Reset with");
                  lcd.setCursor(0, 1);
                  lcd.print("rangefinder: ");
                  if (resetWithVL)
                    lcd.print("Yes");
                  else
                    lcd.print("No");
                  while (setBut())
                    ;
                }
              }
              if (i == 6) {
                if (setBut()) {
                  if (AVGset == 5)
                    AVGset = 12;
                  else if (AVGset == 12)
                    AVGset = 5;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Calculate ");
                  if (AVGset == 5)
                    lcd.print("AVG5");
                  if (AVGset == 12)
                    lcd.print("AVG12");
                  it = 0;
                  while (setBut())
                    ;
                }
              }
              if (i == 7) {
                if (setBut()) {
                  if (soundTime == -1)
                    soundTime = 10000;
                  else if (soundTime == 10000)
                    soundTime = 20000;
                  else if (soundTime == 20000)
                    soundTime = 30000;
                  else if (soundTime == 30000)
                    soundTime = -1;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Beep");
                  lcd.setCursor(0, 1);
                  if (soundTime == -1)
                    lcd.print("never");
                  if (soundTime == 10000)
                    lcd.print("every 10 seconds");
                  if (soundTime == 20000)
                    lcd.print("every 20 seconds");
                  if (soundTime == 30000)
                    lcd.print("every 30 seconds");
                  while (setBut())
                    ;
                }
              }
              if (i == 8) {
                if (setBut()) {
                  button = !button;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Button: ");
                  if (button)
                    lcd.print("ON");
                  else
                    lcd.print("OFF");
                  while (setBut())
                    ;
                }
              }
              if (i == 9) {
                if (setBut()) {
                  AVGmode = !AVGmode;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  if (AVGmode)
                    lcd.print("  AVGmode: ON");
                  else
                    lcd.print("  AVGmode: OFF");
                  while (setBut())
                    ;
                }
              }
            }
          }//конец настроек
          lcd.clear();
          leave1 = true;
        }
        if (leave1) { //если нужно пойти в начало
          if (print_scramble)
            printScramble(s);
          else {
            lcd.clear();
            lcd.print("     0:00.00");
          }
          while (resetBut())
            ;
          break;
        }
        if ((sensor.readRangeSingleMillimeters() <= distance && !button) || (setBut() && button)) { //если дальномер что-то запалил
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("     0:00.00");
          if (AVGmode) {
            lcd.print(',');
            lcd.print(it + 1);
            lcd.print('A');
          }
          lcd.setCursor(0, 1);
          if (AVGmode)
            lcd.print(" AVGmode is ON");
          else
            lcd.print(" AVGmode is OFF");
          temp_time = millis();
          while ((sensor.readRangeSingleMillimeters() <= distance && !button) || (setBut() && button)) {
            digitalWrite(11, HIGH);
            if (millis() - temp_time >= wait_time)
              digitalWrite(10, HIGH);
          }
          digitalWrite(11, LOW);
          digitalWrite(10, LOW);
          if (millis() - temp_time >= wait_time) { //если держали предмет достаточно долго
            leave2 = true;
            break;
          }
          else
          {
            leave1 = true;
            break;
          }
        }
        if (leave1 || leave2) {
          break;
        }
      }
      if (leave1) //если держали предмет недостаточно долго
        break;
      old = s;
      place = 1;
      s = scramble();
      leave2 = false;
      long long main_time;
      long long start_time = millis();
      lcd.clear();
      lcd.setCursor(0, 0);
      leave1 = false;
      while (1) { //время пошло
        if (resetBut()) { //если нажали копку сброса
          leave1 = true;
          break;
        }
        main_time = millis();
        printTime(main_time - start_time);
        if (soundTime >= 0 && (main_time - start_time) % soundTime >= (soundTime - 100)) {
          analogWrite(9, 10);
        }
        else
        {
          analogWrite(9, 0);
        }
        if (lights) {
          if ((main_time - start_time) % 100 < 50) { //мигаем светодиодами
            analogWrite(10, 10);
            analogWrite(11, 0);
          }
          else
          {
            analogWrite(10, 0);
            analogWrite(11, 10);
          }
        }
        if ((sensor.readRangeSingleMillimeters() <= distance && !button) || (setBut() && button)) { //если дальномер что-то запалил
          analogWrite(9, 0);
          analogWrite(10, 0);
          analogWrite(11, 0);
          while (((sensor.readRangeSingleMillimeters() <= distance && !button) || (setBut() && button)) && resetWithVL && lights) //пока держали предмет
            digitalWrite(11, HIGH);
          place = 0;
          while (1) { //после окончания сборки
            if (((sensor.readRangeSingleMillimeters() <= distance && !button)) && lights) //если дальномер что-то запалил
              digitalWrite(11, HIGH);
            else
              digitalWrite(11, LOW);
            if (resetBut() || (((sensor.readRangeSingleMillimeters() <= distance && !button)) && resetWithVL)) { //сбрасываем и ливаем
              digitalWrite(11, LOW);
              long long temp_time2 = millis();
              while (resetBut() || (((sensor.readRangeSingleMillimeters() <= distance && !button)) && resetWithVL)) {
                if (lights && (((sensor.readRangeSingleMillimeters() <= distance && !button)) && resetWithVL))
                  digitalWrite(11, HIGH);
                else
                  digitalWrite(11, LOW);
                if ((millis() - temp_time2) >= 1000 && AVGmode)
                  break;
              }
              if ((millis() - temp_time2) >= 1000 && AVGmode) {
                lcd.clear();
                lcd.setCursor(0, 0);
                digitalWrite(11, LOW);
                digitalWrite(10, LOW);
                lcd.print("Attempt deleted");
                delay(1000);
                while ((sensor.readRangeSingleMillimeters() <= distance && !button) || (setBut() && button))
                  ;
                leave1 = true;
              }
              break;
            }
            if (setBut()) { //если нажали кнопку настроек
              long long temp_time2 = millis();
              while (setBut())
                ;
              if (millis() - temp_time2 >= 1000)
                place--;
              place++;
              digitalWrite(11, LOW);
              if (place == 2 && (!AVGmode))
                place = 0;
              if (place == 4 && AVGmode)
                place = 0;
              if (place == 1 && AVGmode && !print_scramble)
                place++;
              if (place == 0) {
                printTime(main_time - start_time);
                while (setBut())
                  ;
              }
              else if (place == 1) {
                lcd.clear();
                if (print_scramble) {
                  printScramble(old);
                }
                else
                {
                  if (!AVGmode)
                    printTime(main_time - start_time);
                }
              }
              else if (place == 2) {
                lcd.clear();
                lcd.setCursor(5, 0);
                lcd.print("Put +2");
                if (millis() - temp_time2 >= 1000) {
                  if (ch[it] != 2) {
                    ch[it] = 2;
                    lcd.setCursor(0, 1);
                    lcd.print("+2 was put");
                    delay(1000);
                    lcd.setCursor(0, 1);
                    lcd.print("                ");
                  }
                  else
                  {
                    ch[it] = 0;
                    lcd.setCursor(0, 1);
                    lcd.print("+2 was removed");
                    delay(1000);
                    lcd.setCursor(0, 1);
                    lcd.print("                ");
                  }
                }
              }
              else if (place == 3) {
                lcd.clear();
                lcd.setCursor(5, 0);
                lcd.print("Put DNF");
                if (millis() - temp_time2 >= 1000) { //если зажали на секунду
                  if (ch[it] != -1) {
                    ch[it] = -1;
                    for (int test = it - 1; test >= 0; test--) {
                      if (ch[test] == -1) {
                        it = 0;
                        lcd.clear();
                        lcd.setCursor(1, 0);
                        lcd.print("Too many DNFs");
                        for (int i = 0; i < 12; i++) {
                          ch[i] = 0;
                          m[i] = 0;
                        }
                        while (!resetBut())
                          ;
                        lcd.clear();
                        while (resetBut())
                          ;
                        leave1 = true;
                        break;
                      }
                    }
                    if (leave1)
                      break;
                    lcd.setCursor(0, 1);
                    lcd.print("DNF was put");
                    delay(1000);
                    lcd.setCursor(0, 1);
                    lcd.print("                ");
                  }
                  else
                  {
                    ch[it] = 0;
                    lcd.setCursor(0, 1);
                    lcd.print("DNF was removed");
                    delay(1000);
                    lcd.setCursor(0, 1);
                    lcd.print("                ");
                  }
                }
              }
            }
            if (leave1)
              break;
          }
          leave2 = true;
          break;
        }
        if (leave1 || leave2)
          break;
      }
      if (leave1)
        break;
      if (AVGmode) {
        AVG(main_time - start_time);
      }
      if (leave2)
        break;
    }
  }
}
void loop() {
}
void AVG(int a) {
  m[it] = a;
  it++;
  if (it == AVGset) {
    it = 0;
    for (int i = 0; i < 12; i++) {
      if (ch[i] == 2) {
        m[i] = m[i] + 2000;
      }
    }
    long long mint = m[0], imin = 0, maxt = m[0], imax = 0;
    for (int i = 0; i < AVGset; i++) {
      if (m[i] < mint) {
        mint = m[i];
        imin = i;
      }
      if (m[i] > maxt && maxt != -1) {
        maxt = m[i];
        imax = i;
      }
      if (ch[i] == -1) {
        maxt = -1;
        imax = i;
      }
    }
    long long avg;
    avg = 0;
    for (int i = 0; i < AVGset; i++) {
      if (i != imin && i != imax) {
        avg += m[i];
      }
    }
    avg = avg / (AVGset - 2);
    printAVG(avg);
    for (int ij = 0; ; ) {
      if (setBut()) {
        ij++;
        if (ij > AVGset)
          ij = 0;
        if (ij == 0)
          printAVG(avg);
        else
        {
          lcd.clear();
          lcd.print("   ");
          lcd.print(ij);
          lcd.print(" attempt:");
          if (ij - 1 != imin && ij - 1 != imax) {
            lcd.setCursor(5, 1);
            p1(m[ij - 1]);
          }
          else
          {
            lcd.setCursor(4, 1);
            p2(m[ij - 1]);
          }
          if (ch[ij - 1] == 2) {
            lcd.setCursor(0, 1);
            lcd.print("                ");
            lcd.setCursor(0, 1);
            lcd.print("+2: ");
            if (ij - 1 != imin && ij - 1 != imax)
              p1(m[ij - 1]);
            else
              p2(m[ij - 1]);
          }
          if (ch[ij - 1] == -1) {
            lcd.setCursor(0, 1);
            lcd.print("DNF: ");
            p2(m[ij - 1]);
          }
        }
        while (setBut())
          ;
      }
      if (resetBut())
        break;
    }
    while (resetBut())
      ;
    for (int i = 0; i < 12; i++) {
      ch[i] = 0;
      m[i] = 0;
    }
  }
}
void p1(long long a) {
  if (a / 3600000 > 0) {
    lcd.print(long(a / 3600000));
    lcd.print(":");
  }
  lcd.print(long(a % 3600000) / 60000);
  lcd.print(":");
  if (a % 60000 < 10000)
    lcd.print("0");
  lcd.print(long(a % 60000) / 1000);
  lcd.print(".");
  if ((a % 1000) / 10 < 10)
    lcd.print("0");
  lcd.print(long(a % 1000) / 10);
}
void p2(long long a) {
  lcd.print("(");
  if (a / 3600000 > 0) {
    lcd.print(long(a / 3600000));
    lcd.print(":");
  }
  lcd.print(long(a % 3600000) / 60000);
  lcd.print(":");
  if (a % 60000 < 10000)
    lcd.print("0");
  lcd.print(long(a % 60000) / 1000);
  lcd.print(".");
  if ((a % 1000) / 10 < 10)
    lcd.print("0");
  lcd.print(long(a % 1000) / 10);
  lcd.print(")");
}
void printAVG(long long avg) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Your AVG:");
  lcd.setCursor(5, 1);
  if (avg / 3600000 > 0) {
    lcd.print(long(avg / 3600000));
    lcd.print(":");
  }
  lcd.print(long(avg % 3600000) / 60000);
  lcd.print(":");
  if (avg % 60000 < 10000)
    lcd.print("0");
  lcd.print(long(avg % 60000) / 1000);
  lcd.print(".");
  if ((avg % 1000) / 10 < 10)
    lcd.print("0");
  lcd.print(long(avg % 1000) / 10);
}
void printTime(long long a) {
  lcd.clear();
  lcd.setCursor(5, 0);
  if (a / 3600000 > 0) {
    lcd.print(long(a / 3600000));
    lcd.print(":");
  }
  lcd.print(long(a % 3600000) / 60000);
  lcd.print(":");
  if (a % 60000 < 10000)
    lcd.print("0");
  lcd.print(long(a % 60000) / 1000);
  lcd.print(".");
  if ((a % 1000) / 10 < 10)
    lcd.print("0");
  lcd.print(long(a % 1000) / 10);
}
void printScramble(String s) {
  for (int i = 0; i < 16; i++)
  {
    lcd.setCursor(i, 0);
    lcd.print(s[i]);
  }
  for (int i = 0; i < s.length() - 16; i++)
  {
    lcd.setCursor(i, 1);
    lcd.print(s[i + 16]);
  }
}
bool resetBut() {
  return digitalRead(8);
}
bool setBut() {
  return digitalRead(12);
}
