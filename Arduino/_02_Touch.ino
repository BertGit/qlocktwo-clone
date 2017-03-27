void init_Touch() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
}

int getTouchedKey() {
  for (int i = 2; i < 6; i++) {
    if (digitalRead(i) == 1) {
      return i - 1;
    }
  }
  return 0;
}
