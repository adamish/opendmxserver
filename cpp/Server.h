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
#ifndef SERVER_H
#define SERVER_H
#include <vector>
#include <string>
#include "dmxtransmit.h"

using namespace std;
class Server {
private:
	int port;
	std::vector<int> clients;
	void rxLine(string line);
	DmxTransmit * dmxSender;
public:
	Server(int port);
	void connect();
	void setDmxTransmit(DmxTransmit * dmxTransmit);
};

#endif
