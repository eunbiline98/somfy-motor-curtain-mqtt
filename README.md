# somfy-motor-curtain-mqtt
DIY Motorize Curtain Somfy Control Using Esp8266 MQTT Protocol
- Motorize Curtain Somfy Glydea                                       
![image](https://user-images.githubusercontent.com/50385294/150663952-9b5efd5b-25a2-4542-b6ee-5193704c602c.png)

Datasheet Motorize Curtain                                                     
- ![image](https://user-images.githubusercontent.com/50385294/150664033-c2ac19dd-dc7f-4562-808a-cde2637e8508.png)
- https://www.somfypro.ca/documents/3931489/18635940/Glydea-Databook.pdf

List Item

- Esp8266 (Wemos D1 Mini)
- Optocoupler PC817
- Power Module Hi-Link 220VAC to 5VDC
- Resistor 1k 1/4 Watt
- Terminal Screw 2 Pin Hole

# Schematic
![schematic](https://user-images.githubusercontent.com/50385294/149084904-f7d43823-94c6-4acb-9ad4-0a0f2bf50791.png)

# Control
- Open Curtain
- Close Curtain
- Stop Curtain

# Home Assistant Setup 

Under covers in HA, use this config Motorize Curtain Control
```
  - platform: mqtt
    name: "Curtain Living Room"
    command_topic: "home/curtain/livingroom"
    state_topic: "home/curtain/state"
    qos: 0
    retain: true
    payload_open: "u"
    payload_close: "d"
    payload_stop: "s"
    state_open: "open"
    state_opening: "opening"
    state_closed: "closed"
    state_closing: "closing"
    optimistic: false
```

Under covers in HA, use this config MQTT

```
mqtt:
  broker: "Your IP Broker/server"
  port: Your port MQTT
  username: "Your username"
  password: Your password
  
```

# Documentation
https://user-images.githubusercontent.com/50385294/149776929-379bbfac-4cdb-4a9a-a8d9-ceeab45c952e.mp4

