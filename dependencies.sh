echo "Insalling OpenCV dependencies..."
sleep 2
sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

echo "\nInstalling dev environment..."
sleep 2
sudo apt-get install eclipse g++

echo "\nCloning opencv..."
sleep 2
sudo git clone https://github.com/Itseez/opencv.git
cd ./opencv

echo "\nBuilding opencv..."
sleep 2
sudo mkdir release
cd release
sudo cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
sudo make
sudo make install

echo "Done!"
