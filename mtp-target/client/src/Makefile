#############################################################################
#
# Don't forget to change the paths of different external libraries
#

ODE_INCLUDE     = $(HOME)/cvs/ode/include
FMOD_INCLUDE    = $(HOME)/external/fmodapi371linux/api/inc
STLPORT_INCLUDE = /usr/include/stlport
LIBXML2_INCLUDE = /usr/include/libxml2
NEL_INCLUDE     = $(HOME)/cvs/code/nel/include

# The names of the executables
CXX            = g++
RM             = rm -f
MAKE           = make

DBG            = off

FLAGS_DBG_on   = -g -pipe -O0 -finline-functions -DNL_DEBUG -D__STL_DEBUG -D_REENTRANT -D_GNU_SOURCE
FLAGS_DBG_off  = -g -pipe -O3 -finline-functions -DNL_RELEASE -D_REENTRANT -D_GNU_SOURCE

DIR_DBG_on     = debug
DIR_DBG_off    = release

# The flags for the C++ compiler
CXXFLAGS       = $(FLAGS_DBG_$(DBG)) \
		-I../../common \
		-I$(ODE_INCLUDE) \
		-I$(FMOD_INCLUDE) \
		-I$(STLPORT_INCLUDE) \
		-I$(LIBXML2_INCLUDE) \
		-I$(NEL_INCLUDE) \
		-I$(NEL_INCLUDE)/../src \
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
		-llualib50 \
		-L$(HOME)/external/fmodapi371linux/api \
		-lfmod-3.71 		
# Add the fmod lib if you want sound

#############################################################################
# The bit that changes each time we cut paste and hack this file :o)

# The list of targets to build
TARGETS		= link client

# The default build rule
all:	$(TARGETS)

OBJS  = \
3d_task.o \
background_task.o \
camera.o \
chat_task.o \
config_file_task.o \
controler.o \
entity.o \
entity_manager.o \
font_manager.o \
game_task.o \
gui_bin.o \
gui_bitmap.o \
gui_box.o \
gui_button.o \
gui_container.o \
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
main.o \
mouse_listener.o \
mtp_target.o \
net_callbacks.o \
network_task.o \
resource_manager.o \
score_task.o \
sky_task.o \
sound_manager.o \
stdpch.o \
swap_3d_task.o \
task_manager.o \
time_task.o \
lua_nel.o \
lua_utility.o \
net_message.o \
login_client.o \
water_task.o \
editor_task.o \
start_point.o \
start_point_common.o \
module.o \
module_common.o \
load_mesh.o \
editable_element_common.o \
gui_event_behaviour.o

link:
	-ln -s ../../common/lua_nel.cpp lua_nel.cpp
	-ln -s ../../common/lua_utility.cpp lua_utility.cpp
	-ln -s ../../common/net_message.cpp net_message.cpp
	-ln -s ../../common/start_point_common.cpp start_point_common.cpp
	-ln -s ../../common/editable_element_common.cpp editable_element_common.cpp
	-ln -s ../../common/module_common.cpp module_common.cpp
	-ln -s ../../common/load_mesh.cpp load_mesh.cpp

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
