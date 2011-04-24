#ifndef RCL_DOD_HIER_H__INCLUDED
#define RCL_DOD_HIER_H__INCLUDED

#include "payload.h"

/*
 * Component approach
 */
struct O1Comp
{
	Payload1 P;

	void Update();
};

struct O2Comp
{
	Payload2 P;

	void Update();
};

struct O3Comp
{
	Payload3 P;

	void Update();
};

struct O4Comp
{
	Payload4 P;
	
	void Update();
};

struct O5Comp
{
	Payload5 P;
	
	void Update();
};

struct O6Comp
{
	Payload6 P;
	
	void Update();
};

struct O7Comp
{
	Payload7 P;
	
	void Update();
};

struct O8Comp
{
	Payload8 P;
	
	void Update();
};

#endif // RCL_DOD_HIER_H__INCLUDED
