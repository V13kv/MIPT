CXXFLAGS = -g -std=c++17 -Wall -Wextra -Weffc++ -Wc++0x-compat -Wc++11-compat -Wc++14-compat -Waggressive-loop-optimizations \
-Walloc-zero -Walloca -Walloca-larger-than=8192 -Warray-bounds -Wcast-align -Wcast-qual -Wchar-subscripts \
-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wdangling-else -Wduplicated-branches -Wempty-body -Wfloat-equal \
-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Winline \
-Wlarger-than=8192 -Wvla-larger-than=8192 -Wlogical-op -Wmissing-declarations -Wnon-virtual-dtor -Wopenmp-simd \
-Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls  -Wrestrict -Wshadow -Wsign-promo -Wstack-usage=8192 \
-Wstrict-null-sentinel -Wstrict-overflow=2 -Wstringop-overflow=4 -Wsuggest-attribute=noreturn -Wsuggest-final-types \
-Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros \
-Wno-literal-suffix -Wno-missing-field-initializers -Wnarrowing -Wno-old-style-cast -Wvarargs -Waligned-new \
-Walloc-size-larger-than=1073741824 -Walloc-zero -Walloca -Walloca-larger-than=8192 -Wdangling-else \
-Wduplicated-branches -Wformat-overflow=2 -Wformat-truncation=2 -Wmissing-attributes -Wmultistatement-macros -Wrestrict \
-Wshadow=global -Wsuggest-attribute=malloc -fcheck-new -fsized-deallocation -fstack-check -fstrict-overflow \
-flto-odr-type-merging -fno-omit-frame-pointer -Wno-unknown-pragmas

SrcDir = src
IncDir = include
BinDir = bin

OBJECTS = $(BinDir)/main.o $(BinDir)/stack.o $(BinDir)/hash.o

prog.exe: $(OBJECTS)
	g++ $(OBJECTS) -o prog.exe

$(BinDir)/main.o: $(SrcDir)/main.cpp $(IncDir)/stack.h
	g++ -c $(SrcDir)/main.cpp $(CXXFLAGS) -o $(BinDir)/main.o

$(BinDir)/stack.o: $(SrcDir)/stack.cpp $(IncDir)/stack.h $(IncDir)/debug.h $(IncDir)/settings.h $(IncDir)/hash.h
	g++ -c $(SrcDir)/stack.cpp $(CXXFLAGS) -o $(BinDir)/stack.o

$(BinDir)/hash.o: $(SrcDir)/hash.cpp $(IncDir)/debug.h $(IncDir)/hash.h
	g++ -c $(SrcDir)/hash.cpp $(CXXFLAGS) -o $(BinDir)/hash.o

.PHONY: init
init:
	mkdir bin
