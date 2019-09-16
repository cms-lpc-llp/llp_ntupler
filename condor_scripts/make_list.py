import os
import subprocess, time, sys, shlex

#/mnt/hadoop/store/group/phys_exotica/jmao/aodsim/RunIISummer16/AODSIM/MSSM-2d-prod/x1n2-n1-wlv-hbb_mh200_pl10000_ev100000/crab_CMSSW_8_0_21_x1n2-n1-wlv-hbb_mchi200_pl10000_ev100000_AODSIM_CaltechT2/190912_142459/0000/n3n2-n1-hbb-hbb_step2_98.root

#model = ['n3n2-n1-hbb-hbb']
model = ['x1n2-n1-wlv-hbb']
mh = [200]
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
	        print(j,mm,k,ct)
		str2 = m+'_mh'+str(mm)+'_'+ct+'_ev100000'
		if ct=='prompt':
			str2 = m+'_m'+str(mm)+'_'+ct+'_ev100000'
		str3 = 'crab_CMSSW_8_0_21_'+m+'_mchi'+str(mm)+'_'+ct+'_ev100000_AODSIM_CaltechT2'
		#print(str2, str3)        
		list_dir = pwd.replace('condor_scripts','lists/')+m+'/'
		os.system('mkdir -p '+list_dir)
		list_name = str2+'.list'
		cmd = 'ls '+home_dir+str1+'/'+str2+'/'+str3+'/*/*/*root > '+list_dir+list_name
		#print(cmd)
		make_list = subprocess.Popen([cmd],stdout=subprocess.PIPE, shell=True);
	        out = make_list.communicate()
	        print(list_dir+list_name)
	        print(out)

		
