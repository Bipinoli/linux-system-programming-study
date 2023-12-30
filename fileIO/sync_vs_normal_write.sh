
gcc normal_write.c -o normal_write.out
gcc sync_write.c -o sync_write.out

time ./normal_write.out
time ./sync_write.out

rm *.out
rm file_*.txt
