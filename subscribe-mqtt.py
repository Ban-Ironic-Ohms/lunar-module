from paho.mqtt import client as mqtt_client
import random
import json
from data_sort import parse

# RECIVING DATA IN FOLLOWING FORMAT:
"""
LM-TIDE:GROUPNAME:TEMP:HUMID:ECO2:TVOC:H2:ETHANOL
"""

# broker = "test.mosquitto.org"
broker = "broker.hivemq.com"
# broker = "mqtt.tideyb.org"
port = 1883
topic = "tide-lm-data-send"
client_id = f'python-mqtt-{random.randint(0, 1000)}'


def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
            client.subscribe(topic)
        else:
            print("Failed to connect, return code %d\n", rc)
    # Set Connecting Client ID
    client = mqtt_client.Client(client_id)
    # client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        print(msg.payload.decode(), type(msg.payload.decode()))
        parse(msg.payload.decode())

    client.subscribe(topic)
    client.on_message = on_message


def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()
    
