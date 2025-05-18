#include "GalilTester.h"
#include "EmbeddedFunctions.h"

void GalilTester::testDigitalOutputs() {;
	// Testing Digital Output, 
	// eDO = expected result of digital output, 
	// aDO = actual result of digital output
	std::cout << "Testing Digital Outputs"<< std::endl;

	// Testing Functionality
	// Write Random Large 16 Bit Number to Digital Output
	galil->DigitalOutput(41135);
	std::string eDO1 = "1010000010101111";
	std::string aDO1 = currentDigitalOutputBits(funcs);
	expectEqual(eDO1, aDO1);

	// Write to one byte, turning all high bits on
	galil->DigitalOutput(41135);
	galil->DigitalByteOutput(1, 255);
	std::string eDO2 = "1111111110101111";
	std::string aDO2 = currentDigitalOutputBits(funcs);
	expectEqual(eDO2, aDO2);
	
	// Write to Digital Output, turning on high byte and alternating low byte
	galil->DigitalOutput(41135);
	galil->DigitalByteOutput(1, 255);
	galil->DigitalByteOutput(0, 170);
	std::string eDO3 = "1111111110101010";
	std::string aDO3 = currentDigitalOutputBits(funcs);
	expectEqual(eDO3, aDO3);

	// Write to Digital Output turning on high, alternating low, then alternating high using Digital Bit Output
	galil->DigitalOutput(41135);
	galil->DigitalByteOutput(1, 255);
	galil->DigitalByteOutput(0, 170);
	galil->DigitalBitOutput(0, 14);
	galil->DigitalBitOutput(0, 12);
	galil->DigitalBitOutput(0, 10);
	galil->DigitalBitOutput(0, 8);
	std::string eDO4 = "1010101010101010";
	std::string aDO4 = currentDigitalOutputBits(funcs);
	expectEqual(eDO4, aDO4);

	// Testing Edge Cases
	// All Off
	galil->DigitalOutput(0);
	std::string eDO5 = "0000000000000000";
	std::string aDO5 = currentDigitalOutputBits(funcs);
	expectEqual(eDO5, aDO5);

	// Only less than 255
	galil->DigitalOutput(255);
	std::string eDO6 = "0000000011111111";
	std::string aDO6 = currentDigitalOutputBits(funcs);
	expectEqual(eDO6, aDO6);

	// Only greater than 255
	galil->DigitalOutput(43520);
	std::string eDO7 = "1010101000000000";
	std::string aDO7 = currentDigitalOutputBits(funcs);
	expectEqual(eDO7, aDO7);

	//Change Nothing with Digital Byte Output
	galil->DigitalOutput(43690);
	galil->DigitalByteOutput(0, 170);
	std::string eDO8 = "1010101010101010";
	std::string aDO8 = currentDigitalOutputBits(funcs);
	expectEqual(eDO8, aDO8);

	//Change Nothing with Digital Bit Output
	galil->DigitalOutput(43690);
	galil->DigitalBitOutput(1,15);
	galil->DigitalBitOutput(0, 15);
	galil->DigitalBitOutput(1, 15);
	std::string eDO9 = "1010101010101010";
	std::string aDO9 = currentDigitalOutputBits(funcs);
	expectEqual(eDO9, aDO9);

	//Change Bothsides
	galil->DigitalOutput(43690);
	galil->DigitalOutput(255);
	galil->DigitalByteOutput(0, 0);
	galil->DigitalByteOutput(1, 255);
	std::string eDO10 = "1111111100000000";
	std::string aDO10 = currentDigitalOutputBits(funcs);
	expectEqual(eDO10, aDO10);

	// Between low and high
	galil->DigitalOutput(256);
	galil->DigitalByteOutput(1, 1);
	galil->DigitalBitOutput(1, 8);
	std::string eDO11 = "0000000100000000";
	std::string aDO11 = currentDigitalOutputBits(funcs);
	expectEqual(eDO11, aDO11);

	// Test All On
	galil->DigitalOutput(65535);
	std::string eDO12 = "1111111111111111";
	std::string aDO12 = currentDigitalOutputBits(funcs);
	expectEqual(eDO12, aDO12);

	std::cout << "Successfully Passed all Digital Output Tests!!\n" << std::endl;

}

void GalilTester::testDigitalInputs() {
	// Testing Digital Input, 
	// eDI = expected result of digital input, 
	// aDI = actual result of digital input
	std::cout << "\nTesting Digital Inputs" << std::endl;

	// Read 16 bits of Digital Input, only low bank is on
	galil->DigitalOutput(170);
	std::string eDI1 = "170";
	std::string aDI1 = std::to_string(galil->DigitalInput());
	expectEqual(eDI1, aDI1);

	// Read 16 bits of Digital Input, both high and low of Digital Output are on
	galil->DigitalOutput(65535);
	std::string eDI2 = "255";
	std::string aDI2 = std::to_string(galil->DigitalInput());
	expectEqual(eDI2, aDI2);

	// Read low bank of Digital Input
	galil->DigitalOutput(41135);
	std::string eDI3 = "175";
	std::string aDI3 = std::to_string(galil->DigitalByteInput(0));
	expectEqual(eDI3, aDI3);

	// Read high bank of Digital Input
	galil->DigitalOutput(41135);
	std::string eDI4 = "0";
	std::string aDI4 = std::to_string(galil->DigitalByteInput(1));
	expectEqual(eDI4, aDI4);

	// Read a bit that is on
	galil->DigitalOutput(41135);
	bool eDI5 = true;
	bool aDI5 = galil->DigitalBitInput(5);
	expectEqual(eDI5, aDI5);

	// Read a bit that is off
	galil->DigitalOutput(41135);
	bool eDI6 = false;
	bool aDI6 = galil->DigitalBitInput(4);
	expectEqual(eDI6, aDI6);

	// Read a bit that is always off, i.e. in high bank of Digital Input
	galil->DigitalOutput(41135);
	bool eDI7 = false;
	bool aDI7 = galil->DigitalBitInput(10);
	expectEqual(eDI7, aDI7);

	// Read 16 bit and low bit and comparing
	galil->DigitalOutput(41135);
	std::string sixteen = std::to_string(galil->DigitalInput());
	std::string low = std::to_string(galil->DigitalByteInput(0));
	expectEqual(sixteen, low);

	// Read a bit when all are off
	galil->DigitalOutput(0);
	bool eDI8 = false;
	bool aDI8 = galil->DigitalBitInput(5);
	expectEqual(eDI8, aDI8);

	// Read a byte when all are off
	galil->DigitalOutput(0);
	std::string eDI9 = "0";
	std::string aDI9 = std::to_string(galil->DigitalByteInput(0));
	expectEqual(eDI9, aDI9);

	// Read Digital Input when all are off
	galil->DigitalOutput(0);
	std::string eDI10 = "0";
	std::string aDI10 = std::to_string(galil->DigitalInput());
	expectEqual(eDI10, aDI10);

	std::cout << "Successfully Passed all Digital Input Tests!!\n" << std::endl;

}

void GalilTester::testAnalogOutputs() {
	// Testing Analog Output, 
	// eAO = expected result of analog output, 
	// aAO = actual result of analog output
	std::cout << "\nTesting Analog Output" << std::endl;

	// Test if Analog Output can write to any channel of the Galil
	galil->AnalogOutput(0, 0);
	galil->AnalogOutput(1, 1);
	galil->AnalogOutput(2, 2);
	galil->AnalogOutput(3, 3);
	galil->AnalogOutput(4, 4);
	galil->AnalogOutput(5, 5);
	galil->AnalogOutput(6, 6);
	galil->AnalogOutput(7, 7);
	float eAO1 = 0;
	float aAO1 = funcs->GetAnalogOutput(0);
	float eAO2 = 1;
	float aAO2 = funcs->GetAnalogOutput(1);
	float eAO3 = 2;
	float aAO3 = funcs->GetAnalogOutput(2);
	float eAO4 = 3;
	float aAO4 = funcs->GetAnalogOutput(3);
	float eAO5 = 4;
	float aAO5 = funcs->GetAnalogOutput(4);
	float eAO6 = 5;
	float aAO6 = funcs->GetAnalogOutput(5);
	float eAO7 = 6;
	float aAO7 = funcs->GetAnalogOutput(6);
	float eAO8 = 7;
	float aAO8 = funcs->GetAnalogOutput(7);
	expectEqual(eAO1, aAO1);
	expectEqual(eAO2, aAO2);	

	// Test if Analog Output sends voltage as 2 decimal place
	galil->AnalogOutput(0, 3.234);
	float eAO9 = 3.23;
	float aAO9 = funcs->GetAnalogOutput(0);
	expectEqual(eAO9, aAO9);

	// Test Max Output Voltage
	galil->AnalogOutput(0, 9.9998);
	float eAO10 = 9.99;
	float aAO10 = funcs->GetAnalogOutput(0);
	expectEqual(eAO10, aAO10);

	// Test Greater than Max Output Voltage
	galil->AnalogOutput(0, 10);
	float eAO11 = 9.99;
	float aAO11 = funcs->GetAnalogOutput(0);
	expectEqual(eAO11, aAO11);

	//// Test Less than Min Output Voltage
	galil->AnalogOutput(0, -10);
	float eAO12 = -9.99;
	float aAO12 = funcs->GetAnalogOutput(0);
	expectEqual(eAO12, aAO12);

	// Test Negative Output Voltage
	galil->AnalogOutput(0, -3);
	float eAO13 = -3;
	float aAO13 = funcs->GetAnalogOutput(0);
	expectEqual(eAO13, aAO13);
	galil->AnalogOutput(0, 0);

	std::cout << "Successfully Passed all Analog Output Tests!!\n" << std::endl;
}

std::string GalilTester::currentDigitalOutputBits(EmbeddedFunctions* Funcs) {
	std::string currentDigitalOutputBits;
	for (int i = 15; i >= 0; i--) {
		currentDigitalOutputBits += std::to_string(Funcs->GetDigitalOutput(i));
	}
	return currentDigitalOutputBits;
}