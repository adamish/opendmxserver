/*
  opendmxserver is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  opendmxserver is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with opendmxserver.  If not, see <http://www.gnu.org/licenses/>.
  
  (C) Adam Granger 2012
  
*/
using namespace std;

#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>

#include "dmxtransmit.h"
#include "ftd2xx.h"
#include "Thread.h"
#include "system.h"

/* Linux doesn't require these windows constants for FT_W32_CreateFile */
#define GENERIC_READ 0
#define GENERIC_WRITE 0
#define OPEN_EXISTING 0
#define FILE_ATTRIBUTE_NORMAL 0

void DmxTransmit::run() {

	FT_STATUS ftStatus;
	char buf[256];

	ftStatus = FT_ListDevices(0, buf, FT_LIST_BY_INDEX | FT_OPEN_BY_DESCRIPTION);
	if (ftStatus != FT_OK) {
		cout << "Could not list devices. error code : " << ftStatus << strerror( errno ) << endl;
	} else {
		cout << buf << endl;
	}

	ftStatus = FT_Open(0, &ftHandle);
	if (ftStatus != FT_OK) {
		cout << "Could not open device. error code : " << ftStatus << strerror( errno ) << endl;
	}

	Thread::sleep(1000);

	FT_ResetDevice(ftHandle);
	FT_ResetPort(ftHandle);

	ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_2, FT_PARITY_NONE);
	if (ftStatus != FT_OK) {
		cout << "Could not set data characteristics. error code : " << ftStatus << endl;
	}

	FT_SetBaudRate(ftHandle, 250000);

	ULONG bytesWritten;

	// set RS485 for sending
	FT_W32_EscapeCommFunction(ftHandle, CLRRTS);

	while (true) {

		this->updateLock.lock();
		updateValues();
		FT_W32_SetCommBreak(ftHandle);
		FT_W32_ClearCommBreak(ftHandle);

		FT_W32_WriteFile(ftHandle, &startCode, 1, &bytesWritten, NULL);
		FT_W32_WriteFile(ftHandle, dmxData, 512, &bytesWritten, NULL);

		this->updateLock.unlock();

		Thread::sleep(interFrameDelay);
		framesSent++;

	}
}

DmxTransmit::DmxTransmit()
{
	framesSent = 0;
	interFrameDelay = 30;
	lastChange = 0;
	updateInterval = 1500;
	startCode = 0;
	channels = 512;
	for (int i = 0; i < channels; i++) {
		dmxData[i] = 0;
		dmxDataRequest[i] = 0;
		dmxDataRequestTmp[i] = 0;
	}

}

void DmxTransmit::setRefreshRate(int rate)
{
	float packet_time;

	if((rate>44) || (rate < 1)) {
		return;
	}
	packet_time = (1/(float)rate) * 1000;
	interFrameDelay = packet_time - 20;
}

/**
 * Copy values from request.
 */
void DmxTransmit::updateValues() {
	for (int i = 0; i < channels; i++) {
		int request = this->dmxDataRequest[i];
		unsigned char newValue = 0;
		if (request >= 0 && request <= 255) {
			newValue = request;
		}
		this->dmxData[i] = newValue;
	}

	double now = System::currentTimeMillis();
	if (now - lastChange > updateInterval) {
		for (int i = 0; i < channels; i++) {
			int request = this->dmxDataRequest[i];
			unsigned char newValue = 0;
			if (256 == request) {
				if (2 < (rand() % 5)) { // off some of time
					newValue = 0;
				} else {
					newValue = rand() % 255;
				}
				cout << i << " " << (int)newValue << endl;
			}
			this->dmxData[i] = newValue;
		}
		lastChange = now;
	}
}

void DmxTransmit::setDmxValue(int channel, int value) {
	this->dmxDataRequestTmp[channel] = value;
}

void DmxTransmit::commit() {
	this->updateLock.lock();
	for (int i = 0; i < channels; i++) {
		this->dmxDataRequest[i] = this->dmxDataRequestTmp[i];
	}
	this->updateLock.unlock();
}
