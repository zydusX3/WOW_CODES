#include <Servo.h>

Servo esc; // ESC control object
#define ESC_PIN 9       // Pin for ESC
#define RX_TIMEOUT 100  // RX check interval (ms)

unsigned long stateStartTime = 0; // Start time of the current state
bool newCommand = false;          // Flag for new command

enum MotorState { OFF, P25, P50, P75, P100 };
MotorState currentState = OFF;

void setup() {
    Serial.begin(9600);      // Initialize RX
    esc.attach(ESC_PIN);     // Attach ESC to pin
    esc.writeMicroseconds(1000); // Set ESC to idle
    Serial.println("System initialized.");
}

void loop() {
    handleRxCommands(); // Non-blocking RX handler
    handleMotorState(); // Handle motor state transitions
}

void handleRxCommands() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();

        if (command == "OFF") {
            currentState = OFF;
            newCommand = true;
        } else if (command == "25P") {
            currentState = P25;
            newCommand = true;
        } else if (command == "50P") {
            currentState = P50;
            newCommand = true;
        } else if (command == "75P") {
            currentState = P75;
            newCommand = true;
        } else if (command == "100P") {
            currentState = P100;
            newCommand = true;
        }
    }
}

void handleMotorState() {
    static int subState = 0; // Sub-state for multi-step sequences
    unsigned long elapsedTime = millis() - stateStartTime;

    if (newCommand) {
        subState = 0;               // Reset sub-state
        stateStartTime = millis();  // Reset timer
        newCommand = false;
    }

    switch (currentState) {
        case OFF:
            esc.writeMicroseconds(1000); // 0% power
            break;

        case P25:
            if (elapsedTime < 50000) { // 50 seconds
                esc.writeMicroseconds(1250); // 25% power
            } else {
                currentState = OFF; // Transition to OFF
            }
            break;

        case P50:
            if (subState == 0 && elapsedTime < 30000) { // 30 seconds
                esc.writeMicroseconds(1500); // 50% power
            } else if (subState == 0) {
                subState = 1; // Move to next sub-state
                stateStartTime = millis();
            } else if (elapsedTime < 20000) { // 20 seconds
                esc.writeMicroseconds(1250); // 25% power
            } else {
                currentState = OFF;
            }
            break;

        case P75:
            if (subState == 0 && elapsedTime < 30000) { // 30 seconds
                esc.writeMicroseconds(1750); // 75% power
            } else if (subState == 0) {
                subState = 1; // Move to next sub-state
                stateStartTime = millis();
            } else if (elapsedTime < 20000) { // 20 seconds
                esc.writeMicroseconds(1250); // 25% power
            } else {
                currentState = OFF;
            }
            break;

        case P100:
            if (subState == 0 && elapsedTime < 25000) { // 25 seconds
                esc.writeMicroseconds(1900); // 90% power
            } else if (subState == 0) {
                subState = 1; // Move to next sub-state
                stateStartTime = millis();
            } else if (elapsedTime < 25000) { // 25 seconds
                esc.writeMicroseconds(1250); // 25% power
            } else {
                currentState = OFF;
            }
            break;
    }
}
