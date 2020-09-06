source /opt/rh/devtoolset-7/enable
g++ -std=c++11 -I/opt/dependency/package/include/ -I./ -L. main.cc -O2 -ljsoncpp 