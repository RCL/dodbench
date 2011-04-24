#include "dod_hier.h"

#ifdef DEBUG
extern int g_NumUpdatesDOD[ 8 ];
#endif // DEBUG

/*
 * Since components do not call each other, we can lump them together into single .cpp unit
 * without fear of inlining.
 */

void O1Comp::Update()
{
	P.Work();

#ifdef DEBUG
	++g_NumUpdatesDOD[ 0 ];
#endif // DEBUG
}

void O2Comp::Update()
{
	P.Work();

#ifdef DEBUG
	++g_NumUpdatesDOD[ 1 ];
#endif // DEBUG
}

void O3Comp::Update()
{
	P.Work();

#ifdef DEBUG
	++g_NumUpdatesDOD[ 2 ];
#endif // DEBUG
}

void O4Comp::Update()
{
	P.Work();

#ifdef DEBUG
	++g_NumUpdatesDOD[ 3 ];
#endif // DEBUG
}

void O5Comp::Update()
{
	P.Work();

#ifdef DEBUG
	++g_NumUpdatesDOD[ 4 ];
#endif // DEBUG
}

void O6Comp::Update()
{
	P.Work();

#ifdef DEBUG
	++g_NumUpdatesDOD[ 5 ];
#endif // DEBUG
}

void O7Comp::Update()
{
	P.Work();

#ifdef DEBUG
	++g_NumUpdatesDOD[ 6 ];
#endif // DEBUG
}

void O8Comp::Update()
{
	P.Work();

#ifdef DEBUG
	++g_NumUpdatesDOD[ 7 ];
#endif // DEBUG
}
