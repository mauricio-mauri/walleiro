#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(uint8_t trig, uint8_t echo, unsigned int maxDistance)
  : _sonar(trig, echo, maxDistance) {}

unsigned int UltrasonicSensor::readDistance() {
  return _sonar.ping_cm();
}

bool UltrasonicSensor::obstacleDetected(unsigned int threshold) {
  unsigned int d = readDistance();
  return (d > 0 && d <= threshold);
}
