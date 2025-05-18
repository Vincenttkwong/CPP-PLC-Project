#include "Galil.h"
#include "GalilTester.h"
#include <chrono>
#include <thread>
#include "conio.h"

int main(void) {
	
	// Run PLC
	EmbeddedFunctions funcs = EmbeddedFunctions(false);			// Physical (false) || Simulator (true)
	Galil myGalil = Galil(&funcs, "192.168.0.120 -d");

	// Run Tests
	//GalilTester tests = GalilTester(true);
	//tests.runTests();

	using namespace std::this_thread;
	using namespace std::chrono_literals;
	using std::chrono::system_clock;

	myGalil.DigitalOutput(0);
	
	for (int i = 0; i < 8; i++) {
		sleep_for(0.25s);
		myGalil.DigitalBitOutput(1,i);
		std::cout << i << std::endl;
	}

	for (int i = 7; i >= 0; i--) {
		sleep_for(0.25s);
		myGalil.DigitalBitOutput(0, i);
		std::cout << i << std::endl;
	}

	myGalil.AnalogOutput(0, 3);

	while (1);
	return 0;
}