int pin = 2;
//int index;
char buffer[6];
void setup() {
  // initialize GPIO 2 as an output.
  pinMode(pin, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  if(Serial.available()>0)
  {
    int index;
    index = Serial.readBytesUntil('\n', buffer,5);
    buffer[index]= '\0';
    String status (buffer);
    if (status =="ON")
    {
      digitalWrite(pin, LOW);
    }
    else if (status =="OFF")
    {
      digitalWrite(pin, HIGH);
    }
  }
            // wait for a second
}