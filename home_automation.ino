int state;
int LED=0;
int x=0; // Initialize LED status variable 
void setup()
{ Serial.begin(9600); // Configure dev machine serial port // baud rate
//Serial1.begin(9600); // Configure baud rate for Bluetooth // connection
pinMode(2, OUTPUT); 
pinMode(3, OUTPUT); }
void loop() 
{ if(Serial.available() > 0)

  { 
    state = Serial.read();
  }
 if (state == '1')
 { 
  digitalWrite(2,HIGH); 
 } 
   else if(state == '0')
  {
    digitalWrite(2,LOW);
  }
  else if(state == '2')
  {
    digitalWrite(3,HIGH);
  }
  else if(state == '3')
  {
    digitalWrite(3,LOW);
  } 
  else if(state == '8')
  {
    digitalWrite(9,HIGH);
  } 
  else if(state == '9')
  {
    digitalWrite(9,LOW);
  } 
}
