import os
import subprocess, time, sys, shlex
import fileinput


#model = ['x1n2-n1-wlv-hbb']
model = ['n3n2-n1-hbb-hbb']
mh = [200]
#ctau = [100]
filePerJob = 3
#ctau = [ 'pl100', 'prompt']
ctau = ['pl100', 'pl1000', 'pl10000','prompt']

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
		list_name = str2+'.list'
	
		script_dir = list_dir.replace('lists/','scripts/')
		os.system('mkdir -p '+script_dir)
	
		temp_name = 'signal_template_aod.py'
	
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
			#script_name = script_dir+script_name
			print(script_dir+script_name)
		
			with open(list_dir+list_name,'r') as lin:
				with open(temp_name,'r') as fin:
					with open(script_dir+script_name,'w') as fout:
						for line in fin:
							output_name = script_name.replace('.py','.root')
							#print(output_name)
							if '.root' in line:
								if 'x1n2' in line:
									new_line = line.replace('ntuple_RunIISummer16_x1n2_wlv_hbb_pl10000.root', 'ntuple_RunIISummer16_'+output_name)
									fout.write(new_line)	
								if 'CMSSW' in line:
									#print(line)
									lcnt = 0
									for rt in lin:
										if lcnt in range(job*filePerJob, (job+1)*filePerJob) and lcnt<nfiles:
											#print(lcnt)
											#print(rt)
											rt_name = rt.replace('\n','')
											#new_line = line.replace(line,"\'file:"+str(lcnt)+"\',\n")
											new_line = line.replace(line,"\'file:"+rt_name+"\',\n")
											fout.write(new_line)	
										lcnt += 1
							else:
								new_line = line
								#print(line.replace('ntuple_RunIISummer16_bbbb_ggH.root', 'ntuple_RunIISummer16_'+output_name))
								fout.write(new_line)	
				fin.close()
				fout.close()
				lin.close()
	
	
