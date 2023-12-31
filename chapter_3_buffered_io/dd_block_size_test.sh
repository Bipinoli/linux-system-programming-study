time dd bs=1 count=2097152 if=/dev/zero of=pirate1
echo "----- \n"

time dd bs=1023 count=2097152 if=/dev/zero of=pirate2
echo "----- \n"

time dd bs=1024 count=2097152 if=/dev/zero of=pirate2
echo "----- \n"

rm pirate*