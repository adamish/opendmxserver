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

#include "Thread.h"
#include "Runnable.h"
#include <pthread.h>
#include <iostream>

void * runRedirect(void * ptr) {
	((Runnable *)ptr)->run();

	return 0;
}

Thread::Thread(Runnable * runnable) {
	this->runnable = runnable;
}

void Thread::start() {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
	pthread_create(&(this->thread), &attr, runRedirect, (void *)this->runnable);
}

void Thread::stop() {
}

void Thread::sleep(long ms) {
	struct timespec time;
	time.tv_sec = ms / 1000;
	time.tv_nsec = (ms % 1000) * 1e6;
	nanosleep(&time,NULL);
}
