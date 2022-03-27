/*#include <Arduino.h>

#include "motor.h"
MOTOR_DERIVER motor(50);


#include "SGP30.h"

uint16_t CO2Data,TVOCData;//定义CO2浓度变量与TVOC浓度变量
uint32_t sgp30_dat;
SGP mySGP30;

#include <WiFiClient.h>

#include "Adafruit_CCS811.h"
Adafruit_CCS811 ccs;

#include<Wire.h> 
#include<SPI.h>

uint16_t CO2;
uint16_t TVOC;


#include <SimpleDHT.h>
int pinDHT11 = 13;
SimpleDHT11 dht11(pinDHT11);

#define OLED_CLK_PIN 22
#define OLED_SDA_PIN 21
# include<U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,  /*SCL*/  OLED_CLK_PIN,  /*SDA*/  OLED_SDA_PIN,   /*reset*/  U8X8_PIN_NONE);//构造
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

/*#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
char Bt_CHAR;


//NTP服务器
static const char ntpServerName[] = "ntp6.aliyun.com";
const int timeZone = 8;     //东八区

#include <WiFiUdp.h>
WiFiUDP Udp;


#include <WiFi.h>
#include <WiFiClient.h>
#include <TimeLib.h>
time_t getNtpTime();


const int NTP_PACKET_SIZE = 48; // NTP时间在消息的前48字节中
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

// 向NTP服务器发送请求
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

int lightValue=0,backLight_hour=0;
//星期
String week(){
  String wk[7] = {"日","一","二","三","四","五","六"};
  String s = "周" + wk[weekday()-1];
  return s;
}



//月日
String monthDay(){
  String s = String(month());
  s = s + "月" + day() + "日";
  return s;
}
//时分
String hourMinute(){
  String s = num2str(hour());
  backLight_hour = s.toInt();
  s = s + ":" + num2str(minute());
  return s;
}

String num2str(int digits)
{ 
  String s = "";
  if (digits < 10)
    s = s + "0";
  s = s + digits;
  return s;
}



time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  //Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  //Serial.print(ntpServerName);
  //Serial.print(": ");
  //Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("NTP同步成功");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      //Serial.println(secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR);
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  //ESP.restart(); //时间获取失败直接重启
  Serial.println("NTP同步失败");
  return 0; // 无法获取时间时返回0
}

void drawTime()
{
  //绘制时间
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
 // u8g2.drawStr(0,20,&(num2str(hour())));	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);  
}

void CCS811_init(void){
  Serial.println("CCS811 test");

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  // Wait for the sensor to be ready
  while(!ccs.available());
}

uint16_t CCS811_DATA(void){
  if(ccs.available()){
    if(!ccs.readData()){
      Serial.print("CO2: ");
      CO2=ccs.geteCO2();
      Serial.print(CO2);


      Serial.print("ppm, TVOC: ");
      TVOC = ccs.getTVOC();
      Serial.println(TVOC);
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  delay(500);
}


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

  u8g2.begin();
  //CCS811_init();

  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  

}

void loop() {
  // put your main code here, to run repeatedly:
  //motor.move_forward();
  //SGP30_GET_DATA();
  //CCS811_DATA();
  //Serial.println("ERROR!");

  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  
  
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
    Bt_CHAR=SerialBT.read();
    
    if(Bt_CHAR== *"BT_UP") motor.move_forward();
    if(Bt_CHAR== *"BT_DOWM") motor.move_back();
    
  }
  delay(20);
  
  /*u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  u8g2.drawStr(0,20,"Hello World!");	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);  

  //DHT11_DATA();
  delay(1000);  */

}

