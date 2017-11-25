# anavilightd
Standalone daemon process for controlling ANAVI Light pHAT over MQTT

# Installation

* Install wiringpi

```
sudo apt-get update
sudo apt-get install -y wiringpi
```

* Install Paho (library for MQTT clients)

```
cd ~
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c.git
make
sudo make install
```
