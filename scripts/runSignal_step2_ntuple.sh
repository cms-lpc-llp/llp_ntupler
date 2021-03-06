#!/bin/sh

hostname
echo "Job started"
date
start_time=`date +%s`
config1=$1
config2=$2
inputfilelist=$3
filePerJob=$4
jobnumber=$5
outputfile=$6
outputDirectory=$7
CMSSW_BASE=$8
homeDir=$9
year=$10
currentDir=`pwd`
#user=${homeDir#*/data/}
user=${homeDir#*/storage/user/}
runDir=${currentDir}/${user}_${code_dir_suffix}/



rm -rf ${runDir}
mkdir -p ${runDir}

if [ -f /cvmfs/cms.cern.ch/cmsset_default.sh ]
then
	#setup cmssw
	#cd $CMSSW_BASE/src/
	cd ${CMSSW_BASE}/src/
	workDir=`pwd`
	echo "entering directory: ${workDir}"
	source /cvmfs/cms.cern.ch/cmsset_default.sh
	#export SCRAM_ARCH=slc6_amd64_gcc630
	if [ ${year} == "Summer16" ]
	then
		echo "export SCRAM_ARCH=slc6_amd64_gcc530"
		export SCRAM_ARCH=slc6_amd64_gcc530
        elif [ ${year} == "Fall17" ]
	then
		echo "export SCRAM_ARCH=slc6_amd64_gcc630"
		export SCRAM_ARCH=slc6_amd64_gcc630
	else
		echo "export SCRAM_ARCH=slc6_amd64_gcc700"
		export SCRAM_ARCH=slc6_amd64_gcc700
	fi


	ulimit -c 0
	eval `scram runtime -sh`
	echo `which root`

	cd ${runDir}
	echo "entering directory: ${runDir}"

	cp $CMSSW_BASE/src/cms_lpc_llp/llp_ntupler/python/${config1} ./
	cp $CMSSW_BASE/src/cms_lpc_llp/llp_ntupler/python/${config2} ./
	#get grid proxy
	export X509_USER_PROXY=${homeDir}x509_proxy
	echo "${homeDir}x509_proxy"
	voms-proxy-info


	#run the job
	ls ${CMSSW_BASE}${inputfilelist}
	cat ${CMSSW_BASE}${inputfilelist} | awk "NR > (${jobnumber}*${filePerJob}) && NR <= ((${jobnumber}+1)*${filePerJob})" > inputfilelistForThisJob_${jobnumber}.txt
	
	
	echo ""
	echo "************************************"
	echo "Running on these input files:"
	cat inputfilelistForThisJob_${jobnumber}.txt
	echo "************************************"
	sed -i "s\/mnt\'file:/mnt\g" inputfilelistForThisJob_${jobnumber}.txt
#        sed -i "s\/mnt/hadoop\'\g" inputfilelistForThisJob_${jobnumber}.txt
	sed -i "s:.root:.root',:g" inputfilelistForThisJob_${jobnumber}.txt
	sed -i "/inputfilename/r inputfilelistForThisJob_${jobnumber}.txt" ${config1}
	sed -i "s/outputfilename/${outputfile}/g" ${config2}
	
	echo "************************************"
        echo "Running on this config:"
	cat ${config1}
        echo "************************************"
	echo ""
	echo "running cmsRun now"
	echo "cmsRun ${config1}"
	echo "here"
#        cmsRun /storage/user/christiw/login-1/christiw/LLP/CMSSW_10_2_16/src/cms_lpc_llp/llp_ntupler/python/displacedJetMuon_step2_ntupler_MC_Fall18_cfg.py
	cmsRun ${config1}
	cmsRun ${config2}
	echo ${outputfile}

	echo ${outputDirectory}

	##^_^##
	echo "RazorRun_T2 finished"
	date

	sleep 2
	echo "I slept for 2 second"

	##job finished, copy file to T2
	echo "copying output file to /mnt/hadoop/${outputDirectory}"
	if [ -f ${outputfile} ]
	then
		eval `scram unsetenv -sh`
		gfal-mkdir -p gsiftp://transfer.ultralight.org//${outputDirectory}
		gfal-copy --checksum-mode=both ${outputfile} gsiftp://transfer.ultralight.org//${outputDirectory}/${outputfile}
		#mkdir -p ${outputDirectory}
		#cp ${outputfile} ${outputDirectory}/${outputfile}
	else
		echo "output doesn't exist"
	fi
	if [ -f /mnt/hadoop/${outputDirectory}/${outputfile} ]
	#if [ -f /${outputDirectory}/${outputfile} ]
	then
		echo "ZZZZAAAA ============ good news, job finished successfully "
	else
		echo "YYYYZZZZ ============ somehow job failed, please consider resubmitting"
	fi

else
	echo "VVVVYYYY ============= failed to access file /cvmfs/cms.cern.ch/cmsset_default.sh, job abandoned"
fi

cd ${currentDir}
#rm -rf ${runDir}
echo "Job finished"
date
end_time=`date +%s`
runtime=$((end_time-start_time))
echo ${runtime}
