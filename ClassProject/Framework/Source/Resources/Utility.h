#include "Core/CoreHeaders.h"

namespace fw
{
	static const float PI = 3.141592653f;

	double getSystemTimeSinceReboot();
	double getSystemTime();
	void outputMessage(const char* message, ...);
	char* loadCompleteFile(const char* filename, long* length);
}
