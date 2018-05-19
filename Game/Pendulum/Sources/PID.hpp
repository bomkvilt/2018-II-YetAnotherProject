#ifndef PID_HPP
#define PID_HPP
#pragma once


class PID
{
public:

	PID(float Kp, float Ki, float Kd, float min = -1, float max = 1);

	float GetValue(float input, float dt);

	void SetKp(float newKp);
	void SetKi(float newKi);
	void SetKd(float newKd);

	void FlushSumm();
	void FlushInput();

protected:

	float lastSumm;
	float lastInput;

	float Kp;
	float Ki;
	float Kd;
	float min;
	float max;
};



#endif // !PID_HPP
