#include <Wire.h>

#define d0 0
#define d1 1
#define d2 2
#define d3 3
#define d4 4
#define d5 5
#define d6 6
#define d7 7
#define rs 8
#define en 9

#define RTC_MEM_SIZE 7

enum Week {
    SUNDAY=0,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};

byte clock_data[RTC_MEM_SIZE];

//LCD functions
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

void send_number(int num)
{
    int first = num/10;
    send_data('0' + first);
    send_data('0' + num%10);
}
//RTC functions

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
    //print date
    int i = RTC_MEM_SIZE -1;
    for(;i>4;i--){
        send_number(clock_data[i]);
        send_data(':');
    }
    send_number(clock_data[i]);
    i--;
    //print day
    send_command(0x14);
    switch (i)
    {
        case MONDAY:
            send_string("Mon");
            break;
        case TUESDAY:
            send_string("Tue");
            break;
        case WEDNESDAY:
            send_string("Wed");
            break;
        case THURSDAY:
            send_string("Thur");
            break;
        case FRIDAY:
            send_string("Fri");
            break;
        case SATURDAY:
            send_string("Sat");
            break;
        case SUNDAY:
            send_string("Sun");
            break;
    }
    i--;
    //print hour:minutes:seconds
    send_command(0xc0);
    for(;i>=1;i--){
        send_number(clock_data[i]);
        send_data(':');
    }
    send_number(clock_data[i]);
    //reset cursor position of LCD to start
    send_command(0x02);


}

void setup() {
    // put your setup code here, to run once:
    Wire.begin();
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

    send_command(0x0c);
    //send_string("HELLO WORLD");
}

void loop() {
    // put your main code here, to run repeatedly:

    Wire.beginTransmission(0x68);
    Wire.write(0x00); // Indirizzo del registro dei secondi
    Wire.endTransmission();

    Wire.requestFrom(0x68, RTC_MEM_SIZE);
    for(int i=0;Wire.available();++i){
        clock_data[i]=BCD_to_int(Wire.read());
    }

    print_array();
    delay(1000);
}
