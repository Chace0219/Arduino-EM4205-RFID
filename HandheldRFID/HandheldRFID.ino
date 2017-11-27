#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "FiniteStateMachine.h"
#include "EM4205.h"

const uint32_t DETECTEDDISPLAYTIME = 4000; // ms unit

void standbyEnter();
void standbyUpdate();
void standbyExit();

void idleEnter();
void idleUpdate();
void idleExit();

void detectedEnter();
void detectedUpdate();
void detectedExit();

State standby(standbyEnter, standbyUpdate, NULL);
State idle(idleEnter, idleUpdate, NULL);
State detected(detectedEnter, detectedUpdate, NULL);
FiniteStateMachine controller = FiniteStateMachine(standby);
Adafruit_7segment matrix = Adafruit_7segment();

String tagIDstr = "";

void displayTagID(String tagID)
{
	tagID = tagID.substring(tagID.length() - 4, tagID.length());
	Serial.print(F("tag id is "));
	Serial.println(tagID);

	uint16_t tagNumber = tagID.toInt();

	matrix.writeDigitNum(4, tagNumber % 10);
	tagNumber /= 10;

	matrix.writeDigitNum(3, tagNumber % 10);
	tagNumber /= 10;

	matrix.writeDigitNum(1, tagNumber % 10);
	tagNumber /= 10;

	matrix.writeDigitNum(0, tagNumber % 10);
	tagNumber /= 10;

	matrix.writeDisplay();
}

void standbyEnter()
{
	Serial.println(F("Standby status"));

	//matrix.writeDigitNum(0, 0);
	//matrix.writeDigitNum(1, 0);
	//matrix.writeDigitNum(3, 0);
	matrix.writeDigitNum(4, 0);
	matrix.writeDisplay();
	initRFID();
}

void idleEnter()
{
	matrix.clear();
	matrix.writeDisplay();

	initRFID();

	Serial.println(F("Idle status"));
	matrix.clear();
	matrix.writeDigitNum(4, 0);
	matrix.writeDisplay();
	
	//initRFID();
}

void idleUpdate()
{
	String newTag = getResponse();
	if (newTag.length() > 4)
	{
		Serial.println(newTag);
		displayTagID(newTag);
		controller.transitionTo(detected);
	}
}

void standbyUpdate()
{
	String newTag = getResponse();
	if (newTag.length() > 4)
	{
		Serial.println(newTag);
		displayTagID(newTag);
		controller.transitionTo(detected);
	}

	if (controller.timeInCurrentState() > DETECTEDDISPLAYTIME)
		controller.transitionTo(idle);

}

void detectedEnter()
{
	Serial.println(F("Detected status"));
	initRFID();
}

void detectedUpdate()
{
	String newTag = getResponse();
	if (newTag.length() > 4)
	{
		Serial.println(newTag);
		displayTagID(newTag);
		controller.transitionTo(detected);
	}

	if (controller.timeInCurrentState() > DETECTEDDISPLAYTIME)
		controller.transitionTo(idle);
}


void setup()
{
	Serial.begin(9600);
	Serial.println("Program started");
	matrix.begin(0x70);
}

void loop() {
	controller.update();
}
