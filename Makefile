# smallset -- efficient subsets of {0..4095}
# Copyright (C) 2016  Falk Hüffner
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

EXECS	  = testSmallSet

CXX	  = g++
CXXFLAGS  = -std=c++11 -g -Ofast -march=native -W -Wall -Werror 

all: .deps $(EXECS)

testSmallSet: testSmallSet.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test: testSmallSet
	./testSmallSet

.deps:
	mkdir -p .deps

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -MD -o $@ $<
	@cp $*.d .deps/$*.P;					    \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> .deps/$*.P;    \
		rm -f $*.d

clean:
	rm -rf *.o $(EXECS) testSmallSet core gmon.out

-include $(wildcard .deps/*.P)
