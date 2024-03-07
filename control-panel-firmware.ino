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
const int OPEN_NOS2_B = 6;
const int CLOSE_NOS2_B = 3;
const int OPEN_NOS1_B = 7;
const int CLOSE_NOS1_B = 24;
const int OPEN_N2_B = 8;
const int CLOSE_N2_B = 4;
const int OPEN_ETOH_B = 9;
const int CLOSE_ETOH_B = 5;
const int START_1_B = 25;
//const int FILL_1_B = 11;
//const int FILL_2_B = 12;
//const int FILL_3_B = 13;
const int ABORT_B = 12;
const int CHECK_STATE_B = 18;

// Switches
const int CLOSE_ALL_S = 10;
const int ACTIVATE_IGNITER_S = 11;
const int ACTIVATE_SERVOS_S = 2;

// LEDs
const int NOS2_OPENED_L = 18;
const int NOS1_OPENED_L = 19;
const int N2_OPENED_L = 20;
const int ETOH_OPENED_L = 21;
const int IGNITE_ACTIVATED_L = 22;

// Serial Check
uint8_t receiveAck = 0x00;
uint8_t bitMask = 0x01;

// LED Booleans
bool NOS2_OPEN = false;
bool NOS1_OPEN = false;
bool N2_OPEN = false;
bool ETOH_OPEN = false;
bool IGNITE_ACTIVE = false;

// enums
enum COMMANDS {
  OPEN_NOS2 = 0,
  CLOSE_NOS2 = 1,
  OPEN_NOS1 = 2,
  CLOSE_NOS1 = 3,
  OPEN_N2 = 4,
  CLOSE_N2 = 5,
  OPEN_ETOH = 6,
  CLOSE_ETOH = 7,
  START_1 = 8,
  FILL_1 = 9,
  FILL_2 = 10,
  FILL_3 = 11,
  CLOSE_ALL = 12,
  DECLOSE_ALL = 13,
  ACTIVATE_IGNITER = 14,
  DEACTIVATE_IGNITER = 15,
  ABORT = 16,
  ACTIVATE_SERVOS = 17,
  DEACTIVATE_SERVOS = 18,
  DEABORT = 19,
  CHECK_STATE = 20
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
  pinMode(START_1_B, INPUT_PULLUP);
  //pinMode(FILL_1_B, INPUT);
  //pinMode(FILL_2_B, INPUT);
  //pinMode(FILL_3_B, INPUT);
  pinMode(ABORT_B, INPUT_PULLUP);
  pinMode(CHECK_STATE_B, INPUT);

  // Initialize Switches as INPUTs
  pinMode(CLOSE_ALL_S, INPUT_PULLUP);
  pinMode(ACTIVATE_IGNITER_S, INPUT_PULLUP);
  pinMode(ACTIVATE_SERVOS_S, INPUT_PULLUP);

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

    // Is NOS2 valve open?
    if (receiveAck & bitMask)
    {
      if (NOS2_OPEN == false)
      {
        NOS2_OPEN = true;
        digitalWrite(NOS2_OPENED_L, HIGH);
      }
      else
      {
        NOS2_OPEN = false;
        digitalWrite(NOS2_OPENED_L, LOW);
      }
    }
    receiveAck = receiveAck >> 1;

    // Is NOS1 valve open?
    if (receiveAck & bitMask)
    {
      if (NOS1_OPEN == false)
      {
        NOS1_OPEN = true;
        digitalWrite(NOS1_OPENED_L, HIGH);
      }
      else
      {
        NOS1_OPEN = false;
        digitalWrite(NOS1_OPENED_L, LOW);
      }
    }
    receiveAck = receiveAck >> 1;
      
    // Is N2 valve open?
    if (receiveAck & bitMask)
    {
      if (N2_OPEN == false)
      {
        N2_OPEN = true;
        digitalWrite(N2_OPENED_L, HIGH);
      }
      else
      {
        N2_OPEN = false;
        digitalWrite(N2_OPENED_L, LOW);
      }
    }
    receiveAck = receiveAck >> 1;
      
    // Is ETOH valve open?
    if (receiveAck & bitMask)
    {
      if (ETOH_OPEN == false)
      {
        ETOH_OPEN = true;
        digitalWrite(ETOH_OPENED_L, HIGH);
      }
      else
      {
        ETOH_OPEN = false;
        digitalWrite(ETOH_OPENED_L, LOW);
      }
    }
    receiveAck = receiveAck >> 1;

    // Is IGNITE active?
    if (receiveAck & bitMask)
    {
      if (IGNITE_ACTIVE == false)
      {
        IGNITE_ACTIVE = true;
        digitalWrite(IGNITE_ACTIVATED_L, HIGH);
      }
      else
      {
        IGNITE_ACTIVE = false;
        digitalWrite(IGNITE_ACTIVATED_L, LOW);
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

/*
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
  */

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
