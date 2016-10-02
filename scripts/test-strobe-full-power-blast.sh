#!/bin/bash

echo -e "S016255\nS017255\nC" | netcat -w 1 localhost 9999

sleep 3

echo -e "S016000\nS017000\nC" | netcat -w 1 localhost 9999
