# !/usr/bin/bash

make > /dev/null

cap=1000 # system capacity
thres=10 # threshold for buffering packets, without servicing them
max_lim=10000 # maximum number of simulation iterations
eps=0.01 # eps
ver=0 # verbose output or not?

Initial=0 # not an input argument

# arr_rate --> packet arrival rate
# dep_rate --> packet service rate

echo "Simulating queueing system..."

for arr_rate in {2..6}
do
	for dep_rate in {2..10}
	do
		if [ $Initial -eq 0 ] 
			then 
				./markov "$cap" "$thres" "$arr_rate" "$dep_rate" "$max_lim" "$eps" "$ver" > out.txt
				Initial=1
		fi 
		
		if [ $Initial -ne 0 ]
			then
				./markov "$cap" "$thres" "$arr_rate" "$dep_rate" "$max_lim" "$eps" "$ver" >> out.txt
		fi
	done
done

echo "Done!"
