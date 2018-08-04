import socket
import struct
import time

from plotly.utils import numpy

nintendo_ip = "10.42.0.251"
nintendo_port = 8080

nds_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
nds_client.connect((nintendo_ip, nintendo_port))

timestamps = []
temperatures = []
temp_probes = 300

while temp_probes > 0:
    timestamp_total = time.time()
    nds_client.send('A'.encode('ascii'))
    temp = struct.unpack('I', nds_client.recv(4))[0]
    temp_c = temp / 4096.0

    print("Temp as int: " + str(temp))
    print("Temp in *C: " + str(temp_c))

    total_time = time.time() - timestamp_total
    print("Total time: " + str(total_time))
    temperatures.append(temp_c)
    timestamps.append(time.time())
    temp_probes -= 1
    time.sleep(1)

numpy.savetxt("temperatures.npy", temperatures, fmt='%1.5f')
numpy.savetxt("timestamps.npy", timestamps, fmt='%i')
