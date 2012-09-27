import sys
import time
interval = 0.1
while True:
    sys.stdout.write('\b-')
    sys.stdout.flush()
    time.sleep(interval)
    sys.stdout.write('\b\\')
    sys.stdout.flush()
    time.sleep(interval)
    sys.stdout.write('\b|')
    sys.stdout.flush()
    time.sleep(interval)
    sys.stdout.flush()
    sys.stdout.write('\b/')
    sys.stdout.flush()
    time.sleep(interval)
