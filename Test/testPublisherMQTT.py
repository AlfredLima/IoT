import os
import time

ip = input('Digite o IP do servidor:')
print()
topic = input('Digite o nome do tópico:')
print()
message = "mosquitto_pub -h " + ip + " -t \"" + topic + "\" -m "


for i in range(10):
	a = "\"" + str(i) + "\""
	time.sleep(2)
	os.system(message + a)
	print(message + a)