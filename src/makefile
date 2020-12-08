
#### PROJECT SETTINGS #####

# the name of the executable to be created
BIN_NAME = httpd

# compiler used
CXX ?= g++

# extension of source files used in the project
SRC_EXT = cpp

# path to the source directory, relative to the makefile
SRC_PATH = .

# space-separated pkg-config libraries used by this project
LIBS = 

# general compiler flags
COMPILE_FLAGS = -std=c++17 -g

# additional release-specific flags
RCOMPILE_FLAGS = -D NDEBUG

# additional debug-specific flags
DCOMPILE_FLAGS = -D DEBUG

# add additional include paths
INCLUDES = -I $(SRC_PATH) \
		   -I $(SRC_PATH)/core \
		   -I $(SRC_PATH)/global \
		   -I $(SRC_PATH)/net \
		   -I $(SRC_PATH)/net/tcp \
		   -I $(SRC_PATH)/net/udp \
		   -I $(SRC_PATH)/global \
		   -I $(SRC_PATH)/http \
		   -I $(SRC_PATH)/tools \


VPATH = core:global:net:net/tcp:net/udp:http:tools

# general linker setting
LINK_FLAGS = 

# additional release-specific linker setting
RLINK_FLAGS =

# additional debug-specific linker setting
DLINK_FLAGS =

# destination directory, like a jail or mounted system
DESTDIR = /

# install path (bin/ is appended automatically)
INSTALL_PREFIX = usr/local

#### END PROJECT SETTINGS ####


# obtains the OS type, either 'Darwin' or 'Linux'
UNAME := $(shell uname -s)


SOURCES = main.cpp tassert.cpp global.cpp core.cpp tcp.cpp httpsession.cpp httpserver.cpp

OBJECTS = $(SOURCES:.cpp=.o)

DEPENDENCIES = $(SOURCES:.cpp=.d)


.PHONY : all
all : $(BIN_NAME)

$(BIN_NAME) : $(OBJECTS)
	$(CXX) $^ -o $@


%.d : %.cpp
	@set -e; rm -f $@; \
	 $(CXX) -MM $(COMPILE_FLAGS) -I $(INCLUDES) $< > $@.$$$$; \
	 cat $@.$$$$; \
	 sed 's, \($*\)\.o[ :]*,\1.o $@ :,g' < $@.$$$$ > $@; \
	 echo -e "\t$(CXX) $(COMPILE_FLAGS) -I $(INCLUDES) -c $^" >> $@; \
	 rm -f $@.$$$$

-include $(DEPENDENCIES)



.PHONY : clean
clean :
	rm *.o *.d $(BIN_NAME)



.PHONY : help
help :
	@echo $(OBJECTS)