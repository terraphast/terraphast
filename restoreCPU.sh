#!/bin/bash
# Run the CPU at the minimum frequency.
# For Intel CPUs using the intel_pstate driver (Sandy Bridge and newer).
sudo cpupower frequency-set --governor powersave
# Scales the frequency dynamically according to current load.
# Jumps to the highest frequency and then possibly back off as the idle time increases. F
# For AMD and older Intel CPU.
#sudo cpupower frequency-set --governor ondemand