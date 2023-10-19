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

#protocolprotocoldefine RTC_MEM_SIZE 7

enum Week {
    SUNDAY=0,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};

//LCD functions
/*
* tells to the LCD screen to consume the input data 
*/
void pulse()
{
    digitalWrite(en, HIGH);
    delay(1);
    digitalWrite(en, LOW);
}

/*
* send a byte to the LCD without consume it
*/
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

/*
* send a command byte (based on the ISA of the LCD screen) to the screen and tells to the LCD
* screen to consume it
*/
void send_command(byte com)
{
    digitalWrite(rs, LOW);
    digitalWrite(en, LOW);
    apply_message(com);
    pulse();
}

/*
* send a single byte of data, only as data to write,
* to the LCD screen and then tells to the screen to consume it
*/
void send_data(byte data)
{
    digitalWrite(rs, HIGH);
    digitalWrite(en, LOW);
    apply_message(data);
    pulse();
}

/*
* send a string to the LCD screen
*/
void send_string(const char *str)
{
    while(*str)
    {
        send_data(*str);
        str++;
    }
}

/*
* send a num between 0-99 to LCD screen
* it uses the / and % operator
*/
void send_number(int num)
{
    int first = num/10;
    send_data('0' + first);
    send_data('0' + num%10);
}
//RTC functions

/*
 *convert BCD number format to an integer(base 10) number
 * without using / or %
 */
int BCD_to_int(byte seconds)
{
    byte remainder = seconds & 0b1111;
    return ((seconds >> 4)*10) + remainder;
}

/*
* Obtain data from the RTC component using the i2c protocol,
* then write the data obtained, with a better structure, on the screen,
* At the end of execution the cursor of the LCD is positioned at the home cell (starting position)
*/
void print_date()
{
    byte clock_data[RTC_MEM_SIZE];

    Wire.requestFrom(0x68, RTC_MEM_SIZE);
    for(int i=0;Wire.available();++i){
        clock_data[i]=BCD_to_int(Wire.read());
    }

    //print date
    send_number(20); //first part of the year 20xx in the RTC is not present
    int i = RTC_MEM_SIZE -1; //start from the last element in the array, it's easier to write on the LCD
    for(;i>4;i--){
        send_number(clock_data[i]); //year and month
        send_data(':');
    }
    send_number(clock_data[i]);     //number of the day
    i--;
    //print day
    send_command(0x14); //move the cursor one to the left 
    switch (i) //print the corresponding day of the week 
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
    send_command(0xc0); //move the cursor to the second line
    //print hour:minutes:seconds
    for(;i>=1;i--){
        send_number(clock_data[i]); //hours:minutes:
        send_data(':');
    }
    send_number(clock_data[i]); //seconds
    //reset cursor position of LCD to start
    send_command(0x02);
}

void setup() {
    // put your setup code here, to run once:
    Wire.begin(); //enable i2c protocol
    //set the  pins for LCD screen to output mode
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
    
    //power on the screen, clear it, set the cursor to the starting position (0) 
    //disable the render of the cursor in the screen
    send_command(0x0c);
}

void loop() {
    //set the starting register of the RTC to 0x00 starting address of seconds
    Wire.beginTransmission(0x68);
    Wire.write(0x00); 
    Wire.endTransmission();
    print_date();
    delay(1000);
}
