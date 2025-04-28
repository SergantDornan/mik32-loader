OUTPUT=
INSTALLOUTPUT=./installer
INCDIR=./include
SOURCEDIR=./source
deps=./depsAndObjects
CPPC=g++
C++standart=-std=c++23
OPT=-O3
DEPFLAGS=-MP -MD
GENERALFLAGS=$(C++standart) -g3 -w
CFLAGS=$(GENERALFLAGS) $(OPT) $(DEPFLAGS)
CFILES=$(foreach D, $(SOURCEDIR), $(wildcard $(D)/*.cpp))
OBJECTS=$(patsubst $(SOURCEDIR)%.cpp, $(deps)%.o, $(CFILES))
DEPFILES= $(patsubst $(SOURCEDIR)%.cpp, $(deps)%.d, $(CFILES)) $(deps)/installer.d
INSTALLOBJECTS= $(deps)/installer.o $(deps)/alias.o $(deps)/BuilderFilework.o $(deps)/uninstall.o $(deps)/filework.o $(deps)/inputs.o

all:$(OUTPUT)

install:$(INSTALLOUTPUT)
	@./installer

$(INSTALLOUTPUT):$(INSTALLOBJECTS)
	$(CPPC) $^ -o $@

$(OUTPUT):$(OBJECTS)
	$(CPPC) $^ -o $@

mrproper:
	rm -rf $(OBJECTS) $(DEPFILES) $(INSTALLOBJECTS) $(INSTALLOUTPUT)

$(deps)/%.o:$(SOURCEDIR)/%.cpp
	$(CPPC) $(CFLAGS) $(foreach D,$(INCDIR),-I$(D)) -c $< -o $@

$(deps)/installer.o:./installer.cpp
	$(CPPC) $(CFLAGS) $(foreach D,$(INCDIR),-I$(D)) -c $< -o $@

-include $(DEPFILES) 
