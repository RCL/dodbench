#ifndef RCL_PAYLOAD_H__INCLUDED
#define RCL_PAYLOAD_H__INCLUDED

#include <string.h>		// memset

#ifndef PAYLOAD_ITERATIONS
	#error "PAYLOAD_ITERATIONS is not defined"
#endif //

struct Payload1
{
	int A[ 4 ];

	inline void Work()
	{
		__builtin_prefetch( &A[ 0 ], 0, 0 );
		
		for ( unsigned int i = 0; i < sizeof( A ) / sizeof( A[ 0 ] ); ++i )
		{
			int Temp = A[ i ];
			
			for ( int j = 0; j < PAYLOAD_ITERATIONS; ++j )
			{
				Temp = ( ( Temp * 1103515241 ) + 12341 ) & 0x7fffffff;
			}
			
			A[ i ] = Temp;
		}
	}

	Payload1() { memset( this, 1, sizeof( *this ) ); };
};

struct Payload2
{
	int A[ 8 ];
	
	inline void Work()
	{
		__builtin_prefetch( &A[ 0 ], 0, 0 );
		
		for ( unsigned int i = 0; i < sizeof( A ) / sizeof( A[ 0 ] ); ++i )
		{
			int Temp = A[ i ];
			
			for ( int j = 0; j < PAYLOAD_ITERATIONS; ++j )
			{
				Temp = ( ( Temp * 1103515242 ) + 12342 ) & 0x7fffffff;
			}
			
			A[ i ] = Temp;
		}
	}
	
	Payload2() { memset( this, 2, sizeof( *this ) ); };
};

struct Payload3
{
	int A[ 16 ];
	
	inline void Work()
	{
		__builtin_prefetch( &A[ 0 ], 0, 0 );
		
		for ( unsigned int i = 0; i < sizeof( A ) / sizeof( A[ 0 ] ); ++i )
		{
			int Temp = A[ i ];
			
			for ( int j = 0; j < PAYLOAD_ITERATIONS; ++j )
			{
				Temp = ( ( Temp * 1103515243 ) + 12343 ) & 0x7fffffff;
			}
			
			A[ i ] = Temp;
		}
	}
	
	Payload3() { memset( this, 3, sizeof( *this ) ); };
};

struct Payload4
{
	int A[ 32 ];
	
	inline void Work()
	{
		__builtin_prefetch( &A[ 0 ], 0, 0 );
		
		for ( unsigned int i = 0; i < 16; ++i )
		{
			int Temp = A[ i ];
			
			for ( int j = 0; j < PAYLOAD_ITERATIONS; ++j )
			{
				Temp = ( ( Temp * 1103515244 ) + 12344 ) & 0x7fffffff;
			}
			
			A[ i ] = Temp;
		}

		__builtin_prefetch( &A[ 16 ], 0, 0 );
		
		for ( unsigned int i = 16; i < 32; ++i )
		{
			int Temp = A[ i ];
			
			for ( int j = 0; j < PAYLOAD_ITERATIONS; ++j )
			{
				Temp = ( ( Temp * 1103515244 ) + 12344 ) & 0x7fffffff;
			}
			
			A[ i ] = Temp;
		}
	}
	
	Payload4() { memset( this, 4, sizeof( *this ) ); };
};

struct Payload5
{
	int A[ 64 ];
	
	inline void Work()
	{
		__builtin_prefetch( &A[ 0 ], 0, 0 );
		
		for ( unsigned int k = 0; k < sizeof( A ) / sizeof( A[ 0 ] ); k += 16 )
		{
			__builtin_prefetch( &A[ k + 16 ], 0, 0 );
			
			for ( unsigned i = k; i < k + 16; ++i )
			{
				int Temp = A[ i ];
				
				for ( int j = 0; j < PAYLOAD_ITERATIONS; ++j )
				{
					Temp = ( ( Temp * 1103515248 ) + 12348 ) & 0x7fffffff;
				}
				
				A[ i ] = Temp;
			}
		}
	}
	
	Payload5() { memset( this, 5, sizeof( *this ) ); };
};

struct Payload6
{
	int A[ 128 ];
	
	inline void Work()
	{
		__builtin_prefetch( &A[ 0 ], 0, 0 );
		
		for ( unsigned int k = 0; k < sizeof( A ) / sizeof( A[ 0 ] ); k += 16 )
		{
			__builtin_prefetch( &A[ k + 16 ], 0, 0 );
			
			for ( unsigned i = k; i < k + 16; ++i )
			{
				int Temp = A[ i ];
				
				for ( int j = 0; j < PAYLOAD_ITERATIONS; ++j )
				{
					Temp = ( ( Temp * 1103515248 ) + 12348 ) & 0x7fffffff;
				}
				
				A[ i ] = Temp;
			}
		}
	}
	
	Payload6() { memset( this, 6, sizeof( *this ) ); };
};

struct Payload7
{
	int A[ 256 ];
	
	inline void Work()
	{
		__builtin_prefetch( &A[ 0 ], 0, 0 );
		
		for ( unsigned int k = 0; k < sizeof( A ) / sizeof( A[ 0 ] ); k += 16 )
		{
			__builtin_prefetch( &A[ k + 16 ], 0, 0 );
			
			for ( unsigned i = k; i < k + 16; ++i )
			{
				int Temp = A[ i ];
				
				for ( int j = 0; j < PAYLOAD_ITERATIONS; ++j )
				{
					Temp = ( ( Temp * 1103515248 ) + 12348 ) & 0x7fffffff;
				}
				
				A[ i ] = Temp;
			}
		}
	}
	
	Payload7() { memset( this, 7, sizeof( *this ) ); };
};

struct Payload8
{
	int A[ 512 ];
	
	inline void Work()
	{
		__builtin_prefetch( &A[ 0 ], 0, 0 );
		
		for ( unsigned int k = 0; k < sizeof( A ) / sizeof( A[ 0 ] ); k += 16 )
		{
			__builtin_prefetch( &A[ k + 16 ], 0, 0 );

			for ( unsigned i = k; i < k + 16; ++i )
			{
				int Temp = A[ i ];
				
				for ( int j = 0; j < PAYLOAD_ITERATIONS; ++j )
				{
					Temp = ( ( Temp * 1103515248 ) + 12348 ) & 0x7fffffff;
				}
				
				A[ i ] = Temp;
			}
		}
	}
	
	Payload8() { memset( this, 8, sizeof( *this ) ); };
};

#endif // RCL_PAYLOAD_H__INCLUDED
