import Jetson.GPIO as GPIO
import time

# Define GPIO pins connected to the ULN2003 driver for Motor 2
M2_IN1 = 10
M2_IN2 = 11
M2_IN3 = 12
M2_IN4 = 13

# Define GPIO pins connected to the ULN2003 driver for Motor 1
M1_IN1 = 21
M1_IN2 = 22
M1_IN3 = 23
M1_IN4 = 24

# Full-step sequence for 28BYJ-48 (clockwise)
fullStepSequence = [
    [1, 0, 0, 0],  # step 1
    [1, 1, 0, 0],  # step 2
    [0, 1, 0, 0],  # step 3
    [0, 1, 1, 0],  # step 4
    [0, 0, 1, 0],  # step 5
    [0, 0, 1, 1],  # step 6
    [0, 0, 0, 1],  # step 7
    [1, 0, 0, 1]   # step 8
]

# Initialize variables
content = ""       # Command given by user
currentStepM2 = 0  # Global variable to iterate through fullStepSequence for motor 2
currentStepM1 = 0  # Global variable to iterate through fullStepSequence for motor 1

def setup():
    # Set up GPIO pins as outputs
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(M2_IN1, GPIO.OUT)
    GPIO.setup(M2_IN2, GPIO.OUT)
    GPIO.setup(M2_IN3, GPIO.OUT)
    GPIO.setup(M2_IN4, GPIO.OUT)
    GPIO.setup(M1_IN1, GPIO.OUT)
    GPIO.setup(M1_IN2, GPIO.OUT)
    GPIO.setup(M1_IN3, GPIO.OUT)
    GPIO.setup(M1_IN4, GPIO.OUT)
    print("M1 and M2 ready")

def rotateMotor(in1, in2, in3, in4, steps, currentStep):
    direction = 1 if steps > 0 else -1  # 1 for clockwise rotation; -1 for anticlockwise rotation
    steps = abs(steps)

    # Traverse through fullStepSequence
    for _ in range(steps):
        currentStep += direction
        if currentStep > 7:
            currentStep = 0
        if currentStep < 0:
            currentStep = 7
        setStep(in1, in2, in3, in4, currentStep)
        time.sleep(0.001)  # Adjust delay for speed control

    return currentStep

def setStep(in1, in2, in3, in4, step):
    GPIO.output(in1, fullStepSequence[step][0])
    GPIO.output(in2, fullStepSequence[step][1])
    GPIO.output(in3, fullStepSequence[step][2])
    GPIO.output(in4, fullStepSequence[step][3])

def main():
    global content, currentStepM1, currentStepM2

    setup()
    try:
        while True:
            content = input("Enter command: ").strip().lower()

            if content == "rotatem":
                currentStepM1 = rotateMotor(M1_IN1, M1_IN2, M1_IN3, M1_IN4, 128, currentStepM1)
                print("Motor 1 rotated by 11.25 degrees")
            elif content == "rotatemm":
                currentStepM2 = rotateMotor(M2_IN1, M2_IN2, M2_IN3, M2_IN4, 56, currentStepM2)
                print("Motor 2 rotated by 5 degrees")
            elif content == "rotatemmm":
                currentStepM2 = rotateMotor(M2_IN1, M2_IN2, M2_IN3, M2_IN4, -56, currentStepM2)
                print("Motor 2 rotated by -5 degrees")

    except KeyboardInterrupt:
        print("Program interrupted")

    finally:
        GPIO.cleanup()
        print("GPIO cleaned up")

if __name__ == "__main__":
    main()
