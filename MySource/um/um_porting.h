
// if this pragma is not supported by your compiler, then
// add include guards to all header files. Sorry.
#pragma once

#include <cassert>

// 2 integer types are required to build the machine:
// 32bit platters and values from "legacy" 8 bit scrolls
//
// Anyone porting this code should select from the menu:
//
// * If stdint.h is available, use that.
//
// * If stdint.h isn't available, or doesn't have 8bit and 32bit types,
// then if possible specify another type of the exact size.
//
// * If there is no exact-32 or exact-8 bit integer type, then
// a user-defined type is required. Using both together currently
// doesn't work, due to some ambiguous conversions.
//

// Is stdint.h available? 1 for yes, 0 for no
#if 1
	#include "stdint.h"
	#define HAVE_8BIT_TYPE uint8_t
	#define HAVE_32BIT_TYPE uint32_t
	#define HAVE_64BIT_TYPE uint_least64_t
#else
	// Is there an exact 8 bit type? 1 for yes, 0 for no
	#if 1
		// if unsigned char isn't 8 bits, specify something that is
		#define HAVE_8BIT_TYPE unsigned char
	#else
		// Go check the class below, make sure it works
		#define USER_8BIT_TYPE 1
	#endif

	// Is there an exact 32 bit type? 1 for yes, 0 for no
	#if 1
		// if unsigned int isn't 32 bits, specify something that is
		#define HAVE_32BIT_TYPE unsigned int
	#else
		// Go check the class below, make sure it works
		#define USER_32BIT_TYPE 1
	#endif

	// Is there a type at least 64 bits (used to count instructions executed)
	#if 1
		// if long isn't 64 bits, specify something that is
		#define HAVE_32BIT_TYPE unsigned long
	#else
		// If there's no big type either use the counter class
		// below, or else just use a smaller count and let it wrap
		#define USER_64BIT_TYPE 1
	#endif
#endif

// debug option. Dumps the program to cout, so best not
// used with a real program written to the terminal.
#define UM_DUMP_SCROLL_AT_STARTUP 0
// this one is necessary for a sandboxed UM, but we trust
// sandmark and the codex so opt for speed by default.
#define UM_CHECK_BOUNDS 0

// This block isn't intended to be modified.
namespace um {
	#ifdef HAVE_8BIT_TYPE
		// "legacy" 8 bit scrolls
		typedef HAVE_8BIT_TYPE platelet;
	#endif
	#ifdef HAVE_32BIT_TYPE
		// 32bit platters
		typedef HAVE_32BIT_TYPE platter;
	#endif
	#ifdef HAVE_64BIT_TYPE
		// 64bit insn counter
		typedef HAVE_64BIT_TYPE counter;
	#endif
}

// You only need to modify these classes if you need user-defined
// types, and the versions below aren't working.
//
// They aren't well tested, and in particular not tested on any
// platform that actually needs them.

#if USER_8BIT_TYPE
	namespace um {
		// Platform porter can replace this if there's anything more efficient,
		// and might have to tinker anyway depending what underlying type
		// the compiler uses for the bitfield.
		struct platelet {
			unsigned val : 8;
			// access as an unsigned char
			platelet(unsigned char v) : val(v) {}
			operator unsigned char() { return val; }
			// suppress warnings about conversions
			platelet(int v) : val(static_cast<unsigned char>(v)) {}
			// make this unambiguous
			platelet(unsigned int v) : val(static_cast<unsigned char>(v)) {}
		};
	}
#endif

#if USER_32BIT_TYPE
	namespace um {
		// Platform porter can replace this if there's anything more efficient,
		// and might have to tinker anyway depending what underlying type
		// the compiler uses for the bitfield.
		struct platter {
			unsigned val : 32;
			// We use an array of platters for the registers, hence this constructor.
			// No point 0-initing: it won't be when platter is a builtin type.
			platter() {}
			// access as an unsigned long
			platter(unsigned long v) : val(v) {}
			operator unsigned long() { return val; }
			// can't convert both sides of &, apparently
			friend platter operator&(platter a, platter b) { return a.val & b.val; }
		};
	}
#endif

#if USER_64BIT_TYPE
	#include <iostream>
	#include <iomanip>
	namespace um {
		struct counter {
			platter lo;
			platter hi;
			counter(platter a): lo(a), hi(0) {}
			counter &operator++() {
				++lo;
				if (lo == 0) ++hi;
				return *this;
			}
			friend std::ostream &operator<<(std::ostream &s, const counter &c) {
				s << "0x";
				if (c.hi != 0) {
					s << std::hex << std::setfill('0') << std::setw(8) << c.hi;
				}
				return s << std::hex << std::setfill('0') << std::setw(8) << c.lo;
			}
		};
	}
#endif

// Now for the "Memory" class. We need to represent a
// memory allocation in a platter, which is fine if pointers
// are 32bit, but not quite so fun with 64bit addresses.

namespace um {
	// responsibility of this class is to store and recover values.
	// If they can be stored in the id, that's fine, otherwise
	// they need to be stored in some structure.

	namespace {
		const int g_size_assert[sizeof(platter) - sizeof(void*) + 1] = { 0 };
	}

	// If the above is valid, this simple version is OK.
	// Otherwise platform porter must replace it, with a map or whatever.
	//
	// If desired, this function can track the pointers added to it
	// and free them in its destructor. This allows for leak debugging,
	// up to a point (since of course it won't detect leaks in the UM's program)
	template <typename T>
	class Memory {
	public:
		Memory() {
			// just to make sure the compiler hasn't gone soft on us and
			// allowed a 0-sized array at compile time.
			assert(sizeof(platter) >= sizeof(void*));
		}
	private:
		Memory(const Memory &);
		Memory &operator=(const Memory &);
	public:
		// "Adds" the pointer to memory.
		// If this function throws, then it must first delete ptr,
		// just like a smart pointer.
		platter convert(T* ptr) { return reinterpret_cast<platter>(ptr); }

		// May throw an exception if the identifier is not valid.
		// Not implemented in this version, but implementing it results
		// in a sandboxed UM implementation.
		T *convert(platter id) const { return reinterpret_cast<T*>(id); }

		// As convert, but make the indentifier invalid if applicable
		T *remove(platter id) const { return convert(id); }
	};
}
