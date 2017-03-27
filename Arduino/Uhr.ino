#include <Wire.h>
#include <EEPROM.h>

const int ROWS = 10;
const int COLUMNS = 11;
const int MINUTE_ARRAY_LENGTH = 4;
const int REGISTERS = 15;

const int ON = 1;
const int OFF = 0;

const int BUTTON_UP = 2;
const int BUTTON_DOWN = 3;
const int BUTTON_CENTER = 1;

const int AMBIENT_THRESHOLD_DARK = 100;
const int AMBIENT_THRESHOLD_BRIGHT = 70;
const int AMBIENT_MODE_BRIGHT = 0;
const int AMBIENT_MODE_DARK = 1;
const int DARKNESS_DIM_FACTOR = 254;
int ambient_mode = AMBIENT_MODE_BRIGHT;

const int runThroughLEDs = 0;
const int printTime = 0;

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 7;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 9;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 8;

const int ambientLightPin = A0;

const int outputEnable = 6;
float dimFactor = 0;
const float DIM_CHANGE_SPEED = 3.0;

const byte REGISTER_OFF_STATE = B00001111;

//display modes
const int MODE_TIME = 0;
const int MODE_SECONDS = 1;
const int MODE_SET_TIME = 2;
//set time modes
const int MODE_SET_TIME_HOUR = 0;
const int MODE_SET_TIME_MINUTE = 1;
//start in time-mode
int current_mode = MODE_TIME;
int set_time_mode = MODE_SET_TIME_HOUR;

long start_set_time = 0;
const long MILLIS_TO_SET_TIME = 1000;
long start_blink_interval = 0;
int blink_interval_state = ON;
const long BLINK_INTERVAL = 500;

const int KEY_ARRAY_SIZE = 2;
int last_keys[KEY_ARRAY_SIZE];
int active_key = 0;

int current_row = 0;
int current_column = 0;

byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
int set_time_minute, set_time_hour;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  
  init_HC595();
  init_Touch();
  
  analogWrite(outputEnable, (int)dimFactor);
  
  for (int i=0; i < KEY_ARRAY_SIZE; i++) {
    last_keys[i] = 0;
  }
  
  dimFactor = EEPROM.read(0);
  
  //digitalWrite(6, HIGH);
  //setDateDs1307();

  display_led_matrix();
}

void loop()
{
  getDateDs1307();
  switch (current_mode) {
    case MODE_TIME:
      display_time(hour, minute);
      break;
    case MODE_SECONDS:
      display_seconds();
      break;
    case MODE_SET_TIME:
      if (blink_interval_state == ON) {
        if (set_time_mode == MODE_SET_TIME_HOUR) {
          display_time(set_time_hour, (set_time_minute / 5)*5);
        } else if (set_time_mode == MODE_SET_TIME_MINUTE) {
          display_time(set_time_hour, set_time_minute);
        }
        
        if (millis() - start_blink_interval > BLINK_INTERVAL) {
          blink_interval_state = OFF;
          start_blink_interval = millis();
        }
      } else {
        if (set_time_mode == MODE_SET_TIME_HOUR) {
          display_nothing();
        } else if (set_time_mode == MODE_SET_TIME_MINUTE) {
          display_time(set_time_hour, (set_time_minute / 5) * 5);
        }
        if (millis() - start_blink_interval > BLINK_INTERVAL) {
          blink_interval_state = ON;
          start_blink_interval = millis();
        }
      }
      break;
  }
  //read ambient light sensor
  int ambientLightValue = analogRead(ambientLightPin);
  //hysteresis to avoid flickering
  if (ambient_mode == AMBIENT_MODE_BRIGHT && ambientLightValue > AMBIENT_THRESHOLD_DARK) {
    ambient_mode = AMBIENT_MODE_DARK;
  } else if (ambient_mode == AMBIENT_MODE_DARK && ambientLightValue < AMBIENT_THRESHOLD_BRIGHT) {
    ambient_mode = AMBIENT_MODE_BRIGHT;
  }
  
  //shift last_keys array
  for (int i=KEY_ARRAY_SIZE - 1; i > 0; i--) {
    last_keys[i] = last_keys[i-1];
  }
  //update key
  last_keys[0] = getTouchedKey();
  
  boolean key_pressed_flag = true;
  for (int i=1; i<KEY_ARRAY_SIZE; i++) {
    if (last_keys[i] != last_keys[0]) {
      key_pressed_flag = false;
    }
  }
  if (active_key == 0 && key_pressed_flag) {
    //Key has been pressed
    active_key = last_keys[0];
    if (active_key == BUTTON_CENTER) {
      if (current_mode == MODE_TIME) {
        current_mode = MODE_SECONDS;
        start_set_time = millis();
      } else if (current_mode == MODE_SECONDS) {
        current_mode = MODE_TIME;
        start_set_time = millis();
      } else if (current_mode == MODE_SET_TIME) {
        if (set_time_mode == MODE_SET_TIME_HOUR) {
          set_time_mode = MODE_SET_TIME_MINUTE;
        } else if (set_time_mode == MODE_SET_TIME_MINUTE) {
          //set new time
          setDateDs1307(set_time_hour, set_time_minute);
          
          current_mode = MODE_TIME;
          start_set_time = millis();
        }
      }
    }
    if (current_mode == MODE_SET_TIME) {
      if (set_time_mode == MODE_SET_TIME_HOUR) {
        if (active_key == BUTTON_UP) {
          set_time_minute += 5;
          if (set_time_minute > 59) {
            set_time_minute -= 60;
            set_time_hour++;
            if (set_time_hour > 23) {
              set_time_hour = 0;
            }
          }
        } else if (active_key == BUTTON_DOWN) {
          set_time_minute -= 5;
          if (set_time_minute < 0) {
            set_time_minute += 60;
            set_time_hour--;
            if (set_time_hour < 0) {
              set_time_hour = 23;
            }
          }
        }
      } else if (set_time_mode == MODE_SET_TIME_MINUTE) {
        if (active_key == BUTTON_UP) {
          set_time_minute += 1;
          if (set_time_minute % 5 == 0) {
            set_time_minute -= 5;
          }
        } else if (active_key == BUTTON_DOWN) {
          set_time_minute -= 1;
          if (set_time_minute % 5 == 0) {
            set_time_minute += 5;
          }
        }
      }
    }
  }

  if (key_pressed_flag && last_keys[0] == 0) {
    //Key has been released
    int old_key = active_key;
    active_key = 0;
    if ((old_key == BUTTON_DOWN || old_key == BUTTON_UP) && ambient_mode == AMBIENT_MODE_BRIGHT && current_mode != MODE_SET_TIME) {
      EEPROM.write(0, dimFactor);
      //Serial.println("Write dimFactor");
    }
  }
  
  if (active_key == BUTTON_DOWN && current_mode != MODE_SET_TIME) {
    if (dimFactor < 255 - DIM_CHANGE_SPEED) {
      dimFactor += DIM_CHANGE_SPEED;
    } else {
      dimFactor = 255;
    }
  } else if (active_key == BUTTON_UP && current_mode != MODE_SET_TIME) {
    if (dimFactor > 0 + DIM_CHANGE_SPEED) {
      dimFactor -= DIM_CHANGE_SPEED;
    } else {
      dimFactor = 0;
    }
  } else if (active_key == BUTTON_CENTER) {
    if (millis() - start_set_time > MILLIS_TO_SET_TIME && current_mode != MODE_SET_TIME) {
      blink_interval_state = OFF;
      start_blink_interval = millis();
      current_mode = MODE_SET_TIME;
      set_time_mode = MODE_SET_TIME_HOUR;
      set_time_hour = hour;
      set_time_minute = (minute / 5) * 5 + 4;
    }
  }
  if (ambient_mode == AMBIENT_MODE_DARK) {
    //it's dark, so reduce brightness
    analogWrite(outputEnable, DARKNESS_DIM_FACTOR);
  } else if (ambient_mode == AMBIENT_MODE_BRIGHT) {
    //user chooses brightness
    analogWrite(outputEnable, (int)dimFactor);
  }
  //Serial.println(ambientLightValue);
  //Serial.println(last_keys[0]);
  //Serial.println(current_mode);
  //Serial.println(dimFactor);
  
  delay(25);




/************************************
          Other Routines
************************************/






  if (runThroughLEDs == 1) {
    ++current_column;
    if (current_column == COLUMNS) {
      current_row++;
      current_column = 0;
    }
    if (current_row == ROWS) {
      current_row = 0;
    }
    set_matrix_zero();
    set_led(current_row, current_column);
    display_led_matrix();
    delay(100);
  }
  
  if (Serial.available() > 0) {
    // ASCII '0' through '9' characters are
    // represented by the values 48 through 57.
    // so if the user types a number from 0 through 9 in ASCII, 
    // you can subtract 48 to get the actual value:
    int bitToSet = Serial.read() - 48;

  // write to the shift register with the correct bit set high:
    //registerWrite(bitToSet, HIGH);
    
    if (bitToSet == 0) 
    {
      setRegister(B10100101);
    } else if (bitToSet == 1) 
    {
      setRegister(B01011010);
    } else if (bitToSet == 2) 
    {
      setRegister(B10000000);
    } else if (bitToSet == 3) 
    {
      setRegister(B00001111);
    }
    
    Serial.println(bitToSet);
  }
  delay(0);
}






