#include "class_hier.h"

#ifdef DEBUG
extern int g_NumUpdatesOO[ 8 ];
#endif // DEBUG

void O7::Update()
{
	O6::Update();

	Weight.Work();

#ifdef DEBUG
	++g_NumUpdatesOO[ 6 ];
#endif // DEBUG
}
