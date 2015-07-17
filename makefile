.SUFFIXES: .tex .dvi .pdf .cpp .hpp .c .h .exe .out
CC			= i686-w64-mingw32-g++ 
WINDRES		=i686-w64-mingw32-windres
CFLAGS		= -std=c++11  -Os 
LDFLAGS		= -Wl,-lpthread 
LIBS		= -static -static-libgcc -static-libstdc++ -mwindows  -s
SRC			= main.cpp tkzngui.cpp
MANIFEST	= manifest.res
OBJ			= $(SRC:%.cpp=%.o)
PROGRAM		= main
WINSUFFIXES	=.exe

all:		$(PROGRAM)$(WINSUFFIXES)
$(PROGRAM)$(WINSUFFIXES):	$(OBJ) $(MANIFEST)
			$(CC) -o $(PROGRAM)$(WINSUFFIXES) $(OBJ) $(MANIFEST) $(LDFLAGS) $(LIBS)  
.cpp.o:
			$(CC) -c $(CFLAGS) $<  
clean:
			rm -f *.o $(PROGRAM)$(WINSUFFIXES) *.res
			
manifest: $(MANIFEST)
$(MANIFEST):	manifest.rc
			$(WINDRES) --input $< --output $(MANIFEST) --output-format=coff -F pe-i386

main.o		: main.cpp tkzngui.hpp
tkzngui.o		: tkzngui.cpp tkzngui.hpp

