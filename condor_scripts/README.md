# Steps to run condor on cmsRun job at Caltech tier2

## Step 1
Make lists on signal samples:

```
python make_list.py
```


output in `../lists/`.
## Step 2
Make cmsRun scripts from templates.

```
python make_cmsrun_script.py
```
with template `signal_template_aod.py`.

output in `../scripts/`.
## Step 3
Make condor scripts from templates.

```
python make_condor_script.py
```

with template `signal_template_aod_job.jbl` and `signal_template_aod_job.sh`.

REMEMBER to change directory in `signal_template_aod_job.sh`.

output in `jobs/`.

It will submit the condor jobs automatically.

If you want to test it first, comment out the following lines at the end of `make_condor_scripts.py`:

```
make_job = subprocess.Popen([cmd],stdout=subprocess.PIPE, shell=True);
out = make_job.communicate()
print(out)
```
