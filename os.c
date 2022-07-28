uint64_t os_get_time(void); // get time in nanoseconds since some arbitrary point
/*
get a number corresponding to when `filename' was last edited.
0 if the file does not exist
*/
uint64_t os_file_last_modified_timestamp(const char *filename);
void os_sleep_ms(uint32_t ms); // sleep for milliseconds

#if _WIN32

#include <windows.h>
uint64_t os_get_time(void) {
	static uint64_t starting_time;
	FILETIME ftime = {0};
	uint64_t t;
	GetSystemTimeAsFileTime(&ftime);
	t = ftime.dwLowDateTime | (uint64_t)ftime.dwHighDateTime << 32;
	if (starting_time == 0) starting_time = t;
	return (t - starting_time) * 100;
}

uint64_t os_file_last_modified_timestamp(const char *filename) {
	uint64_t timestamp = 0;
	HANDLE h = CreateFileA(filename, GENERIC_READ, 7, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FILETIME t = {0};
	if (h != INVALID_HANDLE_VALUE) GetFileTime(h, NULL, NULL, &t);
	timestamp = t.dwLowDateTime | (uint64_t)t.dwHighDateTime << 32;
	if (h != INVALID_HANDLE_VALUE) CloseHandle(h);
	return timestamp;
}

void os_sleep_ms(uint32_t ms) {
	Sleep(ms);
}

#else /* if !_WIN32 */

#include <unistd.h>
#include <time.h>

uint64_t os_get_time(void) {
	struct timespec t = {0};
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (uint64_t)t.tv_sec * 1000000000 + (uint64_t)t.tv_nsec;
}

uint64_t os_file_last_modified_timestamp(const char *filename) {
	struct stat statbuf = {0};
	uint64_t timestamp = 0;
	if (stat(filename, &statbuf) == 0) {
	 	struct timespec t = statbuf.st_mtim;
		timestamp = (uint64_t)t.tv_sec * 1000000000 + (uint64_t)t.tv_nsec;
	}
	return timestamp;
}

void os_sleep_ms(uint32_t ms) {
	/* gah useconds_t is 32-bit
	it's conceivable that someone would want to sleep for 2^32 us ≈ 1.2 hours,
	 so we'd better support it
	*/
	while (ms > 1000000) {
		usleep(1000000000);
		ms -= 1000000;
	}
	
	usleep(ms * 1000);
}

#endif /* _WIN32 */
