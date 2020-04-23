
#include "um.h"

#include <fstream>
#include <iostream>
//#include <iterator>
#include <string>
#include <sstream>
#include <libgen.h>

namespace {
	// returns true if b is a prefix of a, else false
	bool prefix(const std::string &a, const char *b) {
		size_t l = std::char_traits<char>::length(b);
		return (l <= a.size()) && (a.compare(0, l, b) == 0);
	}

	// ignores options we don't understand, configures machine
	// according to options we do understand.
	void processOptions(um::Um &machine, const char *const *argp) {
		for (; *argp != 0; ++argp) {
			std::string arg(*argp);
			if (prefix(arg, "in=")){
				// input file
				machine.setInput(*argp+3);
			} else if (prefix(arg, "of=")) {
				// output file (also writes to terminal)
				machine.setOutput(*argp+3);
			} else if (prefix(arg, "on=")) {
				// output file, noshadow (doesn't write to terminal)
				machine.setOutput(*argp+3, false);
			}
		}
	}

	// return empty string on failure (since an empty program is invalid)
	std::string readFile(const char *name) {
		// open the file for reading
		std::ifstream scroll(name, std::ios::binary);
		if (scroll.is_open()) {
			// read the bytes
			std::stringstream ss;
			ss << scroll.rdbuf();
			return ss.str();
		}
		return "";
	}
}

int main(int argc, char **argv) {
	// ensure we have a program scroll argument
	if (argc < 2) {
		std::cerr << "usage: " << basename(argv[0]) << " <program scrollname>\n";
		return 1;
	}

	// read the scroll
	std::string program(readFile(argv[1]));
	if (program.size() == 0) {
		std::cerr << "error opening " << argv[1] << "\n";
		return 1;
	}
	// std::cout << "string size is " << program.size() << " bytes\n";

	// create and configure the UM, then get on with it
	um::Um machine(program.begin(), program.end());
	processOptions(machine, argv+2);
	return machine.spin_cycle();
}
