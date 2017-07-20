#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver rPwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver lPwm = Adafruit_PWMServoDriver(0x41);

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

int servo = -1;
int freq = -1;
int side = 0;

void setup() {

	servo = -1;
	freq = -1;
	side = 0;

	Serial.begin(19200);
	Serial.write("Power On");

	rPwm.begin();
	rPwm.setPWMFreq(75);  // Analog servos run at ~60 Hz updates

	lPwm.begin();
	lPwm.setPWMFreq(75);  // Analog servos run at ~60 Hz updates

	InitialPosition();

	yield();
}

void InitialPosition() {

	rPwm.setPWM(0, 0, 460);
	rPwm.setPWM(1, 0, 725);
	rPwm.setPWM(2, 0, 680);

	rPwm.setPWM(3, 0, 325);
	rPwm.setPWM(4, 0, 725);
	rPwm.setPWM(5, 0, 680);

	rPwm.setPWM(6, 0, 325);
	rPwm.setPWM(7, 0, 725);
	rPwm.setPWM(8, 0, 680);


	lPwm.setPWM(0, 0, 340);
	lPwm.setPWM(1, 0, 225);
	lPwm.setPWM(2, 0, 225);

	lPwm.setPWM(3, 0, 425);
	lPwm.setPWM(4, 0, 225);
	lPwm.setPWM(5, 0, 225);

	lPwm.setPWM(6, 0, 425);
	lPwm.setPWM(7, 0, 225);
	lPwm.setPWM(8, 0, 225);

	Serial.println(" -> Reseted to Initial Position! ");
}

/**
 * Function that reads serial strings and returns it
 **/
String ReadSerialString() {
	String content = "";
	char letter;

	// While receiving something from serial...
	while (Serial.available() > 0) {
		// Read byte from serial
		letter = Serial.read();
		// Ignore break row char
		if (letter != '\n') {
			// Concact values
			content.concat(letter);
		}
		// wait for serial buffer read next letters
		delay(10);
	}
	return content;
}



void loop() {
	// If receives sothing from serial...
	if (Serial.available() > 0) {
		// read string received
		String received = ReadSerialString();
		received.toUpperCase();

		if (received.substring(0, 1) == "I") {
			InitialPosition();
		}

		if (received.substring(0, 1) == "S") {
			servo = received.substring(1).toInt();
			Serial.print(" -> Servo: ");
			Serial.println(servo);
		}

		if (received.substring(0, 1) == "F") {
			freq = received.substring(1).toInt();
			Serial.print(" -> Frequency: ");
			Serial.println(freq);
		}

		if (received.substring(0, 1) == "L" || received.substring(0, 1) == "E") {
			Serial.println(" -> Left Side ");
			side = 0;
		}

		if (received.substring(0, 1) == "R" || received.substring(0, 1) == "D") {
			Serial.println(" -> Right Side ");
			side = 1;
		}

		delay(250);
	}

	if (servo > -1 && freq > -1)
	{
		if (side == 0) {
			lPwm.setPWM(servo, 0, freq);
		}
		else {
			rPwm.setPWM(servo, 0, freq);
		}
		freq = -1;
		delay(2000);
	}
}