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

FLAGS_DBG_on   = -g -pipe -O0 -finline-functions -DMTPT_SERVER -DNL_DEBUG -D__STL_DEBUG -DNL_DEBUG_FAST -D_REENTRANT -D_GNU_SOURCE
FLAGS_DBG_off  = -g -pipe -O3 -finline-functions -DMTPT_SERVER -DNL_RELEASE_DEBUG -D_REENTRANT -D_GNU_SOURCE

DIR_DBG_on     = debug
DIR_DBG_off    = release

# The flags for the C++ compiler
CXXFLAGS       = $(FLAGS_DBG_$(DBG)) \
		-I../../common \
		-I$(HOME)/cvs/code/nel/include \
		-I$(HOME)/cvs/code/nel/src \
		-I$(HOME)/external/ode-0.039/include \
		-I/usr/include/stlport \
		-I/usr/include/libxml2 \
		-I/usr/include/lua50

# The flags for the linker
LDFLAGS      = 	-L$(HOME)/install/$(DIR_DBG_$(DBG))/lib \
		-L$(HOME)/external/ode-0.039/lib \
		-L$(HOME)/external/ode-0.039/OPCODE \
		-llualib50 \
		-lode \
		-lopcode \
		-L/usr/lib \
		-L/usr/X11R6/lib \
		-lnelnet \
		-lnel3d \
		-lnelmisc \
		-lfreetype \
		-lc \
		-lxml2 \
		-lz \
		-lm \
		-ldl \
		-lstlport \
		-lpthread \
		-lcrypt \
		-lfreetype

#############################################################################
# The bit that changes each time we cut paste and hack this file :o)

# The list of targets to build
TARGETS		= link mtp_target_service

# The default build rule
all:			$(TARGETS)

OBJS  = \
bot.o \
client.o \
ending_session_state.o \
entity.o \
entity_manager.o \
level.o \
level_manager.o \
load_mesh.o \
lua_engine.o \
main.o \
module.o \
net_callbacks.o \
network.o \
physics.o \
running_session_state.o \
session_manager.o \
session_state.o \
stdpch.o \
variables.o \
waiting_clients_session_state.o \
waiting_ready_session_state.o \
waiting_start_session_state.o \
welcome.o \
lua_nel.o \
lua_utility.o \
net_message.o \
entity_lua_proxy.o \
level_lua_proxy.o \
editable_element_common.o \
start_point_common.o \
start_point.o \
module_common.o \
module_lua_proxy.o \
custom_floating_point.o

link:
	-ln -s ../../common/lua_nel.cpp lua_nel.cpp
	-ln -s ../../common/lua_utility.cpp lua_utility.cpp
	-ln -s ../../common/net_message.cpp net_message.cpp
	-ln -s ../../common/start_point_common.cpp start_point_common.cpp
	-ln -s ../../common/editable_element_common.cpp editable_element_common.cpp
	-ln -s ../../common/module_common.cpp module_common.cpp
	-ln -s ../../common/load_mesh.cpp load_mesh.cpp
	-ln -s ../../common/custom_floating_point.cpp

mtp_target_service: $(OBJS)
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
