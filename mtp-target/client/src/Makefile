#############################################################################
#
# Don't forget to change the paths of different external libraries
#

ODE_INCLUDE     = $(HOME)/cvs/ode/include
#FMOD_INCLUDE    = $(HOME)/external/fmodapi372linux/api/inc
#FMOD_LIBRARY    = $(HOME)/external/fmodapi372linux/api
STLPORT_INCLUDE = /usr/include/stlport
LIBXML2_INCLUDE = /usr/include/libxml2
NEL_INCLUDE     = $(HOME)/install/release/include
NEL_SOURCE      = $(HOME)/cvs/code/nel/src

# The names of the executables
CXX            = g++
RM             = rm -f
MAKE           = make

DBG            = off

FLAGS_DBG_on   = -g -pipe -O0 -finline-functions -DNL_DEBUG -D__STL_DEBUG -D_REENTRANT -D_GNU_SOURCE
FLAGS_DBG_off  = -g -pipe -O3 -finline-functions -DNL_RELEASE_DEBUG -D_REENTRANT -D_GNU_SOURCE

DIR_DBG_on     = debug
DIR_DBG_off    = release

# The flags for the C++ compiler
CXXFLAGS       = $(FLAGS_DBG_$(DBG)) \
		-I../../common \
		-I$(ODE_INCLUDE) \
		-I$(STLPORT_INCLUDE) \
		-I$(LIBXML2_INCLUDE) \
		-I$(NEL_INCLUDE) \
		-I$(NEL_SOURCE) \
		-I/usr/include/lua50


# The flags for the linker
LDFLAGS        = -L$(HOME)/install/$(DIR_DBG_$(DBG))/lib \
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
		-llualib50

# Add the fmod lib if you want sound

#############################################################################
# The bit that changes each time we cut paste and hack this file :o)

# The list of targets to build
TARGETS		= link client

# The default build rule
all:	$(TARGETS)

#############################################################################
###### PLEASE!!!! put the .o files in alphabetic order !!!!!
#############################################################################

OBJS  = \
3d_task.o \
background_task.o \
camera.o \
chat_task.o \
config_file_task.o \
controler.o \
custom_floating_point.o \
editable_element_common.o \
editor_task.o \
entity.o \
entity_lua_proxy.o \
entity_manager.o \
font_manager.o \
game_task.o \
gui_bin.o \
gui_bitmap.o \
gui_box.o \
gui_button.o \
gui_container.o \
gui_event_behaviour.o \
gui_frame.o \
gui_mouse_listener.o \
gui_multiline_text.o \
gui_object.o \
gui_progress_bar.o \
gui_quad.o \
gui_scale.o \
gui_scrollbar.o \
gui_spacer.o \
gui_spg.o \
gui_stretched_quad.o \
gui_task.o \
gui_text.o \
gui_xml.o \
hud_task.o \
interface.o \
interpolator.o \
intro_task.o \
lens_flare_task.o \
level.o \
level_manager.o \
load_mesh.o \
login_client.o \
lua_nel.o \
lua_utility.o \
main.o \
module.o \
module_common.o \
module_lua_proxy.o \
mouse_listener.o \
mtp_target.o \
net_callbacks.o \
net_message.o \
network_task.o \
particles.o \
particles_common.o \
particles_lua_proxy.o \
resource_manager.o \
score_task.o \
sky_task.o \
sound_manager.o \
start_point.o \
start_point_common.o \
start_position.o \
stdpch.o \
swap_3d_task.o \
task_manager.o \
time_task.o \
water_task.o

#############################################################################
###### PLEASE!!!! put the .o files in alphabetic order !!!!!
#############################################################################

link:
	test -f lua_nel.cpp || ln -s ../../common/lua_nel.cpp lua_nel.cpp
	test -f lua_utility.cpp || ln -s ../../common/lua_utility.cpp lua_utility.cpp
	test -f net_message.cpp || ln -s ../../common/net_message.cpp net_message.cpp
	test -f start_point_common.cpp || ln -s ../../common/start_point_common.cpp start_point_common.cpp
	test -f editable_element_common.cpp || ln -s ../../common/editable_element_common.cpp editable_element_common.cpp
	test -f module_common.cpp || ln -s ../../common/module_common.cpp module_common.cpp
	test -f load_mesh.cpp || ln -s ../../common/load_mesh.cpp load_mesh.cpp
	test -f custom_floating_point.cpp || ln -s ../../common/custom_floating_point.cpp custom_floating_point.cpp
	test -f particles_common.cpp || ln -s ../../common/particles_common.cpp particles_common.cpp

client: $(OBJS)
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
	$(RM) *.o *~ .#* core mtLevelManager.cpp mtLevelManager.h

# remove object files, core dump, and executable (if any)
distclean:
	$(MAKE) clean
	$(RM) $(TARGETS)

# make the thing again from scratch
again:
	$(MAKE) distclean
	$(MAKE) $(TARGETS)
