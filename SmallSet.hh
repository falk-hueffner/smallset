/* smallset -- efficient subsets of {0..4095}
   Copyright (C) 2016  Falk Hüffner

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.  */

#include <cstdint>
#include <limits>

#define ALLOCA_SMALLSET(maxElts) (new(alloca(SmallSet::allocSize(maxElts))) SmallSet)

class SmallSet {
private:
    typedef std::uint64_t word;
    static constexpr size_t WORD_BITS = std::numeric_limits<word>::digits;
    static constexpr size_t MAX_WORDS = WORD_BITS;
public:
    static constexpr size_t MAX_N = WORD_BITS * WORD_BITS;
    static constexpr size_t MAX_X = MAX_N - 1;

    static size_t allocSize(size_t n) { return sizeof (word) * (1 + std::min(n, MAX_WORDS)); }
    SmallSet() : mask_(0) { }

    void add(unsigned x) {
	if (x > MAX_X)
	    throw std::out_of_range("SmallSet::add: " + std::to_string(x) + " > " + std::to_string(MAX_X));
	size_t xSetNum = x / WORD_BITS;
	word* xSet = sets_ + numBitsBelow(mask_, xSetNum);
	if (!isBitSet(mask_, xSetNum)) {
	    memmove(xSet + 1, xSet, sizeof (word) * numBitsAbove(mask_, xSetNum));
	    *xSet = bitMask(x % WORD_BITS);
	    mask_ |= bitMask(xSetNum);
	} else {
	    *xSet |= bitMask(x % WORD_BITS);
	}
    }

    bool contains(unsigned x) const {
	if (x > MAX_X)
	    throw std::out_of_range("SmallSet::contains: " + std::to_string(x) + " > " + std::to_string(MAX_X));
	size_t xSetNum = x / WORD_BITS;
	if (!isBitSet(mask_, xSetNum))
	    return false;
	else
	    return isBitSet(sets_[numBitsBelow(mask_, xSetNum)], x % WORD_BITS);
    }

private:
    static size_t popcount(word x) {
	static_assert(sizeof (word) <= sizeof (int) ||
		      sizeof (word) == sizeof (long) ||
		      sizeof (word) == sizeof (long long),
		      "cannot determine popcount intrinsic");
	if (sizeof (word) <= sizeof (int))
	    return __builtin_popcount(x);
	else if (sizeof (word) == sizeof (long))
	    return __builtin_popcountl(x);
	else
	    return __builtin_popcountll(x);
    }
    static word bitMask(unsigned i) { return  word(1) << i; }
    static bool isBitSet(word w, unsigned i) { return w & bitMask(i); }
    static size_t numBitsBelow(word x, size_t i) { return popcount(x & (bitMask(i) - 1)); }
    static size_t numBitsAbove(word w, unsigned i) { w >>= i; w >>= 1; return popcount(w); }
    word mask_;
    word sets_[];
};
