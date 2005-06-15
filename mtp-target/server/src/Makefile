#############################################################################
# Setting up the compiler settings...

# Global variables
include ../../Variables.mk

ifeq (Objects.mk,$(wildcard Objects.mk))
include Objects.mk
endif

# The flags for the C++ compiler
CXXFLAGS       = $(FLAGS_CMN) $(FLAGS_DBG_$(DBG)) \
		-DMTPT_SERVER \
		-I../../common \
		-I$(ODE_INCLUDE) \
		-I$(STLPORT_INCLUDE) \
		-I$(LIBXML2_INCLUDE) \
		-I$(NEL_INCLUDE) \
		-I$(NEL_SOURCE) \
		-I$(LUA_INCLUDE)

# The flags for the linker
LDFLAGS        = -L$(HOME)/install/$(DIR_DBG_$(DBG))/lib \
		-L$(ODE_LIB) \
		-L$(OPCODE_LIB) \
		-L$(NEL_LIB) \
		-L$(LUA_LIB) \
		-L$(LIBXML2_LIB) \
		-lnel3d \
		-lnelnet \
		-lnelmisc \
		-L/usr/lib \
		-L/usr/X11R6/lib \
		-llua \
		-llualib \
		-lc \
		-lxml2 \
		-lz \
		-lm \
		-lstlport_gcc \
		-lpthread \
		-lcrypt \
		-ljpeg \
		-lode

#############################################################################

TARGET		= mtp_target_service


#############################################################################
# Rules

include ../../Rules.mk
