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
#ifndef MTPT_GUI_SPG_H
#define MTPT_GUI_SPG_H

class guiSPGCounter
{
public:
	void *p;
	int refCounter;
	guiSPGCounter() {refCounter=0;p=NULL;}
	guiSPGCounter(void *p_) : p(p_),refCounter(0) {}

	void addRef()
	{
		refCounter++;
	}
	bool release();
};


typedef std::map<void *,guiSPGCounter *> pointer2Counter;

class guiSPGManager
{
public:
	static guiSPGManager &instance();
	guiSPGCounter *get(void *p);
	void add(void *p,guiSPGCounter *counter);
	void remove(void *p);
private:
	static guiSPGManager *_instance;
	pointer2Counter _pointer2Counter;
};



namespace guiSmartPointer
{

// Smart pointer wraper class that implements reference counting 
template <class T> class guiSPG
{
private:
	typedef guiSPGCounter Counter;

	void addRef()  
	{
		if(counter) 
			counter->addRef();
	}
	void release() 
	{
		bool del;
		if(counter) 
			del = counter->release(); 
		if(del)
			delete p;
		counter=NULL;
	}
protected:	
public:
	T *p;
	Counter *counter;

	//!constructor
	guiSPG() : counter(NULL),p(NULL) {};
	//!constructor
	guiSPG( T* p_ )  { if(p_) counter=guiSPGManager::instance().get(p_); p=p_; counter->addRef();};
	//!Copy constructor.
	template <class U> guiSPG<T>(const guiSPG<U>& sp_ ) { 
		if(sp_.counter)
		{
			p=sp_.p;
			counter=(guiSPGCounter *)sp_.counter;
			addRef(); 
		}
		else
		{
			counter=NULL;
			p=NULL;
		}
	} 
	//!Copy constructor.
	guiSPG<T>(const guiSPG<T>& sp_ ) {
		if(sp_.counter)
		{
			p=sp_.p;
			counter=sp_.counter;
			addRef(); 
		}
		else
		{
			counter=NULL;
			p=NULL;
		}
	} 

	//!constructor.
	guiSPG( int Null ) : counter(NULL),p(NULL) {nlassert(Null==NULL);}	// Assignment to NULL
	
	//!destructor
	~guiSPG() { release(); }

	//!Convert & Replace .
	template <class U> guiSPG<T>&  operator=(guiSPG<U> &newp ) {
		release();
		if (newp.counter)
		{
			p = newp.p; //this line provide a compile-time convertion check (inheritance check)
			counter = (guiSPGCounter *) newp.counter;
			addRef();
		}
		return *this;
	}

	//!Casting to normal pointer.
	operator T*() const { return p; }
	
	//!Casting to const pointer.
	operator const T*() const { return p; }
	
	//!Dereference operator, allow dereferencing smart pointer just like normal pointer.
	T& operator*() const { return *p; }

	//! Arrow operator, allow to use regular C syntax to access members of class.
	T* operator->(void) const { 
		return p; 
	}
	

	//!Replace pointer.
	guiSPG<T>&  operator=(const guiSPG<T> &newp ) {
		release();
		if (newp.counter)
		{	
			counter = newp.counter;
			addRef();
			p = newp.p;
		}
		return *this;
	}

	//!Replace pointer.
	guiSPG<T>&  operator=( T* newp ) {
		release();
		if(newp)
		{
			counter = guiSPGManager::instance().get(newp);
			counter->addRef();
		}
		p = newp;
		return *this;
	}

	
	
	
	//!Cast to boolean, simplify if statements with smart pointers.
	operator bool() { return p != NULL; };
	//!Cast to boolean, simplify if statements with smart pointers.
	operator bool() const { return p != NULL; };
	//!Cast to boolean, simplify if statements with smart pointers.
	bool  operator !() { return p == NULL; };
	
	//!Misc compare functions.
	bool  operator == ( const T* p1 ) const { return p == p1; };
	//!Misc compare functions.
	bool  operator != ( const T* p1 ) const { return p != p1; };
	//!Misc compare functions.
	bool  operator <  ( const T* p1 ) const { return p < p1; };
	//!Misc compare functions.
	bool  operator >  ( const T* p1 ) const { return p > p1; };
	//!Misc compare functions.
	bool operator == ( const guiSPG<T> &p1 ) const { return p == p1.p; };
	//!Misc compare functions.
	bool operator != ( const guiSPG<T> &p1 ) const { return p != p1.p; };
	//!Misc compare functions.
	bool operator < ( const guiSPG<T> &p1 ) const { return p < p1.p; };
	//!Misc compare functions.
	bool operator > ( const guiSPG<T> &p1 ) const { return p > p1.p; };
	//!Misc compare functions.
	friend bool operator == <> ( T *p, const guiSPG<T> &p1 );
	//!Misc compare functions.
	friend bool operator == <> ( const guiSPG<T> &p1,const int null );
	//!Misc compare functions.
	friend bool operator != <> ( const guiSPG<T> &p1,const int null );
	//!Misc compare functions.
	friend bool operator == <> ( const int null,const guiSPG<T> &p1 );
	//!Misc compare functions.
	friend bool operator != <> ( const int null,const guiSPG<T> &p1 );
};

template <class T>
inline bool operator == ( T *p, const guiSPG<T> &p1 )	{
	return p1.p == p;
}

template <class T>
inline bool operator == ( const guiSPG<T> &p1,const int null )	{
	return p1.p==NULL;
}

template <class T>
inline bool operator != ( const guiSPG<T> &p1,const int null )	{
	return p1.p!=NULL;
}

template <class T>
inline bool operator == ( const int null,const guiSPG<T> &p1 )	{
	return p1.p==NULL;
}

template <class T>
inline bool operator != ( const int null,const guiSPG<T> &p1 )	{
	return p1.p!=NULL;
}

	
}//namespace guiSmartPointer

// Pretend the hack namespace doesn't exist to clients
using namespace guiSmartPointer;




#endif //MTPT_GUI_SPG_H

