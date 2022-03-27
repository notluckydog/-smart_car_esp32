#include <Arduino.h>

#include "motor.h"
MOTOR_DERIVER motor(50);


#include "SGP30.h"

uint16_t CO2Data,TVOCData;//定义CO2浓度变量与TVOC浓度变量
uint32_t sgp30_dat;
SGP mySGP30;

/*#include <WiFiClient.h>

#include "Adafruit_CCS811.h"
Adafruit_CCS811 ccs;

#include<Wire.h> 
#include<SPI.h>*/

uint16_t CO2;
uint16_t TVOC;

/*#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 13
DHT dht(DHTPIN, DHTTYPE);*/

#include <SimpleDHT.h>
int pinDHT11 = 13;
SimpleDHT11 dht11(pinDHT11);

#define OLED_CLK_PIN 22
#define OLED_SDA_PIN 21
# include<U8g2lib.h>
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,  /*SCL*/  OLED_CLK_PIN,  /*SDA*/  OLED_SDA_PIN,   /*reset*/  U8X8_PIN_NONE);//构造
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
uint8_t Bt_CHAR;


//NTP服务器
static const char ntpServerName[] = "ntp6.aliyun.com";
const int timeZone = 8;     //东八区




void SGP30_GET_DATA_init(void){
  
  //模块LDO已经被击穿，无法正常工作
  mySGP30.SGP30_Init();
  mySGP30.SGP30_Write(0x20,0x08);
  sgp30_dat = mySGP30.SGP30_Read();//读取SGP30的值
  CO2Data = (sgp30_dat & 0xffff0000) >> 16;
  TVOCData = sgp30_dat & 0x0000ffff; 
  //SGP30模块开机需要一定时间初始化，在初始化阶段读取的CO2浓度为400ppm，TVOC为0ppd且恒定不变，因此上电后每隔一段时间读取一次
  //SGP30模块的值，如果CO2浓度为400ppm，TVOC为0ppd，发送“正在检测中...”，直到SGP30模块初始化完成。
  //初始化完成后刚开始读出数据会波动比较大，属于正常现象，一段时间后会逐渐趋于稳定。
  //气体类传感器比较容易受环境影响，测量数据出现波动是正常的，可自行添加滤波函数。
  while(CO2Data == 400 && TVOCData == 0)
  {
    mySGP30.SGP30_Write(0x20,0x08);
    sgp30_dat = mySGP30.SGP30_Read();//读取SGP30的值
    CO2Data = (sgp30_dat & 0xffff0000) >> 16;//取出CO2浓度值
    TVOCData = sgp30_dat & 0x0000ffff;       //取出TVOC值
    Serial.println("正在检测中...");
    delay(500);
  }

}

uint16_t SGP30_GET_DATA(void){
  mySGP30.SGP30_Write(0x20,0x08);
  sgp30_dat = mySGP30.SGP30_Read();//读取SGP30的值
  CO2Data = (sgp30_dat & 0xffff0000) >> 16;//取出CO2浓度值
  TVOCData = sgp30_dat & 0x0000ffff;       //取出TVOC值
  Serial.print("CO2:");
  Serial.print(CO2Data,DEC);
  Serial.println("ppm");
  Serial.print("TVOC:");
  Serial.print(TVOCData,DEC);
  Serial.println("ppd");
  delay(500);    //延时
}

void DHT11_DATA(){
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");
  
  // DHT11 sampling rate is 1HZ.
  delay(1500);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //SGP30_GET_DATA_init();

  delay(3000);
  Serial.println("just_test");
  Serial.println(F("DHTxx test!"));

  /*u8g2.begin();
  //CCS811_init();*/

  SerialBT.begin("ESP32"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  

}

void loop() {
 

  
  
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  
  //motor.move_forward();
  //Serial.println("main start ");
  delay(1000);
  
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
    Bt_CHAR=SerialBT.read();
    Serial.println("the keyvalue is:");
    Serial.println(Bt_CHAR);
    if(SerialBT.read()== *"1") motor.move_forward();
    if(SerialBT.read()== *"2") motor.move_back();
    
  }
 
  
  /*u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  u8g2.drawStr(0,20,"Hello World!");	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);  

  //DHT11_DATA();
  delay(1000);  */

}