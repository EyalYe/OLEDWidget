#include <Wire.h>
#include <MPU6050_light.h>

// Create an MPU6050 object using the default I2C interface.
MPU6050 mpu(Wire);

// Calibration period in milliseconds (e.g., 5000ms = 5 seconds)
const unsigned long calibrationPeriod = 5000;

// Variables to accumulate sensor values during calibration.
float accX_sum = 0, accY_sum = 0, accZ_sum = 0;
float gyroX_sum = 0, gyroY_sum = 0, gyroZ_sum = 0;
unsigned long sampleCount = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial Monitor to open (optional)

  // Initialize I2C communication.
  Wire.begin();
  
  // Initialize MPU6050.
  byte status = mpu.begin();
  if (status != 0) {
    Serial.print("MPU6050 initialization failed with status: ");
    Serial.println(status);
    while (1); // Halt if initialization fails.
  }
  
  // Allow sensor to stabilize.
  delay(1000);
  
  Serial.println("Keep the MPU6050 sensor completely still for calibration.");
  Serial.println("Calibrating...");

  // Record the starting time.
  unsigned long startTime = millis();

  // Gather sensor data for the calibrationPeriod.
  while (millis() - startTime < calibrationPeriod) {
    mpu.update(); // Fetch latest sensor data.
    
    // Accumulate raw accelerometer values.
    accX_sum += mpu.getAccX();
    accY_sum += mpu.getAccY();
    accZ_sum += mpu.getAccZ();
    
    // Accumulate raw gyroscope values.
    gyroX_sum += mpu.getGyroX();
    gyroY_sum += mpu.getGyroY();
    gyroZ_sum += mpu.getGyroZ();
    
    sampleCount++;
    delay(5); // Small delay between samples.
  }
  
  // Compute average offsets.
  float accX_offset = accX_sum / sampleCount;
  float accY_offset = accY_sum / sampleCount;
  // For the accelerometer Z-axis, assuming the sensor is laying flat,
  // the reading in g's should be approximately 1g when at rest.
  float accZ_offset = (accZ_sum / sampleCount) - 1.0;
  float gyroX_offset = gyroX_sum / sampleCount;
  float gyroY_offset = gyroY_sum / sampleCount;
  float gyroZ_offset = gyroZ_sum / sampleCount;
  
  // Output the calculated calibration offsets to the Serial Monitor.
  Serial.println("Calibration complete. The computed offsets are:");
  Serial.print("Accelerometer X offset: "); Serial.println(accX_offset, 4);
  Serial.print("Accelerometer Y offset: "); Serial.println(accY_offset, 4);
  Serial.print("Accelerometer Z offset: "); Serial.println(accZ_offset, 4);
  Serial.print("Gyroscope X offset: "); Serial.println(gyroX_offset, 4);
  Serial.print("Gyroscope Y offset: "); Serial.println(gyroY_offset, 4);
  Serial.print("Gyroscope Z offset: "); Serial.println(gyroZ_offset, 4);
  
  // At this point, you can copy these numbers to your main sketch and hardcode them.
  // For example:
  //   #define ACC_X_OFFSET 0.1234
  //   #define ACC_Y_OFFSET -0.5678
  //   ...
}

void loop() {
  // Do nothing once calibration is complete.
}
