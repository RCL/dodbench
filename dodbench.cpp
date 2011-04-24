#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <assert.h>
#include <string.h>		// strerror
#ifdef __GLIBC__
	#include <malloc.h> // for mallopt()
#endif // __GLIBC__

#include "class_hier.h"
#include "dod_hier.h"

/**
 * Global table that defines proportions between objects of different types.
 * Since flat distribution of objects is rare, this is used to make test a bit more meaningful
 */
int g_Proportions[ 8 ] = 
		//{ 1, 1, 1, 1, 1, 1, 1, 1 }; 
		{ 20, 10, 5, 4, 3, 2, 1, 1 };

#ifdef DEBUG
/** debugging facility to check that we are actually doing the same work */
int g_NumUpdatesOO[ 8 ]	= { 0 };
/** debugging facility to check that we are actually doing the same work */
int g_NumUpdatesDOD[ 8 ] = { 0 };
#endif // DEBUG

/**
 * This function calculates the budget of all objects to be created.
 * No object type will be 0, at least one object of this type will be created (which means
 * that we can go out of budget if NumObjects < 8)
 */
void CalcDistributionByObject( const int NumObjects, int ( & ObjectNumbers )[ 8 ] )
{
	int Sum =	g_Proportions[ 0 ] + g_Proportions[ 1 ] + g_Proportions[ 2 ] + g_Proportions[ 3 ] + 
				g_Proportions[ 4 ] + g_Proportions[ 5 ] + g_Proportions[ 6 ] + g_Proportions[ 7 ];

	int ObjectsAllocated = 0;
	for ( int i = 7; i > 0; --i )
	{
		int NumObjOfType = g_Proportions[ i ] * NumObjects / Sum;
		if ( NumObjOfType < 1 )
			NumObjOfType = 1;

		ObjectsAllocated += NumObjOfType;
		ObjectNumbers[ i ] = NumObjOfType;
	}

	// assuming that 0th kind is the most populous one
	ObjectNumbers[ 0 ] = ( NumObjects > ObjectsAllocated ) ? NumObjects - ObjectsAllocated : 1;
}

/**
 * This function calculates the budget of all components to be created.
 */
void CalcDistributionByComponent( const int NumObjects, int ( & ComponentNumbers )[ 8 ] )
{
	CalcDistributionByObject( NumObjects, ComponentNumbers );

	// since i-th Object has all 0...i components, we need to account for this
	for ( int i = 1; i < 8; ++i )
	{
		for ( int j = 0; j < i; ++j )
		{
			ComponentNumbers[ j ] += ComponentNumbers[ i ];
		}
	}
}


/**
 * Runs OO loop N times and returns average time in microseconds (note: not milli)
 */
unsigned long long OOLoop( const int NumObjects, const int NumIter, unsigned long long & TotalTimeWorked )
{
	assert( NumObjects > 0 );
	assert( NumIter > 0 );

	// setup
	int Budget[ 8 ];
	CalcDistributionByObject( NumObjects, Budget );
	int RealNumObjects = 0;
	for ( int i = 0; i < 8; ++i )
	{
		RealNumObjects += Budget[ i ];
	}

	OBase ** HeteroTable = new OBase *[ RealNumObjects ];
	
	srand( 105 );	// make it deterministic
	
	// setup
	for ( int i = 0; i < RealNumObjects; ++i )
	{
		int ObjType = 0;
		int MaxAttempts = 128;
		while( --MaxAttempts > 0 )
		{
			ObjType = rand() & 0x07;
			if ( Budget[ ObjType ] > 0 )
			{
				--Budget[ ObjType ];
				break;
			}
		};

		switch( ObjType )
		{
			case 0: HeteroTable[ i ] = new O1; break;
			case 1: HeteroTable[ i ] = new O2; break;
			case 2: HeteroTable[ i ] = new O3; break;
			case 3: HeteroTable[ i ] = new O4; break;
			case 4: HeteroTable[ i ] = new O5; break;
			case 5: HeteroTable[ i ] = new O6; break;
			case 6: HeteroTable[ i ] = new O7; break;
			case 7: HeteroTable[ i ] = new O8; break;
		}
	}
	
	// measurement proper
	timeval		Start, End;
	unsigned long long MicroSecondsTotal = 0;
	if ( 0 != gettimeofday( &Start, NULL ) )
	{
		fprintf( stderr, "gettimeofday failed, errno = %d (%s)\n", errno, strerror( errno ) );
	}
	else
	{
		for ( int Iter = 0; Iter < NumIter; ++Iter )
		{
#ifndef PREFETCH
			// vanilla loop
			for ( int Obj = 0; Obj < RealNumObjects; ++Obj )
			{
				HeteroTable[ Obj ]->Update();
			}
#else
			// prefetch loop
			__builtin_prefetch( HeteroTable[ 0 ] );
			__builtin_prefetch( *reinterpret_cast< void ** >( HeteroTable[ 0 ] ) );

			for ( int Obj = 0; Obj < RealNumObjects - 2; ++Obj )
			{
				__builtin_prefetch( HeteroTable[ Obj + 1 ], 0, 0 );	// no temporal locality
				__builtin_prefetch( *reinterpret_cast< void ** >( HeteroTable[ Obj + 1 ] ), 0, 3 );	// high temporal locality (vtable)
				__builtin_prefetch( HeteroTable[ Obj + 2 ], 0, 1 );	// some temporal locality (access on +2 iteration)

				HeteroTable[ Obj ]->Update();				
			}	

			__builtin_prefetch( HeteroTable[ RealNumObjects - 1 ] );
			__builtin_prefetch( *reinterpret_cast< void ** >( HeteroTable[ RealNumObjects - 1 ] ) );

			HeteroTable[ RealNumObjects - 2 ]->Update();

			HeteroTable[ RealNumObjects - 1 ]->Update();
#endif
		}

		if ( 0 != gettimeofday( &End, NULL ) )
		{
			fprintf( stderr, "gettimeofday failed, errno = %d (%s)\n", errno, strerror( errno ) );
		}
		else
		{
			MicroSecondsTotal = ( static_cast< unsigned long long >( End.tv_usec ) + static_cast< unsigned long long >( End.tv_sec ) * 1000000ULL ) -
				( static_cast< unsigned long long >( Start.tv_usec ) + static_cast< unsigned long long >( Start.tv_sec ) * 1000000ULL );
			TotalTimeWorked += MicroSecondsTotal;		
		}
	}

	// teardown
	for ( int i = 0; i < RealNumObjects; ++i )
	{
		delete HeteroTable[ i ];
	}

	delete[] HeteroTable;

	return MicroSecondsTotal / NumIter;
}

/**
 * Runs DOD loop N times and returns average time in microseconds (note: not milli)
 */
unsigned long long DODLoop( const int NumObjects, const int NumIter, unsigned long long & TotalTimeWorked )
{
	// setup
	int Budget[ 8 ];
	CalcDistributionByComponent( NumObjects, Budget );

	O1Comp * HomoTable1 = new O1Comp[ Budget[ 0 ] ];
	O2Comp * HomoTable2 = new O2Comp[ Budget[ 1 ] ];
	O3Comp * HomoTable3 = new O3Comp[ Budget[ 2 ] ];
	O4Comp * HomoTable4 = new O4Comp[ Budget[ 3 ] ];
	O5Comp * HomoTable5 = new O5Comp[ Budget[ 4 ] ];
	O6Comp * HomoTable6 = new O6Comp[ Budget[ 5 ] ];
	O7Comp * HomoTable7 = new O7Comp[ Budget[ 6 ] ];
	O8Comp * HomoTable8 = new O8Comp[ Budget[ 7 ] ];

	// measurement proper
	timeval		Start, End;
	unsigned long long MicroSecondsTotal = 0;
	if ( 0 != gettimeofday( &Start, NULL ) )
	{
		fprintf( stderr, "gettimeofday failed, errno = %d (%s)\n", errno, strerror( errno ) );
	}
	else
	{
		for ( int Iter = 0; Iter < NumIter; ++Iter )
		{
			const int Num1 = Budget[ 0 ];
			for ( int Obj = 0; Obj < Num1; ++Obj )
			{
#ifdef PREFETCH
				// prefetch of invalid addrs is allowed
				__builtin_prefetch( &HomoTable1[ Obj + 1 ], 0, 1 );
#endif
				HomoTable1[ Obj ].Update();
			}

			const int Num2 = Budget[ 1 ];
			for ( int Obj = 0; Obj < Num2; ++Obj )
			{
#ifdef PREFETCH
				// prefetch of invalid addrs is allowed
				__builtin_prefetch( &HomoTable2[ Obj + 1 ], 0, 1 );
#endif
				HomoTable2[ Obj ].Update();
			}

			const int Num3 = Budget[ 2 ];
			for ( int Obj = 0; Obj < Num3; ++Obj )
			{
#ifdef PREFETCH
				// prefetch of invalid addrs is allowed
				__builtin_prefetch( &HomoTable3[ Obj + 1 ], 0, 1 );
#endif
				HomoTable3[ Obj ].Update();
			}

			const int Num4 = Budget[ 3 ];
			for ( int Obj = 0; Obj < Num4; ++Obj )
			{
#ifdef PREFETCH
				// prefetch of invalid addrs is allowed
				__builtin_prefetch( &HomoTable4[ Obj + 1 ], 0, 1 );
#endif
				HomoTable4[ Obj ].Update();
			}
			
			const int Num5 = Budget[ 4 ];
			for ( int Obj = 0; Obj < Num5; ++Obj )
			{
#ifdef PREFETCH
				// prefetch of invalid addrs is allowed
				__builtin_prefetch( &HomoTable5[ Obj + 1 ], 0, 1 );
#endif
				HomoTable5[ Obj ].Update();
			}

			const int Num6 = Budget[ 5 ];
			for ( int Obj = 0; Obj < Num6; ++Obj )
			{
#ifdef PREFETCH
				// prefetch of invalid addrs is allowed
				__builtin_prefetch( &HomoTable6[ Obj + 1 ], 0, 1 );
#endif
				HomoTable6[ Obj ].Update();
			}

			const int Num7 = Budget[ 6 ];
			for ( int Obj = 0; Obj < Num7; ++Obj )
			{
#ifdef PREFETCH
				// prefetch of invalid addrs is allowed
				__builtin_prefetch( &HomoTable7[ Obj + 1 ], 0, 1 );
#endif
				HomoTable7[ Obj ].Update();
			}

			const int Num8 = Budget[ 7 ];
			for ( int Obj = 0; Obj < Num8; ++Obj )
			{
#ifdef PREFETCH
				// prefetch of invalid addrs is allowed
				__builtin_prefetch( &HomoTable8[ Obj + 1 ], 0, 1 );
#endif
				HomoTable8[ Obj ].Update();
			}
		}
		
		if ( 0 != gettimeofday( &End, NULL ) )
		{
			fprintf( stderr, "gettimeofday failed, errno = %d (%s)\n", errno, strerror( errno ) );
		}
		else
		{
			MicroSecondsTotal = ( static_cast< unsigned long long >( End.tv_usec ) + static_cast< unsigned long long >( End.tv_sec ) * 1000000ULL ) -
			( static_cast< unsigned long long >( Start.tv_usec ) + static_cast< unsigned long long >( Start.tv_sec ) * 1000000ULL );
			TotalTimeWorked += MicroSecondsTotal;
		}
	}
	
	// teardown
	delete[] HomoTable1;
	delete[] HomoTable2;
	delete[] HomoTable3;
	delete[] HomoTable4;
	delete[] HomoTable5;
	delete[] HomoTable6;
	delete[] HomoTable7;
	delete[] HomoTable8;
	
	return MicroSecondsTotal / NumIter;
}

/**
 * Single iteration with fixed number of objects
 */
void TestNumberOfObjects( int DesiredNumObjects )
{
	const int kNumSetups = 32, kNumItersPerSetup = 128;

	unsigned long long DODLoopTime = 0, OOLoopTime = 0;
	unsigned long long DODTime = 0, OOTime = 0;	
	for ( int i = 0; i < kNumSetups; ++i )
	{
		DODLoopTime += DODLoop( DesiredNumObjects, kNumItersPerSetup, DODTime );
		OOLoopTime += OOLoop( DesiredNumObjects, kNumItersPerSetup, OOTime );
	}
	
    DODLoopTime /= kNumSetups;
	OOLoopTime /= kNumSetups;

	int ObjectsByType[ 8 ], ComponentsByType[ 8 ];
	CalcDistributionByObject( DesiredNumObjects, ObjectsByType );
	CalcDistributionByComponent( DesiredNumObjects, ComponentsByType );
	int NumObjects = 0, NumComponents = 0;
	for ( int i = 0; i < 8; ++i )
	{
#ifdef DEBUG
		printf( "Type%d: objects: %d   components: %d\n", i, ObjectsByType[ i ], ComponentsByType[ i ] ); 
#endif // DEBUG

		NumObjects += ObjectsByType[ i ];
		NumComponents += ComponentsByType[ i ];
	}

	size_t WorkingSetObj =	ObjectsByType[ 0 ] * sizeof( O1 ) + ObjectsByType[ 1 ] * sizeof( O2 ) +
							ObjectsByType[ 2 ] * sizeof( O3 ) + ObjectsByType[ 3 ] * sizeof( O4 ) +
							ObjectsByType[ 4 ] * sizeof( O5 ) + ObjectsByType[ 5 ] * sizeof( O6 ) +
							ObjectsByType[ 6 ] * sizeof( O7 ) + ObjectsByType[ 7 ] * sizeof( O8 );

	size_t WorkingSetComp =	ComponentsByType[ 0 ] * sizeof( O1Comp ) + ComponentsByType[ 1 ] * sizeof( O2Comp ) +
							ComponentsByType[ 2 ] * sizeof( O3Comp ) + ComponentsByType[ 3 ] * sizeof( O4Comp ) +
							ComponentsByType[ 4 ] * sizeof( O5Comp ) + ComponentsByType[ 5 ] * sizeof( O6Comp ) +
							ComponentsByType[ 6 ] * sizeof( O7Comp ) + ComponentsByType[ 7 ] * sizeof( O8Comp );
	
	printf( "%16d|%16d|%15luK|%15luK|%16llu|%16llu|%16f|%16llu|%16llu|%16f|\n",
		   NumObjects,
		   NumComponents,
		   static_cast< unsigned long >( WorkingSetObj ) / 1024,
		   static_cast< unsigned long >( WorkingSetComp ) / 1024,
		   OOLoopTime,
		   DODLoopTime,
		   ( DODLoopTime && OOLoopTime ) ? static_cast< float >( static_cast< long long >( OOLoopTime ) ) / static_cast< float >( DODLoopTime ) : 0.0f,
		   OOTime,
		   DODTime,
		   ( DODTime && OOTime ) ? static_cast< float >( static_cast< long long >( OOTime ) ) / static_cast< float >( DODTime ) : 0.0f
		);
}

/**
 * Main
 */
int main( int argc, const char * argv[] )
{
	int DesiredNumObjectsStart = 1024;
	int DesiredNumObjectsEnd = 2048;
	int DesiredNumSteps = 9;
	if ( argc > 3 )
	{
		int PassedNumSteps = atoi( argv[ 3 ] );
		if ( PassedNumSteps > 0 )
		{
			DesiredNumSteps = PassedNumSteps;
		}
	}
	if ( argc > 2 )
	{
		int PassedNumObjectsEnd = atoi( argv[ 2 ] );
		if ( PassedNumObjectsEnd > 0 )
		{
			DesiredNumObjectsEnd = PassedNumObjectsEnd;
		}
	}
	if ( argc > 1 )
	{
		int PassedNumObjectsStart = atoi( argv[ 1 ] );
		if ( PassedNumObjectsStart > 0 )
		{
			DesiredNumObjectsStart = PassedNumObjectsStart;
		}
	}

	if ( DesiredNumObjectsStart > DesiredNumObjectsEnd )
	{
		int Temp = DesiredNumObjectsEnd;
		DesiredNumObjectsEnd = DesiredNumObjectsStart;
		DesiredNumObjectsStart = Temp;
	}

	printf( "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n" );
	printf( "Non-linear payload\n" );
	printf( "Payload iterations = %d\n", PAYLOAD_ITERATIONS );
	printf( "Object proportions: %d:%d:%d:%d:%d:%d:%d:%d\n",
		   g_Proportions[ 0 ], g_Proportions[ 1 ], g_Proportions[ 2 ], g_Proportions[ 3 ],
		   g_Proportions[ 4 ], g_Proportions[ 5 ], g_Proportions[ 6 ], g_Proportions[ 7 ] );

#ifdef PREFETCH
	printf( "Prefetch\n" );
#endif // PREFETCH

#ifdef UNROLL_LOOPS
	printf( "Compiled with unrolled loops\n" );
#endif // PREFETCH	
	
#ifdef DEBUG
	printf( "DEBUG\n" );
#endif // DEBUG

#ifdef M_MMAP_MAX	// forbid malloc() to return mmapped memory (from swap) and always require physical
	printf( "malloc() will not allocate mmapped memory\n" );
	mallopt( M_MMAP_MAX, 0 );
#endif // M_MMAP_MAX
	
#ifdef M_TRIM_THRESHOLD	// less memory-efficient, but faster
	printf( "malloc() will not trim unused memory\n" );
	mallopt( M_TRIM_THRESHOLD, -1 );
#endif // M_TRIM_THRESHOLD
	
	printf( "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n" );
	printf( "%16s|%16s|%16s|%16s|%16s|%16s|%16s|%16s|%16s|%16s|\n",
		   "NumObjs",
		   "NumComps",
		   "OO working set",
		   "DOD working set",
		   "OO loop (usec)",
		   "DOD loop (usec)",
		   "DOD loop speedup",
		   "OO time (usec)",
		   "DOD time (usec)",
		   "DOD time speedup" ); 
	printf( "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n" );

	for ( int i = 0; i < DesiredNumSteps; ++i )
	{
		int NumObjects = DesiredNumObjectsStart + ( DesiredNumObjectsEnd - DesiredNumObjectsStart ) * i / ( DesiredNumSteps - 1 );
		TestNumberOfObjects( NumObjects );
	}

#ifdef DEBUG
	printf( "DEBUG: checking number of calls:\n" );
	for ( int i = 0; i < 8; ++i )
	{
		printf( "Type %d, updates OO: %d  updates DOD: %d\n", i, g_NumUpdatesOO[ i ], g_NumUpdatesDOD[ i ] );
		if ( g_NumUpdatesOO[ i ] != g_NumUpdatesDOD[ i ] )
		{
			fprintf( stderr, "Different number of updates between OO and DOD versions!\n" );
			return 1;
		}
	}
#endif // DEBUG		
	
	return 0;
}

