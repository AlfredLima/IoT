import os

ip = "localhost"
topic = "home"

message = "mosquitto_sub -h " + ip + " -t \"" + topic + "\""

print(message)
os.system(message)