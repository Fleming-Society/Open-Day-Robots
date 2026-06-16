// **************************************************************************************************
// * This is the testing block for the robot                                                        *
// * It contains several functions that can make the robot move around and sensor to sense value    *
// * The function includes:                                                                         *
// * - moving_forward();                                                                            *
// * - moving_backward();                                                                           *
// * - moving_left();                                                                               *
// * - moving_right();                                                                              *
// * - button_state_debug();                                                                        *
// * - distance_measurement();                                                                      *
// * Good luck and have fun playing around the robot!                                               *
// *                                                                  Junzhe Chen, Jun. 21st 2023,  *
// *                                                    Updated by Yung Cheung Zhou, Jun. 25th 2025 *
// **************************************************************************************************


// Constants (Do NOT change anything between the lines)
// **************************************************************************************************
const int LEFT_BUTTON_PIN = 4;
const int RIGHT_BUTTON_PIN = 5;

const int FORWARD_MOTOR_PIN = 12;
const int FORWARD_MOTOR_SPEED = 3;

const int HORIZONTAL_MOTOR_PIN = 13;
const int HORIZONTAL_MOTOR_SPEED = 11;

const int ULTRASONIC_ECHO_PIN = 6;
const int ULTRASONIC_TRIGGER_PIN = 7;

// Values that will be used in the program
int LEFT_BUTTON_STATE = 0;
int RIGHT_BUTTON_STATE = 0;

int DIRECTION_STATE = 0;  // 0 means going to the right, 1 means going to the left

float duration_us;
float distance_cm;

unsigned long startMillis;
unsigned long currentMillis;
// **************************************************************************************************


// Variables that will be used in the program
// Play around with the values below
// **************************************************************************************************
const int OFFSET_TIME = 750;          // Offset time in milliseconds

const int FORWARD_SPEED = 50;         // Set the value between 0 and 255
const int BACKWARD_SPEED = 20;        // Set the value between 0 and 255
const int HORIZONTAL_SPEED = 50;      // Set the value between 0 and 255

const float DISTANCE_LOWER_BOUNDARY = 4.0;
const float DISTANCE_UPPER_BOUNDARY = 5.5;
// **************************************************************************************************


// Setup
// **************************************************************************************************
void setup() {
  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);

  pinMode(FORWARD_MOTOR_PIN, OUTPUT);
  pinMode(HORIZONTAL_MOTOR_PIN, OUTPUT);

  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  Serial.begin(9600);

  startMillis = millis();
  currentMillis = millis();

  Serial.println("Robot test mode started");
  Serial.println("-----------------------------");
}
// **************************************************************************************************


// Main loop
// **************************************************************************************************
void loop() {
  distance_measurement();
  button_state_debug();
  horizontal_movement_debug();
  forward_backward_movement_debug();

  delay(200);  // Slows down Serial Monitor output so it is readable
}
// **************************************************************************************************


// Motor control functions
// **************************************************************************************************
void moving_forward() {
  analogWrite(FORWARD_MOTOR_SPEED, FORWARD_SPEED);
  digitalWrite(FORWARD_MOTOR_PIN, HIGH);
}

void moving_backward() {
  analogWrite(FORWARD_MOTOR_SPEED, BACKWARD_SPEED);
  digitalWrite(FORWARD_MOTOR_PIN, LOW);
}

void moving_left() {
  analogWrite(HORIZONTAL_MOTOR_SPEED, HORIZONTAL_SPEED);
  digitalWrite(HORIZONTAL_MOTOR_PIN, HIGH);
}

void moving_right() {
  analogWrite(HORIZONTAL_MOTOR_SPEED, HORIZONTAL_SPEED);
  digitalWrite(HORIZONTAL_MOTOR_PIN, LOW);
}

void stop_moving_horizontal() {
  analogWrite(HORIZONTAL_MOTOR_SPEED, 0);
}

void stop_moving_forward_backward() {
  analogWrite(FORWARD_MOTOR_SPEED, 0);
}
// **************************************************************************************************


// Debug switch reading
// **************************************************************************************************
void button_state_debug() {
  LEFT_BUTTON_STATE = digitalRead(LEFT_BUTTON_PIN);
  RIGHT_BUTTON_STATE = digitalRead(RIGHT_BUTTON_PIN);

  if (LEFT_BUTTON_STATE == LOW) {
    Serial.println("SWITCH: Left button pressed");
    DIRECTION_STATE = 0;
  } else {
    Serial.println("SWITCH: Left button not pressed");
  }

  if (RIGHT_BUTTON_STATE == LOW) {
    Serial.println("SWITCH: Right button pressed");
    DIRECTION_STATE = 1;
  } else {
    Serial.println("SWITCH: Right button not pressed");
  }
}
// **************************************************************************************************


// Ultrasonic distance measurement
// **************************************************************************************************
void distance_measurement() {
  if (millis() > currentMillis + 100) {
    currentMillis = millis();

    digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);

    duration_us = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);

    distance_cm = 0.017 * duration_us;

    Serial.print("SENSOR: Distance = ");
    Serial.print(distance_cm);
    Serial.println(" cm");
  }
}
// **************************************************************************************************


// Debug horizontal movement
// **************************************************************************************************
void horizontal_movement_debug() {
  if (distance_cm < DISTANCE_UPPER_BOUNDARY) {
    if (DIRECTION_STATE == 0) {
      Serial.println("HORIZONTAL MOTOR: Moving RIGHT");
      moving_right();
    } else if (DIRECTION_STATE == 1) {
      Serial.println("HORIZONTAL MOTOR: Moving LEFT");
      moving_left();
    }
  } else {
    Serial.println("HORIZONTAL MOTOR: STOP");
    stop_moving_horizontal();
  }
}
// **************************************************************************************************


// Debug forward/backward movement
// **************************************************************************************************
void forward_backward_movement_debug() {
  Serial.print("CONTROL: Distance used = ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  if (distance_cm > DISTANCE_UPPER_BOUNDARY) {
    if (currentMillis - startMillis >= OFFSET_TIME) {
      Serial.println("FORWARD/BACKWARD MOTOR: Moving FORWARD");
      moving_forward();
      stop_moving_horizontal();
    } else {
      Serial.println("FORWARD/BACKWARD MOTOR: Waiting before moving FORWARD");
      stop_moving_forward_backward();
    }
  } else if (distance_cm < DISTANCE_LOWER_BOUNDARY) {
    Serial.println("FORWARD/BACKWARD MOTOR: Moving BACKWARD");
    startMillis = currentMillis;
    moving_backward();
  } else {
    Serial.println("FORWARD/BACKWARD MOTOR: STOP");
    stop_moving_forward_backward();
    startMillis = currentMillis;
  }

  Serial.println("-----------------------------");
}
// **************************************************************************************************
