// class to detect edges courtesy of Mark
class EdgeDetector {
  private:
    bool prevButtonState = false;
    bool risen = false;
    bool fallen = false;

  public:
    void update(bool currButtonState) {
      risen = !prevButtonState && currButtonState;
      fallen = prevButtonState && !currButtonState;

      prevButtonState = currButtonState;
    }

    bool hasRisen() { return risen; }
    bool hasFallen() { return fallen; }
};

// Buttons
const int OPEN_NOS2_B = 2;
const int CLOSE_NOS2_B = 3;
const int OPEN_NOS1_B = 4;
const int CLOSE_NOS1_B = 5;
const int OPEN_N2_B = 6;
const int CLOSE_N2_B = 7;
const int OPEN_ETOH_B = 8;
const int CLOSE_ETOH_B = 9;
const int START_1_B = 10;
const int FILL_1_B = 11;
const int FILL_2_B = 12;
const int FILL_3_B = 13;
const int ABORT_B = 16;
const int CHECK_STATE_B = 18;

// Switches
const int CLOSE_ALL_S = 14;
const int ACTIVATE_IGNITER_S = 15;
const int ACTIVATE_SERVOS_S = 17;

// LEDs
const int NOS2_OPENED_L = 18;
const int NOS1_OPENED_L = 19;
const int N2_OPENED_L = 20;
const int ETOH_OPENED_L = 21;
const int IGNITE_ACTIVATED_L = 22;

// Serial Check
uint8_t receiveAck = 0x00;
uint8_t bitMask = 0x01;

// enums
enum COMMANDS { 
  OPEN_NOS2 = 2,
  CLOSE_NOS2 = 3,
  OPEN_NOS1 = 4,
  CLOSE_NOS1 = 5,
  OPEN_N2 = 6,
  CLOSE_N2 = 7,
  OPEN_ETOH = 8,
  CLOSE_ETOH = 9,
  START_1 = 10,
  FILL_1 = 11,
  FILL_2 = 12,
  FILL_3 = 13,
  CLOSE_ALL = 14,
  DECLOSE_ALL = 14,
  ACTIVATE_IGNITER = 15,
  DEACTIVATE_IGNITER = 15,
  ABORT = 16,
  ACTIVATE_SERVOS = 17,
  DEACTIVATE_SERVOS = 17,
  DEABORT = 16,
  CHECK_STATE = 18
};

void setup() {
  Serial.begin(9600);

  // Initialize Buttons as INPUTs
  pinMode(OPEN_NOS2_B, INPUT);
  pinMode(CLOSE_NOS2_B, INPUT);
  pinMode(OPEN_NOS1_B, INPUT);
  pinMode(CLOSE_NOS1_B, INPUT);
  pinMode(OPEN_N2_B, INPUT);
  pinMode(CLOSE_N2_B, INPUT);
  pinMode(OPEN_ETOH_B, INPUT);
  pinMode(CLOSE_ETOH_B, INPUT);
  pinMode(START_1_B, INPUT);
  pinMode(FILL_1_B, INPUT);
  pinMode(FILL_2_B, INPUT);
  pinMode(FILL_3_B, INPUT);
  pinMode(ABORT_B, INPUT);
  pinMode(CHECK_STATE_B, INPUT);

  // Initialize Switches as INPUTs
  pinMode(CLOSE_ALL_S, INPUT);
  pinMode(ACTIVATE_IGNITER_S, INPUT);
  pinMode(ACTIVATE_SERVOS_S, INPUT);

  // Initialize LEDs
  pinMode(NOS2_OPENED_L, OUTPUT);
  pinMode(NOS1_OPENED_L, OUTPUT);
  pinMode(N2_OPENED_L, OUTPUT);
  pinMode(ETOH_OPENED_L, OUTPUT);
  pinMode(IGNITE_ACTIVATED_L, OUTPUT);
}

// Button Edge Detectors
EdgeDetector OPEN_NOS2_EDGE_DETECTOR;
EdgeDetector CLOSE_NOS2_EDGE_DETECTOR;
EdgeDetector OPEN_NOS1_EDGE_DETECTOR;
EdgeDetector CLOSE_NOS1_EDGE_DETECTOR;
EdgeDetector OPEN_N2_EDGE_DETECTOR;
EdgeDetector CLOSE_N2_EDGE_DETECTOR;
EdgeDetector OPEN_ETOH_EDGE_DETECTOR;
EdgeDetector CLOSE_ETOH_EDGE_DETECTOR;
EdgeDetector START_1_EDGE_DETECTOR;
EdgeDetector FILL_1_EDGE_DETECTOR;
EdgeDetector FILL_2_EDGE_DETECTOR;
EdgeDetector FILL_3_EDGE_DETECTOR;
EdgeDetector ABORT_EDGE_DETECTOR;
EdgeDetector CHECK_STATE_EDGE_DETECTOR;

// Switch Edge Detectors
EdgeDetector CLOSE_ALL_EDGE_DETECTOR;
EdgeDetector ACTIVATE_IGNITER_EDGE_DETECTOR;
EdgeDetector ACTIVATE_SERVOS_EDGE_DETECTOR;

void loop() {
  // Check if there are open valves
  if (Serial.available() > 0) {
    // Receive serial value
    receiveAck = Serial.read();

    bitMask = 0x01;
    // Check serial bits to see which valves are open
    // and turn on corresponding LED
    while (bitMask != 0x20)
    {
      if (receiveAck & bitMask && bitMask == 0x01)
      {
        digitalWrite(NOS2_OPENED_L, HIGH);
      }
      bitMask = bitMask << 1;

      if (receiveAck & bitMask && bitMask == 0x02)
      {
        digitalWrite(NOS1_OPENED_L, HIGH);
      }
      bitMask = bitMask << 1;
      
      if (receiveAck & bitMask && bitMask == 0x04)
      {
        digitalWrite(N2_OPENED_L, HIGH);
      }
      bitMask = bitMask << 1;
      
      if (receiveAck & bitMask && bitMask == 0x08)
      {
        digitalWrite(ETOH_OPENED_L, HIGH);
      }
      bitMask = bitMask << 1;

      if (receiveAck & bitMask && bitMask == 0x10)
      {
        digitalWrite(IGNITE_ACTIVATED_L, HIGH);
      }
    }
  }

  // Buttons

  // Edge Detection for OPEN_NOS2 Button | Pin 2
  OPEN_NOS2_EDGE_DETECTOR.update(!digitalRead(OPEN_NOS2_B));
  if (OPEN_NOS2_EDGE_DETECTOR.hasRisen()) {
    Serial.write(OPEN_NOS2);
  }

  // Edge Detection for CLOSE_NOS2 Button | Pin 3
  CLOSE_NOS2_EDGE_DETECTOR.update(!digitalRead(CLOSE_NOS2_B));
  if (CLOSE_NOS2_EDGE_DETECTOR.hasRisen()) {
    Serial.write(CLOSE_NOS2);
  }

  // Edge Detection for OPEN_NOS1 Button | Pin 4
  OPEN_NOS1_EDGE_DETECTOR.update(!digitalRead(OPEN_NOS1_B));
  if (OPEN_NOS1_EDGE_DETECTOR.hasRisen()) {
    Serial.write(OPEN_NOS1);
  }

  // Edge Detection for CLOSE_NOS1 Button | Pin 5
  CLOSE_NOS1_EDGE_DETECTOR.update(!digitalRead(CLOSE_NOS1_B));
  if (CLOSE_NOS1_EDGE_DETECTOR.hasRisen()) {
    Serial.write(CLOSE_NOS1);
  }

  // Edge Detection for OPEN_N2 Button | Pin 6
  OPEN_N2_EDGE_DETECTOR.update(!digitalRead(OPEN_N2_B));
  if (OPEN_N2_EDGE_DETECTOR.hasRisen()) {
    Serial.write(OPEN_N2);
  }

  // Edge Detection for CLOSE_N2 Button | Pin 7
  CLOSE_N2_EDGE_DETECTOR.update(!digitalRead(CLOSE_N2_B));
  if (CLOSE_N2_EDGE_DETECTOR.hasRisen()) {
    Serial.write(CLOSE_N2);
  }

  // Edge Detection for OPEN_ETOH Button | Pin 8
  OPEN_ETOH_EDGE_DETECTOR.update(!digitalRead(OPEN_ETOH_B));
  if (OPEN_ETOH_EDGE_DETECTOR.hasRisen()) {
    Serial.write(OPEN_ETOH);
  }

  // Edge Detection for CLOSE_ETOH Button | Pin 9
  CLOSE_ETOH_EDGE_DETECTOR.update(!digitalRead(CLOSE_ETOH_B));
  if (CLOSE_ETOH_EDGE_DETECTOR.hasRisen()) {
    Serial.write(CLOSE_ETOH);
  }

  // Edge Detection for START_1 Button | Pin 10
  START_1_EDGE_DETECTOR.update(!digitalRead(START_1_B));
  if (START_1_EDGE_DETECTOR.hasRisen()) {
    Serial.write(START_1);
  }

  // Edge Detection for FILL_1 Button | Pin 11
  FILL_1_EDGE_DETECTOR.update(!digitalRead(FILL_1_B));
  if (FILL_1_EDGE_DETECTOR.hasRisen()) {
    Serial.write(FILL_1);
  }

  // Edge Detection for FILL_2 Button | Pin 12
  FILL_2_EDGE_DETECTOR.update(!digitalRead(FILL_2_B));
  if (FILL_2_EDGE_DETECTOR.hasRisen()) {
    Serial.write(FILL_2);
  }

  // Edge Detection for FILL_3 Button | Pin 13
  FILL_3_EDGE_DETECTOR.update(!digitalRead(FILL_3_B));
  if (FILL_3_EDGE_DETECTOR.hasRisen()) {
    Serial.write(FILL_3);
  }

  // Edge Detection for ABORT Button | Pin 16
  ABORT_EDGE_DETECTOR.update(!digitalRead(ABORT_B));
  if (ABORT_EDGE_DETECTOR.hasRisen()) {
    Serial.write(ABORT);
  }
  else if (ABORT_EDGE_DETECTOR.hasFallen()) {
    Serial.write(DEABORT);
  }

  // Edge Detection for CHECK_STATE Button | Pin 18
  CHECK_STATE_EDGE_DETECTOR.update(!digitalRead(CHECK_STATE_B));
  if (CHECK_STATE_EDGE_DETECTOR.hasRisen()) {
    Serial.write(CHECK_STATE);
  }

  // Switches

  // Edge Detection for CLOSE_ALL Switch | Pin 14
  CLOSE_ALL_EDGE_DETECTOR.update(!digitalRead(CLOSE_ALL_S));
  if (CLOSE_ALL_EDGE_DETECTOR.hasRisen()) {
    Serial.write(CLOSE_ALL);
  }
  else if (CLOSE_ALL_EDGE_DETECTOR.hasFallen()) {
    Serial.write(DECLOSE_ALL);
  }

  // Edge Detection for ACTIVATE_IGNITER Switch | Pin 15
  ACTIVATE_IGNITER_EDGE_DETECTOR.update(!digitalRead(ACTIVATE_IGNITER_S));
  if (ACTIVATE_IGNITER_EDGE_DETECTOR.hasRisen()) {
    Serial.write(ACTIVATE_IGNITER);
  }
  else if (ACTIVATE_IGNITER_EDGE_DETECTOR.hasFallen()) {
    Serial.write(DEACTIVATE_IGNITER);
  }

  // Edge Detection for ACTIVATE_SERVOS Button | Pin 17
  ACTIVATE_SERVOS_EDGE_DETECTOR.update(!digitalRead(ACTIVATE_SERVOS_S));
  if (ACTIVATE_SERVOS_EDGE_DETECTOR.hasRisen()) {
    Serial.write(ACTIVATE_SERVOS);
  }
  else if (ACTIVATE_SERVOS_EDGE_DETECTOR.hasFallen()) {
    Serial.write(DEACTIVATE_SERVOS);
  }

  // Delay to prevent buttons from sending more than one signal
  delay(100);
}
