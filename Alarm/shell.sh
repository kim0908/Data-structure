

#!/bin/sh
  
while :   
do   
echo "Current DIR is " $PWD   
stillRunning=$(ps -ef |grep "$PWD/alarm" |grep -v "grep")   
if [ "$stillRunning" ] ; then   
echo "TWS service was already started by another way"   
echo "Kill it and then startup by this shell, other wise this shell will loop out this message annoyingly"   
kill -9 $pidof $PWD/alarm   
else   
echo "TWS service was not started"   
echo "Starting service ..."   
$PWD/alarm   
echo "TWS service was exited!"   
fi   
sleep 10   
done  
