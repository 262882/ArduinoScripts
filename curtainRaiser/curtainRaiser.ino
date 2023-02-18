/*
Controls the opening, closing and position recording of a stepper controlled automated curtain
*/

#include <EEPROM.h>

const int A=2, B=3, C=4, D=5;  // Assign stepper coils
const int top = 1250;  // Number of steps to reach the end
const long del = 1300;  // Delay between steps
const int buttonPinA = 11;  // the number of the pushbutton pin
const int buttonPinB = 10;  // the number of the pushbutton pin

int buttonStateA;  // switch up = LOW
int buttonStateB;  // switch down = LOW
int stepcounter;  // Current position
int breakStore = 0;  // Abort status
float progress;  // Position as a percentage

int addr = 1;  // Memory position
int memState;  // Can record one byte 

void setup() {                
  pinMode(A, OUTPUT);     
  pinMode(B, OUTPUT);     
  pinMode(C, OUTPUT);     
  pinMode(D, OUTPUT);     
  pinMode(buttonPinA, INPUT);
  pinMode(buttonPinB, INPUT);
  digitalWrite(buttonPinA, HIGH);
  digitalWrite(buttonPinB, HIGH);
  Serial.begin(9600);
  Serial.println("Initialised");
      
  // Load last known position
  memState = EEPROM.read(addr);
  progress = memState/float(255);
  stepcounter = int(progress*top);
  }

// Define stepper coil activation 
void step_one(){
  digitalWrite(A, LOW);   
  digitalWrite(B, HIGH);   
  digitalWrite(C, HIGH);   
  digitalWrite(D, LOW);   
  delayMicroseconds(del);
}
void step_two(){
  digitalWrite(A, LOW);   
  digitalWrite(B, HIGH);   
  digitalWrite(C, LOW);   
  digitalWrite(D, HIGH);   
  delayMicroseconds(del);
}
void step_three(){
  digitalWrite(A, HIGH);   
  digitalWrite(B, LOW);   
  digitalWrite(C, LOW);   
  digitalWrite(D, HIGH);   
  delayMicroseconds(del);
}
void step_four(){
  digitalWrite(A, HIGH);   
  digitalWrite(B, LOW);   
  digitalWrite(C, HIGH);   
  digitalWrite(D, LOW);   
  delayMicroseconds(del);
}
void motor_off(){
  digitalWrite(A, LOW);   
  digitalWrite(B, LOW);   
  digitalWrite(C, LOW);   
  digitalWrite(D, LOW);   
}
      
void loop() {

  // Read the state of the pushbutton value:
  buttonStateA = digitalRead(buttonPinA);
  Serial.print("Button State A: ");
  Serial.print(buttonStateA); 
  Serial.print(" ");
  
  buttonStateB = digitalRead(buttonPinB);
  Serial.print("Button State B: ");
  Serial.print(buttonStateB); 
  Serial.print(" ");

  // Reset abort status
  breakStore = 0;

  // Check stored position
  memState = EEPROM.read(addr);
  Serial.print("Stored Position: ");    
  Serial.print(memState/float(255)); 
  Serial.println(" "); 
    
  // Lever up = Start moving towards top
  if (buttonStateA == LOW) {
    while(stepcounter < top){

      buttonStateA = digitalRead(buttonPinA);
      Serial.print("Button State A: ");
      Serial.print(buttonStateA);  
      Serial.print(" ");                     

      stepcounter++;
      step_one();
      step_two(); 
      step_three();     
      step_four();
    
      // Direction change
      if (buttonStateA != LOW){
        Serial.println("Move towards top abort");      
        break;
        } 
    
      // Position status
      progress =  stepcounter/float(top);
      Serial.print("Expected Position: ");  
      Serial.println(progress);

      // Record position
      EEPROM.write(addr, int(progress*255));
      }
    }

  // Lever down = Start moving towards bot        
  if (buttonStateB == LOW) {
    while (stepcounter > 0){

      buttonStateB = digitalRead(buttonPinB);        
      Serial.print("Button State B: ");
      Serial.print(buttonStateB);     
      Serial.print(" ");    
      
      stepcounter--; 
      step_four();          
      step_three();
      step_two();   
      step_one();  

      // Direction change
      if (buttonStateB != LOW){
        Serial.println("Move towards bot abort ");      
        break;
        }

      // Position status
      progress =  stepcounter/float(top);
      Serial.print("Expected Position: ");  
      Serial.println(progress);

      // Record position
      EEPROM.write(addr, int(progress*255));
      }
    }
    
  motor_off();  // Idle
  }
  