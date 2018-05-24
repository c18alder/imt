#!/bin/bash
export SIM_PARAMS="RANDOM_SEED_NUMBER=1,DATA_SEND_START_TIME_IN_S=15*60,DATA_SEND_INTERVAL_TIME_IN_S=15,TSCH_SCHEDULE_PRINT_INTERVAL_IN_S=1*60,WITH_THUNDER_ADAPTIVE_EB_SLOT=1,TSCH_CONF_MAC_MAX_FRAME_RETRIES=3,LEAPFROG_RETRIES=0,SIMULATION_END_TIME_IN_S=30*60,POWERTRACKER_INTERVAL_TIME_IN_S=1*60,POWERTRACKER_INITIAL_DELAY_TIME_IN_S=10*60,RADIO_QUALITY_SET_TIME_IN_S=10*60,RADIO_QUALITY_SET_QUALITY_IN_PERCENT=70,GRANDPARENT_KILL_TIME_IN_S=20*60,GRANDCHILD_OF_GRANDPARENT_KILL_MOTE_ID=8,MAKE_WITH_LINKPEEK=1,MAKE_WITH_THUNDER=1" ; ./run_simulation.py ../.. ../src/ . ../../../experiments/ 2ladder_8motes "${SIM_PARAMS}" y y

export SIM_PARAMS="RANDOM_SEED_NUMBER=1,DATA_SEND_START_TIME_IN_S=15*60,DATA_SEND_INTERVAL_TIME_IN_S=15,TSCH_SCHEDULE_PRINT_INTERVAL_IN_S=1*60,WITH_THUNDER_ADAPTIVE_EB_SLOT=1,TSCH_CONF_MAC_MAX_FRAME_RETRIES=3,LEAPFROG_RETRIES=0,SIMULATION_END_TIME_IN_S=30*60,POWERTRACKER_INTERVAL_TIME_IN_S=1*60,POWERTRACKER_INITIAL_DELAY_TIME_IN_S=10*60,RADIO_QUALITY_SET_TIME_IN_S=10*60,RADIO_QUALITY_SET_QUALITY_IN_PERCENT=100,GRANDPARENT_KILL_TIME_IN_S=20*60,GRANDCHILD_OF_GRANDPARENT_KILL_MOTE_ID=8,MAKE_WITH_LEAPFROG=1,WITH_OVERHEARING=1,MAKE_WITH_THUNDER=1" ; ./run_simulation.py ../.. ../src/ . ../../../experiments/ 2ladder_8motes "${SIM_PARAMS}" y y


export SIM_PARAMS="RANDOM_SEED_NUMBER=1,DATA_SEND_START_TIME_IN_S=15*60,DATA_SEND_INTERVAL_TIME_IN_S=15,TSCH_SCHEDULE_PRINT_INTERVAL_IN_S=1*60,WITH_THUNDER_ADAPTIVE_EB_SLOT=1,TSCH_CONF_MAC_MAX_FRAME_RETRIES=3,LEAPFROG_RETRIES=0,SIMULATION_END_TIME_IN_S=30*60,POWERTRACKER_INTERVAL_TIME_IN_S=1*60,POWERTRACKER_INITIAL_DELAY_TIME_IN_S=10*60,RADIO_QUALITY_SET_TIME_IN_S=10*60,RADIO_QUALITY_SET_QUALITY_IN_PERCENT=100,GRANDPARENT_KILL_TIME_IN_S=20*60,GRANDCHILD_OF_GRANDPARENT_KILL_MOTE_ID=8,MAKE_WITH_LEAPFROG=1,WITH_OVERHEARING=1,MAKE_WITH_THUNDER=1" ; ./run_simulation.py ../.. ../src/ . ../../../experiments/ 2ladder_8motes "${SIM_PARAMS}" y n

./combinations1.py | head -n 10 | parallel --bar --no-notice --joblog joblogall.log -j 1 echo {}
./combinations1.py | head -n 10 | parallel --bar --no-notice --joblog joblogall.log -j 1 ./run_simulation.py ../.. ../src/ . ../../../experiments/ 2ladder_8motes {} y n
./combinations1.py | head -n 1 | parallel --bar --no-notice --joblog joblogall.log -delay 2 -j 1 ./run_simulation.py ../.. ../src/ . ../../../experiments/ 2ladder_8motes {} y n

./combinations1.py | parallel --bar --no-notice --joblog joblogall.log --delay 3 -j 0 ./run_simulation.py ../.. ../src/ . ../../../experiments/ 2ladder_8motes {} y n

./combinations1.py | parallel --bar --no-notice --joblog joblog_linkpeek.log --delay 3 -j 0 ./run_simulation.py ../.. ../src/ . ../../../experiments/ 2ladder_8motes {} y n ; echo DONE ; date > done_at ; sleep 10 ; poweroff