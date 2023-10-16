#include <Wire.h>

#define rs 8
#define en 9
#define d0 0
#define d1 1
#define d2 2
#define d3 3
#define d4 4
#define d5 5
#define d6 6
#define d7 7

#define ARRAY_SIZE 7
byte clock_data[ARRAY_SIZE];

enum Week {
    SUNDAY=0,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};

void print_array();
int BCD_to_int(byte seconds);

void pulse()
{
  digitalWrite(en, HIGH);
  delay(1);
  digitalWrite(en, LOW);
}

void apply_message(byte val)
{
  digitalWrite(d0,(val >> 0) & 0x01);
  digitalWrite(d1,(val >> 1) & 0x01);
  digitalWrite(d2,(val >> 2) & 0x01);
  digitalWrite(d3,(val >> 3) & 0x01);
  digitalWrite(d4,(val >> 4) & 0x01);
  digitalWrite(d5,(val >> 5) & 0x01);
  digitalWrite(d6,(val >> 6) & 0x01);
  digitalWrite(d7,(val >> 7) & 0x01);
}

void send_command(byte com)
{
  digitalWrite(rs, LOW);
  digitalWrite(en, LOW);
  apply_message(com);
  pulse();
}

void send_data(byte data)
{
  digitalWrite(rs, HIGH);
  digitalWrite(en, LOW);
  apply_message(data);
  pulse();
}

void send_string(const char *str)
{
  while(*str)
  {
    send_data(*str);
    str++;
  }
}

void setup() {
    // put your setup code here, to run once:
    Wire.begin();
    Serial.begin(115200);
    pinMode(rs,OUTPUT);
    pinMode(en,OUTPUT);
    pinMode(d0,OUTPUT);
    pinMode(d1,OUTPUT);
    pinMode(d2,OUTPUT);
    pinMode(d3,OUTPUT);
    pinMode(d4,OUTPUT);
    pinMode(d5,OUTPUT);
    pinMode(d6,OUTPUT);
    pinMode(d7,OUTPUT);
       
    send_command(0x0f);
    send_data(126);
    send_data(127);
}

void loop() {
    // put your main code here, to run repeatedly:

    Wire.beginTransmission(0x68);
    Wire.write(0x00); // Indirizzo del registro dei secondi
    Wire.endTransmission();

    Wire.requestFrom(0x68, ARRAY_SIZE);
    for(int i=0;Wire.available();++i){
        clock_data[i]=BCD_to_int(Wire.read());
    }

    print_array();
    delay(1000);
}

/*
 *convert BCD number format to an integer(base 10) number
 */
int BCD_to_int(byte seconds)
{
    byte rest = seconds & 0b1111;
    return ((seconds >> 4)*10) + rest;
}

void print_array()
{
    int i = ARRAY_SIZE -1;
    for(;i>3;i--){
        Serial.print(clock_data[i]);
        Serial.print(":");
    }
    switch (i)
    {
        case MONDAY:
            Serial.print("Monday");
            break;
        case TUESDAY:
            Serial.print("Tuesday");
            break;
        case WEDNESDAY:
            Serial.print("Wednesday");
            break;
        case THURSDAY:
            Serial.print("Thursday");
            break;
        case FRIDAY:
            Serial.print("Friday");
            break;
        case SATURDAY:
            Serial.print("Saturday");
            break;
        case SUNDAY:
            Serial.print("Sunday");
            break;
    }
    i--;
    Serial.println();
    for(;i>=0;i--){
        Serial.print(clock_data[i]);
        Serial.print(":");
    }
    Serial.println();
}
