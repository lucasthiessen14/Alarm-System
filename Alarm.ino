//Added libraries
#include <Keypad.h>
#include <LiquidCrystal.h>
#define echoPin 10
#define trigPin 11


int array1[5]{0,0,0,0,0}; // Array to store passcode attempts
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int buzzer = 12; // variable for buzzer pin
int soundSensor = 52; // variable for sound pin

// LED pins
int LEDG = 22;
int LEDR = 23;
int LEDY = 24;

boolean LEDStatus=false; // LED Status
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns

// Keypad input
char keys[ROWS][COLS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

// Initializing keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
// Initializing Liquid Crystal Display
LiquidCrystal lcd(26, 27, 28, 29, 30, 31);

void setup(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(buzzer,OUTPUT);//initialize the buzzer pin as an output

  // Initialize the LED pins as an output
  pinMode(LEDG, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDY, OUTPUT);
  
  pinMode(soundSensor,INPUT); // Initialize the sound sensor pin as an input

  // Default Display when program starts
  lcd.begin(16, 2);
  lcd.write("Disarmed");
  lcd.setCursor(0,1);
  lcd.write("Passcode: ");
  lcd.setCursor(0,0);
  Serial.begin(9600);
  Serial.print("Enter Passcode: ");
}


void loop(){
  static int arm = 0; // Variable for starting the alarm system arming
  static int keycounter = 0; // Variable to keep track of number of keys pressed

  // Default password variables
  static int a = 5;
  static int b = 2;
  static int c = 8;
  static int d = 7;
  
  static int chng = 0; // Variable for password change
  static int wait = 0; // Variable for countdown until arming
  static int alarm = 0; // Variable for if the alarm is armed
  static int countd = 120; // Variable for count down before alarm sound
  static int trig = 0; // Variable for triggering the alarm
  char key = keypad.getKey(); // Variable to keep track of the key pressed
  int SensorData = digitalRead(soundSensor); // Variable to keep track of sounds sensor input
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Calculates distance

  //Turns on the green led if the alarm system is not armed
  if (arm == 0){
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDR, LOW);
  }
  else
    //Starts the 30 second count down before the alarm system arms
    digitalWrite(LEDG, LOW);
    if(wait == 1){
      int x = 30;
      for(int i = 0; i < 15; i++){
        lcd.clear();
        lcd.print(x);
        digitalWrite(LEDY, HIGH);
        delay(1000);
        x -= 1;
        lcd.clear();
        lcd.print(x);
        digitalWrite(LEDY, LOW);
        delay(1000);
        x -= 1;
      }
    }
    wait = 0;
    alarm = 1; //Arms the alarm system
    digitalWrite(LEDR, HIGH); //Turns on the Red led to show that the system is armed

  lcd.clear();
  if(arm == 1){
    lcd.write("Armed"); //Displays Armed on the screen
  }
  else{
    lcd.write("Disarmed"); //Displays disarmed on the screen
  }
  lcd.setCursor(0,1);
  lcd.write("Passcode: "); //Displays "Passcode:" screen
  lcd.setCursor(0,0);

  //Clears and numbers already on the screen
  if(keycounter == 0){
    lcd.setCursor(10,1);
    lcd.write("    ");
    lcd.setCursor(0,0);
  }
  
  //Displays the first number input into the keypad on the screen
  else if(keycounter == 1){
    lcd.setCursor(10,1);
    lcd.print(array1[0]);
    lcd.setCursor(0,0);
  }

  //Dispays the second number input into the keypad on the screen
  else if(keycounter == 2){
    lcd.setCursor(10,1);
    lcd.print(array1[0]);
    lcd.setCursor(11,1);
    lcd.print(array1[1]);
    lcd.setCursor(0,0);
  }

  //Dispays the third number input into the keypad on the screen
  else if(keycounter == 3){
    lcd.setCursor(10,1);
    lcd.print(array1[0]);
    lcd.setCursor(11,1);
    lcd.print(array1[1]);
    lcd.setCursor(12,1);
    lcd.print(array1[2]);
    lcd.setCursor(0,0);
  }

  //Dispays the forth number input into the keypad on the screen
  else if(keycounter == 4){
    lcd.setCursor(10,1);
    lcd.print(array1[0]);
    lcd.setCursor(11,1);
    lcd.print(array1[1]);
    lcd.setCursor(12,1);
    lcd.print(array1[2]);
    lcd.setCursor(13,1);
    lcd.print(array1[3]);
    lcd.setCursor(0,0);
  }

  //Checks to see if the bottons clicked on the keypad are numbers and make sure that no more then 4 numbers have been entered
  if ((key != NO_KEY) && (key >= 48) && (key <= 58) && (keycounter != 4)){
      keycounter += 1;
      array1[keycounter - 1] = key - 48;
      Serial.print(key);
    }

  //Stores the action that is to be done after the passcode is entered
  else if((key != NO_KEY) && (keycounter == 4)){
      array1[4] = key;
      keycounter += 1;
 }

 //Changes the alarm passcode
 if((chng == 1) && (keycounter == 4)){
      a = array1[0];
      b = array1[1];
      c = array1[2];
      d = array1[3];
      chng = 0;
      keycounter = 0;
      //Clears all the entries in the array
      for(int i; i < 5; i++){
        array1[i] = 0;
    }
      Serial.println();
      Serial.println("Passcode Changed"); //Displays passcode changed on the screen after the passcode has been changed
      
      //Resets the screen to the default display
      lcd.clear();
      lcd.write("Passcode");
      lcd.setCursor(0,1);
      lcd.write("Changed");
      lcd.setCursor(0,0);
      delay(1000);
      Serial.print("Enter Passcode: ");
    }
  

  if(keycounter == 5){

    //Checks if the correct passcode was entered and if the action entered is to arm the alarm system
    if((array1[0] == a) && (array1[1] == b) && (array1[2] == c) && (array1[3] == d) && (array1[4] == 65)){
      Serial.println();
      Serial.println("Armed");
      Serial.print("Enter Passcode: ");
      keycounter = 0;
      arm = 1;
      wait = 1;
      alarm = 0;
    }

    //Checks if the correct passcode was entered and if the action entered is to disarm the alarm system
    else if((array1[0] == a) && (array1[1] == b) && (array1[2] == c) && (array1[3] == d) && (array1[4] == 68)){
      Serial.println();
      Serial.println("Disarmed");
      Serial.print("Enter Passcode: ");
      countd = 30;
      trig = 0;
      keycounter = 0;
      arm = 0;
      alarm = 0;
    }

    //Checks if the correct passcode was entered and if the action entered is to change the passcode
    else if ((array1[0] == a) && (array1[1] == b) && (array1[2] == c) && (array1[3] == d) && (array1[4] == 67) && (arm == 0)){
      chng = 1;
      Serial.println();
      Serial.print("Type in New Passcode: ");
      lcd.clear();
      lcd.write("Type in");
      lcd.setCursor(0,1);
      lcd.write("New Passcode");
      lcd.setCursor(0,0);
      delay(1000);
      keycounter = 0;
    }
    
    //If the incorrect passcode is entered incorrect passcode is displayed on the screen
    else{
    keycounter = 0;
    Serial.println();
    Serial.println("Incorrect Passcode");
    lcd.clear();
    lcd.write("Incorrect");
    lcd.setCursor(0,1);
    lcd.write("Password");
    lcd.setCursor(0,0);
    delay(1000);
    Serial.print("Enter Passcode: ");
    }
    //The numbers in the array are erased
    for(int i; i < 5; i++){
      array1[i] = 0;
    }
  }

  if(alarm == 1){

    //If the alarm is armed and somethins is within the specified distance from the alarms system the alarm count down will be triggered
    if ((distance <= 100) || (SensorData==1)){
      Serial.println("alarm");
      trig = 1;
    }

    //Sounds the alarm if one of the sensors is triggered
    if((trig == 1) && (countd == 0)){

      for(int i=0;i<80;i++){
        digitalWrite(buzzer,HIGH);
        delay(1);//wait for 1ms
        digitalWrite(buzzer,LOW);
        delay(1);//wait for 1ms
      }

      for(int i=0;i<100;i++){
        digitalWrite(buzzer,HIGH);
        delay(2);//wait for 2ms
        digitalWrite(buzzer,LOW);
        delay(2);//wait for 2ms
      }

    }
  }

  //Blinks the yellow led for 30 seconds after one of the sensors is triggered before sounding the alarm
  if((trig == 1) && (countd != 0)){
    digitalWrite(LEDY, HIGH);
    delay(125);
    digitalWrite(LEDY, LOW);
    delay(125);
    countd -= 1;
  }
  
}
