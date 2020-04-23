
#include "um.h"
#include <cassert>
#include <cstddef>

namespace um {
	// numbers needed to interpret instructions
	namespace {
		const int bits_per_platter = bits_per_platelet * platelets_per_platter;
		const int opno_shift = bits_per_platter - 4;

		// Standard Operations
		const int reg_mask = 0x7;
		const int reg_A = 6;
		const int reg_B = 3;
		const int reg_C = 0;

		// Orthography op has its own interpretation
		const int reg_O = 25;
		const platter value_mask = (1U << reg_O) - 1; // rest of bits
	}

	// We use the Memory base class to look after pointer <-> platter conversions
	platter Um::allocate(platter size) {
		return mem.convert(new plattarray(size,0));
	}
	void Um::abandon(platter identifier) {
		assert(identifier != 0);
		delete mem.remove(identifier);
	}
	plattarray &Um::array_lu(platter identifier) {
		if (identifier == 0) return zarray;
		return *mem.convert(identifier);
	}

	// helper
	platter &Um::reg(platter op, int shift) {
		return gpr[(op >> shift) & reg_mask];
	}

	struct OpCounter {
		counter opcount;
		OpCounter(platter size) : opcount(0) {
			std::cerr << "UM starting, with program size " << size * platelets_per_platter << "\n";
		}
		~OpCounter() {
			std::cerr << "UM finished, " << opcount << " insns executed\n";
		}
	};

	// and the big work
	int Um::spin_cycle() {
		OpCounter oc(zarray.size());
		for(;;) {
			// pick up the next instruction
			assert(ef < zarray.size());
			platter op = zarray[ef++];
			++oc.opcount;

			// assume a standard operator for the time being, there's only one non-standard op
			platter &regA = reg(op, reg_A);
			platter &regB = reg(op, reg_B);
			platter &regC = reg(op, reg_C);

			unsigned int op_number = op >> opno_shift;
			switch (op_number) {
				case 0: // Conditional Move
					if (regC != 0) regA = regB;
					break;
				case 1: // Array Index
					#if UM_CHECK_BOUNDS
						assert(array_lu(regB).size() >= regC);
					#endif
					regA = array_lu(regB)[regC];
					break;
				case 2: // Array Amendment
					#if UM_CHECK_BOUNDS
						assert(array_lu(regA).size() >= regB);
					#endif
					array_lu(regA)[regB] = regC;
					break;
				case 3: // Addition
					regA = regB + regC;
					break;
				case 4: // Multiplication
					regA = regB * regC;
					break;
				case 5: // Division
					assert(regC != 0);
					regA = regB / regC;
					break;
				case 6: // Not-And
					regA = ~(regB & regC);
					break;
				case 7: // Halt
					return 0;
				case 8: // Allocation
					regB = allocate(regC);
					break;
				case 9: // Abandonment
					abandon(regC);
					break;
				case 10: // Output
					assert(regC <= 255);
					console.write(regC);
					break;
				case 11: // Input
					regC = console.read();
					break;
				case 12: // Load Program
					if (regB != 0) zarray = array_lu(regB);
					ef = regC;
					break;
				case 13: { // Orthography
					reg(op, reg_O) = op & value_mask;
					break;
				}
				default:
					return 1;
			}
		}
	}
}
