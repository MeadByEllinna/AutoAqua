

#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>
#include <EEPROM.h>

#define TFT_CS 27
#define TFT_DC 26
#define TFT_RST 25
#define SensorPin A0  //按键读取PIN
#define Offset 0.0    //PH校正值
unsigned long int avgValue;
float NEUTRAL = 0;  //6.86标准溶液PH电压值
float ACIDITY = 0;  //4.00标准溶液PH电压值
float ALKALIN = 0;  //9.18标准溶液PH电压值
float M1 = 0;       //电机M1 10000ms 液体流量(mL)
float M2 = 0;       //电机M2 10000ms 液体流量(mL)
float M1Per = 0.0;  //电机M1每ms流速
float M2Per = 0.0;  //电机M2ms秒流速
int correct_menu = 0;
float phValue = 0.0;
int menu[5][7] = { { 2, 0 }, { 0, 6 }, { 0, 3 }, { 0, 4 }, { 2, 5 } };


Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const int Control_Btn_Pin = A3;
int pointer[] = { 0, 0 };
bool is_Selected = false;
const int EEPROM_SIZE = 4096;
void setup(void) {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  Serial.println(" ");
  Serial.println(" 全自动海缸检测设备 V 1.2 启动中。。。");
  Serial.println("-----------------------------------");
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("存储数据读取失败！");
  } else {
    Serial.println("存储数据读取成功！");
    //读取之前保存的酸碱标准电压值
    NEUTRAL = float(EEPROM.readInt(0));
    ALKALIN = float(EEPROM.readInt(4));
    ACIDITY = float(EEPROM.readInt(8));
    M1 = EEPROM.readFloat(12);
    M2 = EEPROM.readFloat(16);
    M1Per = M1 / 10000;
    M2Per = M2 / 10000;
    Serial.print("PH计 中性标准电位:");
    Serial.println(NEUTRAL);
    Serial.print("PH计 碱性标准电位::");
    Serial.println(ALKALIN);
    Serial.print("PH计 酸性标准电位::");
    Serial.println(ACIDITY);
    Serial.print("电机 M1 1000ms 液体流量(mL):");
    Serial.println(M1);
    Serial.print("电机 M2 1000ms 液体流量(mL):");
    Serial.println(M2);
    Serial.print("电机 M1 1ms 液体流量(mL):");
    Serial.println(M1Per);
    Serial.print("电机 M2 1ms 液体流量(mL):");
    Serial.println(M2Per);
    EEPROM.end();
  }
  tft.init(240, 240);  // Init ST7789 240x240
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);
  setViewer(0, 0, 0, 0);  //初始化数据区
  setPointer(0);          //初始化选择区
  Serial.println("-----------------------------------");
  Serial.println(" 全自动海缸检测设备 V 1.2 启动成功！");
  Serial.println("                 -By：MeadByEllinna！");
}

void loop() {
  Btn_Handle();
  PhGeter();
  delay(200);
}
void setViewer(float AngValue, float V2, float PH, float KH) {
  if (correct_menu == 0) {
    tft.setTextColor(ST77XX_YELLOW);
    tft.setCursor(0, 0);
    tft.setTextSize(4);
    tft.print("PH");
    tft.setCursor(32, 48);
    tft.setTextSize(3);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(String(PH));
    tft.setTextColor(ST77XX_ORANGE);
    tft.setCursor(128, 0);
    tft.setTextSize(4);
    tft.print("KH");
    tft.setCursor(160, 48);
    tft.setTextSize(3);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(String(KH));

    tft.setTextColor(ST77XX_CYAN);
    tft.setCursor(0, 96);
    tft.setTextSize(4);
    tft.print("Ca");
    tft.setCursor(32, 144);
    tft.setTextSize(3);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(300);

    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(128, 96);
    tft.setTextSize(4);
    tft.print("Mg");
    tft.setCursor(160, 144);
    tft.setTextSize(3);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(1400);


    tft.setTextColor(ST77XX_MAGENTA);
    tft.setCursor(48, 192);
    tft.setTextSize(2);
    tft.print("MeadByEllinna");
  } else if (correct_menu == 3) {
    if (AngValue == 0) {
      tft.setTextColor(ST77XX_ORANGE);
      tft.setCursor(0, 32);
      tft.setTextSize(2);
      tft.print("Ph:4.00=");
      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(0, 64);
      tft.setTextSize(2);
      tft.print("Ph:6.86=");
      tft.setTextColor(ST77XX_CYAN);
      tft.setCursor(0, 96);
      tft.setTextSize(2);
      tft.print("Ph:9.18=");
      tft.setTextColor(ST77XX_YELLOW);
      tft.setCursor(0, 164);
      tft.setTextSize(2);
      tft.print("Value:");
      tft.setCursor(0, 196);
      tft.setTextSize(2);
      tft.print(AngValue);
    } else {
      tft.fillRect(0, 196, 256, 32, ST77XX_BLACK);
      tft.setCursor(0, 196);
      tft.setTextSize(2);
      tft.setTextColor(ST77XX_YELLOW);
      tft.print(AngValue);
    }
  } else if (correct_menu == 4) {
    if (AngValue == 0) {
      tft.setTextColor(ST77XX_ORANGE);
      tft.setCursor(0, 8);
      tft.setTextSize(2);
      tft.print("M1:");
      tft.setTextColor(ST77XX_ORANGE);
      tft.setCursor(96, 8);
      tft.setTextSize(2);
      tft.print(M1);
      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(0, 72);
      tft.setTextSize(2);
      tft.print("M2:");
      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(96, 72);
      tft.setTextSize(2);
      tft.print(M2);
    }
  }
}


void setPointer(int key) {
  Serial.print("correct_menu:");
  Serial.println(correct_menu);
  if (correct_menu == 0) {  //首页
    if (key == 5) {
      correct_menu = pointer[0] == 0 ? 1 : 2;
      tft.fillScreen(ST77XX_BLACK);
      pointer[0] = 0;
      pointer[1] = 0;
      setViewer(0, 0, 0, 0);
      setPointer(0);
      return;
    }
    pointer[0] = key - 2 < 0 ? 0 : key - 2;
    tft.fillRect(0, 216, 256, 32, ST77XX_BLACK);
    tft.fillRect(pointer[0] == 0 ? 0 : 148, 216, pointer[0] == 0 ? 72 : 108, 32, ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setTextColor(pointer[0] == 0 ? ST77XX_BLACK : ST77XX_WHITE);
    tft.setCursor(8, 220);
    tft.print("START");
    tft.setTextColor(pointer[0] == 1 ? ST77XX_BLACK : ST77XX_WHITE);
    tft.setCursor(160, 220);
    tft.print("OPTION");
  } else if (correct_menu == 1) {  //测试页面
    if (key == 5) {
      if (pointer[1] == 5) {
        correct_menu = 0;
        tft.fillScreen(ST77XX_BLACK);
        pointer[0] = 0;
        pointer[1] = 0;
        setViewer(0, 0, 0, 0);
        setPointer(0);
      } else if (pointer[1] == 2) {  //KH校准被按下
        AUTO_KH_TEST();
      }
      return;
    }
    tft.drawRoundRect(24, 16, 188, 208, 5, ST77XX_BLUE);
    if (key == 1) {
      tft.fillRect(32, pointer[1] * 32 + 24, 172, 32, ST77XX_BLACK);
      pointer[1] = pointer[1] - 1;
    } else if (key == 4) {
      tft.fillRect(32, pointer[1] * 32 + 24, 172, 32, ST77XX_BLACK);
      pointer[1] = pointer[1] + 1;
    }
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.fillRect(32, pointer[1] * 32 + 24, 172, 32, ST77XX_WHITE);
    tft.setTextColor(pointer[1] == 0 ? ST77XX_BLACK : ST77XX_MAGENTA);
    tft.setCursor(40, 32);
    tft.print("ALL AUTO TEST");
    tft.setTextColor(pointer[1] == 1 ? ST77XX_BLACK : ST77XX_YELLOW);
    tft.setCursor(48, 64);
    tft.print("PH AUTO TEST");
    tft.setTextColor(pointer[1] == 2 ? ST77XX_BLACK : ST77XX_ORANGE);
    tft.setCursor(48, 96);
    tft.print("KH AUTO TEST");
    tft.setTextColor(pointer[1] == 3 ? ST77XX_BLACK : ST77XX_CYAN);
    tft.setCursor(48, 128);
    tft.print("Ca AUTO TEST");
    tft.setTextColor(pointer[1] == 4 ? ST77XX_BLACK : ST77XX_GREEN);
    tft.setCursor(48, 160);
    tft.print("Mg AUTO TEST");
    tft.setTextColor(pointer[1] == 5 ? ST77XX_BLACK : ST77XX_RED);
    tft.setCursor(96, 196);
    tft.print("BACK");
  } else if (correct_menu == 2) {  //设置页面
    if (key == 5) {
      if (pointer[1] == menu[correct_menu][1] - 1) {  //各个按钮触发
        correct_menu = 0;
      } else if (pointer[1] == 0) {  //PH传感器校正按钮被按下
        correct_menu = 3;
      } else if (pointer[1] == 1) {  //PH传感器校正按钮被按下
        correct_menu = 4;
      }
      pointer[0] = 0;
      pointer[1] = 0;
      tft.fillScreen(ST77XX_BLACK);
      setViewer(0, 0, 0, 0);
      setPointer(0);
      return;
    }
    tft.drawRoundRect(24, 16, 188, 208, 5, ST77XX_BLUE);
    if (key == 1) {
      tft.fillRect(32, pointer[1] * 32 + 24, 172, 32, ST77XX_BLACK);
      pointer[1] = pointer[1] - 1;
    } else if (key == 4) {
      tft.fillRect(32, pointer[1] * 32 + 24, 172, 32, ST77XX_BLACK);
      pointer[1] = pointer[1] + 1;
    }
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.fillRect(32, pointer[1] * 32 + 24, 172, 32, ST77XX_WHITE);
    tft.setTextColor(pointer[1] == 0 ? ST77XX_BLACK : ST77XX_MAGENTA);
    tft.setCursor(40, 32);
    tft.print("PH SENESOR");
    tft.setTextColor(pointer[1] == 1 ? ST77XX_BLACK : ST77XX_YELLOW);
    tft.setCursor(40, 64);
    tft.print("MOTOR");
    tft.setTextColor(pointer[1] == 5 ? ST77XX_BLACK : ST77XX_RED);
    tft.setCursor(96, 96);
    tft.print("BACK");
  } else if (correct_menu == 3) {  //PH校准页面
    if (key == 5) {
      if (pointer[1] == menu[correct_menu][1] - 1) {  //BACK按钮触发
        correct_menu = 0;
        tft.fillScreen(ST77XX_BLACK);
        setViewer(0, 0, 0, 0);
        setPointer(0);
        pointer[0] = 0;
        pointer[1] = 0;
        return;
      } else if (pointer[1] == 0) {  //PH = 4.00 校正按钮被按下
        ACIDITY = avgValue;
        EERPOM_WRITE_PH();
      } else if (pointer[1] == 1) {  //PH = 6.86 校正按钮被按下
        NEUTRAL = avgValue;
        EERPOM_WRITE_PH();
      } else if (pointer[1] == 2) {  //PH = 9.18 校正按钮被按下
        ALKALIN = avgValue;
        EERPOM_WRITE_PH();
      }
    }
    if (key == 1) {
      tft.fillRect(pointer[1] == menu[correct_menu][1] - 1 ? 48 : 96, pointer[1] * 32 + 24, 128, 32, ST77XX_BLACK);
      pointer[1] = pointer[1] - 1;
    } else if (key == 4) {
      tft.fillRect(96, pointer[1] * 32 + 24, 128, 32, ST77XX_BLACK);
      pointer[1] = pointer[1] + 1;
    }
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.fillRect(pointer[1] == menu[correct_menu][1] - 1 ? 48 : 96, pointer[1] * 32 + 24, 128, 32, ST77XX_WHITE);
    tft.setTextColor(pointer[1] == 0 ? ST77XX_BLACK : ST77XX_ORANGE);
    tft.setCursor(96, 32);
    tft.print(ACIDITY);
    tft.setTextColor(pointer[1] == 1 ? ST77XX_BLACK : ST77XX_GREEN);
    tft.setCursor(96, 64);
    tft.print(NEUTRAL);
    tft.setTextColor(pointer[1] == 2 ? ST77XX_BLACK : ST77XX_CYAN);
    tft.setCursor(96, 96);
    tft.print(ALKALIN);
    tft.setTextColor(pointer[1] == 5 ? ST77XX_BLACK : ST77XX_RED);
    tft.setCursor(88, 128);
    tft.print("BACK");
  } else if (correct_menu == 4) {  //电机校准页面
    Serial.print("X:");
    Serial.print(pointer[0]);
    Serial.print("Y:");
    Serial.print(pointer[1]);
    if (key == 5) {
      if (pointer[1] == menu[correct_menu][1] - 1) {  //BACK按钮触发
        correct_menu = 0;
        tft.fillScreen(ST77XX_BLACK);
        setViewer(0, 0, 0, 0);
        setPointer(0);
        pointer[0] = 0;
        pointer[1] = 0;
        return;
      } else if (pointer[0] == 0 && pointer[1] == 0) {  //M1  - 按钮被按下
        M1 - 0.1 <= 0 ? M1 = 0 : M1 -= 0.1, 1;
        tft.fillRect(96, 8, 72, 32, ST77XX_BLACK);
        tft.setTextColor(ST77XX_ORANGE);
        tft.setCursor(96, 8);
        tft.setTextSize(2);
        tft.print(M1);
        Serial.println("M1 - 按键触发");
      } else if (pointer[0] == 1 && pointer[1] == 0) {  //M1  + 按钮被按下
        M1 += 0.1;
        tft.fillRect(96, 8, 72, 32, ST77XX_BLACK);
        tft.setTextColor(ST77XX_ORANGE);
        tft.setCursor(96, 8);
        tft.setTextSize(2);
        tft.print(M1);
        Serial.println("M1 + 按键触发");
      } else if (pointer[0] == 0 && pointer[1] == 1) {  //M1  START 按钮被按下
        //M1电机启动部分代码
        Tank_water_IN(0);
        Serial.print("M1 START 按键触发");
      } else if (pointer[0] == 1 && pointer[1] == 1) {  //M1  SAVE 按钮被按下
        EERPOM_WRITE_MOTOR();
        Serial.println("M1 SAVE 按键触发");
      } else if (pointer[0] == 0 && pointer[1] == 2) {  //M2  - 按钮被按下
        M2 - 0.1 <= 0 ? M2 = 0 : M2 -= 0.1, 1;
        tft.fillRect(96, 8, 72, 32, ST77XX_BLACK);
        tft.setTextColor(ST77XX_GREEN);
        tft.setCursor(96, 72);
        tft.setTextSize(2);
        tft.print(M2);
        Serial.println("M2 - 按键触发");
      } else if (pointer[0] == 1 && pointer[1] == 2) {  //M2  + 按钮被按下
        M2 += 0.1;
        tft.fillRect(96, 72, 72, 32, ST77XX_BLACK);
        tft.setTextColor(ST77XX_GREEN);
        tft.setCursor(96, 72);
        tft.setTextSize(2);
        tft.print(M2);
        Serial.println("M2 + 按键触发");
      } else if (pointer[0] == 0 && pointer[1] == 3) {  //M2  START 按钮被按下
        //M12电机启动部分代码
        Serial.println("M2 START 按键触发");
        Hcl_water_IN(1);
        delay(10000);
        Hcl_water_IN(0);
      } else if (pointer[0] == 1 && pointer[1] == 3) {  //M2  SAVE 按钮被按下
        EERPOM_WRITE_MOTOR();
        Serial.println("M2 SAVE 按键触发");
      }
    }
    if (key == 1) {  //上键
      tft.fillRect(pointer[1] == 4 ? 72 : pointer[0] * 104 + 52, pointer[1] * 32, pointer[1] == 1 || pointer[1] == 3 || pointer[1] == 4 ? 80 : 32, 32, ST77XX_BLACK);
      pointer[1] = pointer[1] - 1;
      Serial.print("上按键触发,X:");
      Serial.print(pointer[1] == 4 ? 72 : pointer[0] * 104 + 52);
      Serial.print(",Y:");
      Serial.println(pointer[1] * 32);
    } else if (key == 4) {  //下键
      tft.fillRect(pointer[1] == 4 ? 72 : pointer[0] * 104 + 52, pointer[1] * 32, pointer[1] == 1 || pointer[1] == 3 || pointer[1] == 4 ? 80 : 32, 32, ST77XX_BLACK);
      pointer[1] = pointer[1] + 1;
      Serial.print("下按键触发,X:");
      Serial.print(pointer[1] == 4 ? 72 : pointer[0] * 104 + 52);
      Serial.print(",Y:");
      Serial.println(pointer[1] * 32);
    } else if (key == 3 || key == 2) {
      Serial.println("左右按键触发");
      tft.fillRect(pointer[1] == 4 ? 72 : pointer[0] * 104 + 52, pointer[1] * 32, pointer[1] == 1 || pointer[1] == 3 || pointer[1] == 4 ? 80 : 32, 32, ST77XX_BLACK);
      pointer[0] = key - 2 < 0 ? 0 : key - 2;
    }
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.fillRect(pointer[1] == 4 ? 72 : pointer[0] * 104 + 52, pointer[1] * 32, pointer[1] == 1 || pointer[1] == 3 || pointer[1] == 4 ? 80 : 32, 32, ST77XX_WHITE);
    tft.setTextColor(pointer[0] == 0 && pointer[1] == 0 ? ST77XX_BLACK : ST77XX_ORANGE);
    tft.setCursor(64, 8);
    tft.print("-");
    tft.setTextColor(pointer[0] == 1 && pointer[1] == 0 ? ST77XX_BLACK : ST77XX_ORANGE);
    tft.setCursor(168, 8);
    tft.print("+");
    tft.setTextColor(pointer[0] == 0 && pointer[1] == 1 ? ST77XX_BLACK : ST77XX_ORANGE);
    tft.setCursor(64, 40);
    tft.print("START");
    tft.setTextColor(pointer[0] == 1 && pointer[1] == 1 ? ST77XX_BLACK : ST77XX_ORANGE);
    tft.setCursor(168, 40);
    tft.print("SAVE");
    tft.setTextColor(pointer[0] == 0 && pointer[1] == 2 ? ST77XX_BLACK : ST77XX_GREEN);
    tft.setCursor(64, 72);
    tft.print("-");
    tft.setTextColor(pointer[0] == 1 && pointer[1] == 2 ? ST77XX_BLACK : ST77XX_GREEN);
    tft.setCursor(168, 72);
    tft.print("+");
    tft.setTextColor(pointer[0] == 0 && pointer[1] == 3 ? ST77XX_BLACK : ST77XX_GREEN);
    tft.setCursor(64, 104);
    tft.print("START");
    tft.setTextColor(pointer[0] == 1 && pointer[1] == 3 ? ST77XX_BLACK : ST77XX_GREEN);
    tft.setCursor(168, 104);
    tft.print("SAVE");
    tft.setTextColor(pointer[1] == 5 ? ST77XX_BLACK : ST77XX_RED);
    tft.setCursor(88, 138);
    tft.print("BACK");
  }
}
void Btn_Handle() {
  int Base_Key_Num = analogRead(Control_Btn_Pin);
  if (Base_Key_Num != 4095) {
    delay(10);
    int Confirm_Key_Num = Base_Key_Num - analogRead(Control_Btn_Pin);
    if (abs(Confirm_Key_Num) <= 10) {
      // Serial.println(Base_Key_Num);
      if (Base_Key_Num >= 250 && Base_Key_Num <= 550) {
        //下键
        if (pointer[1] + 1 < menu[correct_menu][1]) {
          setPointer(4);
        }
      } else if (Base_Key_Num <= 150) {
        //右键
        if (pointer[0] + 1 < menu[correct_menu][0]) {
          setPointer(3);
        }
      } else if (Base_Key_Num >= 1800 && Base_Key_Num <= 2100) {
        //左键
        if (pointer[0] - 1 >= 0) {
          setPointer(2);
        }
      } else if (Base_Key_Num >= 1050 && Base_Key_Num <= 1250) {
        //上键
        if (pointer[1] - 1 >= 0) {
          setPointer(1);
        }
      } else if (Base_Key_Num >= 2700 && Base_Key_Num <= 3000) {
        setPointer(5);
      }
    }
  }
}
void EERPOM_WRITE_PH() {
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("EEPROM 初始化失败！");
  } else {
    Serial.println("EEPROM 初始化成功！");
    EEPROM.writeInt(0, (int)NEUTRAL);
    EEPROM.writeInt(4, (int)ALKALIN);
    EEPROM.writeInt(8, (int)ACIDITY);
    EEPROM.commit();
    Serial.print("写入");
    Serial.print(" [");
    Serial.print(sizeof(avgValue));
    Serial.print("] ");
    Serial.print(",数值:");
    Serial.println(avgValue);
    Serial.println("-----------------");
    Serial.print("NEUTRAL:");
    Serial.println(NEUTRAL);
    Serial.print("ALKALIN:");
    Serial.println(ALKALIN);
    Serial.print("ACIDITY:");
    Serial.println(ACIDITY);
    EEPROM.end();
  }
}

void EERPOM_WRITE_MOTOR() {
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("EEPROM 初始化失败！");
  } else {
    Serial.println("EEPROM 初始化成功！");
    EEPROM.writeFloat(12, M1);
    EEPROM.writeFloat(16, M2);
    EEPROM.commit();
    Serial.print("写入");
    Serial.print(" [");
    Serial.print(sizeof(avgValue));
    Serial.print("] ");
    Serial.println("-----------------");
    Serial.print("M1:");
    Serial.println(M1);
    Serial.print("M2:");
    Serial.println(M2);
    EEPROM.end();
  }
}
void PhGeter() {
  if (correct_menu == 1 || correct_menu == 3 || correct_menu == 4) {
    // Serial.println("——————————");
    int buf[10];                  //buffer for read analog
    for (int i = 0; i < 10; i++)  //Get 10 sample value from the sensor for smooth the value
    {
      buf[i] = analogRead(SensorPin);
      if (correct_menu != 4) {
        delay(10);
      }
    }
    for (int i = 0; i < 9; i++)  //sort the analog from small to large
    {
      for (int j = i + 1; j < 10; j++) {
        if (buf[i] > buf[j]) {
          int temp = buf[i];
          buf[i] = buf[j];
          buf[j] = temp;
        }
      }
    }
    avgValue = 0;
    for (int i = 2; i < 8; i++)  //中值平均滤波
      avgValue += buf[i];
    // Serial.println(avgValue);
    phValue = abs((float)avgValue - NEUTRAL) * 5.0 / 4096 / 6;  //本意上是取 模拟输入转电压再转 模拟输入 此处为ΔX，ΔY为PH差
    if (avgValue < NEUTRAL) {
      phValue = ((2.32 / abs((NEUTRAL / 6 * 5 / 4096) - (ALKALIN / 6 * 5 / 4096))) * phValue) + Offset + 6.86;  //碱性
    } else if (avgValue > NEUTRAL) {
      phValue = 6.86 - ((2.86 / abs((NEUTRAL / 6 * 5 / 4096) - (ACIDITY / 6 * 5 / 4096))) * phValue) + Offset;  //酸性
    } else {
      phValue = Offset + 6.86;  //中性
    }
    // Serial.println(phValue);
    if (correct_menu == 3) {
      setViewer(avgValue, 0.5, phValue, 7.5);
    }
  }
}
void AUTO_KH_TEST() {
  Serial.println("开始测量KH！");
  Serial.println("清理缓冲水。。");
  Dirty_Out(30);
  Serial.println("正在注入20mL原缸水");
  Tank_water_IN(20);
  digitalWrite(17, HIGH);
  digitalWrite(16, LOW);
  delay(50000);
  digitalWrite(17, LOW);
  digitalWrite(16, LOW);
  Cycle();
}
void Tank_water_IN(float ml) {
  if (ml == 0) {
    digitalWrite(2, HIGH);
    digitalWrite(4, LOW);
    delay(10000);
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
  } else {
    digitalWrite(2, HIGH);
    digitalWrite(4, LOW);
    int second = (int)(ml / M1Per);
    delay(second);
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
  }
}
void Hcl_water_IN(float open) {
  if (open == 1) {
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
  } else {
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }
}

void Dirty_Out(float ml) {
  digitalWrite(17, LOW);
  digitalWrite(16, HIGH);
  delay(5000);
  digitalWrite(17, LOW);
  digitalWrite(16, LOW);

  digitalWrite(15, HIGH);
  digitalWrite(14, LOW);
  delay((int)(ml / M1Per) + 5000);
  digitalWrite(15, LOW);
  digitalWrite(14, LOW);
}
void Cycle() {
  int COST_TIME = 0;
  PhGeter();
  Serial.print("滴定起始PH为:");
  Serial.println(phValue);
  Serial.print("开始滴定,起始时间为：");
  Serial.println(millis());
  digitalWrite(17, HIGH);
  digitalWrite(16, LOW);
  while (phValue >= 4.5) {
    Hcl_water_IN(1);
    if (phValue >= 5) {
      delay(50);
    } else {
      delay(10);
    }
    Hcl_water_IN(0);
    if (phValue >= 5) {
      delay(1000);
    } else {
      delay(500);
    }
    if (phValue >= 5) {
      COST_TIME += 50;
    } else {
      COST_TIME += 10;
    }
    PhGeter();
    Serial.print("当前PH：");
    Serial.println(phValue);
  }
  Hcl_water_IN(0);
  double COST_HCL_ML = COST_TIME * M2Per;
  Serial.print("滴定结束,截止时间为:");
  Serial.print(millis());
  Serial.print(",共使用 ");
  Serial.print(COST_TIME);
  Serial.print(" ms,共滴定 ");
  Serial.print(COST_HCL_ML);
  Serial.println("mL。");
  Serial.print("测定结束。根据计算得出KH为 [");
  float KH = KH_CALC(COST_HCL_ML);
  Serial.print(KH);
  Serial.println("] ！");
  digitalWrite(17, LOW);
  digitalWrite(16, LOW);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(0, 0);
  tft.setTextSize(4);
  tft.print("KH");
  tft.setCursor(64, 64);
  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(KH);
}

double KH_CALC(double HCL) {
  double KH = (HCL * 0.05 * 50.04 * 1000 / 20) / 17.85;
  return KH;
}