void display_nothing() {
  set_matrix_zero();
  set_array_zero();
  display_led_matrix();
}

void display_time(byte hour, byte minute) {
  //set matrix to zero
  set_matrix_zero();
  set_array_zero();
  
  set_it();
  set_is();
  if (minute < 5) {
    set_oclock();
  } else if (minute < 10) {
    set_five_minute();
  } else if (minute < 15) {
    set_ten_minute();
  } else if (minute < 20) {
    set_quarter();
  } else if (minute < 25) {
    set_twenty();
  } else if (minute < 30) {
    set_twenty();
    set_five_minute();
  } else if (minute < 35) {
    set_half();
  } else if (minute < 40) {
    set_twenty();
    set_five_minute();
  } else if (minute < 45) {
    set_twenty();
  } else if (minute < 50) {
    set_quarter();
  } else if (minute < 55) {
    set_ten_minute();
  } else if (minute < 60) {
    set_five_minute();
  }
  
  int this_hour = hour;
  if (minute >= 5 && minute < 35) {
    set_past();
  } else if (minute >= 35) {
    set_to();
    this_hour++;
  }
  
  if (this_hour > 23) {
    this_hour = 0; 
  } else if (this_hour > 12) {
    this_hour -= 12;
  }
  
  if (this_hour == 0) {
    set_twelve();
  } else if (this_hour == 1) {
    set_one();
  } else if (this_hour == 2) {
    set_two();
  } else if (this_hour == 3) {
    set_three();
  } else if (this_hour == 4) {
    set_four();
  } else if (this_hour == 5) {
    set_five();
  } else if (this_hour == 6) {
    set_six();
  } else if (this_hour == 7) {
    set_seven();
  } else if (this_hour == 8) {
    set_eight();
  } else if (this_hour == 9) {
    set_nine();
  } else if (this_hour == 10) {
    set_ten();
  } else if (this_hour == 11) {
    set_eleven();
  } else if (this_hour == 12) {
    set_twelve();
  }
  
  //set minutes in bottom led array
  for (int i=0; i<(minute % 5); i++) {
    set_minute(i);
  }
  
  display_led_matrix();
}


void set_it() {
  set_led(0,0); set_led(0,1);
}
void set_is() {
  set_led(0,3); set_led(0,4);
}
void set_quarter() {
  for (int i=2; i<=8; i++) {
   set_led(1,i);
  }
}
void set_twenty() {
  for (int i=0; i<=5; i++) {
   set_led(2,i);
  }
}
void set_five_minute() {
  for (int i=6; i<=9; i++) {
   set_led(2,i);
  }
}
void set_half() {
  for (int i=0; i<=3; i++) {
   set_led(3,i);
  }
}
void set_ten_minute() {
  for (int i=5; i<=7; i++) {
   set_led(3,i);
  }
}
void set_to() {
  set_led(3,9); set_led(3,10);
}
void set_past() {
  for (int i=0; i<=3; i++) {
   set_led(4,i);
  }
}
void set_nine() {
  for (int i=7; i<=10; i++) {
   set_led(4,i);
  }
}
void set_one() {
  for (int i=0; i<=2; i++) {
   set_led(5,i);
  }
}
void set_six() {
  for (int i=3; i<=5; i++) {
   set_led(5,i);
  }
}
void set_three() {
  for (int i=6; i<=10; i++) {
   set_led(5,i);
  }
}
void set_four() {
  for (int i=0; i<=3; i++) {
   set_led(6,i);
  }
}
void set_five() {
  for (int i=4; i<=7; i++) {
   set_led(6,i);
  }
}
void set_two() {
  for (int i=8; i<=10; i++) {
   set_led(6,i);
  }
}
void set_eight() {
  for (int i=0; i<=4; i++) {
   set_led(7,i);
  }
}
void set_eleven() {
  for (int i=5; i<=10; i++) {
   set_led(7,i);
  }
}
void set_seven() {
  for (int i=0; i<=4; i++) {
   set_led(8,i);
  }
}
void set_twelve() {
  for (int i=5; i<=10; i++) {
   set_led(8,i);
  }
}
void set_ten() {
  for (int i=0; i<=2; i++) {
   set_led(9,i);
  }
}
void set_oclock() {
  for (int i=5; i<=10; i++) {
   set_led(9,i);
  }
}
