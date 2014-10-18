#define WIN32_LEAN_AND_MEAN
#include <time.h>
#include <Windows.h>
#include <stdint.h>

class Timer{

public:
	
	typedef struct timeval {
		long tv_sec;
		long tv_usec;
	} timeval;

	int gettimeofday(struct timeval * tp, struct timezone * tzp)
	{
		// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
		static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

		SYSTEMTIME  system_time;
		FILETIME    file_time;
		uint64_t    time;

		GetSystemTime(&system_time);
		SystemTimeToFileTime(&system_time, &file_time);
		time = ((uint64_t)file_time.dwLowDateTime);
		time += ((uint64_t)file_time.dwHighDateTime) << 32;

		tp->tv_sec = (long)((time - EPOCH) / 10000000L);
		tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
		return 0;
	}

	Timer(){}

	void StartTimer()
	{
		struct timeval tim;
		gettimeofday(&tim, NULL);
		startTime = tim.tv_sec+(tim.tv_usec/1000000.0);
	}
	double GetElapsedTime()	// Returns the time in seconds
	{
		struct timeval tim;
		gettimeofday(&tim, NULL);
		double currentTime = tim.tv_sec+(tim.tv_usec/1000000.0);
		
		return currentTime - startTime;
	}
private:
	double startTime;
};
