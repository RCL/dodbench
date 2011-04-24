#include "class_hier.h"

#ifdef DEBUG
extern int g_NumUpdatesOO[ 8 ];
#endif // DEBUG

void O3::Update()
{
	O2::Update();
	
	Weight.Work();

#ifdef DEBUG
	++g_NumUpdatesOO[ 2 ];
#endif // DEBUG
}
