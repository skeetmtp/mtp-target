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
		-lnelnet \
		-lfreetype \
		-lnel3d \
		-lnelmisc \
		-L/usr/lib \
		-L/usr/X11R6/lib \
		-llualib50 \
		-lc \
		-lxml2 \
		-lz \
		-lm \
		-ldl \
		-lstlport \
		-lpthread \
		-lcrypt \
		-ljpeg \
		-llualib50 \
		-lode \
		-lopcode

#############################################################################

TARGET		= mtp_target_service


#############################################################################
# Rules

include ../../Rules.mk
