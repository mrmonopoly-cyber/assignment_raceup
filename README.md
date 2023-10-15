# RaceUP Recruitment Assignment

Hello to everyone, this is an optional (but it will obiouvsly make a good impression) task assigned to every candidate for a position in the software department here at RaceUP-e.
You are asked to write an Arduino sketch for the Arduino Uno that reads time data from an RTC device (DS1307 based [datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/DS1307.pdf)) communicating via i2c.
This data then needs to be displayed on an LCD display (HD44780 based [wiki](https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller)) that gets updated each second to show current date and time in the format dd/mm/yy hh:mm:ss.
To let everyone develop and test the code, the assignment is based on the Wokwi simulator that can emulate real hardware on the [browser](https://wokwi.com) ([docs](https://docs.wokwi.com)), or inside VS Code.

### Rules

- Code **ONLY** on the .ino file don't use any other file
- Use the provided ```diagram.json```, otherwise when we test your code it might not work because you used a different pin setup
- Comments that explain what you are doing and your way of thinking are very welcomed
- The only allowed library is ```Wire.h``` for i2c, **NO OTHER INCLUDES ALLOWED**

### Instructions for the VS Code extension (Recommended)

1. Clone the repository ```git clone https://github.com/raceup-electric/assignment```
2. Install the Arduino extension from the VS Code marketplace
3. Install the Wokwi extension (you are asked to login do it, don't worry it is free)
4. Everything is already set up you just need to edit the assignment.ino file
5. To compile the sketch open the command palette (```Ctrl/Cmd+Shift+P ```) and select ```Arduino: Verify```
6. To run the sketch open the command palette (```Ctrl/Cmd+Shift+P ```) and select ```Wokwi: Start Simulator```

### Instructions for the browser

1. Create a new empty Arduino Uno example
2. Copy the content of this repository ```diagram.json``` into the one in the browser
3. Code on the provided .ino file

### How to hand in your work

When you have finished upload your .ino file on this Drive folder ([link](https://drive.google.com/drive/u/2/folders/1QNPZIOmZPI7KaI9Esfqvc2IddM774P5Q)), naming the file as following: ```Name_Surname.ino```.
Don't worry if it is not working completely or at all, just comment the code and hand in your code anyway: we are interested in your approach to problem solving and your line of reasoning.

Good work to everyone, see you at the interwiew!