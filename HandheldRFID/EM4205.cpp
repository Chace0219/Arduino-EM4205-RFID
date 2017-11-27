#include "EM4205.h"
SoftwareSerial rfidSerial(2, 3); // RX, TX

bool initRFID()
{
	rfidSerial.end();

	// 
	rfidSerial.begin(9600);
	rfidSerial.flush();
	rfidSerial.print(F("SD2\r"));

	uint32_t nStartTime = millis();
	bool bReceving = false;
	String resp = "";
	while (1)
	{
		if (rfidSerial.available())
		{
			bReceving = true;
			resp += String((char)rfidSerial.read());
			nStartTime = millis();
		}
		if (bReceving == false)
		{
			if (millis() - nStartTime > 1000)
				return false;
		}
		else
		{
			if (millis() - nStartTime > 100)
				break;
		}
	}

	Serial.println(resp);
	if (resp.indexOf("OK") != -1)
		return true;
}


bool checkTag()
{
	/*
	while (RFIDSerial.available())
		char ch = RFIDSerial.read();

	String req = "LTG\r";
	//RFIDSerial.flush();
	RFIDSerial.print(req);

	uint32_t nStartTime = millis();
	bool bReceving = false;
	String resp = "";
	while (1)
	{
		if (RFIDSerial.available())
		{
			bReceving = true;
			resp += String((char)RFIDSerial.read());
			nStartTime = millis();
		}
		if (bReceving == false)
		{
			if (millis() - nStartTime > 1000)
			{
				Serial.println("Timeout in communication!");
				return false;
			}
		}
		else
		{
			if (millis() - nStartTime > 10)
				break;
		}
	}

	Serial.println(resp);

	if (resp.indexOf("OK") != -1)
	{
		return true;

	}*/

	return false;
}

String getResponse()
{

	uint32_t nStartTime = millis();
	bool bReceving = false;
	String resp = "";
	if (rfidSerial.available())
	{
		while (1)
		{
			if (rfidSerial.available())
			{
				bReceving = true;
				char ch = rfidSerial.read();
				if (ch == 0x0D)
					break;
				else
					resp += String(ch);
				nStartTime = millis();
			}
			if (bReceving == false)
			{
				if (millis() - nStartTime > 1000)
				{
					Serial.println(F("Timeout in communication!"));
				}
			}
			else
			{
				if (millis() - nStartTime > 30)
					break;
			}
		}
	}
	return resp;
}
