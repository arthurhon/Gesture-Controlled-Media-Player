#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <BleKeyboard.h>
#include <gesture_control_inferencing.h>

Adafruit_MPU6050 mpu;

#define N_SAMPLES EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE
#define AXES 3  

float features[N_SAMPLES];  
BleKeyboard bleKeyboard("Gesture Controller", "ESP32", 100); 

void setup() {

  Serial.begin(9600);
  Wire.begin();
  bleKeyboard.begin();

  if (bleKeyboard.isConnected()) {
    Serial.println("BLE Keyboard connected");
  }


  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);
  Serial.println("MPU6050 + Edge Impulse Inference Ready");
}

void loop() {

  uint16_t sampleIndex = 0;

  while (sampleIndex < N_SAMPLES / AXES) { 
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);


    features[sampleIndex * AXES + 0] = a.acceleration.x;
    features[sampleIndex * AXES + 1] = a.acceleration.y;
    features[sampleIndex * AXES + 2] = a.acceleration.z;

    sampleIndex++;

    delay(20);  
  }

  signal_t signal;
  int err = numpy::signal_from_buffer(features, N_SAMPLES, &signal);
  if (err != 0) {
    Serial.println("Failed to create signal");
    return;
  }

  ei_impulse_result_t result;
  EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);
  if (res != EI_IMPULSE_OK) {
    Serial.printf("ERR: %d\n", res);
    return;
  }

  Serial.println("Predictions:");
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    Serial.printf("%s:\t%.5f\n", result.classification[ix].label, result.classification[ix].value);
  }

  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
    if (result.classification[ix].value > 0.95) {   

      if (strcmp(result.classification[ix].label, "swipe_left") == 0) {
        Serial.println(">>> Detected SWIPE LEFT!");
        if (bleKeyboard.isConnected()) {
          bleKeyboard.press(KEY_MEDIA_PREVIOUS_TRACK);
          delay(100);
          bleKeyboard.release(KEY_MEDIA_PREVIOUS_TRACK);
          delay(300);
        }
      }

      else if (strcmp(result.classification[ix].label, "swipe_right") == 0) {
        Serial.println(">>> Detected SWIPE RIGHT!");
        if (bleKeyboard.isConnected()) {
           bleKeyboard.press(KEY_MEDIA_NEXT_TRACK);
           delay(100);
           bleKeyboard.release(KEY_MEDIA_NEXT_TRACK);
           delay(300);
        }
      }

      else if (strcmp(result.classification[ix].label, "poke") == 0) {
        Serial.println(">>> Detected POKE!");
        if (bleKeyboard.isConnected()) {
          bleKeyboard.press(KEY_MEDIA_PLAY_PAUSE);
          delay(100);
          bleKeyboard.release(KEY_MEDIA_PLAY_PAUSE);
          delay(300);
        }
      }

      else if (strcmp(result.classification[ix].label, "idle") == 0) {
        Serial.println(">>> Detected IDLE");
      }

      else if (strcmp(result.classification[ix].label, "clockwise") == 0) {
        Serial.println(">>> Detected CLOCKWISE!");
        if (bleKeyboard.isConnected()) {
          bleKeyboard.press(KEY_MEDIA_VOLUME_UP);
          delay(100);
          bleKeyboard.release(KEY_MEDIA_VOLUME_UP);
          delay(100);
          bleKeyboard.press(KEY_MEDIA_VOLUME_UP);
          delay(100);
          bleKeyboard.release(KEY_MEDIA_VOLUME_UP);
          delay(300);
        }
      }

      else if (strcmp(result.classification[ix].label, "anticlockwise") == 0) {
        Serial.println(">>> Detected ANTICLOCKWISE!");
        if (bleKeyboard.isConnected()) {
          bleKeyboard.press(KEY_MEDIA_VOLUME_DOWN);
          delay(100);
          bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);
          delay(100);
          bleKeyboard.press(KEY_MEDIA_VOLUME_DOWN);
          delay(100);
          bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);
          delay(300);
        }
      }

    }
  }
  Serial.println("-------------------------------");
}
