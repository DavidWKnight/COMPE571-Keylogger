import os

print("Clearing dmesg")
os.system("sudo dmesg -C")

print("Inserting kernel module")
os.system("sudo insmod kbdirq/kbdirq.ko")

print("Start typing for keylogger")
os.system("./keylogger/keylogger.out")

print("\nRemoving kernel module")
os.system("sudo rmmod kbdirq/kbdirq.ko")

print("\nCopying dmesg")
os.system("dmesg > kernel_module.txt")

print("Interpreting data")

kmod = []
with open("kernel_module.txt", 'r') as kmodFile:
    kmod = kmodFile.readlines()
kmod = kmod[1:-1]

with open("keylogger.txt", 'r') as keyFile:
    key = keyFile.readlines()

if (len(key) != len(kmod)):
    print("Invalid log file lengths")
    exit(1)

time = []
for i in range(len(key)):
    keyTime = int(key[i].split()[-2].rstrip(',')) + int(key[i].split()[-1])/1000000
    kmodTime = int(kmod[i].split()[-2].rstrip(',')) + int(kmod[i].split()[-1])/1000000000
    #print(f"{keyTime}, {kmodTime}, {keyTime - kmodTime}")
    time.append(keyTime - kmodTime)

total = 0
for timestamp in time:
    timestamp = timestamp*1000 # get ms
    total += timestamp
print(f"Average extra time taken by X11: {round(total/len(time), 3)}ms")

with open("results.txt", 'w') as resultFile:
    for i in range(len(key)):
        resultFile.write(key[i].split()[0].rstrip(',') + ": " + str(time[i]*1000) + "ms\n")