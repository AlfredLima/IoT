# MQTT
Learning about MQTT and NodeMCU.

## Install
- sudo apt install mosquitto
- sudo apt instsll mosquitto-clients

## Enable server
- sudo systemctl enable mosquitto.service

## Query
- systemctl status mosquitto.service

## Publisher
- mosquitto_sub -h IP -t "TOPIC" -m "MESSAGE"

## Subcribe
-mosquitto_sub -h IP -t "TOPIC"
