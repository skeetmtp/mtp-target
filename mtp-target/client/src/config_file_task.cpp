/* Copyright, 2003 Melting Pot
 *
 * This file is part of MTP Target.
 * MTP Target is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * MTP Target is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with MTP Target; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */


//
// Includes
//

#include "stdpch.h"

#include <nel/misc/variable.h>

#include "config_file_task.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
// Variables
//


//
// Functions
//
	
void CConfigFileTask::init()
{
	string ConfigFileName = "mtp_target.cfg";

	try
	{
		if(!NLMISC::CFile::fileExists(ConfigFileName))
		{
			FILE *fp = fopen(ConfigFileName.c_str(), "wt");
			nlassert(fp);
			fprintf(fp,"RootConfigFilename = \"mtp_target_default.cfg\";\n");
			fclose(fp);
		}
		ConfigFile.load (ConfigFileName);

		// link variables with config file
		IVariable::init(ConfigFile);
	}
	catch (NLMISC::EConfigFile &e)
	{
		nlerror(e.what());
	}
}

void CConfigFileTask::update()
{
	CConfigFile::checkConfigFiles();
}
