#python script file for extracting data from Cooja simulation result
#NOT needed now (9/Sep/2016) put this file in the same directory where "leapfrog" "normal" etc... exist
#This script is for the result with RealSim over DGRM. get_result_from_file_realsim must be used to readfiles

import numpy as np
import matplotlib.pyplot as plt
import csv
import matplotlib.mlab as mlab
import math
import os.path # for checking file existance

#To change the datapath
datapath = "/tmp/fakehome/mnt/contiki_sims_results_1/"

# To change the number of simulations done
sim_cnt_min = 1
sim_cnt_max = 1

# To define the scenario we are going to consider
#scenario_name = ["scenario_8nodes_100_5min_2times", "scenario_8nodes_90_5min_2times", "scenario_8nodes_80_5min_2times", "scenario_8nodes_70_5min_2times", "scenario_8nodes_60_5min_2times", "scenario_8nodes_50_5min_2times"]
scenario_name = ["scenario_8nodes_80_5min_2times"]

# Z1 zolertia sensor average power total energy consumption calculation.
# Logfile from Contiki os powertrace.
#Declaring the currents as they are in Zoletrias Z1 datasheet.
CURRENTS = {
    "voltage" : 3,
    "power_lpm_mA" : 0.05,
    "power_cpu_mA" : 1.8,
    "power_tx_mA"  : 17.4,
    "power_rx_mA"  : 18.8,
    "power_idle_mA": 0.416,
}

NUM_NODES = 8 #the number of nodes in network

# For the Graphics
graph_label_dict = {"normal_0ret" : "RT0", "normal_2ret" : "RT2", "normal_4ret" : "RT4", "normal_6ret" : "RT6", "normal_8ret" : "RT8", "leapfrog" : "LC"}
graph_color_dict = {"normal_0ret" : "orange", "normal_2ret" : "red", "normal_4ret" : "magenta", "normal_6ret" : "blue", "normal_8ret" : "cyan", "leapfrog" : "green"}
graph_format_dict = {"normal_0ret" : "x", "normal_2ret" : "s", "normal_4ret" : "v", "normal_6ret" : "p", "normal_8ret" : "d", "leapfrog" : "o"}
graph_hatch_dict = {"normal_0ret" : "/", "normal_2ret" : "*", "normal_4ret" : "o", "normal_6ret" : "O", "normal_8ret" : ".", "leapfrog" : "\\"}


def get_graph_array_from_data(datapath, directory_name, sim_cnt_min, sim_cnt_max): #for REALSIM, return[0] = graph_array, return[1] = directory_name
# return data in following array structure from all simulation result files in same configration (leapfrog, normal,...)
#  [[sim1, sim2, sim3, ...], directory_name]
#    sim = [[event1, event2, event3,...], sim_cnt]
#    event = dict of {"event_id", "name", "pdr", "rep", "eli", "delay", "jitter", "energy", "efficiency",...}

  result_scenario_array = []
  result_array = []

  for scenario in scenario_name:
	result_scenario_array = []
	cnt_skip = 0
	cnt_data = 0
	#gather data for each different random seeds
	for sim_cnt in range(sim_cnt_min, sim_cnt_max+1):
		# print sim_cnt
		input_file_name = datapath + scenario + "/" + directory_name + "/" + str(sim_cnt)
		result = get_result_from_file_realsim(input_file_name)
		if result is None:
			cnt_skip += 1
	      		continue #if no data, skip
	    	result_scenario_array.append([result, sim_cnt])
	    	cnt_data += 1
  	#print directory_name, "cnt_data", cnt_data, "cnt_skip", cnt_skip
	result_array.append([result_scenario_array, scenario])
  return result_array, directory_name

def get_result_from_file_realsim(input_file_name): #get result from indicated file
# return data in following array structure from 1 simulation file
#  [event1,event2,event3,...]
#    event = dict of {"event_id", "name", "pdr", "rep", "eli", "delay", "jitter", "energy", "efficiency",...}
	
	PDR = 0.0
	parents_set = 0	
	r_array = []
	test_check_array = []
	line_counter = 0

	input_cooja_file = input_file_name + "/" + "COOJA.log"
	print "Reading file: ", input_cooja_file

	if not os.path.isfile(input_cooja_file): #if we cannot find file, continue to next loop
		print "Could not find:", input_cooja_file
		return None

	input_file = open(input_cooja_file)
	input_lines = input_file.readlines()
	input_file.close

	for line in input_lines:
		line_counter += 1
        if "Test script at" in line: #simulation finished properly		
            print "simulation finished properly"
            test_check_array.append(line_counter)

	# if len(test_check_array)!=19:
	# 	print "Simulation failed in some point of the simulation"
	# 	return None

	# if (test_check_array[18] - test_check_array[0])==18:
	# 	print "Simulation was done, but the scenario failed to load"
	# 	return None

	input_cooja_file = input_file_name + "/" + "COOJA.testlog"	
	print "Reading file: ", input_cooja_file

	if not os.path.isfile(input_cooja_file): #if we cannot find file, continue to next loop
		print "Could not find::", input_cooja_file
		return None

	input_file = open(input_cooja_file)
	input_lines = input_file.readlines()
	input_file.close

	for line in reversed(input_lines):
		if line.find("time expired") > 0: #simulation finished properly
			pdr = line.split(" ")[6]			
			if pdr == "NaN" or pdr == "0":
				pdr = float(line.split(" ")[6])
				sent = int(line.split(" ")[8])
				print "Sent:",sent
				break			
				# return None
			else:
				pdr = float(line.split(" ")[6])
				sent = int(line.split(" ")[8])
				print "Sent:",sent
				break			
	else:
		print "Could not find \"time expired\" in", input_file_name
		return None

	delay_time_send_array = np.zeros(sent)
	delay_time_array = []
	save = 0

	received_array = np.zeros(sent)
	print "Reading:", input_file_name

	for line in input_lines: #reversed is important to speed up
		#gathering data to variables
		if "rpl_set_preferred_parent" in line:
			parents_set += 1	
		elif "Hello" in line:
			if len(line.split(" ")) != 13:
				print "Reading file: ", input_cooja_file	
				print "The len of line is:", len(line.split(" ")) 			
				print "Something failed in line" 
				print line
				print "Returning None"
				return None	
			time = float(line.split(" ")[4]) / 1000 #ms
			if "Sending" in line:
				tmp_packet_id = int(line.split(" ")[12].split("\'")[0])
				save = 1
			elif "received" in line:
				received_array[int(line.split(" ")[12].split("\'")[0])] = 1
				delay_time_array.append(time - delay_time_send_array[int(line.split(" ")[12].split("\'")[0])])
		elif "NOW" in line:
			if "67" in line:
				if save == 1:
					time = float(line.split(" ")[4]) / 1000 #ms
					delay_time_send_array[tmp_packet_id] = time
					save = 0

#	delay_time_array_cut = []
#	arrived_packet = 1
#	for delay in delay_time_array:
#		if delay > 360:		
#			print "Possible Outlier. Arrived packet: ", arrived_packet, "\tDelay: ", delay
#		arrived_packet += 1 

	delay_avg = np.average(delay_time_array)
# My method
	delay_time_array[:] = [abs(delay - delay_avg) for delay in delay_time_array]

#	arrived_packet = 1
#	for delay in delay_time_array:
#		print "Arrived packet: ", arrived_packet, "\tJitter: ", delay
#		arrived_packet += 1 
	jitter_avg = np.average(delay_time_array)

	new_dict = {"pdr" : pdr}
	new_dict["rpl_set_preferred_parent"] = parents_set
	new_dict["delay"] = delay_avg
	new_dict["jitter"] = jitter_avg
	r_array.append(new_dict)
	
#	print "In file: ", input_cooja_file
#	for index, value in enumerate(received_array):
#		if value == 0:
#			print "\tNot Recieved packet number:", index
	
	#print "PDR", pdr, "\tparents", parents_set, "\tdelay", delay_avg, "\tjitter", jitter_avg 
	return r_array
	

# main

print "*********************************************************"
print "Reading and processing files"
print "*********************************************************"
# r_0ret = get_graph_array_from_data(datapath, "normal_0ret", sim_cnt_min, sim_cnt_max)
# r_2ret = get_graph_array_from_data(datapath, "normal_2ret", sim_cnt_min, sim_cnt_max)
# r_4ret = get_graph_array_from_data(datapath, "normal_4ret", sim_cnt_min, sim_cnt_max)
# r_6ret = get_graph_array_from_data(datapath, "normal_6ret", sim_cnt_min, sim_cnt_max)
# r_8ret = get_graph_array_from_data(datapath, "normal_8ret", sim_cnt_min, sim_cnt_max)
r_lfce = get_graph_array_from_data(datapath,    "leapfrog", sim_cnt_min, sim_cnt_max)

# results = [r_0ret, r_2ret, r_4ret, r_6ret, r_8ret, r_lfce]
results = [r_lfce]

# In this print:
#	-k		, chooses between TSCH with 0, 2, 4, 8 ret or leapfrog.
#	-i		, chooses between scenario_easy_10, scenario_easy_20, scenario_easy_40, scenario_easy_80, scenario_easy_100.
#	-j		, chooses one of the sucessful simulations. e.g. 1, 4, 23, 32, etc.
#	-s		, chooses the stage of the successful simulation. e.g. "def", "node2", "def", "node3", "def", etc.
#	-"elementC"	, chooses the parameter. e.g. "pdr", "delay", "jitter".
#
# So, in the end, we get parameter elementC in stage s of simulation j of scenario i, with software configuration k of the nodes.
# 
#   results			== [r_0ret, r_2ret,...,r_8ret, r_lfce]. Array or results for each software configuration of the nodes
#   results[k]			== r_k. Results for software configuration k of the nodes.
#
#   r_lf			== results[k] == r_k
#   r_lf			== [[[],[],...[]], directory_name label]
#   r_lf[1] 			== directory_name label; Example: leapfrog
#   r_lf[0] 			== [[],[],...[]]. Array with the result of all the successful simulations for all the scenarios
#				 
#   r_lf[0][i] 			== []. This has the successful results of scenario i, and the scenario i label. "Zooming": [results_scenario_i, scenario_i label]
#   r_lf[0][i][1]		== scenario_i label; Example: scenario_easy_10
#   r_lf[0][i][0]		== results_scenario_i. Array of results of succesfull simulations in scenario i 
#
#   r_lf[0][i][0][j] 		== A memeber of the array results_scenario_i of the form: [[{},{},...,{}], label j]
#   r_lf[0][i][0][j][1] 	== label j. This is only a number that makes reference to successful simulation j. Example: 6, etc.
#   r_lf[0][i][0][j][0] 	== [{},{},...,{}] is An array of dict that has the results of every stage in simulation j.
#		
#   r_lf[0][i][0][j][0][s]		== {"element0", "element1",..."elementN"} Is the dict that has results in stage s of simulation j.		   
#   r_lf[0][i][0][j][0][s]["elementC"]	== elementC. Result of parameter elementC in stage s of simulation j of scenario i with software config. k of nodes.

#for k in range (0, len(results)):
#	r_lf = results[k]
#	print "*********************************************************"
#	print "Results in directory: ", r_lf[1]
#	print "*********************************************************"
#	for i in range (0, len(r_lf[0])):	
#		print "In scenario: ", r_lf[0][i][1] 
#		for j in range(0, len(r_lf[0][i][0])):
#			print "In sim", r_lf[0][i][0][j][1], "PDR ", "%.3f" % r_lf[0][i][0][j][0][0]["pdr"], "with", r_lf[0][i][0][j][0][0]["rpl_set_preferred_parent"], "rpl_set_preferred_parent", "with", "%.3f" % r_lf[0][i][0][j][0][0]["delay"], "ms average delay and", "with", "%.3f" % r_lf[0][i][0][j][0][0]["jitter"], "ms average jitter"
	

print "*********************************************************"
print "Reading and processing data in sucessful simulations"
print "*********************************************************"

pdr_conf_k_results = []
pdr_conf_k_error = []

parents_set_conf_k_results = []
parents_set_conf_k_error = []

jitter_conf_k_results = []
jitter_conf_k_error = []

delay_conf_k_results = []
delay_conf_k_error = []

f = open('results_details.log', 'w')

for k in range (0, len(results)):
	r_lf = results[k]
#	print "*********************************************************"
#	print "Results in directory: ", r_lf[1]
#	print "*********************************************************"
	f.write('*********************************************************\n')
	f.write("Results in directory: %s\n" % r_lf[1])
	f.write('*********************************************************\n')
	pdr_scenario = []
	pdr_scenario_error = []
	parents_set_scenario = []	
	parents_set_scenario_error = []
	jitter_scenario = []
	jitter_scenario_error = []
	delay_scenario = []
	delay_scenario_error = []

	for i in range (0, len(r_lf[0])):	
#		print "In scenario: ", r_lf[0][i][1] 
#		print "Succesful simulations: ", len(r_lf[0][i][0])
		f.write("In scenario: %s\n" % r_lf[0][i][1]) 
		f.write("Succesful simulations: %s\n" % len(r_lf[0][i][0]))
		pdr_sim = []
		parents_set_sim = []
		jitter_sim = []
		delay_sim = []
		for j in range(0, len(r_lf[0][i][0])):
			pdr_sim.append(r_lf[0][i][0][j][0][0]["pdr"])
			parents_set_sim.append(r_lf[0][i][0][j][0][0]["rpl_set_preferred_parent"])
			jitter_sim.append(r_lf[0][i][0][j][0][0]["jitter"])
			delay_sim.append(r_lf[0][i][0][j][0][0]["delay"])
#			print "Sim", r_lf[0][i][0][j][1], "\tPDR ", "%.3f" % r_lf[0][i][0][j][0][0]["pdr"], "\tparents", r_lf[0][i][0][j][0][0]["rpl_set_preferred_parent"], "\tdelay", "%.3f" % r_lf[0][i][0][j][0][0]["delay"], "ms\tjitter", "%.3f" % r_lf[0][i][0][j][0][0]["jitter"], "ms"
			f.write("Sim %s\tPDR %.3f\tparents %s\tdelay %.3f ms\tjitter %.3f ms\n" % (r_lf[0][i][0][j][1], r_lf[0][i][0][j][0][0]["pdr"], r_lf[0][i][0][j][0][0]["rpl_set_preferred_parent"], r_lf[0][i][0][j][0][0]["delay"],r_lf[0][i][0][j][0][0]["jitter"]))
		pdr_scenario.append(np.average(pdr_sim))
		pdr_scenario_error.append(np.std(pdr_sim))
		parents_set_scenario.append(np.average(parents_set_sim))
		parents_set_scenario_error.append(np.std(parents_set_sim))
		jitter_scenario.append(np.average(jitter_sim))
		jitter_scenario_error.append(np.std(jitter_sim))
		delay_scenario.append(np.average(delay_sim))
		delay_scenario_error.append(np.std(delay_sim))
#	print "PDR_avg:", pdr_scenario
#	print "delay_avg:", delay_scenario
#	print "jitter_avg:", jitter_scenario
#	print "parents_avg:", parents_set_scenario
	f.write("PDR_avg: %s\n" % pdr_scenario)
	f.write("delay_avg: %s\n" % delay_scenario)
	f.write("jitter_avg: %s\n" % jitter_scenario)
	f.write("parents_avg: %s\n" % parents_set_scenario)
	pdr_conf_k_results.append(pdr_scenario)
	pdr_conf_k_error.append(pdr_scenario_error)
	parents_set_conf_k_results.append(parents_set_scenario)
	parents_set_conf_k_error.append(parents_set_scenario_error)
	jitter_conf_k_results.append(jitter_scenario)
	jitter_conf_k_error.append(jitter_scenario_error)
	delay_conf_k_results.append(delay_scenario)
	delay_conf_k_error.append(delay_scenario_error)
	
f.close()

print "*********************************************************"
print "Making graphics"
print "*********************************************************"

connected_flag = 0
xlabel = "Quality of links [%]"
ylabel = "PDR [%]"
number_col = 1
legend_position = "upper left"
lp_x = 1
lp_y = 1
adjust_flag = 1
adjust_left = 0.15
adjust_bottom = 0.15
adjust_right = 0.8
adjust_top = 0.9

xticks = ["100", "90", "80", "70", "60", "50"] #dirty mode

fig, ax = plt.subplots()
for k in range (0, len(results)):
	r_lf = results[k]

	if connected_flag == 1:
		ax.errorbar([i for i in range(0, len(xticks))], pdr_conf_k_results[k], yerr=pdr_conf_k_error[k], label=graph_label_dict[r_lf[1]], color=graph_color_dict[r_lf[1]])
	else:
		ax.errorbar([i for i in range(0, len(xticks))], pdr_conf_k_results[k], yerr=pdr_conf_k_error[k], label=graph_label_dict[r_lf[1]], color=graph_color_dict[r_lf[1]], fmt=graph_format_dict[r_lf[1]])

	plt.xticks([i for i in range(0, len(xticks))], xticks, rotation=0) #must be changed correpoiding to the number of data
	plt.rcParams['font.size'] = 15
	ax.legend(ncol=number_col, loc=legend_position, bbox_to_anchor=(lp_x, lp_y))
	if(adjust_flag == 1):
		plt.subplots_adjust(left=adjust_left, bottom=adjust_bottom, right=adjust_right, top=adjust_top)

	#only for data looks better.
	ax_bar = ax.twinx()
	ax_bar.bar([i for i in range(0, len(xticks))], [0 for i in range(0, len(xticks))], align="center", linewidth=0, color='w')
	ax_bar.tick_params(labelright="off")
  
	ax.set_xlabel(xlabel, fontsize=20)
	ax.set_ylabel(ylabel, fontsize=20)

if connected_flag == 1:
	fig.savefig(datapath + "line/pdr" + r_lf[1] + ".png")
	fig.savefig(datapath + "line/pdr" + r_lf[1] + ".eps")
else:
	fig.savefig(datapath + "results_graphics/pdr_changing_links.png")
	fig.savefig(datapath + "results_graphics/pdr_changing_links.eps")

connected_flag = 0
xlabel = "Quality of links [%]"
ylabel = "Amount of RPL-parents' change"
number_col = 1
legend_position = "upper left"
lp_x = 1
lp_y = 1
adjust_flag = 1
adjust_left = 0.15
adjust_bottom = 0.15
adjust_right = 0.8
adjust_top = 0.9

fig, ax = plt.subplots()
for k in range (0, len(results)):
	r_lf = results[k]

	if connected_flag == 1:
		ax.errorbar([i for i in range(0, len(xticks))], parents_set_conf_k_results[k], yerr=parents_set_conf_k_error[k], label=graph_label_dict[r_lf[1]], color=graph_color_dict[r_lf[1]])
	else:
		ax.errorbar([i for i in range(0, len(xticks))], parents_set_conf_k_results[k], yerr=parents_set_conf_k_error[k], label=graph_label_dict[r_lf[1]], color=graph_color_dict[r_lf[1]], fmt=graph_format_dict[r_lf[1]])

	plt.xticks([i for i in range(0, len(xticks))], xticks, rotation=0) #must be changed correpoiding to the number of data
	plt.rcParams['font.size'] = 15
	ax.legend(ncol=number_col, loc=legend_position, bbox_to_anchor=(lp_x, lp_y))
	if(adjust_flag == 1):
		plt.subplots_adjust(left=adjust_left, bottom=adjust_bottom, right=adjust_right, top=adjust_top)

	#only for data looks better.
	ax_bar = ax.twinx()
	ax_bar.bar([i for i in range(0, len(xticks))], [0 for i in range(0, len(xticks))], align="center", linewidth=0, color='w')
	ax_bar.tick_params(labelright="off")
  
	ax.set_xlabel(xlabel, fontsize=20)
	ax.set_ylabel(ylabel, fontsize=20)

if connected_flag == 1:
	fig.savefig(datapath + "line/pdr" + r_lf[1] + ".png")
	fig.savefig(datapath + "line/pdr" + r_lf[1] + ".eps")
else:
	fig.savefig(datapath + "results_graphics/parents_set_changing_links.png")
	fig.savefig(datapath + "results_graphics/parents_set_changing_links.eps")

connected_flag = 0
xlabel = "Quality of links [%]"
ylabel = "Jitter [ms]"
number_col = 1
legend_position = "upper left"
lp_x = 1
lp_y = 1
adjust_flag = 1
adjust_left = 0.15
adjust_bottom = 0.15
adjust_right = 0.8
adjust_top = 0.9

fig, ax = plt.subplots()
for k in range (0, len(results)):
	r_lf = results[k]

	if connected_flag == 1:
		ax.errorbar([i for i in range(0, len(xticks))], jitter_conf_k_results[k], yerr=jitter_conf_k_error[k], label=graph_label_dict[r_lf[1]], color=graph_color_dict[r_lf[1]])
	else:
		ax.errorbar([i for i in range(0, len(xticks))], jitter_conf_k_results[k], yerr=jitter_conf_k_error[k], label=graph_label_dict[r_lf[1]], color=graph_color_dict[r_lf[1]], fmt=graph_format_dict[r_lf[1]])

	plt.xticks([i for i in range(0, len(xticks))], xticks, rotation=0) #must be changed correpoiding to the number of data
	plt.rcParams['font.size'] = 15
	ax.legend(ncol=number_col, loc=legend_position, bbox_to_anchor=(lp_x, lp_y))
	if(adjust_flag == 1):
		plt.subplots_adjust(left=adjust_left, bottom=adjust_bottom, right=adjust_right, top=adjust_top)

	#only for data looks better.
	ax_bar = ax.twinx()
	ax_bar.bar([i for i in range(0, len(xticks))], [0 for i in range(0, len(xticks))], align="center", linewidth=0, color='w')
	ax_bar.tick_params(labelright="off")
  
	ax.set_xlabel(xlabel, fontsize=20)
	ax.set_ylabel(ylabel, fontsize=20)

if connected_flag == 1:
	fig.savefig(datapath + "line/jitter" + r_lf[1] + ".png")
	fig.savefig(datapath + "line/jitter" + r_lf[1] + ".eps")
else:
	fig.savefig(datapath + "results_graphics/jitter_changing_links.png")
	fig.savefig(datapath + "results_graphics/jitter_changing_links.eps")

connected_flag = 0
xlabel = "Quality of links [%]"
ylabel = "Delay [ms]"
number_col = 1
legend_position = "upper left"
lp_x = 1
lp_y = 1
adjust_flag = 1
adjust_left = 0.15
adjust_bottom = 0.15
adjust_right = 0.8
adjust_top = 0.9

fig, ax = plt.subplots()
for k in range (0, len(results)):
	r_lf = results[k]
	delay_conf_k_error[k][0] = delay_conf_k_error[k][0]
	print "resultados de delay", delay_conf_k_results[k], "error", delay_conf_k_error[k]
	if connected_flag == 1:
		ax.errorbar([i for i in range(0, len(xticks))], delay_conf_k_results[k], yerr=delay_conf_k_error[k], label=graph_label_dict[r_lf[1]], color=graph_color_dict[r_lf[1]])
	else:
		ax.errorbar([i for i in range(0, len(xticks))], delay_conf_k_results[k], yerr=delay_conf_k_error[k], label=graph_label_dict[r_lf[1]], color=graph_color_dict[r_lf[1]], fmt=graph_format_dict[r_lf[1]])
#	print "resultados de delay", delay_conf_k_results[k], "error", delay_conf_k_error[k]
	plt.xticks([i for i in range(0, len(xticks))], xticks, rotation=0) #must be changed correpoiding to the number of data
	plt.rcParams['font.size'] = 15
	ax.legend(ncol=number_col, loc=legend_position, bbox_to_anchor=(lp_x, lp_y))
	if(adjust_flag == 1):
		plt.subplots_adjust(left=adjust_left, bottom=adjust_bottom, right=adjust_right, top=adjust_top)

	#only for data looks better.
	ax_bar = ax.twinx()
	ax_bar.bar([i for i in range(0, len(xticks))], [0 for i in range(0, len(xticks))], align="center", linewidth=0, color='w')
	ax_bar.tick_params(labelright="off")
  
	ax.set_xlabel(xlabel, fontsize=20)
	ax.set_ylabel(ylabel, fontsize=20)

if connected_flag == 1:
	fig.savefig(datapath + "line/delay" + r_lf[1] + ".png")
	fig.savefig(datapath + "line/delay" + r_lf[1] + ".eps")
else:
	fig.savefig(datapath + "results_graphics/delay_changing_links.png")
	fig.savefig(datapath + "results_graphics/delay_changing_links.eps")

print "*********************************************************"
print "Script finished"
print "*********************************************************"

plt.show()


