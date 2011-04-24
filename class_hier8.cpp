#include "class_hier.h"

#ifdef DEBUG
extern int g_NumUpdatesOO[ 8 ];
#endif // DEBUG

void O8::Update()
{
	O7::Update();

	Weight.Work();

#ifdef DEBUG
	++g_NumUpdatesOO[ 7 ];
#endif // DEBUG
}
