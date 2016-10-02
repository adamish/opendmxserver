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
#include "pthread.h"
#include "Runnable.h"

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

class Thread {
private:
	pthread_t thread;
	Runnable * runnable;
public:
	Thread(Runnable * runnable);
	void start();
	void stop();
	static void sleep(long ms);
};

#endif
