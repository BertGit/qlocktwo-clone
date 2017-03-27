byte test;
byte zero;
char  *Day[] = {"","Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
char  *Mon[] = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
 
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

void setDateDs1307(int hour, int minute)                
{
 
   second = 0;
   //minute = 59;
   //hour  = 14;
   dayOfWeek = 1;
   dayOfMonth = 5;
   month = 2;
   year= 13;
   Wire.beginTransmission(0x68);
   Wire.write((byte)0);
   Wire.write(decToBcd(second) & 0x7f);    // 0 to bit 7 starts the clock
   Wire.write(decToBcd(minute));
   Wire.write(decToBcd(hour));      // If you want 12 hour am/pm you need to set
                                   // bit 6 (also need to change readDateDs1307)
   Wire.write(decToBcd(dayOfWeek));
   Wire.write(decToBcd(dayOfMonth));
   Wire.write(decToBcd(month));
   Wire.write(decToBcd(year));
   Wire.endTransmission();
}

void getDateDs1307()
{
  // Reset the register pointer
  Wire.beginTransmission(0x68);
  Wire.write((byte)0);
  Wire.endTransmission();
 
  Wire.requestFrom(0x68, 7);
 
  // A few of these need masks because certain bits are control bits
  second     = bcdToDec(Wire.read() & 0x7f);
  minute     = bcdToDec(Wire.read());
  hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  dayOfWeek  = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month      = bcdToDec(Wire.read());
  year       = bcdToDec(Wire.read());
 
 if (printTime) {
  if (hour < 10)
    Serial.print("0");
  Serial.print(hour, DEC);
  Serial.print(":");
  if (minute < 10)
    Serial.print("0");
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second < 10)
    Serial.print("0");
  Serial.print(second, DEC);
  Serial.print("  ");
  Serial.print(Day[dayOfWeek]);
  Serial.print(", ");
  Serial.print(dayOfMonth, DEC);
  Serial.print(" ");
  Serial.print(Mon[month]);
  Serial.print(" 20");
  if (year < 10)
    Serial.print("0");
  Serial.println(year, DEC);
 }
 
}
