#include <AFMotor.h>

// Motor setup
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// Ultrasonic sensor pins
const int trigF = 23;
const int echoF = 22;
const int trigL = 27;
const int echoL = 26;
const int trigR = 29;
const int echoR = 28;
const int trigB = 25;
const int echoB = 24;

char command;
bool autonomousModeEnabled = false; // Variable to track autonomous mode

// Function prototypes
void handleCommand(char command);
void forward(int speed = 160);
void backward(int speed = 160);
void turnLeft(int speed = 160);
void turnRight(int speed = 160);
void stop();
void autonomousMode();
int getDistance(int trigPin, int echoPin);

void setup() {
  Serial.begin(9600);
  pinMode(trigF, OUTPUT);
  pinMode(echoF, INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);
  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);
  pinMode(trigB, OUTPUT);
  pinMode(echoB, INPUT);
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();
    handleCommand(command);
  }
  
  if (autonomousModeEnabled) {
    autonomousMode();
  }
}

void handleCommand(char command) {
  if (command == 'F') {
    forward();
    autonomousModeEnabled = false; // Disable autonomous mode
  } else if (command == 'B') {
    backward();
    autonomousModeEnabled = false; // Disable autonomous mode
  } else if (command == 'L') {
    turnLeft();
    autonomousModeEnabled = false; // Disable autonomous mode
  } else if (command == 'R') {
    turnRight();
    autonomousModeEnabled = false; // Disable autonomous mode
  } else if (command == 'S') {
    stop();
    autonomousModeEnabled = false; // Disable autonomous mode
  } else if (command == 'A') {
    autonomousModeEnabled = true; // Enable autonomous mode
  }
}

void forward(int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void backward(int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void turnLeft(int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void turnRight(int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void autonomousMode() {
  int distanceF = getDistance(trigF, echoF);
  int distanceL = getDistance(trigL, echoL);
  int distanceR = getDistance(trigR, echoR);
  int distanceB = getDistance(trigB, echoB);

  // Prüfen, ob ein Hindernis vorne ist
  if (distanceF < 20) {
    stop();
    delay(100); // Verzögerung, um sicherzustellen, dass der Roboter angehalten hat

    // Hindernisvermeidung
    if (distanceR > distanceL && distanceR > 20) {
      turnRight(180);
      delay(300);
    } else if (distanceL > distanceR && distanceL > 20) {
      turnLeft(180);
      delay(300);
    } else if (distanceB > 20) {
      backward(160);
      while (distanceB > 20) {
        distanceB = getDistance(trigB, echoB);
        distanceL = getDistance(trigL, echoL);
        distanceR = getDistance(trigR, echoR);
        if (distanceR > 20 || distanceL > 20) {
          stop();
          delay(100);
          if (distanceR > distanceL) {
            turnRight(180);
          } else {
            turnLeft(180);
          }
          delay(300);
          break;
        }
        delay(100);
      }
      stop();
    } else {
      stop();
    }
  } else {
    adjustDirection(distanceL, distanceR);
    forward(map(distanceF, 20, 160, 64, 160));
  }
}

void adjustDirection(int distanceL, int distanceR) {
  if (distanceL < 20) {
    turnRight(180);
    delay(100);
    forward(160);
  } else if (distanceR < 20) {
    turnLeft(180);
    delay(100);
    forward(160);
  }
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}
