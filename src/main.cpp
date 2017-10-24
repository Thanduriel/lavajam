#include "engine/game.hpp"


// CRT's memory leak detection
#if defined(_MSC_BUILD) && (defined(DEBUG) || defined(_DEBUG))
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

int main() 
{
#if defined(_MSC_BUILD) && (defined(DEBUG) || defined(_DEBUG))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetBreakAlloc(325);
#endif
	{
		Game game;
		game.Run();
	}

#if defined(_MSC_BUILD) && (defined(DEBUG) || defined(_DEBUG))
	_CrtDumpMemoryLeaks();
#endif
    return 0;
}
