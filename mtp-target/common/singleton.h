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

#ifndef MTPT_SINGLETON
#define MTPT_SINGLETON


//
// Singleton design pattern
//

template<class T>
class CSingleton
{
public:
	
	static T &instance()
	{
		if(!Instance)
		{
			Instance = new T;
			nlassert(Instance);
		}
		return *Instance;
	}
	
	static void uninstance()
	{
		if(Instance)
		{
			delete Instance;
			Instance = 0;
		}
	}
	
protected:
	
	CSingleton()
	{
	}
	
	static T *Instance;
};

template <class T>
T* CSingleton<T>::Instance = 0;

#endif
