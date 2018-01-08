# anavid
Standalone daemon process for controlling ANAVI Light pHAT over MQTT

# Installation

* Install Hassbian on Raspberry Pi

* Run the command below and enable I2C from *Interfacing Options*:

```
sudo raspi-config
```

* Install the MQTT broken Mosquitto usign the following [work around for Hassbian 1.31](https://github.com/home-assistant/hassbian-scripts/issues/76):

```
sudo curl -o /opt/hassbian/suites/install_mosquitto.sh https://raw.githubusercontent.com/ludeeus/Tools/master/hassbian-config/hotfix/install_mosquitto.sh && sudo hassbian-config install mosquitto

```

* Install wiringpi and piGPIO:

```
sudo apt-get update
sudo apt-get install -y wiringpi pigpio
```

* Install Paho (library for MQTT clients)

```
cd ~
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
make
sudo make install
```

* Build and install anavid

```
cd ~
git clone https://github.com/AnaviTechnology/anavid.git
cd anavid
make
sudo make install
```

* Start the system service

```
sudo systemctl start anavi
```

* Retrieve the machine id

```
journalctl -u anavi | grep "Machine ID"
```

For example, the output should be:
```
pi@hassbian:/$ journalctl -u anavi | grep "Machine ID"
Jan 05 01:28:36 hassbian anavid[387]: Machine ID: 26bbc1a1189c44139e080197cbecc2e6
```

* Replace YOURDEVICEID and add the following lines to configuration.yaml

```
# MQTT Broker (aka Mosquitto)
mqtt:
  discovery: true
  broker: hassbian.local
  port: 1883

# ANAVI Light pHAT
light:
  - platform: mqtt_json
    name: "ANAVI Light pHAT"
    command_topic: "YOURDEVICEID/action/rgbled"
    brightness: true
    rgb: true
```

* Run the following command to restart Home Assistant and reload configurations:

```
sudo systemctl restart home-assistant@homeassistant.service
```

# MQTT Commands

* Change the colors of the 12V RGB LED strip:

```
mosquitto_pub -h hassbian.local -d -p 1883 -t "replace-with-your-device-id/action/rgbled" -m "{ \"red\":255, \"green\":0, \"blue\":0 }"
```
