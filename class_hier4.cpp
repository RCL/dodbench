#include "class_hier.h"

#ifdef DEBUG
extern int g_NumUpdatesOO[ 8 ];
#endif // DEBUG

void O4::Update()
{
	O3::Update();

	Weight.Work();

#ifdef DEBUG
	++g_NumUpdatesOO[ 3 ];
#endif // DEBUG
}


