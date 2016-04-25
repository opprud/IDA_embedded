# IDA_embedded

Example files for Getting started with IoT Diy.

---------------------
Arduino examples
    Requires installed Arduino IDE w ESP8266 extensions + FTDI drivers

TMP 36 UDP connected temeperature sensor:
/IDA_embedded/sensor/examples/UDP_temperature_sensor
NOTE! The provided example requires you to change "ssid" + "password" to your availible AP

first connect Arduino IDE to the ESP8266 board, program it (Tools->Upload using serial), open a 
serial monitor @115200bps (Tools-> Serial Monitor) and observe wifi status
    
run the python testscript "python udpSensor.py" from a console

---------------------
Python examples

add/todo