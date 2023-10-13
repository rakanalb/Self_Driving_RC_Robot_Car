#define IR_R2 29
#define IR_R1 33
#define IR_M  28
#define IR_L1 31
#define IR_L2 30
#define POT A0
#define LM_EN 2
#define LM_IN1 3
#define LM_IN2 4
#define RM_IN1 5
#define RM_IN2 6
#define RM_EN 7
#define USServo 22
#define SW1 53
#define SW2 52
#define TRIG 34
#define ECHO 35
bool runOnce = false;
#include <Servo.h>
Servo US_servo;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display


int dist, rdist, ldist;
byte pwm;
long int duration;
int cm;
boolean sL2, sL1, sM, sR1, sR2, sw1Val, sw2Val;

byte OUTPINS[] = {LM_EN, LM_IN1, LM_IN2, RM_IN1, RM_IN2, RM_EN, TRIG};
byte INPINS[] = {IR_L2, IR_L1, IR_M, IR_R1, IR_R2, SW1, SW2, ECHO};

void setup() {

  for (int i = 0; i<sizeof(OUTPINS); i++) pinMode(OUTPINS[i], OUTPUT);
  for (int i = 0; i<sizeof(OUTPINS); i++) pinMode(OUTPINS[i], INPUT);
  Serial.begin(9600);
  Serial1.begin(9600);

  US_servo.attach(USServo);
  US_servo.write(95); // setting it up to look forward
  delay(500);

  
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.print("   Rakan's Car  ");
  Serial.begin(9600);
}
char BTcmd;
void loop() 
{
  sw1Val=digitalRead(SW1)
  sw2Val=digitalRead(SW2);
  

  if (!sw1Val && !sw2Val) // switch = 00
  {
    // motor_control(LOW,0,LOW,0);
    lcd.setCursor(0,1);
    lcd.print("BluetoothControl");
    BT_control();
    
  }

  else if (!sw1Val && sw2Val) // switch = 01
  {
    lcd.setCursor(0,1);
    lcd.print("ObstacleAvoidan.");
    path_finder();
  }

  else if (sw1Val && !sw2Val) // switch = 10
  {
    lcd.setCursor(0,1);
    lcd.print("  Line Follower ");
    line_finder();
  }
  
  else // switch = 11
  {
    lcd.setCursor(0,1);
    lcd.print("  Lane Follower ");
    lane_finder();
  }

  
}

void BT_control()
{
  
if(!runOnce){
  motor_control(LOW,0,LOW,0);
  runOnce = true;  
}

  
   if(Serial1.available())
  {

    BTcmd = Serial1.read();
    
  }
  
Serial.println(BTcmd);

  if(BTcmd == 'A' || BTcmd == 'a') // FWD
  {
    motor_control(LOW,255,LOW,255);
  }
  
  else if (BTcmd == 'C' || BTcmd == 'c') // BWD
  {
    motor_control(HIGH,255,HIGH,255);
  }

  else if (BTcmd == 'D' || BTcmd == 'd') // Turn Left
  {
    motor_control(LOW,50,LOW,255);
  }
  
  else if (BTcmd == 'B' || BTcmd == 'b') // Turn Right
  {
    motor_control(LOW,255,LOW,50);
  }

  else if (BTcmd == 'F' || BTcmd == 'f') // U Turn
  {
    motor_control(HIGH,255,LOW,255);
  }

else if (BTcmd == 'G' || BTcmd == 'g') // Stop
  {
    motor_control(LOW,0,LOW,0);
  }
}

void path_finder()
{
  int adc = analogRead(POT); // the range of ADC arduino is from 0 to 1023
  byte spd = map(adc,0,1023,0,255);
  
  US_servo.write(95); // Looking forward
  delay(500);
  dist = get_distance();
  Serial.println(dist);
  if (dist > 150) // go striaght at full speed
  {
    motor_control(LOW,200,LOW,200); // ?
    }

  else if (dist > 20 && dist < 150) 
  {
    pwm = map(dist,21, 149, 80, 220);
    motor_control(LOW, pwm , LOW, pwm);
    }
  else if (dist < 20)
  {
    motor_control(HIGH,180 + dist,HIGH,180 + dist); // Break to STOP ?
    motor_control(LOW,0,LOW,0);
    US_servo.write(50); // look at the RIGHT direction and measure the distance
    delay(500);
    rdist = get_distance();
    US_servo.write(150); // look at the LEFT direction and measure the distance
    delay(650);
    ldist = get_distance();
    US_servo.write(95); // look ahead
    delay(200);

    if ( ldist > rdist + 10) //Rotate ot the LEFT 90 degrees
    {
      motor_control(HIGH,255 ,LOW,255);
      delay(250);
      }
    else if ( rdist > ldist + 10) //Rotate ot the LEFT 90 degrees
    {
      motor_control(LOW,255- dist,HIGH,255- dist);
      delay(250);
      }
    else if ( ldist < 20 && rdist < 20 && (abs(rdist - ldist) < 10)) // U-Turn
    {
      motor_control(LOW,255,HIGH,255);
      delay(1000);
      }
    
    }
}




void line_finder()
{
  int Spd1 = 10;
  int Spd2 = 10;
  int Spd3 = 90; // striaught 
  int Spd4 = 255; //  
  int Spd5 = 255;
  int Spd6 = 255;
  int Bck = 120; // SPEED FOR BACKING
  int Utrn = 2000; // Time to uturn (180 degrees)
  int Turn = 0; // How much time it takes to tunr 90 degrees
  int Sturn = 200; // Slight turn time
  
    
  boolean sR2 = digitalRead(IR_R2);
  boolean sR1 = digitalRead(IR_R1);
  boolean sM = digitalRead(IR_M);
  boolean sL1 = digitalRead(IR_L1);
  boolean sL2 = digitalRead(IR_L2);

  Serial.print(sR2);
  Serial.print(" ");
  Serial.print(sR1);
  Serial.print(" ");
  Serial.print(sM);
  Serial.print(" ");
  Serial.print(sL1);
  Serial.print(" ");
  Serial.println(sL2);

  lcd.setCursor(0,0);


if (!sR2&& !sR1&& !sM&& !sL1&& !sL2) // 00000
{
  motor_control(HIGH,Bck,HIGH,Bck);
  Serial.print("Moving BACK slowly ");
  lcd.print("      00000     ");
  }

else if (!sR2&& !sR1&& !sM&& !sL1&& sL2) // 00001
{
  motor_control(LOW,Spd6,HIGH,Spd3);
  Serial.print("Turning RIGHT ");
  lcd.print("      00001     ");

  }

else if (!sR2&& !sR1&& !sM&& sL1&& sL2) // 00011
{
  motor_control(LOW,Spd6,HIGH,Spd3);
  Serial.print("Also turning RIGHT ");
  lcd.print("      00011     ");

  }

else if (!sR2&& !sR1&& !sM&& sL1&& !sL2) // 00010
{
  motor_control(LOW,Spd5,LOW,Spd2);
  Serial.print("Slight RIGHT ");
  lcd.print("      00010     ");

  }
  
else if (!sR2&& !sR1&& sM&& sL1&& !sL2) // 00110
{
  motor_control(LOW,Spd4,LOW,Spd1);
  Serial.print("Staying STRAIGHT R ");
  lcd.print("      00110     ");
  }

else if (!sR2&& !sR1&& sM&& !sL1&& !sL2) // 00100
{
  motor_control(LOW,Spd6,LOW,Spd6);
  Serial.print("Staying STRAIGHT ");
  lcd.print("      00100     ");
  }
else if (!sR2&& sR1&& sM&& sL1&& !sL2) // 01110
{
  motor_control(LOW,Spd3,LOW,Spd3);
  Serial.print("Staying STRAIGHT ");
  delay(1000);
  
  }

else if (!sR2&& sR1&& sM&& !sL1&& !sL2) // 01100
{
  motor_control(LOW,Spd1,LOW,Spd4);
  Serial.print("Staying STRAIGHT L ");
  lcd.print("      01100     ");
  
  }

else if (!sR2&& sR1&& !sM&& !sL1&& !sL2) // 01000
{
  motor_control(LOW,Spd2,LOW,Spd5);
  Serial.print("Slight Left ");
  lcd.print("      01000     ");

  }
else if (sR2&& sR1&& !sM&& !sL1&& !sL2) // 11000
{
  motor_control(HIGH,Spd3,LOW,Spd6);
  Serial.print("also Turning LEFT ");
  lcd.print("      11000     ");

  
  }
  
else if (sR2&& !sR1&& !sM&& !sL1&& !sL2) // 10000
{
  motor_control(HIGH,Spd3,LOW,Spd6);
  Serial.print("Turning LEFT ");
  lcd.print("      10000     ");

  }


  
else if (sR2&& sR1&& sM&& sL1&& sL2) // 11111
{
  motor_control(LOW,255,HIGH,255);
  Serial.print("U-Turn ");
  lcd.print("      1>v<1     ");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("      2>v<2     ");
  lcd.setCursor(0,0);
  delay(1000);
  
  }


else if (sR2&& sR1&& sM&& !sL1&& !sL2) // 11100
{
  motor_control(HIGH,Spd6,LOW,Spd6);
  Serial.print("Turn left by 90 ");
  lcd.print("      11100     ");
  delay(Turn);
  }

else if (sR2&& sR1&& sM&& sL1&& !sL2) // 11110
{
  motor_control(HIGH,Spd6,LOW,Spd6);
  Serial.print("Turn left by 90 A ");
  lcd.print("      11110     ");
  delay(Turn);
  }

else if (!sR2&& !sR1&& sM&& sL1&& sL2) // 00111
{
  motor_control(LOW,Spd6,HIGH,Spd6);
  Serial.print("Turn RIGHT by 90 ");
  lcd.print("      00111     ");
  delay(Turn);
  }

else if (!sR2&& sR1&& sM&& sL1&& sL2) // 01111
{
  motor_control(LOW,Spd6,HIGH,Spd6);
  Serial.print("Turn RIGHT by 90 A ");
  lcd.print("      01111     ");
  delay(Turn);
  }
  
}

void lane_finder()
{


  
}

void motor_control(bool Ldir, byte Lspd, bool Rdir, byte Rspd) // Low means FWD, High means BWD for Ldir and Rdir
{

   digitalWrite(LM_IN1,Ldir);
   digitalWrite(LM_IN2,!Ldir);
   analogWrite(LM_EN,Lspd);
   digitalWrite(RM_IN1,Rdir);
   digitalWrite(RM_IN2,!Rdir);
   analogWrite(RM_EN,Rspd); 
   
}



int get_distance()
{
  
  digitalWrite(TRIG,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG,LOW);

  duration = pulseIn(ECHO,HIGH);
  cm = duration/58;

  if (cm > 200) cm = 200;
  if (cm < 5) cm = 5;

  return cm;
}