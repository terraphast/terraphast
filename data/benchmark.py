#!/usr/bin/python
import glob, os, time, subprocess, re, sys


def benchmarkTime(nwk_file, data_file, output_folder, command):
    start_time = time.time()
    out_file = "%s%s%s.%s.out" % (output_folder, os.sep, os.path.basename(data_file), int(round(time.time() * 1000)))
    arg_list = command % (nwk_file, data_file, out_file)
    subprocess.call(arg_list, shell=True)
    end_time = time.time()
    return (end_time - start_time)

def benchmarkMemory(nwk_file, data_file, output_folder, command):
    fnull = open(os.devnull, 'w')
    massif_file = "%s%s%s.massif.out" % (output_folder, os.sep, os.path.basename(data_file))
    arg_list = command % (nwk_file, data_file, "/dev/null")
    valgrind_args = "valgrind --tool=massif --pages-as-heap=yes --massif-out-file=%s %s" % (massif_file, arg_list)
    subprocess.call(valgrind_args, shell=True, stderr=fnull)
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

def doBenchmark(nwk_file, data_file, output_folder, command):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    times = [benchmarkTime(nwk_file, data_file, output_folder, command) for i in range(10)]
    mem = benchmarkMemory(nwk_file, data_file, output_folder, command)
    print "%s,%s,%.4f,%.3f" % (nwk_file, data_file, sum(times)/len(times),mem/(1024**2))

with open(sys.argv[1]) as f:
    for line in f:
        nwk_file = line.rstrip()
        data_file = nwk_file.replace("nwk","data")
        doBenchmark(nwk_file, data_file, sys.argv[2], sys.argv[3])