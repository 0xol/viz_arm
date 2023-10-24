import config.config as comp
import os
import subprocess
import threading
import multiprocessing
import time

#clean build
if os.path.isdir("bin"):
    subprocess.run("rm -rf bin", shell=True)
os.mkdir("bin")

#get sources
src_files = []
src_lock = threading.Lock()
def get_src(path: str):
    for file in os.listdir(path):
        if file.split(".")[-1] == "c" or file.split(".")[-1] == "S": src_files.append(f"{path}/{file}"); continue
        if os.path.isdir(f"{path}/{file}"): get_src(f"{path}/{file}"); continue

get_src(f"arch/{comp.arch}")
get_src(f"boot/{comp.bootloader}")
get_src(f"lib")
get_src("drivers")
#get_src("fs")
get_src("kernel")
get_src("mm")

#build sources
def builder():
    while True:
        if src_files == []: return
        src_lock.acquire()
        src_file = src_files.pop()
        src_lock.release()

        if src_file.split(".")[-1] == "c":
            print(f"[CC] {src_file}")
            subprocess.run(f"{comp.cc} {comp.cc_flags} -c {src_file} -o bin/{src_file.replace('.c', '').replace('/','_')}.o", shell=True)
            continue
        if src_file.split(".")[-1] == "S":
            print(f"[AS] {src_file}")
            subprocess.run(f"{comp.asm} {src_file} -o bin/{src_file.replace('.c', '').replace('/','_')}.o", shell=True)
            continue

threads = []
for _ in range(multiprocessing.cpu_count()):
    threads.append(threading.Thread(target=builder))

for thread in threads:
    thread.start()

for thread in threads:
    while thread.is_alive():
        time.sleep(1)

#compile obj
print("[LD] kernel.o")
subprocess.run(f"{comp.ld} -r bin/*.o -o kernel.o", shell=True)

#driver map

#build and strip kernel bin
print("[LD] viz.bin")
subprocess.run(f"{comp.ld} {comp.ld_flags} kernel.o -o viz.bin", shell=True)
#subprocess.run(f"{comp.strip} viz.bin", shell=True)
