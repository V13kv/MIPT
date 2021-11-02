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

IncDir = include
LibDir = lib
BinDir = bin

TextBinDir = $(LibDir)/text/bin
TextSrcDir = $(LibDir)/text/src
TextIncDir 	= $(LibDir)/text/include

ASM_OBJECTS =	$(BinDir)/main.o $(BinDir)/labels.o $(BinDir)/assembler.o \
				$(TextBinDir)/text.o $(TextBinDir)/file.o

all: 
	g++ main.cpp labels.cpp assembler.cpp $(TextSrcDir)/text.cpp $(TextSrcDir)/file.cpp -o $(BinDir)/asm.exe

asm.exe: $(ASM_OBJECTS)
	g++ $(ASM_OBJECTS) -o $(BinDir)/asm.exe

$(BinDir)/main.o: main.cpp $(TextIncDir)/text.h $(LibDir)/colors/colors.h #$(IncDir)/asm/assembler.h
	g++ -c main.cpp $(CXXFLAGS) -o $(BinDir)/main.o

$(BinDir)/labels.o: labels.cpp $(IncDir)/asm/labels.h
	g++ -c labels.cpp $(CXXFLAGS) -o $(BinDir)/labels.o

$(BinDir)/assembler.o: 	assembler.cpp $(IncDir)/asm/assembler.h $(TextIncDir)/text.h $(IncDir)/asm/labels.h \
						$(IncDir)/asm/settings.h $(IncDir)/opdefs.h $(IncDir)/registers.h
	g++ -c assembler.cpp $(CXXFLAGS) -o $(BinDir)/assembler.o

$(TextBinDir)/text.o: $(TextSrcDir)/text.cpp $(TextIncDir)/text.h $(TextIncDir)/file.h
	g++ -c $(TextSrcDir)/text.cpp $(CXXFLAGS) -o $(TextBinDir)/text.o

$(TextBinDir)/file.o: $(TextSrcDir)/file.cpp $(TextIncDir)/file.h
	g++ -c $(TextSrcDir)/file.cpp $(CXXFLAGS) -o $(TextBinDir)/file.o
