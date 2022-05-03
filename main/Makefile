# This file is part of the C++ dlopen mini HOWTO. You can find the complete
# HOWTO and/or updated versions at
# http://www.isotton.com/howtos/C++-dlopen-mini-HOWTO/
#
# Copyright 2002-2006 Aaron Isotton <aaron@isotton.com>
# Licensed under the GNU GPL.


all: TestStone run

TestStone:  main.cpp MySerial.cpp stone.cpp panelAddon.hpp plugInFumee.so plugInPorte.so myMqtt.cpp
	g++	-fPIC -std=c++2a -o TestStone MySerial.cpp stone.cpp main.cpp myMqtt.cpp -lpthread -ldl -lmosquittopp

plugInFumee.so: plugInFumee.cpp panelAddon.hpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -o plugInFumee.so plugInFumee.cpp

plugInPorte.so: plugInPorte.cpp panelAddon.hpp stone.cpp MySerial.cpp myMqtt.cpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -o plugInPorte.so plugInPorte.cpp stone.cpp MySerial.cpp myMqtt.cpp

clean:
# 	rm -f SerialPort.so

run:
	./TestStone ttyUSB0

# 	.PHONY: run clean
.PHONY: run clean
