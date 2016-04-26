import socket
import time
import struct


UDP_IP = "192.168.1.101"
UDP_PORT = 8888
MESSAGE = "GET_DATA"
#MESSAGE = "GET_INFO"

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MESSAGE

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

#sock.bind((UDP_IP, UDP_PORT))

sock.sendto("GET_INFO", (UDP_IP, UDP_PORT))
data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
print "received :", len(data)
print "bytes from", addr
print "received message:", data

s1 = struct.Struct('40s 40s 20s c f f ')
i = s1.unpack(data)
print "info:", i


time.sleep(0.5)
sock.sendto("GET_DATA", (UDP_IP, UDP_PORT))
data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
print "received :", len(data)
print "bytes from", addr

s2 = struct.Struct('f f f f f f f f f f c c c c c c c c c c c c')
d = s2.unpack(data)

print "received temp:", d
