# anavid
Standalone daemon process for controlling ANAVI Light pHAT over MQTT

# Installation

* Install Hassbian on Raspberry Pi

* Run the command below and enable I2C from *Interfacing Options*:

```bash
sudo raspi-config
```

* Install the MQTT broken Mosquitto usign the following [work around for Hassbian 1.31](https://github.com/home-assistant/hassbian-scripts/issues/76):

```bash
sudo curl -o /opt/hassbian/suites/install_mosquitto.sh https://raw.githubusercontent.com/ludeeus/Tools/master/hassbian-config/hotfix/install_mosquitto.sh && sudo hassbian-config install mosquitto

```

* Enable anonymous connection to the MQTT broker by editting the following setting in /etc/mosquitto/mosquitto.conf:

```
allow_anonymous true
```

* Start Mosquitto and enable its automatic launch at boot:

```bash
sudo systemctl enable mosquitto.service
sudo systemctl start mosquitto.service
```

* Install wiringpi, piGPIO and OpenSSL:

```bash
sudo apt-get update
sudo apt-get install -y wiringpi pigpio libssl-dev
```

* Install Paho (library for MQTT clients)

```bash
cd ~
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
make
sudo make install
```

* Build and install anavid

```bash
cd ~
git clone https://github.com/AnaviTechnology/anavid.git
cd anavid
make
sudo make install
```

* Start the system service

```bash
sudo systemctl start anavi
```

* Retrieve device ID

```bash
anavid -m
```

* Replace DEVICEID and add the following lines to configuration.yaml

```yaml
# MQTT Broker (aka Mosquitto)
mqtt:
  discovery: true
  broker: hassbian.local
  port: 1883

# ANAVI Light pHAT
light:
  - platform: mqtt_json
    name: "ANAVI Light pHAT"
    command_topic: "DEVICEID/action/rgbled"
    brightness: true
    rgb: true
```

* Run the following command to restart Home Assistant and reload configurations:

```bash
sudo systemctl restart home-assistant@homeassistant.service
```

# Integration with Amazon Echo and Alexa

* Add the following settings to your configuration.yaml:

```yaml
emulated_hue:
  type: alexa
  expose_by_default: true
```

* Run the following command to restart Home Assistant and reload configurations:

```bash
sudo systemctl restart home-assistant@homeassistant.service
```

* Ask alexa to discover new devices


* Say "Alexa, turn on ANAVI Light pHAT" to turn on the lights


* Say "Alexa, turn off ANAVI Light pHAT" to turn off the lights

# MQTT Commands

* Change the colors of the 12V RGB LED strip:

```bash
mosquitto_pub -h hassbian.local -d -p 1883 -t "replace-with-your-device-id/action/rgbled" -m "{ \"red\":255, \"green\":0, \"blue\":0 }"
```
