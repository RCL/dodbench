#ifndef CLASS_HIER_H_INCLUDED
#define CLASS_HIER_H_INCLUDED

#include "payload.h"

/**
 * Object-oriented approach
 */
struct OBase
{
	virtual void	Update()  = 0;

	virtual ~OBase() {};
};

class O1 : public OBase
{
	Payload1		Weight;

public:

	virtual void	Update();

	virtual ~O1() {};
};

class O2 : public O1
{
	Payload2		Weight;
		
public:
		
	virtual void	Update();

	virtual ~O2() {};
};

class O3 : public O2
{
	Payload3		Weight;
		
public:
		
	virtual void	Update();

	virtual ~O3() {};
};

class O4 : public O3
{
	Payload4		Weight;
		
public:
		
	virtual void	Update();

	virtual ~O4() {};
};

class O5 : public O4
{
	Payload5		Weight;
		
public:
		
	virtual void	Update();

	virtual ~O5() {};
};

class O6 : public O5
{
	Payload6		Weight;
		
public:
		
	virtual void	Update();

	virtual ~O6() {};
};

class O7 : public O6
{
	Payload7		Weight;
	
public:
	
	virtual void	Update();

	virtual ~O7() {};
};

class O8 : public O7
{
	Payload8		Weight;
	
public:
	
	virtual void	Update();

	virtual ~O8() {};
};

#endif // CLASS_HIER_H_INCLUDED
