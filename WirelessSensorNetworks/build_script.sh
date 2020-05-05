

cd lib
curr = $(pwd)
echo $curr
if test -d curr/DHT-sensor-lirary/; then echo "DHT-exist"; else git clone https://github.com/adafruit/DHT-sensor-library ; fi
if test -d curr/Adafruit_Python_BMP/; then echo "BMP-exist"; else git clone https://github.com/adafruit/Adafruit_Python_BMP ; fi
if test -d curr/ethercard/; then echo "ether-exist"; else git clone https://github.com/jcw/ethercard ; fi
if test -d curr/Arduino-HMC5883L/; then echo "hmc-exist"; else git clone https://github.com/jarzebski/Arduino-HMC5883L ; fi
 





cp * $HOME/sketchbook/libraries -f -r


mkdir python_lib

cd python_lib
git clone https://github.com/pyserial/pyserial
cd pyserial
sudo python setup.py install
cd ..

