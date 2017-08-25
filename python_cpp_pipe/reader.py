import os  
import time

read_path = "/tmp/vsd_fifo.pipe"

try:
    os.mkfifo(read_path)
except OSError, e:
    print "mkfifo error", e

rf = os.open(read_path, os.O_RDONLY)
print("os.open finished")

while True:
    data = os.read(rf, 1024)
    if len(data) == 0:
        print("nodata")
        time.sleep(1)
    print data

