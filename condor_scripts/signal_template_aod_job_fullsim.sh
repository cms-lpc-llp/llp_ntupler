#!/bin/sh

#Print out all bash commands
set -x

#Abort bash script on any error
set -e

#Print some basic debugging info
echo "whoami="`whoami`
echo "pwd="`pwd`
echo "hostname="`hostname`
echo "date="`date`
env

#print out proxy
voms-proxy-info -all

currentDir=`pwd`
runDir=${currentDir}/jmao_$RANDOM/
rm -rf ${runDir}
mkdir -p ${runDir}

#Inside singularity, the scratch directory is here
#This is also where the job starts out
echo "TMP:" `df -h $TMP`
echo "looking inside scratch directory BEFORE job"
ls -al $TMP

echo "Hello World"

#copy input
eval `scram unsetenv -sh`
cd /storage/user/jmao/login-1/jmao/CMSSW_9_4_9/src/cms_lpc_llp/jet_timing_studies/condor_scripts
gfal-copy bbbb_ggh_mh125_mx50_pl500_signal_aod_test.py ${runDir}

#Run cmsenv in an existing CMSSW directory on login-1
cd /storage/user/jmao/login-1/jmao/CMSSW_9_4_9/src
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scramv1 runtime -sh`

#run
cd ${runDir}
cmsRun bbbb_ggh_mh125_mx50_pl500_signal_aod_test.py
ls -lrtha
pwd

echo "pwd="`pwd`
#unset cmssw
eval `scram unsetenv -sh`
ls -lrtha
pwd
gfal-mkdir gsiftp://transfer.ultralight.org//store/group/phys_exotica/jmao/jet_timing_studies/samples/signal/
gfal-copy -f ntuple_RunIISummer16_bbbb_ggh_mh125_mx50_pl500.root gsiftp://transfer.ultralight.org//store/user/jmao/output_test.root
cd ..
gfal-rm -r ${runDir}

