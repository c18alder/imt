for MAX_Q in 100 90 80 70 60 50; do for REP in `ls | sort | grep ${MAX_Q}_`; do RX=`grep "node-receiver-leapfrog: received from" $REP/results/COOJA.testlog | wc -l`; TX=`grep "node-sender-leapfrog-replicate: Sending" $REP/results/COOJA.testlog | wc -l`; echo "$MAX_Q $REP $TX $RX"  ; done ; done