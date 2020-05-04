
#define ENCODER_A 8
#define ENCODER_B 9


void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
}

int getEncoderState() {
  return digitalRead(ENCODER_B) | (digitalRead(ENCODER_A) << 1);
}

int updateEncoder() {
  int encoderState = digitalRead(ENCODER_B) | (digitalRead(ENCODER_A) << 1);
  static int encoderPreviousState = encoderState;
  static int encoderValue = 0;
  static int rotation = 0;
  switch (encoderPreviousState) {
    case 0b11:
      switch (encoderState) {
        case 0b11:
          break;
        case 0b01:
          rotation++;
          encoderPreviousState = encoderState;
          break;
        case 0b10:
          rotation--;
          encoderPreviousState = encoderState;
          break;
        default:
          rotation = 0;
      }
      break;
    case 0b10:
      switch (encoderState) {
        case 0b10:
          break;
        case 0b11:
          rotation++;
          encoderPreviousState = encoderState;
          break;
        case 0b00:
          rotation--;
          encoderPreviousState = encoderState;
          break;
        default:
          rotation = 0;
      }
      break;
    case 0b01:
      switch (encoderState) {
        case 0b01:
          break;
        case 0b00:
          rotation++;
          encoderPreviousState = encoderState;
          break;
        case 0b11:
          rotation--;
          encoderPreviousState = encoderState;
          break;
        default:
          rotation = 0;
      }
      break;
    case 0b00:
      switch (encoderState) {
        case 0b00:
          break;
        case 0b10:
          rotation++;
          encoderPreviousState = encoderState;
          break;
        case 0b01:
          rotation--;
          encoderPreviousState = encoderState;
          break;
        default:
          rotation = 0;
      }
      break;
  }
  switch (rotation) {
    case 3:
      encoderValue++;
      rotation = 0;
      break;
    case -3:
      encoderValue--;
      rotation = 0;
      break;
  }
  return encoderValue;
}

int prevEncoderValue = 0;
void loop() {
  int encoderValue = updateEncoder();
  if (prevEncoderValue != encoderValue) {
    Serial.println(encoderValue);
    prevEncoderValue = encoderValue;
  }
}
