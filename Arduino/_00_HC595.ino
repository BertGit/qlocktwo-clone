boolean led_matrix[ROWS][COLUMNS];
boolean minute_array[MINUTE_ARRAY_LENGTH];

void init_HC595()
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  //digitalWrite(clockPin, HIGH);
  
  //init led_matrix to zero
  set_matrix_zero();
}

void set_matrix_zero() {
   for (int r=0; r<ROWS; r++) {
    for (int c=0; c<COLUMNS; c++) {
      led_matrix[r][c] = false;
    }
  }
}

void set_array_zero() {
  for (int i=0; i<MINUTE_ARRAY_LENGTH; i++) {
    minute_array[i] = false;
  }
}

void set_led(int row, int col) {
   if (col >= 0 && col < COLUMNS && row >= 0 && row < ROWS) {
     led_matrix[row][col] = true;
   }
}

void set_minute(int m) {
  minute_array[m] = true;
}

void display_led_matrix() {
  //sets the registers for all LEDs corresponding to led_matrix
  
  //array for all registers
  byte registers[15];
  //set to off-state
  for (int i=0; i<REGISTERS; i++) {
    registers[i] = REGISTER_OFF_STATE;
  }
  
  for (int r=0; r<ROWS; r++) {
    for (int c=0; c<COLUMNS; c++) {
      if (led_matrix[r][c] == true) {
        
        int element;
        if (r % 2 == 0) {
          element = r * COLUMNS + (COLUMNS - 1) - c;
        } else {
          
          element = r * COLUMNS + c;
        }
        int this_register = element / 8;
        byte register_element = 1 << (element % 8);
        registers[this_register] = registers[this_register] ^ register_element;
        
      }
    }
  }
  
  //separate treatment for last register (minutes)
  int min_reg = REGISTERS-1;
  registers[min_reg] = B00000000;
  for (int i=0; i<MINUTE_ARRAY_LENGTH; i++) {
    if (minute_array[i] == true) {
      byte register_element = 1 << i;
      registers[min_reg] ^= register_element;
    }
  }
  
  digitalWrite(latchPin, LOW);
  
  for (int i=REGISTERS-1; i >= 0; i--) {
    setRegister(registers[i]);
  }
  
  digitalWrite(latchPin, HIGH);
}

void setRegister(byte value) {
  shiftOut(dataPin, clockPin, MSBFIRST, value); 
}
//
//void registerWrite(int whichPin, int whichState) {
//// the bits you want to send
//  byte bitsToSend = 0;
//
//  // turn off the output so the pins don't light up
//  // while you're shifting bits:
//  digitalWrite(latchPin, LOW);
//
//  // turn on the next highest bit in bitsToSend:
//  bitWrite(bitsToSend, whichPin, whichState);
//
//  // shift the bits out:
//  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);
//
//    // turn on the output so the LEDs can light up:
//  digitalWrite(latchPin, HIGH);
//
//}
