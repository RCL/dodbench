#include "class_hier.h"

#ifdef DEBUG
extern int g_NumUpdatesOO[ 8 ];
#endif // DEBUG

void O2::Update()
{
	O1::Update();
	
	Weight.Work();

#ifdef DEBUG
	++g_NumUpdatesOO[ 1 ];
#endif // DEBUG
}


