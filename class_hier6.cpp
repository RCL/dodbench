#include "class_hier.h"

#ifdef DEBUG
extern int g_NumUpdatesOO[ 8 ];
#endif // DEBUG

void O6::Update()
{
	O5::Update();

	Weight.Work();

#ifdef DEBUG
	++g_NumUpdatesOO[ 5 ];
#endif // DEBUG
}
