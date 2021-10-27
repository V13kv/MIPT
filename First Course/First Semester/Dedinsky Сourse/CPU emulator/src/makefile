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

IncDir 		= ../include
LibDir 		= ../lib
DebugDir 	= $(LibDir)/debug
BinDir 		= ../bin

HashBinDir 	= $(LibDir)/hash/bin
HashSrcDir 	= $(LibDir)/hash/src
HashIncDir 	= $(LibDir)/hash/include

TextBinDir 	= $(LibDir)/text/bin
TextSrcDir 	= $(LibDir)/text/src
TextIncDir 	= $(LibDir)/text/include

AsmBinDir 	= myasm/bin
AsmSrcDir 	= myasm
ASM_OBJECTS = 	$(AsmBinDir)/main.o $(AsmBinDir)/assembler.o $(TextBinDir)/text.o \
				$(TextBinDir)/file.o commands.o

asm.exe: $(ASM_OBJECTS)
	g++ $(ASM_OBJECTS) -o $(BinDir)/myasm/asm.exe

$(AsmBinDir)/main.o: $(AsmSrcDir)/main.cpp $(TextIncDir)/text.h $(IncDir)/myasm/assembler.h
	g++ -c $(AsmSrcDir)/main.cpp $(CXXFLAGS) -o $(AsmBinDir)/main.o

$(AsmBinDir)/assembler.o: $(AsmSrcDir)/assembler.cpp $(IncDir)/myasm/assembler.h $(IncDir)/commands.h
	g++ -c $(AsmSrcDir)/assembler.cpp $(CXXFLAGS) -o $(AsmBinDir)/assembler.o

commands.o: commands.cpp $(IncDir)/commands.h $(DebugDir)/debug.h $(IncDir)/registers.h
	g++ -c commands.cpp $(CXXFLAGS)  -o commands.o

$(TextBinDir)/text.o: $(TextSrcDir)/text.cpp $(TextIncDir)/text.h $(TextIncDir)/file.h
	g++ -c $(TextSrcDir)/text.cpp $(CXXFLAGS) -o $(TextBinDir)/text.o

$(TextBinDir)/file.o: $(TextSrcDir)/file.cpp $(TextIncDir)/file.h
	g++ -c $(TextSrcDir)/file.cpp $(CXXFLAGS) -o $(TextBinDir)/file.o

DisAsmBinDir 	= mydisasm/bin
DisAsmSrcDir 	= mydisasm
DISASM_OBJECTS 	= 	$(DisAsmBinDir)/main.o $(DisAsmBinDir)/disassembler.o $(TextBinDir)/text.o \
					$(TextBinDir)/file.o $(TextBinDir)/lines_sort.o commands.o

disasm.exe: $(DISASM_OBJECTS)
	g++ $(DISASM_OBJECTS) -o $(BinDir)/mydisasm/disasm.exe

$(DisAsmBinDir)/main.o: $(DisAsmSrcDir)/main.cpp $(TextIncDir)/text.h $(IncDir)/mydisasm/disassembler.h
	g++ -c $(DisAsmSrcDir)/main.cpp $(CXXFLAGS) -o $(DisAsmBinDir)/main.o

$(DisAsmBinDir)/disassembler.o: $(DisAsmSrcDir)/disassembler.cpp $(IncDir)/mydisasm/disassembler.h $(IncDir)/commands.h
	g++ -c $(DisAsmSrcDir)/disassembler.cpp $(CXXFLAGS) -o $(DisAsmBinDir)/disassembler.o
