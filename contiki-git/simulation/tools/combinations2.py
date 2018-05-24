#!/usr/bin/python

def dict_to_extra_params_list(d):
    return ','.join([key+"="+d[key] for key in sorted(d.keys())])

if __name__=='__main__':
    combinations = []
    common_parameters = {
        "DATA_SEND_START_TIME_IN_S": str(200),
        "DATA_SEND_INTERVAL_TIME_IN_S": str(15),
        "TSCH_SCHEDULE_PRINT_INTERVAL_IN_S": str(1*60000),
        "WITH_THUNDER_ADAPTIVE_EB_SLOT": str(1),
        "SIMULATION_END_TIME_IN_S": str(1000),
        "POWERTRACKER_INTERVAL_TIME_IN_S": str(1000*60),
        "POWERTRACKER_INITIAL_DELAY_TIME_IN_S": str(10*60),
        "RADIO_QUALITY_SET_TIME_IN_S": str(200),
        "GRANDPARENT_KILL_TIME_IN_S": str(1000000*60), #nev100er
        "GRANDCHILD_OF_GRANDPARENT_KILL_MOTE_ID": str(8),
        "RADIO_QUALITY_RANDOM_SET_MIN_QUALITY_IN_PERCENT": str(100),
        "RADIO_QUALITY_RANDOM_SET_MAX_QUALITY_IN_PERCENT": str(100),
        "RADIO_QUALITY_RANDOM_START_SET_TIME_IN_S": str(30000000*60),
        "RADIO_QUALITY_RANDOM_SET_INTERVAL_TIME_IN_S": str(100000000*60)
        }        
    for RANDOM_SEED_NUMBER in range(1,2):
        for LEAPFROG_RETRIES in [1]:
            for ALGORITHM in ["linkpeek"]:#, "leapfrog"]:
                combination_base = {
                    'RANDOM_SEED_NUMBER': str(RANDOM_SEED_NUMBER), 
                    'LEAPFROG_RETRIES': str(LEAPFROG_RETRIES),
                    'RADIO_QUALITY_SET_QUALITY_IN_PERCENT': str(70),
                    'RADIO_QUALITY_SET_ROOT_QUALITY_IN_PERCENT': str(100),
                    'MAKE_WITH_THUNDER': str(1)
                    }
                if ALGORITHM == "none":
                    for TSCH_CONF_MAC_MAX_FRAME_RETRIES in [0, 2, 4, 6, 8]:
                        combination = {
                            "TSCH_CONF_MAC_MAX_FRAME_RETRIES": str(TSCH_CONF_MAC_MAX_FRAME_RETRIES)
                            }
                        combination.update(combination_base)
                        combination.update(common_parameters)
                        combinations.append(dict_to_extra_params_list(combination))
                elif ALGORITHM == "linkpeek":
                    for TSCH_CONF_MAC_MAX_FRAME_RETRIES in [1]:
                        combination = {
                            "TSCH_CONF_MAC_MAX_FRAME_RETRIES": str(TSCH_CONF_MAC_MAX_FRAME_RETRIES),
                            "MAKE_WITH_LINKPEEK": str(1)
                            }
                        combination.update(combination_base)
                        combination.update(common_parameters)
                        combinations.append(dict_to_extra_params_list(combination))
                elif ALGORITHM == "leapfrog":
                    for WITH_OVERHEARING, TSCH_CONF_MAC_MAX_FRAME_RETRIES in [(True, 1)]:
                        combination = {
                            "MAKE_WITH_LEAPFROG": str(1),
                            "TSCH_CONF_MAC_MAX_FRAME_RETRIES": str(TSCH_CONF_MAC_MAX_FRAME_RETRIES)
                            }
                        if WITH_OVERHEARING:
                            combination.update({
                                'WITH_OVERHEARING': str(1)
                                })
                        combination.update(combination_base)
                        combination.update(common_parameters)
                        combinations.append(dict_to_extra_params_list(combination))        
    try:
        for combination in combinations:
            print(combination)
    except:
        pass
        
    