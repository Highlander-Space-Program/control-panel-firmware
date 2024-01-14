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
const int START_SEQ_1 = 2;
const int FILL_SEQ_1 = 3;
const int FILL_SEQ_2 = 4;
const int FILL_SEQ_3 = 5;
const int NOS_FV_08 = 6;
const int NOS_FV_04 = 7;
const int N2_FV_02 = 8;
const int ETOH_FV_03 = 9;
const int SHUT_DOWN = 10;
const int IGNITE_START = 11;
const int ABORT_CMD = 12;

// enums
enum COMMANDS { 
  NOS_VALVE_2_TOGGLE,
  NOS_VALVE_1_TOGGLE,
  N2_VALVE_TOGGLE,
  ETOH_FLOW_VALVE_TOGGLE,
  START_SEQUENCE_1,
  FILL_SEQUENCE_1,
  FILL_SEQUENCE_2,
  FILL_SEQUENCE_3,
  CLOSE_ALL,
  IGNITE,
  ABORT
};

void setup() {
  Serial.begin(9600);

  // Initialize buttons as INPUT_PULLUPs
  pinMode(START_SEQ_1, INPUT_PULLUP);
  pinMode(FILL_SEQ_1, INPUT_PULLUP);
  pinMode(FILL_SEQ_2, INPUT_PULLUP);
  pinMode(FILL_SEQ_3, INPUT_PULLUP);
  pinMode(NOS_FV_08, INPUT_PULLUP);
  pinMode(NOS_FV_04, INPUT_PULLUP);
  pinMode(N2_FV_02, INPUT_PULLUP);
  pinMode(ETOH_FV_03, INPUT_PULLUP);
  pinMode(SHUT_DOWN, INPUT_PULLUP);
  pinMode(IGNITE_START, INPUT_PULLUP);
  pinMode(ABORT_CMD, INPUT_PULLUP);
}

EdgeDetector START_SEQ_1_EDGE_DETECTOR;
EdgeDetector FILL_SEQ_1_EDGE_DETECTOR;
EdgeDetector FILL_SEQ_2_EDGE_DETECTOR;
EdgeDetector FILL_SEQ_3_EDGE_DETECTOR;
EdgeDetector NOS_FV_08_EDGE_DETECTOR;
EdgeDetector NOS_FV_04_EDGE_DETECTOR;
EdgeDetector N2_FV_02_EDGE_DETECTOR;
EdgeDetector ETOH_FV_03_EDGE_DETECTOR;
EdgeDetector SHUT_DOWN_EDGE_DETECTOR;
EdgeDetector IGNITE_START_EDGE_DETECTOR;
EdgeDetector ABORT_EDGE_DETECTOR;

void loop() {
  // Edge Detection for START_SEQ_1 Button | Pin 2
  START_SEQ_1_EDGE_DETECTOR.update(!digitalRead(START_SEQ_1));
  if (START_SEQ_1_EDGE_DETECTOR.hasFallen()) {
    Serial.write(START_SEQUENCE_1);
  }

  // Edge Detection for FILL_SEQ_1 Button | Pin 3
  FILL_SEQ_1_EDGE_DETECTOR.update(!digitalRead(FILL_SEQ_1));
  if (FILL_SEQ_1_EDGE_DETECTOR.hasFallen()) {
    Serial.write(FILL_SEQUENCE_1);
  }

  // Edge Detection for FILL_SEQ_2 Button | Pin 4
  FILL_SEQ_2_EDGE_DETECTOR.update(!digitalRead(FILL_SEQ_2));
  if (FILL_SEQ_2_EDGE_DETECTOR.hasFallen()) {
    Serial.write(FILL_SEQUENCE_2);
  }

  // Edge Detection for FILL_SEQ_3 Button | Pin 5
  FILL_SEQ_3_EDGE_DETECTOR.update(!digitalRead(FILL_SEQ_3));
  if (FILL_SEQ_3_EDGE_DETECTOR.hasFallen()) {
    Serial.write(FILL_SEQUENCE_3);
  }

  // Edge Detection for NOS_FV_08 Button | Pin 6
  NOS_FV_08_EDGE_DETECTOR.update(!digitalRead(NOS_FV_08));
  if (NOS_FV_08_EDGE_DETECTOR.hasFallen()) {
    Serial.write(NOS_VALVE_2_TOGGLE);
  }

  // Edge Detection for NOS_FV_04 Button | Pin 7
  NOS_FV_04_EDGE_DETECTOR.update(!digitalRead(NOS_FV_04));
  if (NOS_FV_04_EDGE_DETECTOR.hasFallen()) {
    Serial.write(NOS_VALVE_1_TOGGLE);
  }

  // Edge Detection for N2_FV_02 Button | Pin 8
  N2_FV_02_EDGE_DETECTOR.update(!digitalRead(N2_FV_02));
  if (N2_FV_02_EDGE_DETECTOR.hasFallen()) {
    Serial.write(N2_VALVE_TOGGLE);
  }

  // Edge Detection for ETOH_FV_03 Button | Pin 9
  ETOH_FV_03_EDGE_DETECTOR.update(!digitalRead(ETOH_FV_03));
  if (ETOH_FV_03_EDGE_DETECTOR.hasFallen()) {
    Serial.write(ETOH_FLOW_VALVE_TOGGLE);
  }

  // Edge Detection for SHUT_DOWN Button | Pin 10
  SHUT_DOWN_EDGE_DETECTOR.update(!digitalRead(SHUT_DOWN));
  if (SHUT_DOWN_EDGE_DETECTOR.hasFallen()) {
    Serial.write(CLOSE_ALL);
  }

  // Edge Detection for IGNITE_START Button | Pin 11
  IGNITE_START_EDGE_DETECTOR.update(!digitalRead(IGNITE_START));
  if (IGNITE_START_EDGE_DETECTOR.hasFallen()) {
    Serial.write(IGNITE);
  }

  // Edge Detection for ABORT Button | Pin 12
  ABORT_EDGE_DETECTOR.update(!digitalRead(ABORT_CMD));
  if (ABORT_EDGE_DETECTOR.hasFallen()) {
    Serial.write(ABORT);
  }

  delay(100);
}
