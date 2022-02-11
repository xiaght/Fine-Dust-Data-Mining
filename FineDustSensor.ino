//==============================
// 미세먼지, 온습도 관측 프로젝트
//==============================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT11.h>
#include <DS1307RTC.h>
#include <Time.h> 

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

//I2C LCD는 일반적으로 0x27혹은 0x3F입니다
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte temperatureImage[] = {0x04,0x0A,0x0A,0x0A,0x0E,0x1F,0x1F,0x0E};

byte humidityImage[] = {0x04,0x0E,0x0E,0x1F,0x1F,0x1F,0x1F,0x0E};
byte doImage[] = {0x1C,0x14,0x1C,0x00,0x00,0x00,0x00,0x00};

//LCD "m" 이미지
byte microImage[] = {0x11,0x11,0x11,0x13,0x15,0x18,0x10,0x10};

//LCD "3" 이미지
byte threeImage[] = {0x18,0x04,0x18,0x04,0x18,0x00,0x00,0x00};

//=====디지털 핀
const int DHT_PIN = 2;
const int DUST_PIN = 8;
const int BUZZER_PIN = 5;

//=====온습도센서 관련 변수
float humidity = 0;
float temperature = 0;

DHT11 dht11(DHT_PIN);

//=====먼지센서 관련 변수
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000;// 먼지센서의 샘플링시간을 2초로 설정합니다.
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
float dustDensity = 0;
float dustState = 0;
boolean DustCalculate_RUN = false;
boolean DustCalculate_Done = false;
unsigned int buzzer_count = 0;

// 핀 초기화
void initPin() {
  pinMode(DUST_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}
int number=1;
// LCD 초기화
void initLCD() {
  lcd.init();
  lcd.backlight();
  // 그림아이콘을 등록합니다.
  lcd.createChar(0, humidityImage);
  lcd.createChar(1, temperatureImage);
  lcd.createChar(2, doImage);
  lcd.createChar(3, microImage);
  lcd.createChar(4, threeImage);
  lcd.home();
  lcd.print("Loading...");
}

void setup()
{
  Serial.begin(9600);
  initPin();
  initLCD();
  starttime = millis();
//  Serial.println("CLEARDATA");
//  Serial.println("LABEL,number,humidity,temperature,dustDensity");
  //
  setSyncProvider(RTC.get); 
//  if(timeStatus()!= timeSet)
  //   Serial.println("Unable to sync with the RTC");
 // else
  //   Serial.println("RTC has set the system time");  
}

void loop()
{
  

  //부저가 다 울린 뒤에 미세먼지 측정 시작!
  //미세먼지 측정 중에는 부저와 시작시간 초기화 안되도록 함.
  if(DustCalculate_RUN == true)
  {
    calcDustDensity();

    //미세먼지 측정 후에 온습도 측정 / LCD 표시
    if(DustCalculate_Done == true) {
      calcHumidityAndTemperature();
      printLCD();
      DustCalculate_Done = false;
    }
  } else {
    /*
      국제 미세먼지농도에 따른 경계단계 기준분류 
      30ug/m^3 이하 : 좋음 / 30~80ug/m^3 : 보통 / 80~150ug/m^3 : 나쁨 / 150ug/m^3 초과 : 매우 나쁨
      
      먼지농도 경계 단계에 따라 LCD 표시내용이 달라지며, 
      부저가 각각 보통 1번 / 나쁨 2번 / 매우 나쁨 3번 연속으로 울리도록 되어있습니다.
    */
    
    if (buzzer_count > 0 ) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      delay(200);
      buzzer_count--;
    } else digitalWrite(BUZZER_PIN, LOW);


    
    
    //시작시간 초기화
    if((dustState > 0 && buzzer_count == 0) || (dustState == 0)) 
    {
      DustCalculate_RUN = true;
      starttime = millis();
    }
  }
  delay(1000);
  
  Serial.print(temperature); 
   Serial.print(" ");
  Serial.print(humidity); 
 
  Serial.print(" ");
 // Serial.print(dustDensity);    
  Serial.println();
  
}





void digitalClockDisplay(){
  Serial.print(year());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
 

 

 
  Serial.println();
}
 
void printDigits(int digits){
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}



void printLCD() {
    //LCD에 먼지센서와 온습도센서를 출력합니다.
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(dustDensity);
    lcd.write(3);
    lcd.print("g/m");
    lcd.write(4);
    lcd.setCursor(10, 0);
    if(dustState == 0)lcd.print(" (^_^)");
    else if(dustState == 1)lcd.print(" (o_o)");
    else if(dustState == 2)lcd.print(" (T.T)");
    else if(dustState == 3)lcd.print(" (ToT)");
    
    lcd.setCursor(0, 1);
    lcd.write(0);
    lcd.print(" ");
    lcd.print(humidity);
    lcd.print("% ");
    lcd.write(1);
    lcd.print(" ");
    lcd.print(temperature);
    lcd.write(2);
    lcd.print("C ");
}

/** 신뢰할 수 있는 먼지밀도 계산하기
   대부분의 아날로그센서의 경우 값이 튀는 현상이 있는데, 
   이것을 보정하기 위해 여러번 값을 누적한 후, 
   평균값을 내어 신뢰할 수 있는 먼지밀도를 구합니다.
*/
void calcDustDensity() {
  duration = pulseIn(DUST_PIN, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;
    
  if ((millis() - starttime) > sampletime_ms) {
    DustCalculate_RUN = false;
    DustCalculate_Done = true;

    ratio = lowpulseoccupancy / (sampletime_ms * 10.0); // Integer percentage 0=>100
    concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve
    dustDensity = concentration * 100 / 13000;
    lowpulseoccupancy = 0;
    
    if(dustDensity > 150) buzzer_count = 3;
    else if(dustDensity > 80) buzzer_count = 2;
    else if(dustDensity > 30) buzzer_count = 1;
    else buzzer_count = 0;
    
    dustState = buzzer_count;
  }
}

/** 습도,온도 계산
   DHT온습도센서를 이용해서 온도와 습도를 계산합니다.
*/
void calcHumidityAndTemperature() {
  dht11.read(humidity, temperature);
 
}
