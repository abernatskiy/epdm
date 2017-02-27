#!/bin/bash

numTraj=8000
simTime=200.

rm -r out;
mkdir out

# Generate the random seeds library
# Take sha1, truncate to four least signifcant hex digits, convert to decimal, store in file with labels
for i in `seq -w 0 ${numTraj}`; do
	echo "ibase=16; `echo $i | sha1sum | cut -d ' ' -f 1 | cut -c 37- | tr /a-z/ /A-Z/`" | bc | sed -e "s/^/${i} /";
done > out/seeds

# Run simulations:
for i in `seq -w 0 ${numTraj}`; do
	SEED=`grep "^${i} " out/seeds | cut -d ' ' -f 2`
	./epdm simulateTime ${simTime} ${simTime} out/traj${i} -r ${SEED} > /dev/null;
done

# Make a histogram
for traj in out/traj*; do
	tail -1 $traj | sed -e 's/200,A \([0-9]*\),/\1/';
done > out/finalACounts
sort -n out/finalACounts | uniq -c > out/unnormalizedHistogram
