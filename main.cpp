using namespace std;

#define CATCH_CONFIG_RUNNER  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "catch_reporter_appveyor.hpp"

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

#include "Std/String.h"

int main(int argc, char* argv[])
{
	// Enable run-time memory leak check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(21499);
#endif

	{
		Catch::Session session; // There must be exactly one instance

								// writing to session.configData() here sets defaults
								// this is the preferred way to set them

		int returnCode = session.applyCommandLine(argc, argv);
		if (returnCode != 0) // Indicates a command line error
			return returnCode;

		// writing to session.configData() or session.Config() here 
		// overrides command line args
		// only do this if you know you need to

		int numFailed = session.run();

		// numFailed is clamped to 255 as some unices only use the lower 8 bits.
		// This clamping has already been applied, so just return it here
		// You can also do any post run clean-up here
		return numFailed;
	}
}