import paho.mqtt.client as mqtt #import the client1
import time
import requests
import json
from datetime import date
import datetime


############
def on_message(client, userdata, message):
    print("ok1")
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)

    payload = str(message.payload.decode("utf-8"))
    """ print("transformé:",info) """

    headers = {'Accept': '*/*',
             'Accept-Encoding': 'gzip, deflate',
             'Connection': 'close',
             'Content-Length': '16',
             'Content-Type': 'application/json',
             'Host': 'httpbin.org',
             'User-Agent': 'python-requests/2.4.3 CPython/3.4.0',
             'X-Request-Id': 'xx-xx-xx'}
    topic = str(message.topic)
    print("ok2")


    dateTMP = str(date.today().strftime("%d/%m/%Y"))
    heure =str(datetime.datetime.now().strftime("%H:%M")) 
    """ 
    info = json.dumps({"topic": topic, "payload": payload, "date":  dateTMP, "heure": heure})

    print("ok2")
    response = requests.post('http://172.16.199.85:3000/api/historique/log', json = info,headers=headers) """

    data = {}
    data['topic'] = topic
    data['payload'] = payload
    data['date'] = dateTMP
    data['heure'] = heure
    if topic == "demande/rfid":##Clément
        data['nomPeripherique'] = "rfid"
        info = json.dumps(data)
        response = requests.post('http://172.16.199.85:3000/api/historique/log', data = info,headers=headers)
        print (response)
        if topic == "demande/rfid":  
            code = json.loads(str(message.payload.decode("utf-8")))
            codeRFID = {}
            codeRFID['codeRFID'] = code
            response = requests.post('http://172.16.199.85:3000/api/rfid/authRFID', json = codeRFID,headers=headers)
            print ("reponse auth"+response.text)
            if(response.text != "[]"):
                client.publish("zigbee2mqtt/0x00124b0023428a8a/set","{\"state\": \"ON\"}")
                time.sleep(1)
                client.publish("zigbee2mqtt/0x00124b0023428a8a/set","{\"state\": \"OFF\"}")
        else:
            print("Code not found")
    elif topic == "esp/co2" or topic == "zigbee2mqtt/0x00124b00250c0055/set" or topic == "zigbee2mqtt/0x00124b00250c0055/get":##Joséanne
        if topic == "esp/co2":
            data['nom'] = "co2"
            data['evenement'] = "Niveau CO2 " + data['payload']
        elif topic == "zigbee2mqtt/0x00124b00250c0055/set":
            data['nom'] = "lum"
            data['evenement'] = "Changement de l\'état de la lumière"
        else:
            data['evenement'] = "Récupération de l\'état de la lumière"
        info = json.dumps(data)
        response = requests.post('http://172.16.209.242:3002/api/addHistorique', data=info, headers=headers)
        print(response)
    else:##Pierre
        print("...")


########################################

boucle = True
broker_address="172.16.226.101"
print("creating new instance")
client = mqtt.Client("P1") #create new instance
client.on_message=on_message #attach function to callback
print("connecting to broker")
client.connect(broker_address) #connect to broker
client.loop_start() #start the loop
while boucle :
    ##Clément
    client.subscribe("demande/rfid")
    
    ##Joséanne
    client.subscribe("esp/co2")
    client.subscribe("zigbee2mqtt/0x00124b00250c0055/set")
    client.subscribe("zigbee2mqtt/0x00124b00250c0055/get")

    time.sleep(4) # wait
client.loop_stop() #stop the loop


