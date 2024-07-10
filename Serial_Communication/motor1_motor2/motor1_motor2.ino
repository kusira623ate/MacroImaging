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

// Full-step sequence for 28BYJ-48 (clockwise)
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

// variables
String content = "";   // command given by user
int currentStepM2 = 0; //global variable to iterate through fullStepSequence for motor 2
int currentStepM1 = 0; //global variable to iterate through fullStepSequence for motor 2

void setup() {
  Serial.begin(9600);
  Serial.println("M1 and M2 working");

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
}

void loop() {
  // read user input
  if (Serial.available()) {
    check();
  }

  if (content == "rotatem") {
    // Rotate Motor 1 for 11.25-degree steps
    rotateMotor(M1_IN1, M1_IN2, M1_IN3, M1_IN4, 128, currentStepM1);
    Serial.println("Motor 1 rotated by 11.25 degrees");
    content = ""; // Clear the content to avoid repeated execution
    currentStepM1 = 0;
  }

  if (content == "rotatemm") {
    // Rotate Motor 2 clockwise in 5-degree steps
    rotateMotor(M2_IN1, M2_IN2, M2_IN3, M2_IN4, 56, currentStepM2);
    Serial.println("Motor 2 rotated by 5 degrees");
    content = ""; // Clear the content to avoid repeated execution
    currentStepM2 = 0;
  }

  if (content == "rotatemmm") {
    // Rotate Motor 2 anti-clockwise in 5-degree steps
    rotateMotor(M2_IN1, M2_IN2, M2_IN3, M2_IN4, -56, currentStepM2);
    Serial.println("Motor 2 rotated by -5 degrees");
    content = ""; // Clear the content to avoid repeated execution
    currentStepM2 = 0;
  }
}

// Interpret user input
void check() {
  String data = Serial.readStringUntil('\n');
  data.trim(); // Remove any trailing whitespace
  data.toLowerCase();
  Serial.print("Received command: "); // Debugging statement
  Serial.println(data); // Debugging statement
  content = data;
}

void rotateMotor(int in1, int in2, int in3, int in4, int steps, int &currentStep) {
  int direction = (steps > 0) ? 1 : -1; // 1 for clockwise rotation ; -1 for anticlockwise rotation
  steps = abs(steps);

  // Traversal through fullStepSequence[8][4] 
  for (int i = 0; i < steps; i++) {
    currentStep += direction;
    if (currentStep > 7) currentStep = 0;
    if (currentStep < 0) currentStep = 7;
    setStep(in1, in2, in3, in4, currentStep);
    delay(1); // Adjust delay for speed control
  }
}

// Set the pins HIGH and LOW as per fullStepSequence[8][4]
void setStep(int in1, int in2, int in3, int in4, int step) {
  digitalWrite(in1, fullStepSequence[step][0]);
  digitalWrite(in2, fullStepSequence[step][1]);
  digitalWrite(in3, fullStepSequence[step][2]);
  digitalWrite(in4, fullStepSequence[step][3]);
}
