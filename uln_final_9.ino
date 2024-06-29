// Define the Arduino pins connected to the ULN2003 driver for Motor 2
#define M2_IN1 9
#define M2_IN2 8
#define M2_IN3 7
#define M2_IN4 6

// Define the Arduino pins connected to the ULN2003 driver for Motor 1
#define M1_IN1 10
#define M1_IN2 11
#define M1_IN3 12
#define M1_IN4 13

// Full-step sequence for 28BYJ-48
const int fullStepSequence[8][4] = {
  {1, 0, 0, 0},  //step 1
  {1, 1, 0, 0},  //step 2
  {0, 1, 0, 0},  //step 3
  {0, 1, 1, 0},  //step 4
  {0, 0, 1, 0},  //step 5
  {0, 0, 1, 1},  //step 6
  {0, 0, 0, 1},  //step 7
  {1, 0, 0, 1}   //step 8
};

int currentStepM2 = 0;
int currentStepM1 = 0;
bool start = true;  // Flag to ensure the loop runs only once

void setup() {
  // Set pins as outputs for Motor 2
  pinMode(M2_IN1, OUTPUT);
  pinMode(M2_IN2, OUTPUT);
  pinMode(M2_IN3, OUTPUT);
  pinMode(M2_IN4, OUTPUT);

  // Set pins as outputs for Motor 1
  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);
  pinMode(M1_IN3, OUTPUT);
  pinMode(M1_IN4, OUTPUT);

  // Homing sequence to reset motors to a known position
  // homeMotor(M1_IN1, M1_IN2, M1_IN3, M1_IN4, currentStepM1);
  // homeMotor(M2_IN1, M2_IN2, M2_IN3, M2_IN4, currentStepM2);
}

void loop() {

  if (start) {
    // // Rotate Motor 2 anticlockwise by 20 degrees
    // rotateMotor(M2_IN1, M2_IN2, M2_IN3, M2_IN4, -114, currentStepM2);
    // delay(1000); // Delay for demonstration purposes
      
    for (int i = 0; i < 9; i++) {
      if (i > 0) {
        // Rotate Motor 2 clockwise in 5-degree steps until 40 degrees
        rotateMotor(M2_IN1, M2_IN2, M2_IN3, M2_IN4, 56, currentStepM2);
        delay(1000); // Delay for demonstration purposes }
      }

      // Rotate Motor 1 one full revolution in 11.25-degree steps
      for (int j = 0; j < 32; j++) {
        rotateMotor(M1_IN1, M1_IN2, M1_IN3, M1_IN4, 128, currentStepM1);
        delay(1000); // Delay for each step
      }
    }
  
    // Rotate Motor 2 anticlockwise by 40 degrees to its initial position
    rotateMotor(M2_IN1, M2_IN2, M2_IN3, M2_IN4, -456, currentStepM2);
    delay(1000); // Delay for demonstration purposes

    start = false ;
    currentStepM2 = 0;
    currentStepM1 = 0;
  } 
}

void rotateMotor(int in1, int in2, int in3, int in4, int steps, int &currentStep) {
  int direction = (steps > 0) ? 1 : -1;
  steps = abs(steps);
  
  for (int i = 0; i < steps; i++) {
    currentStep += direction;
    if (currentStep > 7) currentStep = 0;
    if (currentStep < 0) currentStep = 7;
    setStep(in1, in2, in3, in4, currentStep);
    delay(1); // Adjust delay for speed control
  }
}

void setStep(int in1, int in2, int in3, int in4, int step) {
  digitalWrite(in1, fullStepSequence[step][0]);
  digitalWrite(in2, fullStepSequence[step][1]);
  digitalWrite(in3, fullStepSequence[step][2]);
  digitalWrite(in4, fullStepSequence[step][3]);
}

// void homeMotor(int in1, int in2, int in3, int in4, int &currentStep) {
//   // Move the motor through a full step cycle to ensure it starts from a known position
//   for (int i = 0; i < 8; i++) {
//     setStep(in1, in2, in3, in4, i);
//     delay(1); // Adjust delay as needed
//   }
//   currentStep = 0; // Reset the current step to the initial position
// }





