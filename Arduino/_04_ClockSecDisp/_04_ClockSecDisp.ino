void display_seconds() {
  //set matrix to zero
  set_matrix_zero();
  set_array_zero();
  
  if (second < 10) {
    display_digit(second, 5);
  } else {
    int digit_1 = second / 10;
    int digit_2 = second % 10;
    display_digit(digit_1, 2);
    display_digit(digit_2, 8);
  }
  
  display_led_matrix();
}

void display_digit(int digit, int x_pos) {
  if (digit == 0) {
    set_zero_sec(x_pos);
  } else if (digit == 1) {
    set_one_sec(x_pos);
  } else if (digit == 2) {
    set_two_sec(x_pos);
  } else if (digit == 3) {
    set_three_sec(x_pos);
  } else if (digit == 4) {
    set_four_sec(x_pos);
  } else if (digit == 5) {
    set_five_sec(x_pos);
  } else if (digit == 6) {
    set_six_sec(x_pos);
  } else if (digit == 7) {
    set_seven_sec(x_pos);
  } else if (digit == 8) {
    set_eight_sec(x_pos);
  } else if (digit == 9) {
    set_nine_sec(x_pos);
  }
}

void set_zero_sec(int x_pos) {
  set_led(2,x_pos-1); set_led(2,x_pos); set_led(2,x_pos+1);
  for (int i=3; i<=7;i++) {
    set_led(i,x_pos-2);
    set_led(i,x_pos+2);
  }
  set_led(8,x_pos-1); set_led(8,x_pos); set_led(8,x_pos+1);
  set_led(4,x_pos+1); set_led(5,x_pos); set_led(6,x_pos-1);
}
void set_one_sec(int x_pos) {
  for (int i=2; i<=8; i++) {
    set_led(i,x_pos);
  }
  set_led(3,x_pos-1);
  set_led(8,x_pos-1); set_led(8,x_pos+1);
}
void set_two_sec(int x_pos) {
  set_led(3,x_pos-2);
  set_led(2,x_pos-1); set_led(2,x_pos); set_led(2,x_pos+1);
  set_led(3,x_pos+2);
  set_led(4,x_pos+2);
  set_led(5,x_pos+1);
  set_led(6,x_pos);
  set_led(7,x_pos-1);
  for (int i=x_pos-2; i<=x_pos+2; i++) {
    set_led(8,i);
  }
}
void set_three_sec(int x_pos) {
  for (int i=x_pos-2; i<=x_pos+2; i++) {
    set_led(2,i);
  }
  set_led(3,x_pos+1);
  set_led(4,x_pos);
  set_led(5,x_pos+1);
  set_led(6,x_pos+2);
  set_led(7,x_pos-2); set_led(7,x_pos+2);
  set_led(8,x_pos-1); set_led(8,x_pos); set_led(8,x_pos+1);
}
void set_four_sec(int x_pos) {
  for (int i=2; i<=8; i++) {
    set_led(i,x_pos+1);
  }
  for (int i=x_pos-2; i<=x_pos+2; i++) {
    set_led(6,i);
  }
  set_led(3,x_pos);
  set_led(4,x_pos-1);
  set_led(5,x_pos-2);
}
void set_five_sec(int x_pos) {
  for (int i=x_pos-2; i<=x_pos+2; i++) {
    set_led(2,i);
  }
  set_led(3,x_pos-2);
  for (int i=x_pos-2; i<=x_pos+1; i++) {
    set_led(4,i);
  }
  set_led(5,x_pos+2);
  set_led(6,x_pos+2);
  set_led(7,x_pos-2); set_led(7,x_pos+2);
  set_led(8,x_pos-1); set_led(8,x_pos); set_led(8,x_pos+1);
}
void set_six_sec(int x_pos) {
  set_led(2,x_pos); set_led(2,x_pos+1);
  set_led(3,x_pos-1);
  set_led(4,x_pos-2);
  for (int i=x_pos-2; i<=x_pos+1; i++) {
    set_led(5,i);
  }
  set_led(6,x_pos-2); set_led(6,x_pos+2);
  set_led(7,x_pos-2); set_led(7,x_pos+2);
  set_led(8,x_pos-1); set_led(8,x_pos); set_led(8,x_pos+1);
}
void set_seven_sec(int x_pos) {
  for (int i=x_pos-2; i<=x_pos+2; i++) {
    set_led(2,i);
  }
  set_led(3,x_pos+2);
  set_led(4,x_pos+1);
  set_led(5,x_pos);
  set_led(6,x_pos-1);
  set_led(7,x_pos-1);
  set_led(8,x_pos-1);
}
void set_eight_sec(int x_pos) {
  set_led(2,x_pos-1); set_led(2,x_pos); set_led(2,x_pos+1);
  set_led(5,x_pos-1); set_led(5,x_pos); set_led(5,x_pos+1);
  set_led(8,x_pos-1); set_led(8,x_pos); set_led(8,x_pos+1);
  set_led(3,x_pos-2); set_led(3,x_pos+2);
  set_led(4,x_pos-2); set_led(4,x_pos+2);
  set_led(6,x_pos-2); set_led(6,x_pos+2);
  set_led(7,x_pos-2); set_led(7,x_pos+2);
}
void set_nine_sec(int x_pos) {
  set_led(8,x_pos); set_led(8,x_pos-1);
  set_led(7,x_pos+1);
  set_led(6,x_pos+2);
  for (int i=x_pos-1; i<=x_pos+2; i++) {
    set_led(5,i);
  }
  set_led(4,x_pos-2); set_led(4,x_pos+2);
  set_led(3,x_pos-2); set_led(3,x_pos+2);
  set_led(2,x_pos-1); set_led(2,x_pos); set_led(2,x_pos+1);
}
