// #include "StdAfx.h"
#include "DeviceWaveman.h"
#include <string>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>

using namespace std;

/*
* Constructor
*/
DeviceWaveman::DeviceWaveman(char *strNewHouse, char *strNewCode)
	:DeviceNexa(strNewHouse, strNewCode){
}

/*
* Turn off this device
*/
void DeviceWaveman::turnOff(void){
	
	try{
		string strCode = getStringCode(intHouse);
		string strUnit = getStringCode(intCode);
		strCode.append(strUnit);
		
		strCode.insert(0, "S");
		strCode.append("$k$k$k$k$k$k$k$k$k+");	//the "turn off"-code, keeps it like this, doesn't have to be regenerated each time
		
		char* strMessage = const_cast<char*>(strCode.c_str());
		
		Device::send(strMessage);
	}
	catch(...){
		throw;
	}
}

/*
* Has the device got the method?
*/
int DeviceWaveman::methods(char* strModel){
	return (TELLSTICK_TURNON | TELLSTICK_TURNOFF);
}