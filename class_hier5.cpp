#include "class_hier.h"

#ifdef DEBUG
extern int g_NumUpdatesOO[ 8 ];
#endif // DEBUG

void O5::Update()
{
	O4::Update();

	Weight.Work();

#ifdef DEBUG
	++g_NumUpdatesOO[ 4 ];
#endif // DEBUG
}
