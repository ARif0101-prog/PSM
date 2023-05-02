int greenLED = 13;   // assign pin 2 to green LED
char buffer[6];
int index;

void setup() {
  pinMode(greenLED, OUTPUT);   // set green LED pin as output

  digitalWrite(greenLED, LOW);   // turn off green LED initially

  
  Serial.begin(9600);   // initialize serial communication with baud rate 9600
}

void loop() {
  
    if (Serial.available() > 0) {        // check if any data is available on serial port
    //int input = Serial.parseInt();     // read the integer input from the serial port
      index = Serial.readBytesUntil('\n', buffer,5);
   buffer[index] = '\0';
  // int input = atoi(buffer);

      String status(buffer);
      // read the integer input from the serial port
    Serial.println(status);
    if (status == "ON") {       // replace XXX with your last three digits
      digitalWrite(greenLED, HIGH);     // turn on green LED
     // turn off yellow LED
    }
    else if (status == "OFF") { // replace XXX with your last three digits
      digitalWrite(greenLED, LOW);      // turn off green LED

    }
    else {                               // if invalid input is given

    }
  }
}
