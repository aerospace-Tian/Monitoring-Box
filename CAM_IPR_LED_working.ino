/*
  XIAO ESP32S3 Sense Camera 
  Takes a picture and turns on LEDs 
  Saves to MicroSD card in JPG format
  
  Tian Lan
*/

// Include required libraries
#include <Arduino.h>
#include <esp_camera.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
// Define camera model & pinout
#define CAMERA_MODEL_XIAO_ESP32S3  // Has PSRAM
#include "camera_pins.h"
//#define DEBUG

/////////////////////////////
#define LED_PIN 2    // GPIO pin connected to the NeoPixel DIN
#define PIR_PIN 3    // GPIO pin connected to the PIR sensor OUT
#define NUM_LEDS 12   // Number of LEDs in the NeoPixel ring

// Define the number of pictures to take when motion is detected
int numPictures = 2; // Change this value to the desired number of pictures

// Define the duration (in milliseconds) to keep the LEDs on after motion is detected
unsigned long ledOnTime = 2000; // 5000 milliseconds = 5 seconds

// Create an Adafruit_NeoPixel object
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

/////////////////////////////

// Camera status variable
bool camera_status = false;

// MicroSD status variable
bool sd_status = false;

// File Counter
int fileCount = 1;


// Save pictures to SD card
void photo_save(const char *fileName) {
  // Take a photo
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Failed to get camera frame buffer");
    return;
  }
  // Save photo to file
  writeFile(SD, fileName, fb->buf, fb->len);

  // Release image buffer
  esp_camera_fb_return(fb);

  Serial.println("Photo saved to file");
}

// SD card write file
void writeFile(fs::FS &fs, const char *path, uint8_t *data, size_t len) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.write(data, len) == len) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}




// Camera Parameters for setup
void CameraParameters() {
  // Define camera parameters
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}



void setup() {
  // Start Serial Monitor, wait until port is ready
  Serial.begin(115200);
#ifdef DEBUG
  while (!Serial)
    ;
#endif
  // Define Camera Parameters and Initialize
  CameraParameters();

  // Camera is good, set status
  camera_status = true;
  Serial.println("Camera OK!");

  // Initialize the MicroSD card
  if (!SD.begin(21)) {
    Serial.println("Failed to mount MicroSD Card!");
    while (1)
      ;
  }

  // Determine what type of MicroSD card is mounted
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No MicroSD card inserted!");
    return;
  }

  // Print card type
  Serial.print("MicroSD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  // MicroSD is good, set status
  sd_status = true;

  // Initialize the NeoPixel strip
  strip.begin();
  strip.setBrightness(50);  // Set brightness (0-255)
  strip.show();  // Initialize all pixels to 'off'

  // Initialize the PIR sensor
  pinMode(PIR_PIN, INPUT);
}

void loop() {

  // Make sure the camera and MicroSD are ready
  if (camera_status && sd_status) {

  
      // Take picture and record sound
      if (digitalRead(PIR_PIN) == HIGH) {
      // Motion detected: turn on all LEDs to white
      Serial.println("Motion detected! Taking Pics&Turning on LEDs.");
      for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(255, 255, 255)); // White color
        strip.setBrightness(150); // 0-255
      }
      strip.show();  // Update the strip to show the new color
      
      for (int i = 0; i < numPictures; i++) {
        // Create image file name
        char imageFileName[32];
        sprintf(imageFileName, "/image%d.jpg", fileCount);

        // Take a picture
        photo_save(imageFileName);
        Serial.printf("Saving picture: %s\r\n", imageFileName);

        
        // Increment file counter
        fileCount++;
        // Add a delay between pictures (e.g., 1 second)
        //delay(1000);
      }
      
      
   /*   else {
    // No motion: turn off all LEDs
    Serial.println("No motion. Turning off LEDs.");
    strip.clear();  // Clear all pixels (turn off)
    strip.show();   // Update the strip to show the cleared pixels
  }  */
     // Keep LEDs on for the specified time
      delay(ledOnTime);

      // After the delay, turn off the LEDs
      Serial.println("Turning off LEDs.");
      strip.clear();  // Clear all pixels (turn off)
      strip.show();   // Update the strip to show the cleared pixels

    } else {
      // No motion: make sure LEDs are off
      Serial.println("No motion. LEDs are off.");
      strip.clear();  // Clear all pixels (turn off)
      strip.show();   // Update the strip to show the cleared pixels
    }
   
    delay(1000); //frequency of IPR detection
  }
}