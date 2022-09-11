import urllib.request
import serial
from time import sleep

arduino = serial.Serial('COM3', 9600)

for c in range(50):
    linha = str(arduino.readline())[2:-5]

    val = float(linha)
    URL = 'https://api.thingspeak.com/update?api_key='
    KEY = 'C617BN2AKZV1IRFU'
    HEADER = f'&field1={val}'
    NEW_URL = URL + KEY + HEADER
    data = urllib.request.urlopen(NEW_URL)
    print(val)

arduino.close()