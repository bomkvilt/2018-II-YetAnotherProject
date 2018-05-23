#include "PID.hpp"

#include <cmath>



PID::PID(float Kp, float Ki, float Kd, float min, float max)
	: lastSumm(0)
	, lastInput(0)
	, Kp(Kp)
	, Ki(Ki)
	, Kd(Kd)
	, min(min)
	, max(max)
{}

float PID::GetValue(float input, float dt)
{
	float summ = lastSumm + Ki * dt * input;
	float diff = Kd * (input - lastInput) / dt;
	float prop = Kp * input;

	float result = prop + summ + diff;
	result = std::fmax(result, min);
	result = std::fmin(result, max);

	lastSumm = summ;
	lastInput = input;

	return result;
}

void PID::SetKp(float newKp)
{
	Kp = newKp;
}

void PID::SetKi(float newKi)
{
	Ki = newKi;
}

void PID::SetKd(float newKd)
{
	Kd = newKd;
}

void PID::FlushSumm()
{
	lastSumm = 0;
}

void PID::FlushInput()
{
	lastInput = 0;
}
