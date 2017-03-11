#!/bin/bash

#sudo date --set="2017-03-06 23:58:00"
a=0

echo "starting...."
./start
echo "wait 240 sec before stop"

while [ $a -lt 240 ]
do
	sleep 1
	echo $a
	a=`expr $a + 1`
done

echo "Stoping......"
./stop
echo "END"

