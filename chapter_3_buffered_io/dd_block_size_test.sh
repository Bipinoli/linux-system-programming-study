echo "\nThis is an experiment to show that the block transfer with block size matching the disk block size is very efficient"
echo "This happens because it requires less system calls to write to the kernel buffer, and as the buffer gets full in fewer"
echo "calls due to the matching of block size the kernel can straight way flush that to the disk."
echo "Carefully evaulate the runtime of following dd calls to evaluate this."
echo "-- Bipin Oli\n"

BLOCK_SIZE=$(stat -f %k)
TOTAL_SIZE=$((BLOCK_SIZE * 50))

echo "\nFile system block size is $BLOCK_SIZE"
echo "We want to transfer $TOTAL_SIZE bytes from /dev/zero to some destination file"
echo "----- \n"


echo "transferring in the block-size of the actual disk block-size"
COMMAND="time dd bs=$BLOCK_SIZE count=$((TOTAL_SIZE / BLOCK_SIZE)) if=/dev/zero of=pirate1"
eval "$COMMAND"
echo "----- \n"

echo "transferring in the block-size of shy away from the actual disk block-size"
COMMAND="time dd bs=$((BLOCK_SIZE - 1)) count=$((TOTAL_SIZE / (BLOCK_SIZE - 1))) if=/dev/zero of=pirate2"
eval "$COMMAND"
echo "----- \n"


echo "transferring in the block-size of 1 byte"
COMMAND="time dd bs=1 count=$TOTAL_SIZE if=/dev/zero of=pirate3"
eval "$COMMAND"
echo "----- \n"

rm pirate*