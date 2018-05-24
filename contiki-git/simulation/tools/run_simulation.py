#!/usr/bin/python

import base64
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys

import os.path as path


def print_syntax():
	print("Syntax: %s <contiki path> <sources path> <tools path> <experiments path> <configuration name> <EXTRA_PARAMS> <overwrite> <gui> [<debug suspend>]"%(sys.argv[0]))
	print("        contiki path: path to the main contiki directory")
	print("        sources path: path to the src directory where the nodes and configs are placed")
	print("        tools path: path to the tools directory where helper scripts are placed")
	print("        experiments path: path to the directory which will contain the experiments")
	print("        configuration name: name of the scenario which is to be used for this experiment (there should be a corresponding directory in <sources path>/configs)")
	print("        EXTRA_PARAMS: comma(,)-separated string of equals(=)-separated name-value pairs")
	print("           Example: A=12,B=123,C=abcd")
	print("           Limitations: commas not allowed anywhere, equals not allowed in names")
	print("           Limitations: *must* contain a 'RANDOM_SEED_NUMBER' named integer value")
	print("        overwrite: y/n overwrite previous files")
	print("        gui: y/n use gui")
	print("        debug suspend: y/n (yes.no)")



def inplace_sed(file_path, find_pattern, replace_pattern):
	f = open(file_path)
	contents = f.read()
	f.close()
	new_contents = re.sub(find_pattern, replace_pattern, contents)
	f2 = open(file_path, "wt")
	f2.write(new_contents)
	f2.close()



def make_hash_sha1(s):
    hasher = hashlib.sha1()
    hasher.update(s.encode('utf-8'))
    return base64.b32encode(hasher.digest()).decode()



def make_simulation_id(all_params_dict):
	all_params_json = json.dumps(all_params_dict, sort_keys=True)
	simulation_id = make_hash_sha1(all_params_json)
	return simulation_id



def make_paths(contiki_path, sources_path, tools_path, experiments_path, configuration_name, simulation_id):
	contiki_path = path.normpath(path.abspath(contiki_path))
	sources_path = path.normpath(path.abspath(sources_path))
	tools_path = path.normpath(path.abspath(tools_path))
	experiments_path = path.normpath(path.abspath(experiments_path))
	
	destination_path = path.join(experiments_path, simulation_id)
	
	base = {
		'contiki_path': contiki_path,
		'sources_path': sources_path,
		'tools_path': tools_path,
		'experiments_path': experiments_path,
		'destination_path': destination_path,
		'results_path': path.join(destination_path, "results"),
		'src_nodes_path': path.join(sources_path, "nodes"),
		'dst_nodes_path': path.join(destination_path, "nodes"),
		'src_config_path': path.join(sources_path, "configs", configuration_name),
		'dst_config_path': path.join(destination_path, "config"),
		'cooja_jar_path': path.join(contiki_path, "tools", "cooja", "dist", "cooja.jar")
		}
		
	additional1 = {
		'src_config_file_path': path.join(base['src_config_path'], "scenario.csc"),
		'dst_config_file_path': path.join(base['dst_config_path'], "scenario.csc"),
		'src_script_file_path': path.join(base['src_config_path'], "script.js"),
		'dst_script_file_path': path.join(base['dst_config_path'], "script.js")
		}
	additional2 = {
		'experiment_config_file_path': path.join(base['dst_config_path'], "config.json"),
		'experiment_paths_file_path': path.join(base['dst_config_path'], "paths.json")
		}
	all_paths = {}
	all_paths.update(base)
	all_paths.update(additional1)
	all_paths.update(additional2)
	return all_paths
	


def copy_and_fix_paths(all_paths, overwrite):
	destination_path_exists = path.exists(all_paths['destination_path'])
	if destination_path_exists:
		if overwrite == False:
			print("Destination path already exists and overwriting is disabled. Exiting.")
			sys.exit(2)
		else:
			shutil.rmtree(all_paths['destination_path'])
	
	os.makedirs(all_paths['destination_path'])

	os.makedirs(all_paths['dst_nodes_path'])
	for base_filename in ["Makefile", "project-conf.h"]:
		src_file_name = path.join(all_paths['src_nodes_path'], base_filename)
		dst_file_name = path.join(all_paths['dst_nodes_path'], base_filename)		 
		shutil.copy2(src_file_name, dst_file_name)
	
	inplace_sed(path.join(all_paths['dst_nodes_path'], "Makefile"), "CONTIKI=../../..", 'CONTIKI=%s' % (all_paths['contiki_path']))


	for node_name in ["sender", "receiver"]:
		os.makedirs(path.join(all_paths['dst_nodes_path'], node_name))
		node_source_file = node_name + ".c"
		for base_filename in ["Makefile", node_source_file]:
			src_file_name = path.join(all_paths['src_nodes_path'], node_name, base_filename)
			dst_file_name = path.join(all_paths['dst_nodes_path'], node_name, base_filename)
			shutil.copy2(src_file_name, dst_file_name)

	os.makedirs(all_paths['dst_config_path'])
	for base_filename in ["scenario.csc", "script.js"]:
			src_file_name = path.join(all_paths['src_config_path'], base_filename)
			dst_file_name = path.join(all_paths['dst_config_path'], base_filename)
			shutil.copy2(src_file_name, dst_file_name)

	for node_name in ["sender", "receiver"]:
		dst_node_source_file = path.join(all_paths['dst_nodes_path'], node_name, node_name + ".c")
		inplace_sed(all_paths['dst_config_file_path'], "<source>[^<]*%s\\.c</source>" % (node_name), '<source>%s</source>' % (dst_node_source_file))

	inplace_sed(all_paths['dst_config_file_path'], "<scriptfile>[^<]*</scriptfile>", '<scriptfile>%s</scriptfile>' % (all_paths['dst_script_file_path']))



def set_random_number_seed(all_paths, extra_params_dict):
	if "RANDOM_SEED_NUMBER" in extra_params_dict:
		random_seed_number = extra_params_dict["RANDOM_SEED_NUMBER"]
		inplace_sed(all_paths['dst_config_file_path'], "<randomseed>[^<]*</randomseed>", '<randomseed>%s</randomseed>' % (random_seed_number))
	else:
		print("No 'RANDOM_SEED_NUMBER' name found in EXTRA_PARAMS")
		sys.exit(1)	

def set_make_params(all_paths, all_params_dict):
	extra_params_pairs = [key+"="+all_params_dict[key] for key in sorted(all_params_dict.keys())]
	extra_make_defines_params_value = "DEFINES="+",".join(extra_params_pairs)
	extra_make_plain_params_value = " ".join(extra_params_pairs)
	
	inplace_sed(all_paths['dst_config_file_path'], "\$\{TEMPLATE_EXTRA_MAKE_PARAMS\}", extra_make_defines_params_value + " " + extra_make_plain_params_value)


def write_config_and_paths(all_paths, all_params_dict):
	print("Saving current experiment config into:" + all_paths['experiment_config_file_path'])
	with open(all_paths['experiment_config_file_path'], 'wt') as f:
		f.write(json.dumps(all_params_dict, sort_keys=True, indent=2))
	
	print("Saving current experiment paths into:" + all_paths['experiment_paths_file_path'])
	with open(all_paths['experiment_paths_file_path'], 'wt') as f:
		f.write(json.dumps(all_paths, sort_keys=True, indent=2))



def run_simulation(all_paths, all_params_dict, debug_suspend, gui):
	os.makedirs(all_paths['results_path'])
	print("Entering result saving directory: " + all_paths['results_path']) 
	os.chdir(all_paths['results_path'])

	print("Starting simulation: " + simulation_id)
	# timeout -sKILL 3600
 	timing_command = ['/usr/bin/time', '--verbose', '--output='+path.join(all_paths['results_path'], 'time.txt')]
#   	timing_command += ['valgrind', '--tool=callgrind', '--instr-atstart=no']#--vex-iropt-register-updates=allregs-at-mem-access']'--smc-check=all', 
#  	timing_command += ['operf', '--callgraph']
#  	timing_command += ['opgprof']#, '--callgraph']
# 	timing_command = [] 
	executable = ['java']
	java_params = ['-jar', "{cooja_jar_path}".format(**all_paths)]
	contiki_params = ['-contiki={contiki_path}/'.format(**all_paths)]
	
	if gui:
		scenario_params = ['-mx512m', '-quickstrt={dst_config_file_path}'.format(**all_paths)]
	else:
		scenario_params = ['-nogui={dst_config_file_path}'.format(**all_paths)]
		
	if debug_suspend is not None:
		debug_params = ["-agentlib:jdwp=transport=dt_socket,server=y,suspend={},address=8000".format(debug_suspend)]
	else:
		debug_params = []#['-Xint']
		
	cmd = timing_command + executable + java_params + debug_params + scenario_params + contiki_params 
	env = {}
	env.update(os.environ)

	extra_params_pairs = [key+"="+all_params_dict[key] for key in sorted(all_params_dict.keys())]
	extra_run_params_value = ",".join(extra_params_pairs)
	env.update({
		"EXTRA_PARAMS": extra_run_params_value
		})
	
	print("Running command: "+str(cmd))
	print("Environment: "+str(env))
	status = subprocess.call(cmd, env=env)
	# gdb debug:
	# sleep 20 ; gdb --pid "$SIM_PID"
	print("Finished simulation: " + simulation_id)
# 	print("Cleaning up obj_cooja dirs: " + simulation_id)
# 	shutil.rmtree(path.join(all_paths['dst_nodes_path'], 'sender', 'obj_cooja'))
# 	shutil.rmtree(path.join(all_paths['dst_nodes_path'], 'receiver', 'obj_cooja'))
# 	print("All done: " + simulation_id)



if __name__ == "__main__":
	if len(sys.argv) < 9 or len(sys.argv) > 10:
		print_syntax()
		sys.exit(1)

	contiki_path = sys.argv[1]
	sources_path = sys.argv[2]
	tools_path = sys.argv[3]
	experiments_path = sys.argv[4]
	configuration_name = sys.argv[5]
	extra_params = sys.argv[6]
	overwrite = True if sys.argv[7] == "y" else False
	gui = True if sys.argv[8] == "y" else False
	debug_suspend = sys.argv[9] if len(sys.argv)==10 else None
	
	#-----------------------------
	extra_params_dict = dict([pair_str.split("=", 1) for pair_str in extra_params.split(",")])
	
	all_params_dict = {}
	all_params_dict.update(extra_params_dict)
	all_params_dict.update({"CONFIGURATION_NAME":configuration_name})
	
	simulation_id = make_simulation_id(all_params_dict)
	
	all_paths = make_paths(contiki_path, sources_path, tools_path, experiments_path, configuration_name, simulation_id)
	
	copy_and_fix_paths(all_paths, overwrite)
	
	set_random_number_seed(all_paths, all_params_dict)
	
	set_make_params(all_paths, all_params_dict)
	
	write_config_and_paths(all_paths, all_params_dict)
	
	run_simulation(all_paths, all_params_dict, debug_suspend, gui)
	
