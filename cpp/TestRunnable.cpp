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
#include <iostream>
using namespace std;

#include "Thread.h"
#include "TestRunnable.h"


TestRunnable::TestRunnable(int id) {
	this->id = id;
}

void TestRunnable::run() {
	while (true) {
		cout << "run" << id << endl;
		Thread::sleep(200);
	}
}

void TestRunnable::horse() {
	cout << "horse";
}

