#############################################################################
# Setting up the global compiler settings...

# The names of the executables
CXX           = g++
RM            = rm -f
MAKE          = make

DBG           = off

FLAGS_CMN     = -g -pipe -Wno-ctor-dtor-privacy -Wno-multichar -D_REENTRANT -D_GNU_SOURCE

FLAGS_DBG_on  = -O0 -finline-functions -DNL_DEBUG -D__STL_DEBUG
FLAGS_DBG_off = -O3 -ftemplate-depth-24 -funroll-loops -DNL_RELEASE_DEBUG
DIR_DBG_on    = debug
DIR_DBG_off   = release

# CHANGE THESE PATHS IF NECESSARY
# warning if you use ode 0.5 remove -lopcode in server Makefile
#ODE_VERSION     = 0.5 
ODE_VERSION     = 0.039
ODE_INCLUDE     = $(HOME)/external/ode-$(ODE_VERSION)/include
ODE_LIB         = $(HOME)/external/ode-$(ODE_VERSION)/lib
OPCODE_LIB      = $(HOME)/external/ode-$(ODE_VERSION)/OPCODE
NEL_INCLUDE     = $(HOME)/install/release/include
NEL_SOURCE      = $(HOME)/cvs/code/nel/src
STLPORT_INCLUDE = /usr/include/stlport
LIBXML2_INCLUDE = /usr/include/libxml2
LUA_INCLUDE	= /usr/include/lua50

# uncomment and setup this 2 lines above if you want fmod sound support
#FMOD_CXXFLAGS  = -DUSE_FMOD -I$(HOME)/external/fmodapi372linux/api/inc
#FMOD_LDFLAGS   = -L$(HOME)/external/fmodapi372linux/api -lfmod-3.72
