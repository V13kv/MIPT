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

ProjectPath = C:/Users/gold4/Desktop/MIPT-main/First Course/First Semester/Dedinsky Course/CPU emulator

IncDir = include
LibDir = lib
BinDir = bin

TextBinDir = $(LibDir)/text/bin
TextSrcDir = $(LibDir)/text/src
TextIncDir = $(LibDir)/text/include

AsmSrcDir = src/asm
AsmBinDir = bin/asm

ASM_OBJECTS =	$(AsmBinDir)/main.o $(AsmBinDir)/labels.o $(AsmBinDir)/assembler.o \
				$(TextBinDir)/text.o $(TextBinDir)/file.o

asm.exe: $(ASM_OBJECTS)
	g++ $(ASM_OBJECTS) -o asm.exe

$(AsmBinDir)/main.o: $(AsmSrcDir)/main.cpp $(TextIncDir)/text.h $(LibDir)/colors/colors.h $(IncDir)/asm/assembler.h
	g++ -c $(AsmSrcDir)/main.cpp $(CXXFLAGS) -o $(AsmBinDir)/main.o

$(AsmBinDir)/labels.o: $(AsmSrcDir)/labels.cpp $(IncDir)/asm/labels.h
	g++ -c $(AsmSrcDir)/labels.cpp $(CXXFLAGS) -o $(AsmBinDir)/labels.o

$(AsmBinDir)/assembler.o:	$(AsmSrcDir)/assembler.cpp $(IncDir)/asm/assembler.h $(TextIncDir)/text.h $(IncDir)/asm/labels.h \
							$(IncDir)/constants.h $(IncDir)/asm/settings.h $(LibDir)/debug/debug.h $(IncDir)/opdefs.h $(IncDir)/regdefs.h
	g++ -c $(AsmSrcDir)/assembler.cpp $(CXXFLAGS) -o $(AsmBinDir)/assembler.o

$(TextBinDir)/text.o: $(TextSrcDir)/text.cpp $(TextIncDir)/text.h $(TextIncDir)/file.h
	g++ -c $(TextSrcDir)/text.cpp $(CXXFLAGS) -o $(TextBinDir)/text.o

$(TextBinDir)/file.o: $(TextSrcDir)/file.cpp $(TextIncDir)/file.h
	g++ -c $(TextSrcDir)/file.cpp $(CXXFLAGS) -o $(TextBinDir)/file.o


ProcBinDir = bin/processor
ProcSrcDir = src/processor

StackBinDir	= $(LibDir)/stack/bin
HashBinDir	= $(LibDir)/hash/bin

PROC_OBJS = $(ProcBinDir)/main.o $(ProcBinDir)/processor.o \
			$(TextBinDir)/text.o $(TextBinDir)/file.o \
			$(StackBinDir)/stack.o $(HashBinDir)/hash.o

proc.exe: $(PROC_OBJS)
	g++ $(PROC_OBJS) -o proc.exe

$(ProcBinDir)/main.o: 	$(ProcSrcDir)/main.cpp $(TextIncDir)/text.h $(LibDir)/colors/colors.h $(IncDir)/processor/processor.h
	g++ -c $(ProcSrcDir)/main.cpp $(CXXFLAGS) -o $(ProcBinDir)/main.o

$(ProcBinDir)/processor.o: 	$(ProcSrcDir)/processor.cpp $(IncDir)/processor/processor.h $(LibDir)/stack/include/stack.h \
							$(TextIncDir)/text.h
	g++ -c $(ProcSrcDir)/processor.cpp $(CXXFLAGS) -o $(ProcBinDir)/processor.o

$(StackBinDir)/stack.o $(HashBinDir)/hash.o:
	"$(MAKE)" -C  "$(ProjectPath)/$(LibDir)/stack/" makefile

