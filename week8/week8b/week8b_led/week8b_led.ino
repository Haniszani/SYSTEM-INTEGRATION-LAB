
char junk;
String inputString="";

void setup()                    
{
 Serial.begin(9600);            
 pinMode(13, OUTPUT);
}

void loop()
{
  if(Serial.available()){
  while(Serial.available())
    {
      char inChar = (char)Serial.read(); //read the input
      inputString += inChar;        
    }
    Serial.println(inputString);
    while (Serial.available() > 0)  
    { junk = Serial.read() ; }      // clear the serial buffer
    if(inputString == "A"){         //in case of 'A' turn the LED on
      digitalWrite(13, HIGH);  
    }else if(inputString == "B"){   //incase of 'B' turn the LED off
      digitalWrite(13, LOW);
    }
    inputString = "";
  }
}