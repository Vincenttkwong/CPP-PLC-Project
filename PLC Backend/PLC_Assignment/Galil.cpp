#include "Galil.h"
#include "string"
#include "sstream"
#include "iomanip"
#include "chrono"

Galil::Galil() : Functions(new EmbeddedFunctions()), g(0), setPoint(0) {
	gReturn = Functions->GOpen("192.168.0.1 -d", &g);
	defaultConstructor = true;
}

Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address) : Functions(Funcs), g(0), setPoint(0) {
	gReturn = Functions->GOpen(address, &g);
	defaultConstructor = false;
}

Galil::~Galil() {
	gReturn = Functions->GClose(g);
	if (defaultConstructor) {
		delete Functions;
	}
}

// Digital Outputs
void Galil::DigitalOutput(uint16_t value) {
	currentBits = value;

	std::string command = "OP ";
	if (currentBits <= 255) {
		command += std::to_string(currentBits) + ",0;";
	}
	else {
		uint16_t high = currentBits >> 8;
		uint16_t low = currentBits & 0xFF;
		command += std::to_string(low) + "," + std::to_string(high) + ";";
	}

	gReturn = Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), &bytesReturned);
}

void Galil::DigitalByteOutput(bool bank, uint8_t value) {

	if (bank) {	// Change high byte
		uint16_t high = value << 8;
		currentBits = (currentBits & 0x00FF) | high;
	}
	else {	// Change low byte
		uint16_t low = value;
		currentBits = (currentBits & 0xFF00) | low;
	}

	DigitalOutput(currentBits);
}

void Galil::DigitalBitOutput(bool val, uint8_t bit) {
	if (val) {
		currentBits |= (1 << bit);
	}
	else {
		currentBits &= ~(1 << bit);
	}
	DigitalOutput(currentBits);
}

uint16_t Galil::DigitalInput() {
	uint16_t inputBuffer = 0;
	for (int i = 15; i >= 0; i--) {

		std::string command = std::string("MG @IN[" + std::to_string(i) + "]");
		gReturn = Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), &bytesReturned);

		inputBuffer = inputBuffer << 1; // Shift Left

		if (ReadBuffer[0] - '0' == 1) {
			inputBuffer |= 0b1;
		}
	}
	return inputBuffer;
}


uint8_t Galil::DigitalByteInput(bool bank) {
	uint16_t inputBuffer = DigitalInput();

	if (bank) { // high byte
		return ((inputBuffer >> 8) & 0xFF);
	}
	else { // low byte
		return (inputBuffer & 0xFF);
	}

}

bool Galil::DigitalBitInput(uint8_t bit) {
	uint16_t inputBuffer = DigitalInput();

	if ((inputBuffer >> bit) & 1) {
		return true;
	}
	else {
		return false;
	}
}

bool Galil::CheckSuccessfulWrite() {
	if (gReturn == 0) {
		return true;
	}
	return false;
}

float Galil::AnalogInput(uint8_t channel) {
	std::string command = std::string("MG @AN[" + std::to_string(channel) + "]");z
	gReturn = Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), &bytesReturned);

	std::string analogInput = ReadBuffer;

	auto nLPOS = analogInput.find('\n');
	if (nLPOS != -1) {
		analogInput.resize(nLPOS);
	}

	return std::stof(analogInput);
}

void Galil::AnalogOutput(uint8_t channel, double voltage) {
	std::ostringstream os;
	os << std::setprecision(3) << voltage;

	if (voltage >= 9.99) {
		std::string command = "AO " + std::to_string(channel) + "," + "9.99" + ";";
		gReturn = Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), &bytesReturned);
		return;
	}
	if (voltage <= -9.99) {
		std::string command = "AO " + std::to_string(channel) + "," + "-9.99" + ";";
		gReturn = Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), &bytesReturned);
		return;
	}

	std::string command = "AO " + std::to_string(channel) + "," + os.str() + ";";
	gReturn = Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), &bytesReturned);
}

void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {
	if (range < 1 || range > 4) {
		std::string command = "AQ " + std::to_string(channel) + "," + "2" + ";"; // Set to Default Range Setting
		gReturn = Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), &bytesReturned);
		return;
	}
	std::string command = "AQ " + std::to_string(channel) + "," + std::to_string(range) + ";";
	gReturn = Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), &bytesReturned);
}

void Galil::WriteEncoder() {
	std::string command = "WE 0,0;";
	gReturn = Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), &bytesReturned);
}

int Galil::ReadEncoder() {
	std::string command = std::string{ "QE 0;" };
	gReturn = Functions->GCommand(g, command.c_str(), ReadBuffer, sizeof(ReadBuffer), &bytesReturned);
	std::string encoder = ReadBuffer;

	auto nLPOS = encoder.find('\n');
	if (nLPOS != -1) {
		encoder.resize(nLPOS);
	}

	return std::stoi(encoder);
}

void Galil::setSetPoint(int s) {
	setPoint = s;
}

void Galil::setKp(double gain) {
	ControlParameters[0] = gain;
}

void Galil::setKi(double gain) {
	ControlParameters[1] = gain;
}

void Galil::setKd(double gain) {
	ControlParameters[2] = gain;
}



std::ostream& operator<<(std::ostream& output, Galil& galil) {
	std::ostringstream ss;
	galil.Functions->GInfo(galil.g, galil.ReadBuffer, galil.bytesReturned);
	output << std::string{ galil.ReadBuffer } << "\n\n\n";


	galil.Functions->GVersion(galil.ReadBuffer, galil.bytesReturned);
	output << std::string{ galil.ReadBuffer } << "\n\n" << std::endl;

	return output;
}