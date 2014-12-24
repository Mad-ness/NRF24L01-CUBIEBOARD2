#!/bin/bash

i=1
while(1>0)
do
     if(($i%2==1))
     then
         ./start 1
     else
         ./start 0
     fi
     i=$(($i+1))
     sleep 1
done
