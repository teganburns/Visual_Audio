#! /bin/bash

wget http://www.sfml-dev.org/files/SFML-2.4.0-linux-gcc-64-bit.tar.gz
tar xvf SFML-2.4.0-linux-gcc-64-bit.tar.gz
cd SFML-2.4.0
sudo cp -r * /usr/
sudo cp -r * /usr/local/
cd ..
ldconfig

