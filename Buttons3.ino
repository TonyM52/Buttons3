/* This sketch is intended to be a "clean-up" of Buttons2, which was originally based around 
Bill Perry's variant of "Hello World", which includes his modified library to 
avoid the design flaw that exists in many (including mine) LCD shields.


The sketch originally printed "Hello, World!" on the lcd - it now takes input from the buttons on the LCD shield, and display the effects on the LCD and the serial monitor.

The Arduino pins use the following pinout:  (pin 1 is the pin closest to the edge of the PCB)

  1 - LCD gnd
  2 - VCC (5v)
  3 - Vo Contrast Voltage
  4 - RS Register Select (rs)
  5 - Read/Write
  6 - Enable (en)
  7 - Data 0 (db0) ----
  8 - Data 1 (db1)     |-------- Not used in 4 bit mode
  9 - Data 2 (db2)     |
  10 - Data 3 (db3) ----
  11 - Data 4 (db4)
  12 - Data 5 (db5)
  13 - Data 6 (db6)
  14 - Data 7 (db7)
  15 - Backlight Anode (+5v)
  16 - Backlight Cathode (Gnd)

 ----------------------------------------------------------------------------
LiquidCrystal compability:

Since hd44780 is LiquidCrystal API compatible, most existing LiquidCrystal
sketches should work with hd44780 hd44780_pinIO i/o class once the
includes are changed to use hd44780 and the lcd object constructor is
changed to use the hd44780_pinIO class.  */

      // Call libraries:
      #include <hd44780.h>
      #include <hd44780ioClass/hd44780_pinIO.h> // Arduino pin i/o class header

/*  declare Arduino pins used for LCD functions and the lcd object:

This sketch does not attempt to use LCD backlight control:  
(Bill's notes for using such control have therefore been removed.)

without backlight control:
The parameters used by hd44780_pinIO are the same as those used by
the IDE bundled LiquidCrystal library  */

      // Define LCD pinout:
      const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7;
      hd44780_pinIO lcd(rs, en, db4, db5, db6, db7);

      // Define LCD geometry
      const int LCD_COLS = 16;
      const int LCD_ROWS = 2;

/*  Smiley:  This defines a special "Smiley" character - I pinched this, 
and the related lines to print it, from the tutorial page on createChar().  */

       byte smiley[8] = {B00000, B10001, B00000, B00000, B10001, B01110, B00000,};

      // Define variables for display on LCD:
      String LCDline1;
      String LCDline2;


void LCDprintfunction () {  //Defines the LCDprintfunction to provide a simple means of outputting.
      lcd.clear();
      lcd.print(LCDline1);
      lcd.setCursor(0,1);
      lcd.print(LCDline2);
      return;
}


void setup()
{
  Serial.begin(9600);
  
  /* initialize LCD with number of columns and rows: 
    Note:  begin() will automatically turn on the backlight 
    if backlight control is specified in the lcd object constructor */
  
  lcd.begin(LCD_COLS, LCD_ROWS);

  // if backlight control was specified, the backlight should be on now

  // Print a preliminary message to the LCD
  
      LCDline1 = "Buttons, by Tony";
      LCDline2 = "Wait a mo... ";
      LCDprintfunction();
      delay(1000);
      lcd.createChar(0,smiley); 
      lcd.setCursor (14, 1);
      lcd.write(byte(0));
      delay(1000);
  // end of setup loop
}



/*  Notes regarding the LCD:
  lcd.setCursor - first column is 0, not 1 - 
  similarly first row is 0, not 1!
When added the smiley, note that, again, setCursor assumes 0 for first char, hence char 15 needs 14 in the 
setCursor statement - using 16 will pushe the character off the end of the display! 

The State number initialises at 0 - using non-zero values for the actual states allows me to see that the state has actually changed.

The buttons return the following analogue read values:
  Right     - 0
  Up        - 99
  Down      - 254
  Left      - 407
  Select    - 639
  No press  - 1023
*/


// Define initial variables:

String laststatename;

void loop() {
  // put your main code here, to run repeatedly

      // Define the string variables to represent the state identified from the button that's pressed.
      String statename0 = "empty";
      String statename1 = "Right";
      String statename2 = "Up";
      String statename3 = "Down";
      String statename4 = "Left";
      String statename5 = "Select";
      String statename6 = "No Press";
      String statename = statename0;  //Sets state to 0 at start of every loop cycle.


      //  Read the analog value produced by pressing the button.
      int x= analogRead(0);     
      // Note: with no button pressed, the analogue read returns a value of 1023, 
      // so a statement is needed to catch values over 1000.

      // Determine statename based on analogue read - should match button title:
      if (x<50) {  statename = statename1;}  // - i.e. Right 
      else if (x<200) {  statename = statename2;} // i.e. Up
      else if (x<400) {  statename = statename3;}  //i.e. Down
      else if (x<600) {  statename = statename4;}  // i.e. Left
      else if (x<800) {  statename = statename5;}  // i.e. Select
      else if (x>1000){ statename = statename6;}  // i.e. No Press


      // Output:  Only produced if the state has changed and a button has been pressed.
  if (statename != laststatename && statename != statename6) {  

      //Serial Monitor output:            
      Serial.print ("x = "); Serial.print (x); Serial.print ("  ");
      Serial.println (statename);
    
      // LCD output:
      lcd.clear();
      LCDline1 = ("x = "); 
      LCDline2 = (statename);
      LCDprintfunction();
      lcd.setCursor(7,0); lcd.print(x);
      delay (50);
      }
      //Reemember the current state for the next loop
      laststatename = statename;

  }


  
