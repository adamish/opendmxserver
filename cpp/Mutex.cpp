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
#include "mutex.h"
#include <pthread.h>

Mutex::Mutex() {
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	//	int OldPrio = 0;
	pthread_mutex_init (&this->mutex, &attr);
	//pthread_mutex_setprioceiling(&this->mutex, 0, &OldPrio);

}

Mutex::~Mutex() {
	pthread_mutex_destroy(&this->mutex);
}

void Mutex::lock() {
	if (pthread_mutex_lock(&this->mutex)) {

	}
}

void Mutex::unlock() {
	if (pthread_mutex_unlock(&this->mutex)) {

	}
}

