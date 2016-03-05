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
public:
    static constexpr size_t MAX_N = WORD_BITS * WORD_BITS;
    static constexpr size_t MAX_X = MAX_N - 1;

    static size_t allocSize(size_t n) { return sizeof (word) * (1 + n); }
    SmallSet() : mask_(0) { }

    void add(unsigned x) {
	if (x > MAX_X)
	    throw std::out_of_range("SmallSet::add: " + std::to_string(x) + " > " + std::to_string(MAX_X));
	size_t xSet = x / WORD_BITS;
	if (!bitSet(mask_, xSet)) {
	    memmove(sets_ + xSet + 1, sets_ + xSet, sizeof (word) * numBitsAbove(mask_, xSet));
	    sets_[xSet] = word(1) << (x % WORD_BITS);
	    mask_ |= word(1) << xSet;
	} else {
	    sets_[xSet] |= word(1) << (x % WORD_BITS);
	}
    }

    bool contains(unsigned x) const {
	if (x > MAX_X)
	    throw std::out_of_range("SmallSet::contains: " + std::to_string(x) + " > " + std::to_string(MAX_X));
	size_t xSet = x / WORD_BITS;
	if (!bitSet(mask_, xSet))
	    return false;
	else
	    return bitSet(sets_[xSet], x % WORD_BITS);
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
    static bool bitSet(word w, unsigned i) { return w & (word(1) << i); }
    static size_t numBitsAbove(word w, unsigned i) { w >>= i; w >>= 1; return popcount(w); }
    word mask_;
    word sets_[];
};
