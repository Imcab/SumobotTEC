// C++ code
//

//SharpDistanceSensorPIN on A0
const int SharpSensorPIN = A0;

// InfraRed sensors on chasis, A1, A0
const int QDR1PIN = A1;
const int QDR2PIN = A2;

//DipSwitch single objects
int buttonD7;
int buttonD8;

//Infrared single objects
int QDR1;
int QDR2;

//Main dipSitch
int dipSwitch[2];

//All posible combinations off dipSwitch button states, customize it as you wish
enum SumoModes {
    SEARCH,
    Mode2,
    Mode3,
    OFF
};

//Track the current dipswitch mode
SumoModes currentMode;

//Initialize sharp distance in cm to later update in main loop
int distanceSharp;

void setup()
{
  Serial.begin(9600);

  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  for(int i = 9; i <= 11; i++){
  pinMode(i, OUTPUT);
  delay(10);
  }
  
  pinMode(6, OUTPUT);
  
}

//Converts sharp raw value to centimeters
int toCentimeters(int rawValue){
  float voltage = rawValue * 0.0048828125;
  int distance = 13 * pow(voltage, -1);
  return distance;
}

//Generic function to move the sumo motors
void move(double m1, double m2, double m3, double m4){
  analogWrite(11, m1);
  analogWrite(10, m2);
  analogWrite(6, m3);
  analogWrite(9, m4);
}

//Stops
void sumoStop(){
  move(0,0,0,0);
}

//Goes back, change the order in the numbers based on your directions of the motors
void sumoback(){
  move(255,0,0,255);
}

//Turns right, change the order in the numbers based on your directions of the motors
void sumoTurnRight(int value){
  move(0,value,0,value);
}

//Turns left, change the order in the numbers based on your directions of the motors
void sumoTurnLeft(){
  move(255,0,255,0);
}

//Goes forward, change the order in the numbers based on your directions of the motors
void sumoForward(){
  move(0,255, 255, 0);
  
}

//Check if the distance is valid (Sharp sensor can only read from 0 to 30 centimeters)
boolean validSharpRead(int read){
  return read >= 0 && read <= 30;
}

//If any of the Infrared sensors is below x, we consider we are touching the white line, change for x to your robot
boolean outOfDojo(int Qdr, int Qdr2){
  return  Qdr < 700 || Qdr2 < 700;
}

void sumoBotActions(SumoModes currentSumoMode){
  //For switch 1 is the sencond number of the array
  //0,1 = DipSwitch 1 on, Dipswitch 2 off
  //Here implement your sumoActions!
  switch(currentMode){
    case SEARCH: // [0,1]

        //Just a test
        if(validSharpRead(distanceSharp) && distanceSharp <= 9  && !outOfDojo(QDR1, QDR2)){
        sumoStop();
        }else{
          sumoTurnRight(50);
          delay(1500);
          sumoStop();
        }

        if(outOfDojo(QDR1, QDR2)){
          sumoTurnRight(200);
        }      

      break;
    case Mode2: // [1,0]
      break;
    case Mode3: // [1,1]
      break;
    case OFF: //[0,0] Both Switches off, stop sumo
      sumoStop();
      break;
    
  }
}

void loop()
{
  
  //update individual dipswitch port
  buttonD7 = digitalRead(7);
  buttonD8 = digitalRead(8);

  //Get infrared sensors values
  QDR1 = analogRead(QDR1PIN);
  QDR2 = analogRead(QDR2PIN);

  //Stores the dipSwitchValues
  dipSwitch[0] = !buttonD7;
  dipSwitch[1] = !buttonD8;

  //Converts the rawRead into centimeters
  distanceSharp = toCentimeters(analogRead(SharpSensorPIN));

  //Handle dipSwitch sumoBot combinations
  if(dipSwitch[0] == 0 && dipSwitch[1] == 1){
    currentMode = SEARCH;
  }
  else if(dipSwitch[0] == 1 && dipSwitch[1] == 0){
    currentMode = Mode2;
  }
  else if(dipSwitch[0] == 1 && dipSwitch[1] == 1){
    currentMode = Mode3;
  }
  else{
    currentMode = OFF;
  }

  //Update your SumoBot!
  sumoBotActions(currentMode);
  
}
