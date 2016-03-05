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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "SmallSet.hh"
#include <alloca.h>

TEST_CASE("SmallSet", "[SmallSet]") {
    SmallSet& s = *ALLOCA_SMALLSET(SmallSet::MAX_N);
    SECTION("contains") {
	s.add(0);
	s.add(1);
	s.add(63);
	s.add(137);
	s.add(3141);
	s.add(4095);
	CHECK(s.contains(0));
	CHECK(s.contains(1));
	CHECK(s.contains(63));
	CHECK(s.contains(137));
	CHECK(s.contains(3141));
	CHECK(!s.contains(2));
	CHECK(!s.contains(62));
	CHECK(!s.contains(64));
	CHECK(!s.contains(100));
	CHECK(!s.contains(138));
	CHECK(!s.contains(1000));
	CHECK(!s.contains(2048));
	CHECK(!s.contains(4094));
	CHECK_THROWS_AS(s.contains(4096), std::out_of_range);
	CHECK_THROWS_AS(s.contains(-1), std::out_of_range);
    }
}
