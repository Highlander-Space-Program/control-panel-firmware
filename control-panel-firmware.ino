#include <XBee.h>

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

// Counters
int presscnt;
int unpresscnt;
int buzzcnt = 50;

// Buttons
const int OPEN_NOS2_B = 6;
const int CLOSE_NOS2_B = 3;
const int OPEN_NOS1_B = 7;
const int CLOSE_NOS1_B = 24;
const int OPEN_N2_B = 8;
const int CLOSE_N2_B = 4;
const int OPEN_ETOH_B = 9;
const int CLOSE_ETOH_B = 5;
//const int FILL_1_B = 11;
//const int FILL_2_B = 12;
//const int FILL_3_B = 13;
const int ABORT_B = 12;
const int CHECK_STATE_B = 18;

// Switches
const int CLOSE_ALL_S = 10;
const int ACTIVATE_IGNITER_S = 11;
const int ACTIVATE_SERVOS_S = 2;
const int START_1_S = 25;

// LEDs
const int NOS2_OPENED_L = 33;
const int NOS1_OPENED_L = 35;
const int N2_OPENED_L = 37;
const int ETOH_OPENED_L = 39;
const int IGNITE_ACTIVATED_L = 41;
const int RECV_ACK = 27;

// Serial Check
uint8_t receiveAck = 0x00;
uint8_t bitMask = 0x01;

// LED Booleans
bool NOS2_OPEN = false;
bool NOS1_OPEN = false;
bool N2_OPEN = false;
bool ETOH_OPEN = false;
bool IGNITE_ACTIVE = false;

// Buzzer
const int BUZZER = 23;

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
  CHECK_STATE = 20,
  DESTART = 21
};

// XBee
XBee xbee = XBee();

enum abortState {ABORT_DEACTIVATED, ABORT_ACTIVATED, ABORT_PRESSED, ABORT_RELEASED} abortState = ABORT_DEACTIVATED;

void setup() {
  Serial.begin(9600);

  // Initialize xbee to use hardware serial
  xbee.setSerial(Serial);

  // Initialize Buttons as INPUTs
  pinMode(OPEN_NOS2_B, INPUT);
  pinMode(CLOSE_NOS2_B, INPUT);
  pinMode(OPEN_NOS1_B, INPUT);
  pinMode(CLOSE_NOS1_B, INPUT);
  pinMode(OPEN_N2_B, INPUT);
  pinMode(CLOSE_N2_B, INPUT);
  pinMode(OPEN_ETOH_B, INPUT);
  pinMode(CLOSE_ETOH_B, INPUT);

  // pinMode(FILL_1_B, INPUT);
  // pinMode(FILL_2_B, INPUT);
  // pinMode(FILL_3_B, INPUT);
  pinMode(ABORT_B, INPUT_PULLUP);
  pinMode(CHECK_STATE_B, INPUT);

  // Initialize Switches as INPUTs
  pinMode(CLOSE_ALL_S, INPUT_PULLUP);
  pinMode(ACTIVATE_IGNITER_S, INPUT_PULLUP);
  pinMode(ACTIVATE_SERVOS_S, INPUT_PULLUP);
  pinMode(START_1_S, INPUT_PULLUP);

  // Initialize LEDs
  pinMode(NOS2_OPENED_L, OUTPUT);
  pinMode(NOS1_OPENED_L, OUTPUT);
  pinMode(N2_OPENED_L, OUTPUT);
  pinMode(ETOH_OPENED_L, OUTPUT);
  pinMode(IGNITE_ACTIVATED_L, OUTPUT);
  pinMode(RECV_ACK, OUTPUT);

  // Initialize Buzzer
  pinMode(BUZZER, OUTPUT);
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

// Buffer for XBee
uint8_t txBuffer = 999;
bool bufferSet = false;

void loop() {
  // turn off buzzer and received ack light
  digitalWrite(BUZZER, LOW);
  digitalWrite(RECV_ACK, LOW);

  XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x0000FFFF);

  // Check if there are open valves
  if (Serial.available() > 0) {
    // Receive serial value
    receiveAck = Serial.read();
    digitalWrite(RECV_ACK, HIGH);
    digitalWrite(BUZZER, HIGH);

    // IGNITE ack
    if (receiveAck & bitMask) {
      digitalWrite(IGNITE_ACTIVATED_L, HIGH);
    }
    else {
      digitalWrite(IGNITE_ACTIVATED_L, LOW);
    }
    receiveAck = receiveAck >> 1;

    // ETOH ack
    if (receiveAck & bitMask) {
      digitalWrite(ETOH_OPENED_L, HIGH);
    }
    else {
      digitalWrite(ETOH_OPENED_L, LOW);
    }
    receiveAck = receiveAck >> 1;

    // N2 ack
    if (receiveAck & bitMask) {
      digitalWrite(N2_OPENED_L, HIGH);
    }
    else {
      digitalWrite(N2_OPENED_L, LOW);
    }
    receiveAck = receiveAck >> 1;

    // NOS1 ack
    if (receiveAck & bitMask) {
      digitalWrite(NOS1_OPENED_L, HIGH);
    }
    else {
      digitalWrite(NOS1_OPENED_L, LOW);
    }
    receiveAck = receiveAck >> 1;

    // NOS2 ack
    if (receiveAck & bitMask) {
      digitalWrite(NOS2_OPENED_L, HIGH);
    }
    else {
      digitalWrite(NOS2_OPENED_L, LOW);
    }
  }

  // Buttons

  // Edge Detection for OPEN_NOS2 Button | Pin 2
  OPEN_NOS2_EDGE_DETECTOR.update(!digitalRead(OPEN_NOS2_B));
  if (OPEN_NOS2_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(OPEN_NOS2);
    txBuffer = OPEN_NOS2;
    bufferSet = true;
  }

  // Edge Detection for CLOSE_NOS2 Button | Pin 3
  CLOSE_NOS2_EDGE_DETECTOR.update(!digitalRead(CLOSE_NOS2_B));
  if (CLOSE_NOS2_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(CLOSE_NOS2);
    txBuffer = CLOSE_NOS2;
    bufferSet = true;
  }

  // Edge Detection for OPEN_NOS1 Button | Pin 4
  OPEN_NOS1_EDGE_DETECTOR.update(!digitalRead(OPEN_NOS1_B));
  if (OPEN_NOS1_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(OPEN_NOS1);
    txBuffer = OPEN_NOS1;
    bufferSet = true;
  }

  // Edge Detection for CLOSE_NOS1 Button | Pin 5
  CLOSE_NOS1_EDGE_DETECTOR.update(!digitalRead(CLOSE_NOS1_B));
  if (CLOSE_NOS1_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(CLOSE_NOS1);
    txBuffer = CLOSE_NOS1;
    bufferSet = true;
  }

  // Edge Detection for OPEN_N2 Button | Pin 6
  OPEN_N2_EDGE_DETECTOR.update(!digitalRead(OPEN_N2_B));
  if (OPEN_N2_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(OPEN_N2);
    txBuffer = OPEN_N2;
    bufferSet = true;
  }

  // Edge Detection for CLOSE_N2 Button | Pin 7
  CLOSE_N2_EDGE_DETECTOR.update(!digitalRead(CLOSE_N2_B));
  if (CLOSE_N2_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(CLOSE_N2);
    txBuffer = CLOSE_N2;
    bufferSet = true;
  }

  // Edge Detection for OPEN_ETOH Button | Pin 8
  OPEN_ETOH_EDGE_DETECTOR.update(!digitalRead(OPEN_ETOH_B));
  if (OPEN_ETOH_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(OPEN_ETOH);
    txBuffer = OPEN_ETOH;
    bufferSet = true;
  }

  // Edge Detection for CLOSE_ETOH Button | Pin 9
  CLOSE_ETOH_EDGE_DETECTOR.update(!digitalRead(CLOSE_ETOH_B));
  if (CLOSE_ETOH_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(CLOSE_ETOH);
    txBuffer = CLOSE_ETOH;
    bufferSet = true;
  }

  // Edge Detection for START_1 Switch | Pin 10
  START_1_EDGE_DETECTOR.update(!digitalRead(START_1_S));
  if (START_1_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(START_1);
    txBuffer = START_1;
    bufferSet = true;
  }
  else if (START_1_EDGE_DETECTOR.hasFallen()) {
    // Serial.write(DESTART);
    txBuffer = DESTART;
    bufferSet = true;
  }

/*
  // Edge Detection for FILL_1 Button | Pin 11
  FILL_1_EDGE_DETECTOR.update(!digitalRead(FILL_1_B));
  if (FILL_1_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(FILL_1);
    txBuffer = FILL_1;
  }

  // Edge Detection for FILL_2 Button | Pin 12
  FILL_2_EDGE_DETECTOR.update(!digitalRead(FILL_2_B));
  if (FILL_2_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(FILL_2);
    txBuffer = FILL_2;
  }

  // Edge Detection for FILL_3 Button | Pin 13
  FILL_3_EDGE_DETECTOR.update(!digitalRead(FILL_3_B));
  if (FILL_3_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(FILL_3);
    txBuffer = FILL_3;
  }
*/
  
  // Edge Detection for ABORT Button | Pin 16
  ABORT_EDGE_DETECTOR.update(!digitalRead(ABORT_B));
  if (ABORT_EDGE_DETECTOR.hasFallen()) {
    // Serial.write(ABORT);
    txBuffer = ABORT;
    bufferSet = true;
  }
  else if (ABORT_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(DEABORT);
    txBuffer = DEABORT;
    bufferSet = true;
  }

  // Edge Detection for CHECK_STATE Button | Pin 18
  CHECK_STATE_EDGE_DETECTOR.update(!digitalRead(CHECK_STATE_B));
  if (CHECK_STATE_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(CHECK_STATE);
    txBuffer = CHECK_STATE;
    bufferSet = true;
  }

  // Switches

  // Edge Detection for CLOSE_ALL Switch | Pin 14
  CLOSE_ALL_EDGE_DETECTOR.update(!digitalRead(CLOSE_ALL_S));
  if (CLOSE_ALL_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(CLOSE_ALL);
    txBuffer = CLOSE_ALL;
    bufferSet = true;
  }
  else if (CLOSE_ALL_EDGE_DETECTOR.hasFallen()) {
    // Serial.write(DECLOSE_ALL);
    txBuffer = DECLOSE_ALL;
    bufferSet = true;
  }

  // Edge Detection for ACTIVATE_IGNITER Switch | Pin 15
  ACTIVATE_IGNITER_EDGE_DETECTOR.update(!digitalRead(ACTIVATE_IGNITER_S));
  if (ACTIVATE_IGNITER_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(ACTIVATE_IGNITER);
    txBuffer = ACTIVATE_IGNITER;
    bufferSet = true;
  }
  else if (ACTIVATE_IGNITER_EDGE_DETECTOR.hasFallen()) {
    // Serial.write(DEACTIVATE_IGNITER);
    txBuffer = DEACTIVATE_IGNITER;
    bufferSet = true;
  }

  // Edge Detection for ACTIVATE_SERVOS Button | Pin 17
  ACTIVATE_SERVOS_EDGE_DETECTOR.update(!digitalRead(ACTIVATE_SERVOS_S));
  if (ACTIVATE_SERVOS_EDGE_DETECTOR.hasRisen()) {
    // Serial.write(ACTIVATE_SERVOS);
    txBuffer = ACTIVATE_SERVOS;
    bufferSet = true;
  }
  else if (ACTIVATE_SERVOS_EDGE_DETECTOR.hasFallen()) {
    // Serial.write(DEACTIVATE_SERVOS);
    txBuffer = DEACTIVATE_SERVOS;
    bufferSet = true;
  }

  // if 2 seconds have passed turn on buzzer
  if (buzzcnt > 20) {
    digitalWrite(BUZZER, HIGH);
    buzzcnt = 0;
  }
  ++buzzcnt;

  ZBTxRequest zbTx = ZBTxRequest(addr64, txBuffer, sizeof(txBuffer));

  if (bufferSet == true)
  {
    xbee.send(zbTx);
  }

  bufferSet = false;

  // Delay to prevent buttons from sending more than one signal
  delay(100);
}
