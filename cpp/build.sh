#!/bin/bash

#g++ -I. -lpthread -lstdc++ -lftd2xx  *.cpp -o opendmxserver
g++ -I.   *.cpp -o opendmxserver -lpthread -lstdc++ -lftd2xx


