import network
import time

l = network.LAN()  # no args; set up OPENETH based LAN interface
print(l.ifconfig())  # ('0.0.0.0', '0.0.0.0', '0.0.0.0', '0.0.0.0')
l.active(1)  # True
time.sleep(4)
print(l.ifconfig())  # ('10.0.2.15', '255.255.255.0', '10.0.2.2', '10.0.2.3')

# https://docs.micropython.org/en/latest/library/network.html
import socket

addr = socket.getaddrinfo('192.168.1.102', 8000)[0][-1]
print(addr)  # ('192.168.1.102', 8000)

s = socket.socket()
s.connect(addr)
s.send(b'GET /README.md HTTP/1.1\r\nHost: 192.168.1.102\r\n\r\n')
data = s.recv(1000)
print(data)
s.close()

