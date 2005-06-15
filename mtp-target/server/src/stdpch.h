
#include <nel/misc/types_nl.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <deque>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <algorithm>
#include <exception>

#ifdef NL_OS_WINDOWS
#	include <winsock2.h>
#	undef min
#	undef max
#elif defined NL_OS_UNIX
#	include <unistd.h>
#	include <sys/time.h>
#	include <sys/types.h>
#endif

#include <nel/misc/path.h>
#include <nel/misc/debug.h>
#include <nel/misc/vector.h>
#include <nel/misc/matrix.h>
#include <nel/misc/common.h>
#include <nel/misc/command.h>
#include <nel/misc/fast_mem.h>
#include <nel/misc/variable.h>
#include <nel/misc/singleton.h>
#include <nel/misc/system_info.h>
#include <nel/misc/config_file.h>
#include <nel/misc/mem_displayer.h>

#include <nel/net/service.h>
