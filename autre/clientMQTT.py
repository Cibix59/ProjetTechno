import paho.mqtt.client as mqtt #import the client1
import time
import requests
import json


############
def on_message(client, userdata, message):
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)



    info = json.loads(str(message.payload.decode("utf-8")))
    print("transformé:",info)
    topic = message.topic
    if topic == "log":
        response = requests.post('http://172.16.203.109:3000/api/historique/log', json = info)
    else:

        print("Code not found")


########################################

boucle = True
broker_address="172.16.206.200"
print("creating new instance")
client = mqtt.Client("P1") #cree une nouvelle instance
client.on_message=on_message #attache une fonction au callback
print("connecting to broker")
client.connect(broker_address) #connection au broker
client.loop_start() #demarre la boucle
while boucle :
    client.subscribe("demande/rfid")
    time.sleep(4) 
client.loop_stop() #stop la boucle


