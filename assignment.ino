#include <Wire.h>

#define rs 8
#define e 9
#define d0 0
#define d1 1
#define d2 2
#define d3 3
#define d4 4
#define d5 5
#define d6 6
#define d7 7

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
void execute_command(byte command);
void send_data(byte command);
void consume_data();
void print_data(byte ele);


#define ARRAY_SIZE 7
byte clock_data[ARRAY_SIZE];

void setup() {
    // put your setup code here, to run once:
    Wire.begin();
    Serial.begin(115200);
    pinMode(rs,OUTPUT);
    pinMode(e,OUTPUT);
    pinMode(d0,OUTPUT);
    pinMode(d1,OUTPUT);
    pinMode(d2,OUTPUT);
    pinMode(d3,OUTPUT);
    pinMode(d4,OUTPUT);
    pinMode(d5,OUTPUT);
    pinMode(d6,OUTPUT);
    pinMode(d7,OUTPUT);

    execute_command(0x01);
    print_data('c');
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

//comandi LCD
void print_data(byte ele)
{
    digitalWrite(rs,HIGH);
    send_data(ele);
    consume_data();
}

void execute_command(byte command)
{
    digitalWrite(rs,LOW);
    send_data(command);
    consume_data();
}

void consume_data(){
    digitalWrite(e,HIGH);
    delay(1);
    digitalWrite(e,LOW);
    delay(1);
}
void send_data(byte command)
{
    digitalWrite(0,(command >> 0) & 0x01 );
    digitalWrite(1,(command >> 1) & 0x01 );
    digitalWrite(2,(command >> 2) & 0x01 );
    digitalWrite(3,(command >> 3) & 0x01 );
    digitalWrite(4,(command >> 4) & 0x01 );
    digitalWrite(5,(command >> 5) & 0x01 );
    digitalWrite(6,(command >> 6) & 0x01 );
    digitalWrite(7,(command >> 7) & 0x01 );
}
