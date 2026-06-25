#include "MotorController.h"

MotorController::MotorController(uint8_t in1, uint8_t in2, uint8_t ena,
                                  uint8_t in3, uint8_t in4, uint8_t enb)
  : _in1(in1), _in2(in2), _ena(ena),
    _in3(in3), _in4(in4), _enb(enb),
    _speedA(180), _speedB(180) {}

void MotorController::begin() {
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
  pinMode(_ena, OUTPUT);
  pinMode(_in3, OUTPUT);
  pinMode(_in4, OUTPUT);
  pinMode(_enb, OUTPUT);
  stop();
}

void MotorController::forward() {
  digitalWrite(_in1, HIGH);  digitalWrite(_in2, LOW);
  digitalWrite(_in3, HIGH);  digitalWrite(_in4, LOW);
  analogWrite(_ena, _speedA);
  analogWrite(_enb, _speedB);
}

void MotorController::backward() {
  digitalWrite(_in1, LOW);   digitalWrite(_in2, HIGH);
  digitalWrite(_in3, LOW);   digitalWrite(_in4, HIGH);
  analogWrite(_ena, _speedA);
  analogWrite(_enb, _speedB);
}

void MotorController::turnLeft() {
  digitalWrite(_in1, LOW);   digitalWrite(_in2, HIGH);
  digitalWrite(_in3, HIGH);  digitalWrite(_in4, LOW);
  analogWrite(_ena, _speedA);
  analogWrite(_enb, _speedB);
}

void MotorController::turnRight() {
  digitalWrite(_in1, HIGH);  digitalWrite(_in2, LOW);
  digitalWrite(_in3, LOW);   digitalWrite(_in4, HIGH);
  analogWrite(_ena, _speedA);
  analogWrite(_enb, _speedB);
}

void MotorController::stop() {
  digitalWrite(_in1, LOW); digitalWrite(_in2, LOW);
  digitalWrite(_in3, LOW); digitalWrite(_in4, LOW);
  analogWrite(_ena, 0);
  analogWrite(_enb, 0);
}

void MotorController::setSpeed(uint8_t speed) {
  _speedA = speed;
  _speedB = speed;
}

void MotorController::setSpeedA(uint8_t speed) {
  _speedA = speed;
}

void MotorController::setSpeedB(uint8_t speed) {
  _speedB = speed;
}

uint8_t MotorController::getSpeedA() const {
  return _speedA;
}

uint8_t MotorController::getSpeedB() const {
  return _speedB;
}
