#include <Servo.h>

const int STEPPER_PIN_1 = 39;
const int STEPPER_PIN_2 = 41;
const int STEPPER_PIN_3 = 43;
const int STEPPER_PIN_4 = 45;

int Red_Led = 13;
int Buzzer = 12;
int Green_Led = 11;
int LDR = A8;
int ServoPin = 8;
int inputPin = 9;
int pirState = LOW;
int val = 0;
int Turn = 0;

const int STEPS_PER_REVOLUTION = 2048;
const float DEGREES_PER_STEP = 360.0 / STEPS_PER_REVOLUTION;
const int TARGET_DEGREES = 21;  // angle

Servo myservo;

enum State {
  IDLE,
  DAY,
  NIGHT,
  CHECK_MOTION,
  DETECT_FACE
};

State currentState = IDLE;
int pos = 0;

void setup() {
  Serial.begin(9600);
  myservo.attach(ServoPin);

  pinMode(Red_Led, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Green_Led, OUTPUT);
  pinMode(inputPin, INPUT);

  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);

  digitalWrite(Red_Led, LOW);
  digitalWrite(Buzzer, LOW);
  digitalWrite(Green_Led, LOW);

  rotateAntiClockwise();
}

void loop() {
  switch (currentState) {
    case IDLE:
      idle();
      break;
    case DAY:
      day();
      break;
    case CHECK_MOTION:
      check_motion();
      break;
    case NIGHT:
      night();
      break;
    case DETECT_FACE:
      detect_face();
      break;
  }
}
void idle() {
  LightSensor();
}
void LightSensor() {
  int LDR_Value = analogRead(LDR);
  Serial.print(LDR_Value);
  if (LDR_Value > 400) {
    digitalWrite(Red_Led, HIGH);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Green_Led, HIGH);
    currentState = NIGHT;
  } else if (LDR_Value < 400) {
    digitalWrite(Red_Led, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Green_Led, LOW);
    currentState = DAY;
  }
}
void day() {
  if (currentState == DAY) {
    currentState = CHECK_MOTION;
  } else {
    //Do Nothing
  }
}
void night() {
  if (currentState == NIGHT) {
    currentState = CHECK_MOTION;
  } else {
    //Do nothing
  }
}
void check_motion() {
  if (currentState == CHECK_MOTION) {
    currentState = DETECT_FACE;
  } else {
    //Do Nothing
  }
}
void detect_face() {
  if (Serial.available() > 0) {
    // Read the incoming command
    char command = Serial.read();
    // Turn on/off the LEDs based on the command
    if (command == '1') {
      //Owner
      digitalWrite(Red_Led, LOW);
      digitalWrite(Buzzer, LOW);
      digitalWrite(Green_Led, HIGH);
      delay(2000);
      currentState = IDLE;
    } else if (command == '2') {
      //Intruder
      if (Turn == 0) {
        digitalWrite(Red_Led, HIGH);
        digitalWrite(Buzzer, HIGH);
        digitalWrite(Green_Led, LOW);
        rotateAntiClockwise();
        Turn = 1;
        currentState = IDLE;
      }
      delay(2000);
    }
  }
}
void ServoOpen() {
  pos = 90;
  myservo.write(pos);
}
void ServoClose() {
  pos = 0;
  myservo.write(pos);
}
void Motionsensor() {
  val = digitalRead(inputPin);
  if (val == HIGH) {
    digitalWrite(Red_Led, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Green_Led, HIGH);
    currentState = DETECT_FACE;
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      pirState = HIGH;
    }
    delay(500);
  } else {
    digitalWrite(Red_Led, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Green_Led, LOW);
    if (pirState == HIGH) {
      Serial.println("Motion ended!");
      pirState = LOW;
    }
    delay(500);
  }
}
void rotateAntiClockwise() {
  int targetSteps = TARGET_DEGREES / DEGREES_PER_STEP;
  const int stepSequence[8][4] = {
    { HIGH, LOW, LOW, HIGH },
    { LOW, LOW, LOW, HIGH },
    { LOW, LOW, HIGH, HIGH },
    { LOW, LOW, HIGH, LOW },
    { LOW, HIGH, HIGH, LOW },
    { LOW, HIGH, LOW, LOW },
    { HIGH, HIGH, LOW, LOW },
    { HIGH, LOW, LOW, LOW }

  };
  for (int i = 0; i < targetSteps; i++) {
    for (int j = 0; j < 8; j++) {
      digitalWrite(STEPPER_PIN_4, stepSequence[j][3]);
      digitalWrite(STEPPER_PIN_3, stepSequence[j][2]);
      digitalWrite(STEPPER_PIN_2, stepSequence[j][1]);
      digitalWrite(STEPPER_PIN_1, stepSequence[j][0]);
      delayMicroseconds(1000);
    }
  }
  delay(500);

  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_1, LOW);
}
void rotateClockwise() {
  int targetSteps = TARGET_DEGREES / DEGREES_PER_STEP;
  const int stepSequence[8][4] = {
    { HIGH, LOW, LOW, HIGH },
    { LOW, LOW, LOW, HIGH },
    { LOW, LOW, HIGH, HIGH },
    { LOW, LOW, HIGH, LOW },
    { LOW, HIGH, HIGH, LOW },
    { LOW, HIGH, LOW, LOW },
    { HIGH, HIGH, LOW, LOW },
    { HIGH, LOW, LOW, LOW }

  };
  for (int i = 0; i < targetSteps; i++) {
    for (int j = 0; j < 8; j++) {
      digitalWrite(STEPPER_PIN_1, stepSequence[j][3]);
      digitalWrite(STEPPER_PIN_2, stepSequence[j][2]);
      digitalWrite(STEPPER_PIN_3, stepSequence[j][1]);
      digitalWrite(STEPPER_PIN_4, stepSequence[j][0]);
      delayMicroseconds(1000);
    }
  }
  delay(500);

  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
}