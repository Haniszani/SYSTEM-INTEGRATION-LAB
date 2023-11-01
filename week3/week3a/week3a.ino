int x;  

void setup() {  

  Serial.begin(9600);   

  pinMode(9, OUTPUT); 

}  

void loop() {  

  int potValue = analogRead(A0); 

  Serial.println(potValue); 

    

   

  if (potValue > 200) { 

    digitalWrite(9, HIGH); 

  } else { 

    digitalWrite(9, LOW); 

    }  

  delay(1000); 

} 
