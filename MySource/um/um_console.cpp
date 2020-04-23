
#include "um_console.h"

#include <cassert>

namespace um {
	platter SimpleConsole::read() {
		// cast value into range 0..255 before widening
		platter value = static_cast<platelet>(in->get());
		// std::cout << "read byte " << (char) value << "\n";
		if (in->fail()) value = -1;
		// This is easy to get wrong - we must not return large values
		// other than 0xFFFFFFFF for end of stream
		assert(value + 1 <= 256);
		// if we're using a log file, echo input to file
		if ((out != &std::cout) && (value != platter(-1))) {
			*out << static_cast<platelet>(value);
		}
		return value;
	}

	void SimpleConsole::write(platter data) {
		platelet d = static_cast<platelet>(data);
		*out << d;
		// out->flush();
		if ((out != &std::cout) && (shadow)) {
			std::cout << d;
		}
	}

	SimpleConsole &SimpleConsole::setInput(const std::string &name) {
		instream.close();
		if (name != "-") {
			instream.open(name.c_str());
			in = &instream;
		} else {
			in = &std::cin;
		}
		return *this;
	}

	SimpleConsole &SimpleConsole::setOutput(const std::string &name, bool s) {
		shadow = s;
		outstream.close();
		if (name != "-") {
			outstream.open(name.c_str(), std::ios::out|std::ios::app);
			outstream << "\n\n ==== New Session started ====\n\n";
			out = &outstream;
		} else {
			out = &std::cout;
		}
		return *this;
	}
}
