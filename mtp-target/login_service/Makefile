#############################################################################
#
# Don't forget to change the paths of different external libraries
# in CXXFLAGS and LDFLAGS
#

# The names of the executables
CXX            = g++
RM             = rm -f
MAKE           = make

DBG            = off

FLAGS_DBG_on   = -g -pipe -O0 -finline-functions -DNL_DEBUG -D__STL_DEBUG -DNL_DEBUG_FAST -D_REENTRANT -D_GNU_SOURCE
FLAGS_DBG_off  = -g -pipe -O3 -finline-functions -DNL_RELEASE_DEBUG -D_REENTRANT -D_GNU_SOURCE

DIR_DBG_on     = debug
DIR_DBG_off    = release

# The flags for the C++ compiler
CXXFLAGS       = $(FLAGS_DBG_$(DBG)) \
		-I$(HOME)/cvs/code/nel/include \
		-I$(HOME)/cvs/code/nel/src \
		-I/usr/include/stlport \
		-I/usr/include/libxml2 \
		-I/usr/include/mysql		

# The flags for the linker
LDFLAGS      = 	-L$(HOME)/install/$(DIR_DBG_$(DBG))/lib \
		-L/usr/lib \
		-lnelnet \
		-lnelmisc \
		-lc \
		-lz \
		-lm \
		-ldl \
		-lstlport \
		-lpthread \
		-lcrypt \
		-lmysqlclient

#############################################################################
# The bit that changes each time we cut paste and hack this file :o)

# The list of targets to build
TARGETS		= login_service

# The default build rule
all:			$(TARGETS)

OBJS  = connection_client.o  connection_web.o  connection_ws.o  login_service.o

login_service: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)


#############################################################################
# A few basic default rules and intrinsic rules

# Start off by over-riding the default build rules with our own intrinsics
.SUFFIXES:
.SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) -c $(CXXFLAGS) $<


# remove object files and core (if any)
clean:
	$(RM) *.o *~ .#* *.log core mtLevelManager.cpp mtLevelManager.h

# remove object files, core dump, and executable (if any)
distclean:
	$(MAKE) clean
	$(RM) $(TARGETS)

# make the thing again from scratch
again:
	$(MAKE) distclean
	$(MAKE) $(TARGETS)
