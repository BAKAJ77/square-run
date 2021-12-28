#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <string>
#include <chrono>

namespace Util
{
	// Returns the retrieved current date and time.
	extern std::string GetTimeStampStr();

	// Returns time that has passed (in seconds) since epoch (1st Jan 1970).
	extern uint64_t GetTimeSinceEpoch();
}

#endif
