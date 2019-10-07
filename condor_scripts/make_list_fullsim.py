import os
import subprocess, time, sys, shlex

#/mnt/hadoop/store/group/phys_exotica/jmao/aodsim/RunIISummer16/AODSIM/MSSM-1d-prod/n3n2-n1-zll-hbb_mh200_pl1000_ev100000/crab_CMSSW_9_4_12_n3n2-n1-zll-hbb_mchi200_pl1000_ev100000_AODSIM_CaltechT2/191003_233732/0000/SUS-RunIIFall17DRPremix-00183_191.root

model = ['x1n2-n1-wlv-hbb', 'n3n2-n1-zll-hbb']
#model = ['n3n2-n1-hbb-hbb', 'n3n2-n1-zll-hbb']
mh = [200]
ctau = ['pl100']

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
	        print(j,mm,k,ct)
		str2 = m+'_mh'+str(mm)+'_'+ct+'_ev100000'
		if ct=='prompt':
			str2 = m+'_m'+str(mm)+'_'+ct+'_ev100000'
		str3 = 'crab_CMSSW_9_4_12_'+m+'_mchi'+str(mm)+'_'+ct+'_ev100000_AODSIM_CaltechT2'
		#print(str2, str3)        
		list_dir = pwd.replace('condor_scripts','lists/')+m+'/'
		os.system('mkdir -p '+list_dir)
		list_name = str2+'_fullsim.list'
		cmd = 'ls '+home_dir+str1+'/'+str2+'/'+str3+'/*/*/*root > '+list_dir+list_name
		#print(cmd)
		make_list = subprocess.Popen([cmd],stdout=subprocess.PIPE, shell=True);
	        out = make_list.communicate()
	        print(list_dir+list_name)
	        print(out)

		
