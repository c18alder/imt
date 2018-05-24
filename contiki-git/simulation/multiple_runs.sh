BASH programming

template:

for SEED in `seq <first numbner> <last number>`; do echo "SEED = $SEED"; <command that uses SEED> ; done

equivalently:

for SEED in `seq <first numbner> <last number>` 
do 
  echo "SEED = $SEED"
  <command that uses SEED>
done


example simplified:

for SEED in `seq 1 10`; do echo "SEED = $SEED"; python run_simulation.py ....RADIO_QUALITY_SET_TIME_IN_S=3,RANDOM_SEED_NUMBER=$SEED,RPL_CONF_MOP=1... y n ; done


example full:
for SEED in `seq 1 10`; do echo "SEED = $SEED"; python run_simulation.py ../.. ../src/ . ../../../experiments/ Xmote DATA_SEND_START_TIME_IN_S=500,GRANDCHILD_OF_GRANDPARENT_KILL_MOTE_ID=8,GRANDPARENT_KILL_TIME_IN_S=6000000,LEAPFROG_RETRIES=0,MAKE_WITH_ORCHESTRA=1,POWERTRACKER_INITIAL_DELAY_TIME_IN_S=300,POWERTRACKER_INTERVAL_TIME_IN_S=300,RADIO_QUALITY_RANDOM_SET_INTERVAL_TIME_IN_S=60000000,RADIO_QUALITY_RANDOM_SET_MAX_QUALITY_IN_PERCENT=100,RADIO_QUALITY_RANDOM_SET_MIN_QUALITY_IN_PERCENT=100,RADIO_QUALITY_RANDOM_START_SET_TIME_IN_S=60000000,RADIO_QUALITY_SET_ROOT_QUALITY_IN_PERCENT=100,RADIO_QUALITY_SET_TIME_IN_S=3,RANDOM_SEED_NUMBER=$SEED,RPL_CONF_MOP=1,SIMULATION_END_TIME_IN_S=1100,TSCH_CONF_MAC_MAX_FRAME_RETRIES=0,TSCH_SCHEDULE_PRINT_INTERVAL_IN_S=100,WITH_THUNDER_ADAPTIVE_EB_SLOT=1,DATA_SEND_INTERVAL_TIME_IN_S=10,RADIO_QUALITY_SET_QUALITY_IN_PERCENT=100 y n ; done


nested for example simplified:

for PARAM1 in `seq 1 10`; do echo "PARAM1=$PARAM1"; for PARAM2 in 70 80 90 100; do echo "PARAM2=$PARAM2"; python run_simulation.py ....RADIO_QUALITY_SET_TIME_IN_S=3,RANDOM_SEED_NUMBER=$PARAM1,LINK_QUALITY=$PARAM2,RPL_CONF_MOP=1... y n ; done ; done

nested for example simplified equivalently:

for PARAM1 in `seq 1 10`
do
  echo "PARAM1=$PARAM1"
  for PARAM2 in 70 80 90 100
  do 
    echo "PARAM2=$PARAM2"
    python run_simulation.py ....RADIO_QUALITY_SET_TIME_IN_S=3,RANDOM_SEED_NUMBER=$PARAM1,LINK_QUALITY=$PARAM2,RPL_CONF_MOP=1... y n
  done
done


Find:
find . -name config.json -exec grep -l '"RADIO_QUALITY_SET_QUALITY_IN_PERCENT": "80"' '{}' \;

