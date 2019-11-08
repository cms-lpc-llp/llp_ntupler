import os
import subprocess, time, sys, shlex
import fileinput


#model = ['n3n2-n1-hbb-hbb', 'n3n2-n1-zll-hbb']
#model = ['n3n2-n1-zll-hbb']
model = ['x1n2-n1-wlv-hbb']
mh = [200]
ctau = ['pl1000']
filePerJob = 1

pwd = os.getcwd()
home_dir = '/mnt/hadoop/store/group/phys_exotica/jmao/aodsim/RunIISummer16/AODSIM/'
for i,m in enumerate(model):
#     print(i,m)
    if m=='x1n2-n1-wlv-hbb':
        str1 = 'MSSM-2d-prod'
    else:
        str1 = 'MSSM-1d-prod'
        
    for j,mm in enumerate(mh):
	    for k,ct in enumerate(ctau):
	        #print(j,mm,ct)
		str2 = m+'_mh'+str(mm)+'_'+ct+'_ev100000'
		if ct=='prompt':
			str2 = m+'_m'+str(mm)+'_'+ct+'_ev100000'
		str3 = 'crab_CMSSW_8_0_21_'+m+'_mchi'+str(mm)+'_'+ct+'_ev100000_AODSIM_CaltechT2'
		#print(str2, str3)        
		list_dir = pwd.replace('condor_scripts','lists/')+m+'/'
		os.system('mkdir -p '+list_dir)
		list_name = str2+'_fullsim.list'
		list_name = 'WplusH_HToSSTobbbb_WToLNu_MH-125_MS-40_ctauS-1000_fullsim.list'
		#list_name = 'WminusH_HToSSTobbbb_WToLNu_MH-125_MS-40_ctauS-1000_fullsim.list'  
		#list_name = 'x1n2-n1-wlv-hbb_mchi200_mlsp150_pl1000_ev100000_fullsim.list'
		#list_name = 'n3n2-n1-zll-hbb_mh200_pl100_ev100000_fullsim.list'
		#print(list_name)
	
		script_dir = list_dir.replace('lists','scripts')
		os.system('mkdir -p '+script_dir)
		#script_name = list_name.replace('.list','_signal_aod.py')
	
		temp_name = 'signal_template_aod.py'
		#script_name = script_dir+script_name
		#print(script_dir)
		#print(script_name)
	
	
	        exe_dir = pwd+'/jobs/job_exes/'+m+'/'
	        jbl_dir = pwd+'/jobs/job_jbls/'+m+'/'
	        log_dir = pwd+'/jobs/job_logs/'+m+'/'
	        os.system('mkdir -p '+exe_dir)
	        os.system('mkdir -p '+jbl_dir)
	        os.system('mkdir -p '+log_dir)
		print(exe_dir)
	
	        exe_temp = 'signal_template_aod_job_fullsim.sh'
	        jbl_temp = 'signal_template_aod_job.jbl'
	
		nfiles=num_lines = sum(1 for line in open(list_dir+list_name))
		#print(nfiles)
		njobs=nfiles/filePerJob
		#print(njobs)
		nleft=nfiles%filePerJob
		#print(nleft)
		if nleft!=0:
			njobs += 1 
		print('Total '+str(nfiles)+' files, '+str(filePerJob)+' file(s) per job, '+str(njobs)+' jobs.')
	
		#n job scripts
		for job in range(njobs):
			script_name = list_name.replace('.list','_signal_aod_'+str(job)+'Of'+str(njobs)+'.py')
			print(script_name)
		
		        exe_name = script_name.replace('.py','.sh')
		        jbl_name = script_name.replace('.py','.jbl')
			print(exe_dir+exe_name)
			#print(exe_name)
			#print(jbl_name)
		
		        label = script_name.replace('.py','')
	
		        output_dir = '/store/group/phys_exotica/jmao/susy_llp/fullsim/'+str1+'/'+m+'/'+str2+'/'
	       		output_name = script_name.replace('.py','.root')
	        	print(output_name)
		        with open(exe_temp,'r') as fin_exe:
		        	with open(exe_dir+exe_name,'w') as fout_exe:
		        		for line in fin_exe:
		        			if 'condor_scripts' in line:
		        				new_line = line.replace(line,'cd '+script_dir+'\n')
		        			elif 'gfal-copy bbbb_ggh_mh125_mx50_pl500_signal_aod_test.py' in line:
		        				new_line = line.replace('gfal-copy','gfal-copy -f').replace('bbbb_ggh_mh125_mx50_pl500_signal_aod_test.py',script_dir+script_name)
		        			elif 'cmsRun bbbb_ggh_mh125_mx50_pl500_signal_aod_test.py' in line:
		        				#new_line = ''
		        				new_line = line.replace('bbbb_ggh_mh125_mx50_pl500_signal_aod_test.py',script_name)
		        			elif '/store/group/phys_exotica/jmao/jet_timing_studies/samples/signal/' in line:
		        				new_line = line.replace('/store/group/phys_exotica/jmao/jet_timing_studies/samples/signal/',output_dir)
		        			elif 'bbbb_ggh_mh125_mx50_pl500.root' in line:
						#gfal-copy -f ntuple_RunIISummer16_bbbb_ggh_mh125_mx50_pl500.root gsiftp://transfer.ultralight.org//store/user/jmao/output_test.root
		        				new_line = line.replace('RunIISummer16_bbbb_ggh_mh125_mx50_pl500.root','RunIIFall17_'+output_name).replace('/store/user/jmao/output_test.root',output_dir+'ntuple_RunIIFall17_'+output_name)
		        				#new_line = ''
							print(new_line)
		        			else: 
		        				new_line = line
		        			fout_exe.write(new_line)
		
		        	fout_exe.close()
		        	fin_exe.close()
		
		        with open(jbl_temp,'r') as fin_jbl:
		        	with open(jbl_dir+jbl_name,'w') as fout_jbl:
		        		for line in fin_jbl:
		        			if 'signal_template_aod_job.sh' in line:
		        				new_line = line.replace('signal_template_aod_job.sh',exe_dir+exe_name)
		        			elif 'logs' in line:
		        				new_line = line.replace('logs/test_job',log_dir+label)
		        			else: 
		        				new_line = line
		        			fout_jbl.write(new_line)
		
		        	fout_jbl.close()
		        	fin_jbl.close()
		
		        cmd = 'condor_submit '+jbl_dir+jbl_name
		        print(cmd)
		        make_job = subprocess.Popen([cmd],stdout=subprocess.PIPE, shell=True);
		        out = make_job.communicate()
		        print(out)
	
	