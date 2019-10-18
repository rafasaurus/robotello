#define EN        8  

//Direction pin
#define X_DIR     52 
#define Y_DIR     50
#define Z_DIR     48
#define A_DIR     46

//Step pin
#define X_STP     53
#define Y_STP     51 
#define Z_STP     49 
#define A_STP     47


//DRV8825
int delayTime=100; //Delay between each pause (uS)
int stps=6400;// Steps to move

void changeDirForward() {
  digitalWrite(X_DIR, 1);
  digitalWrite(Y_DIR, 1);
  digitalWrite(Z_DIR, 1);
  digitalWrite(A_DIR, 1);
}

void changeDirBackward() {
  digitalWrite(X_DIR, 0);
  digitalWrite(Y_DIR, 0);
  digitalWrite(Z_DIR, 0);
  digitalWrite(A_DIR, 0);
}

void changeDirRight() {
  digitalWrite(X_DIR, 0);
  digitalWrite(Y_DIR, 0);
  digitalWrite(Z_DIR, 1);
  digitalWrite(A_DIR, 1);
}

void changeDirLeft() {
  digitalWrite(X_DIR, 1);
  digitalWrite(Y_DIR, 1);
  digitalWrite(Z_DIR, 0);
  digitalWrite(A_DIR, 0);
}

void goLeft() {
  digitalWrite(X_DIR, 0);
  digitalWrite(Y_DIR, 1);
  digitalWrite(Z_DIR, 1);
  digitalWrite(A_DIR, 0);
}
void one_step(boolean dir, byte stpprX, byte stpprY, byte stpprZ, byte stpprA)
{
    digitalWrite(stpprX, HIGH);
    digitalWrite(stpprY, HIGH);
    digitalWrite(stpprA, HIGH);
    digitalWrite(stpprZ, HIGH);
    delayMicroseconds(delayTime); 
    digitalWrite(stpprX, LOW);
    digitalWrite(stpprY, LOW);
    digitalWrite(stpprA, LOW);
    digitalWrite(stpprZ, LOW);
    delayMicroseconds(delayTime); 
}

void step(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite(dirPin, dir);
  delay(100);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(delayTime); 
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(delayTime); 

  }

}

void setup(){

  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
  pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);
  pinMode(A_DIR, OUTPUT); pinMode(A_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

}

void loop(){
   changeDirForward();
   for (int step = 0; step <= 3000; ++step) {
     one_step(0, X_STP, Y_STP, Z_STP, A_STP);
   }
  // changeDirBackward();
  // for (int step = 0; step <= 5000; ++step) {
  //   one_step(0, X_STP, Y_STP, Z_STP, A_STP);
  // }
   changeDirRight();
   for (int step = 0; step <= 5500; ++step) {
     one_step(0, X_STP, Y_STP, Z_STP, A_STP);
   }
  // changeDirLeft();
  // for (int step = 0; step <= 5000; ++step) {
  //   one_step(0, X_STP, Y_STP, Z_STP, A_STP);
  // }
//  goLeft();
//  for (int step = 0; step <= 5000; ++step) {
//    one_step(0, X_STP, Y_STP, Z_STP, A_STP);
//  }

  //step(false, X_DIR, X_STP, stps); //X, Clockwise
  //step(false, Y_DIR, Y_STP, stps); //Y, Clockwise
  //step(false, Z_DIR, Z_STP, stps); //Z, Clockwise
  //step(false, A_DIR, A_STP, stps); //Z, Clockwise
  //delay(500);
  //step(true, X_DIR, X_STP, stps); //X, Counterclockwise
  //step(true, Y_DIR, Y_STP, stps); //Y, Counterclockwise
  //step(true, Z_DIR, Z_STP, stps); //X, Counterclockwise
  //step(true, A_DIR, A_STP, stps); //X, Counterclockwise
  //delay(500);

}
