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

#ifndef LOGIN_SERVICE_H
#define LOGIN_SERVICE_H

#include <mysql.h>

#include <nel/misc/types_nl.h>

#include <nel/misc/log.h>
#include <nel/misc/debug.h>
#include <nel/misc/displayer.h>
#include <nel/misc/config_file.h>

#include <nel/net/service.h>


//
// Variables
//

extern NLMISC::CFileDisplayer	Fd;
extern NLMISC::CLog				*Output;

extern MYSQL *DatabaseConnection;

class CMysqlResult
{
public:
	CMysqlResult() 
	{ 
		_result = NULL;
	}
	CMysqlResult(CMysqlResult &mysqlResult) 
	{ 
		nlwarning("CMysqlResult copy not implemented");
		_result=mysqlResult._result; 
	}
	~CMysqlResult()
	{
		mysql_free_result(_result);
	}

	CMysqlResult(MYSQL_RES *result) 
	{ 
		if(_result!=NULL)
			mysql_free_result(_result);
		_result = result; 
	};
	operator MYSQL_RES*() { return _result; };
private:
	MYSQL_RES *_result;
};

//
// Functions
//

std::string sqlQuery(const std::string &query, sint32 &nbRow, MYSQL_ROW &firstRow, CMysqlResult &result);

#endif // LOGIN_SERVICE_H

/* End of login_service.h */
