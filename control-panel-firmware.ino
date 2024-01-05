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

  // Initialize buttons as INPUTS
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
  pinMode(ABORT, INPUT_PULLUP);


}

EdgeDetector buttonEdgeDetector;
void loop() {
  // Edge Detection for START_SEQ_1 Button
  buttonEdgeDetector.update(!digitalRead(START_SEQ_1));
  if (buttonEdgeDetector.hasFallen()) {
    Serial.write(START_SEQUENCE_1);
  }

  // Edge Detection for FILL_SEQ_1 Button
  buttonEdgeDetector.update(!digitalRead(FILL_SEQ_1));
  if (buttonEdgeDetector.hasFallen()) {
    Serial.write(FILL_SEQUENCE_1);
  }

  // Edge Detection for FILL_SEQ_2 Button
  buttonEdgeDetector.update(!digitalRead(FILL_SEQ_2));
  if (buttonEdgeDetector.hasFallen()) {
    Serial.write(FILL_SEQUENCE_2);
  }

  // Edge Detection for FILL_SEQ_3 Button
  buttonEdgeDetector.update(!digitalRead(FILL_SEQ_3));
  if (buttonEdgeDetector.hasFallen()) {
    Serial.write(FILL_SEQUENCE_3);
  }

  // Edge Detection for NOS_FV_08 Button
  buttonEdgeDetector.update(!digitalRead(NOS_FV_08));
  if (buttonEdgeDetector.hasFallen()) {
    Serial.write(NOS_VALVE_2_TOGGLE);
  }

  // Edge Detection for NOS_FV_04 Button
  buttonEdgeDetector.update(!digitalRead(NOS_FV_04));
  if (buttonEdgeDetector.hasFallen()) {
    Serial.write(NOS_VALVE_1_TOGGLE);
  }

  // Edge Detection for N2_FV_02 Button
  buttonEdgeDetector.update(!digitalRead(N2_FV_02));
  if (buttonEdgeDetector.hasFallen()) {
    Serial.write(N2_VALVE_TOGGLE);
  }

  // Edge Detection for ETOH_FV_03 Button
  buttonEdgeDetector.update(!digitalRead(ETOH_FV_03));
  if (buttonEdgeDetector.hasFallen()) {
    Serial.write(ETOH_FLOW_VALVE_TOGGLE);
  }

  // Edge Detection for SHUT_DOWN Button
  buttonEdgeDetector.update(!digitalRead(SHUT_DOWN));
  if (buttonEdgeDetector.hasFallen()) {
    Serial.write(CLOSE_ALL);
  }

  // Edge Detection for IGNITE_START Button
  buttonEdgeDetector.update(!digitalRead(IGNITE_START));
  if (buttonEdgeDetector.hasFallen()) {
    Serial.write(IGNITE);
  }

  // Edge Detection for ABORT Button
  buttonEdgeDetector.update(!digitalRead(ABORT));
  if (buttonEdgeDetector.hasFallen()) {
    Serial.write(ABORT);
  }
}
