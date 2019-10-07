import os
import subprocess, time, sys, shlex
import fileinput


#model = ['x1n2-n1-wlv-hbb']
model = ['n3n2-n1-hbb-hbb']
mh = [200]
#ctau = [100]
filePerJob = 3
#ctau = [ 'prompt']
#ctau = [ 'pl100', 'prompt']
ctau = ['pl100', 'pl1000', 'pl10000','prompt']
#ctau = ['pl100', 'pl1000', 'pl10000']

pwd = os.getcwd()
home_dir = '/mnt/hadoop/store/group/phys_exotica/jmao/aodsim/RunIISummer16/AODSIM/'
for i,m in enumerate(model):
#     print(i,m)
    if m=='x1n2-n1-wlv-hbb':
        str1 = 'MSSM-2d-prod'
    else:
        str1 = 'MSSM-1d-prod'
        
#/mnt/hadoop/store/group/phys_exotica/jmao/susy_llp/MSSM-1d-prod/n3n2-n1-hbb-hbb/n3n2-n1-hbb-hbb_m200_prompt_ev100000/ntuple_RunIISummer16_n3n2-n1-hbb-hbb_m200_prompt_ev100000_signal_aod_0Of34.root
#/mnt/hadoop/store/group/phys_exotica/jmao/susy_llp/MSSM-1d-prod/n3n2-n1-hbb-hbb/n3n2-n1-hbb-hbb_mh200_pl100_ev100000/ntuple_RunIISummer16_n3n2-n1-hbb-hbb_mh200_pl100_ev100000_signal_aod_0Of33.root 

    for j,mm in enumerate(mh):
	    for k,ct in enumerate(ctau):
	        #print(j,mm,ct)
		str2 = m+'_mh'+str(mm)+'_'+ct+'_ev100000'
		if ct=='prompt':
			str2 = m+'_m'+str(mm)+'_'+ct+'_ev100000'
		root_dir = '/mnt/hadoop/store/group/phys_exotica/jmao/susy_llp/'+str1+'/'+m+'/'+str2+'/'
		root_name = 'ntuple_RunIISummer16_'+str2+'_signal_aod_*.root'
		out_name = 'ntuple_RunIISummer16_'+str2+'_signal_aod.root'
	
	
		hadd_cmd = 'hadd '+out_name+' '+root_dir+root_name
		#print(hadd_cmd)
		#hadd_exe = subprocess.Popen([hadd_cmd],stdout=subprocess.PIPE, shell=True);
		#hadd_out = hadd_exe.communicate()
		#print(hadd_out)
	
		unsetsw_cmd='eval `scram unsetenv -sh`'
		#print(unsetsw_cmd)
		#unsetsw_exe = subprocess.Popen([unsetsw_cmd],stdout=subprocess.PIPE, shell=True);
		#unsetsw_out = unsetsw_exe.communicate()
		#print(unsetsw_out)

		copy_dir = '/store/group/phys_exotica/jmao/susy_llp/'+str1+'/'+m+'/'
		transfer_cmd = 'gfal-copy -f '+out_name+' gsiftp://transfer.ultralight.org/'+copy_dir+out_name
		
		#copy scripts
		script_name = 'copy_'+m+'.sh'
		print(script_name)
		
		with open(script_name,'w') as fout:
			fout.write('#!/bin/sh \n')
			fout.write('voms-proxy-info -all \n')
			fout.write(unsetsw_cmd+' \n')	
			fout.write(transfer_cmd+' \n')	
		fout.close()
	
		copy_cmd='source '+pwd+'/'+script_name
		print(copy_cmd)
		copy_exe = subprocess.Popen([copy_cmd],stdout=subprocess.PIPE, shell=True);
		copy_out = copy_exe.communicate()
		print(copy_out)
	
