#!/usr/bin/python
import glob, os, time, subprocess, re

def benchmarkTime(data_file, nwk_file):
    fnull = open(os.devnull, 'w')
    start_time = time.time()
    subprocess.call(["../../cmake-build-release/terraces_main",
                     data_file,
                     nwk_file], stdout=fnull)
    end_time = time.time()
    return (end_time - start_time)

def benchmarkMemory(data_file, nwk_file):
    fnull = open(os.devnull, 'w')
    massif_file = "%s.massif.out" % data_file
    subprocess.call(["valgrind",
                     "--tool=massif",
                     "--pages-as-heap=yes",
                     "--massif-out-file=%s" % massif_file,
                     "../../cmake-build-release/terraces_main",
                     data_file,
                     nwk_file], stdout=fnull, stderr=fnull)
    mem_peak = 0.0
    with open(massif_file) as f:
        mem_sum = 0.0
        for line in f:
            m = re.search('mem_heap_B\s*=\s*(\d+)', line)
            if m:
                mem_sum += int(m.group(1))
            m = re.search('mem_heap_extra_B\s*=\s*(\d+)', line)
            if m:
                mem_sum += int(m.group(1))
            m = re.search('mem_stacks_B\s*=\s*(\d+)', line)
            if m:
                mem_sum += int(m.group(1))
            m = re.search('snapshot\s*=\s*(\d+)', line)
            if m:
                if mem_sum > mem_peak:
                    mem_peak = mem_sum
                mem_sum = 0.0
    return mem_peak

os.chdir(".")
csv_file = open ("benchmark_count.csv", "w")
print >> csv_file, "\"Newick File\",\"Data File\",\"Runtime (seconds)\",\"Memory Peak (in MB)\""
for nwk_file in glob.glob("*.nwk*"):
    data_file = nwk_file.replace(".nwk",".data")

    print "Processing %s and %s" % (data_file,nwk_file)
    times = [benchmarkTime(data_file, nwk_file) for i in range(1)]
    #mem = benchmarkMemory(data_file, nwk_file)
    mem = 0

    print >> csv_file, "%s,%s,%.4f,%.3f" % (data_file,nwk_file, sum(times)/len(times),mem/(1024**2))