import os

ip = input('Digite o IP do servidor:')
topic = input('Digite o nome do tópico:')

message = "mosquitto_pub -h " + ip + " -t \"" + topic + "\" -m "


for i in range(10):
	a = "\"" + str(i) + "\""
	os.system(message + a)
	print(message + a)