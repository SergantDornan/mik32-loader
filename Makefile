OUTPUT=/home/sergantdornan/mik32Loader/loader
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
DEPFILES= $(patsubst $(SOURCEDIR)%.cpp, $(deps)%.d, $(CFILES))  

all:$(OUTPUT)

$(OUTPUT):$(OBJECTS)
	$(CPPC) $^ -o $@

mrproper:
	rm -rf $(OUTPUT) $(OBJECTS) $(DEPFILES)

$(deps)/%.o:$(SOURCEDIR)/%.cpp
	$(CPPC) $(CFLAGS) $(foreach D,$(INCDIR),-I$(D)) -c $< -o $@

-include $(DEPFILES) 
