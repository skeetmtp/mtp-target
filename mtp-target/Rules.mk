#############################################################################
# A few basic default rules and intrinsic rules

# This rule must be the first, before all others

$(TARGET): check-deps $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

# Load objects dependencies

ifeq (Dependencies.mk,$(wildcard Dependencies.mk))
include Dependencies.mk
check-deps:
	@echo
	@echo Dependencies found [OK]
	@echo
else
check-deps:
	@echo 
	@echo "No dependencies found [ERROR]"
	@echo "You should try 'make update' first"
	@echo
	@exit 1
endif

# Start off by over-riding the default build rules with our own intrinsics
.SUFFIXES:
.SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) -c $(CXXFLAGS) $<
#	$(CXX) -c $(CXXFLAGS) $< -o $@

# remove object files and core (if any)
clean:
	$(RM) core*
	find . -name "*.o" -exec $(RM) {} \;
	find . -name "*~" -exec $(RM) {} \;
	find . -name "Dependencies.mk" -exec $(RM) {} \;
	find . -name "Objects.mk" -exec $(RM) {} \;

# remove object files, core dump, and executable (if any)
distclean:
	$(MAKE) clean
	$(RM) $(TARGET)

# make the thing again from scratch
again:
	$(MAKE) distclean
	$(MAKE) $(TARGET)

UPDATE_SRCS := $(shell cat ../$(TARGET).dsp | grep SOURCE | grep "\.cpp$$" | cut -d\\ -f3- | tr '\n' ' ' | sed -e 's/=/..\\/g' | tr '\n' ' ' | sed -e 's/\\/\\\//g')
UPDATE_OBJS = $(UPDATE_SRCS:.cpp=.o)

dep: update

update:	link
	echo "OBJS=$(UPDATE_OBJS)" > Objects.mk
	$(CXX) $(CXXFLAGS) -M $(UPDATE_SRCS) > Dependencies.mk

touch:
	$(RM) $(TARGET)

all:	link check-deps $(TARGET)

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
