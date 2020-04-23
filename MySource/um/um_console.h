
#pragma once

#include "um_porting.h"

#include <fstream>
#include <iostream>

namespace um {
	// The standard specifies a 1x1 console, but I think
	// we'll do it the traditional way.
	class SimpleConsole {
		std::ifstream instream;
		std::ofstream outstream;
		std::istream *in;
		std::ostream *out;
		bool shadow;

	public:
		SimpleConsole() : in(&std::cin), out(&std::cout), shadow(true) {}

	private:
		SimpleConsole(const SimpleConsole &);
		SimpleConsole &operator=(const SimpleConsole &);

	public:
		platter read();
		void write(platter data);
		SimpleConsole &setInput(const std::string &name);
		SimpleConsole &setOutput(const std::string &name, bool s = true);
	};
}
