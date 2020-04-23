
#pragma once

#include "um_porting.h"
#include "um_console.h"

#include <iostream>
#include <vector>

// For testing the loading code - best not to dump a real program
//#define DUMP_SCROLL_AT_STARTUP 1
#if UM_DUMP_SCROLL_AT_STARTUP
	#include <algorithm>
	#include <iostream>
	#include <iterator>
#endif

namespace um {
	// "Physical specifications"
	const int bits_per_platter = 32;
	const int bits_per_platelet = 8;
	const int platelets_per_platter = 4;

	// machine capacity
	const int num_registers = 8;

	// machine hardware
	typedef std::vector<platter> plattarray;
	class Um {
		// 1x1 console
		SimpleConsole console;
		// general-purpose registers
		platter gpr[num_registers];
		// distinguished 0-array
		plattarray zarray;
		// execution finger, as an offset
		plattarray::size_type ef;
		// memory manager for other arrays
		Memory<plattarray> mem;

	// first para of "Behavior"
	public:
		// requirements: value type of iterator must be convertible to platelet
		template <typename InputIterator>
		Um(InputIterator first, const InputIterator last) : ef(0) {
			// clear registers
			std::fill(gpr, gpr + num_registers, 0);
			// initialize the zero array
			loadProgramScroll(first, last);
			// logging
			#if UM_DUMP_SCROLL_AT_STARTUP
				std::cout << "size of program is " << zarray.size()*sizeof(zarray[0]) << " bytes\n";
				std::copy((char*)&zarray[0], (char*)&zarray[zarray.size()],
					std::ostream_iterator<char>(std::cout));
				std::cout.flush();
			#endif
		}

	// Public interface.
	// This would need to be richer for a debuggable UM
	public:
		Um &setInput(const std::string &in) {
			console.setInput(in);
			return *this;
		}
		Um &setOutput(const std::string &out, bool s = true) {
			console.setOutput(out, s);
			return *this;
		}
		int spin_cycle();

	// UM is not copyable, because of pointers to allocated arrays.
	// We could make it copyable by tracking and cloning them, but there's
	// no need for that unless we implement fork() or something.
	private:
		Um(const Um &);
		Um &operator=(const Um&);

	private:
		platter &reg(platter op, int shift);
		// convert array identifier to usable array
		plattarray &array_lu(platter identifier);
		// memory management
		platter allocate(platter size);
		void abandon(platter identifier);

		template <typename InputIterator>
		void loadProgramScroll(InputIterator first, const InputIterator last) {
			zarray.clear();
			appendProgramScroll(first, last);
		}

		template <typename InputIterator>
		void appendProgramScroll(InputIterator first, const InputIterator last) {
			// second para of "Behavior"
			// convert data (big-endian) to instructions
			while (last != first) {
				platter insn = 0;
				int bytecount = platelets_per_platter;
				do {
					insn = (insn << bits_per_platelet) | static_cast<platelet>(*(first++));
					if (--bytecount == 0) {
						zarray.push_back(insn);
						break;
					}
				} while (last != first);
			}
		}
	};

}
