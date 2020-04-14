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
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "Server.h"
#include <netinet/in.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>

Server::Server(int port) {
	this->port = port;
}

void Server::connect() {
	int serverfd;
	struct sockaddr_in sockaddr;

	if ((serverfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		cerr << "Could create socket" << endl;
	}

	// all instant reuse of socket
	int optval = 1;
	setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	memset(&sockaddr, 0, sizeof(sockaddr)); // YFKM
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(this->port);

	int rt = bind(serverfd, (struct sockaddr *)&sockaddr,  sizeof(sockaddr));

	if (rt < 0) {
		cerr << "Could not bind" << strerror( errno ) << endl;
	} else {
		cout << "connected " << endl;
	}

	if (listen(serverfd, 5)) {
		cerr << "Could not listen"  << strerror( errno ) << endl;
	} else {
		cout << "listening for clients" << endl;
	}

	while (true) {
		struct sockaddr_in caddr;
		socklen_t clen;
		int fd = accept(serverfd, (struct sockaddr *)&caddr, &clen);

    // error handling
		if (fd < 0) {
        perror("accept");
        exit(1);
    }

		if (fd > 0) {
			char *str = new char[INET_ADDRSTRLEN];
			cout << "connect client " << endl;
			inet_ntop(AF_INET, (void *)(&caddr.sin_addr), str, INET_ADDRSTRLEN);
			cout << "Client connect from " << str << endl;
			cout << "Client connect from " << inet_ntoa(caddr.sin_addr) << endl;

			const char * send = "hello\n";
			write(fd, send, strlen(send));
			char buff[1];
			char line[255];
			int i = 0;
			while (read(fd, (void *)buff, 1) > 0) {
				char val = buff[0];
				if (val == 13 || val == 10) {
					if (i > 0) {
						line[i++] = '\0';
						rxLine(line);
						i = 0;
					}
				} else {
					line[i] = val;
					i++;
				}
			}
      // delete mem allocated to str
      delete[] str;
		}
    // done reading from accepted connection, close socket
    close(fd);
	}
}

/**
 * @param line Line received
 */
void Server::rxLine(string line) {
	// protocol
	// SNNNXXX # set value X for channel N
	// DNNNXXX # set demo mode X for channel N
	// C # commit
	//
	if (line[0] == 'S') {
		cout << line << endl;
		string cStr = line.substr(1, 3);
		string vStr = line.substr(4, 3);
		int channel = atoi(cStr.data());
		int value = atoi(vStr.data());
		dmxSender->setDmxValue(channel, value);
		cout << "C = " << channel << ", V = " << value << endl;
	} else if (line[0] == 'C') {
		dmxSender->commit();
	}
}

void Server::setDmxTransmit(DmxTransmit * dmxSender) {
	this->dmxSender = dmxSender;
}
