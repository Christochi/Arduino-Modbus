#include <ModbusMaster.h>
#include <LiquidCrystal.h>

ModbusMaster node;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// variables
float DCbus;
float tempData;
uint8_t result;

void setup() {
  lcd.begin(16, 2);
    
  disply();
  
  Serial.begin( 38400, SERIAL_8E1 );

  node.begin( 1, Serial );

  
}

void loop() {
  static uint32_t i;
  //uint8_t result;
  //uint16_t data[6];
  
  i++;
  
  // set word 0 of TX buffer to least-significant word of counter (bits 15..0)
  node.setTransmitBuffer(0, lowWord(i));
  
  // set word 1 of TX buffer to most-significant word of counter (bits 31..16)
  node.setTransmitBuffer(1, highWord(i));

  //FD04 = 64773 + 1 = 64774
  result = node.readInputRegisters( 40119, 1 );

  if (result == node.ku8MBSuccess)
  {
     tempData = node.getResponseBuffer(0);  
  }
   
  displayUpdate();  
}

// call disply function to display updated data
void displayUpdate()
{
  boolean newData = false;

  if ( DCbus != tempData )
  {
     newData = true;
     DCbus = tempData;
  }

  if( newData == true )
  {
     disply();
  }
  
}

// display data on LCD
void disply()
{
  lcd.clear();
  
   String str;

   str = "val= " + String( DCbus );

   lcd.setCursor(0,0);
   lcd.print(str);
   // display value on serial monitor
   Serial.println( str );
}
