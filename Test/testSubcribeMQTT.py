import os

ip = "localhost"
topic = "home"

message = "mosquitto_pub -h " + ip + " -t \"" + topic + "\""

print(message)
os.system(message)