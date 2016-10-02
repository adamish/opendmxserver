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
#ifndef DMXSENDER_H
#define DMXSENDER_H

#include "Runnable.h"
#include "ftd2xx.h"
#include "mutex.h"

class DmxTransmit : public Runnable {

private:
    int interFrameDelay;
    FT_HANDLE ftHandle;
    /** used directly in send. */
    unsigned char dmxData[512];
    /** requests from user (includes randoms etc). */
    unsigned int dmxDataRequest[512];
    /** incoming buffer before commit(). */
    unsigned int dmxDataRequestTmp[512];
    unsigned char startCode;
    unsigned int framesSent;
    int channels;
	double updateInterval;
	double lastChange;

    Mutex updateLock;
public:
    DmxTransmit();
    DmxTransmit(const DmxTransmit * copy);
    /**
     * Refresh rate.
     * @param rate updates per second.
     */
    void setRefreshRate(int rate);
    /**
     * Set value. Must commit() after new state is complete.
     * @param channel address 0-511
     * @param value value 0-255, > 255 for randoms.
     */
    void setDmxValue(int channel, int value);

    /**
     * Commit changes ready for DMX stream.
     */
    void commit();

    /**
     * Update stream values, including randoms.
     */
    void updateValues();

    virtual void run();
};
#endif
