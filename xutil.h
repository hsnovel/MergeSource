#ifndef XUTIL_H
#define XUTIL_H

/*
 * All defines that can be turned on or off:
 *
 * XUTIL_IMPLEMENTATION
 * XUTIL_WINDOWS_SUPRESS_LINK_ADVAPI_WARNING
 * XUTIL_WINDOWS_ENABLE_LINK_ADVAPI
*/

/* OS FLAGS */
#if defined(__WINDOWS__) || defined(__TOS_WIN__) || defined(__WIN32__) || defined(_WIN64) || defined(_WIN32) || defined(_WIN16)
#define XUTIL_WINDOWS
#elif defined(__gnu_linux__) || defined(__unix__) || defined(__unix) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#define XUTIL_UNIX
#elif defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || defined(__MACH__)
#define XUTIL_MACOS
#endif

#if defined(__gnu_linux__)
#define XUTIL_LINUX
#endif

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#define XUTIL_BSD
#endif

/* COMPILER FLAGS */
#if defined(__GNUC__) || defined(__GNUC_MINOR__) || defined(__GNUC_PATCHLEVEL__)
#define XUTIL_GCC

#elif defined(__clang__) || defined(__clang_major__) || defined(__clang_minor__) || defined(__clang_patchlevel__) || defined(__clang_version__)
#define XUTIL_CLANG

#elif defined(_MSC_VER) || defined(_MSC_FULL_VER) || defined(_MSC_FULL_VER) || defined(_MSC_BUILD)
#define XUTIL_MSVC
#endif

/* UTIL MACROS */

#ifdef _XUTIL_CHECKER_
#define BUILD_BUG_ON_ZERO(e) (0)
#else
#define BUILD_BUG_ON_ZERO(e) ((int)(sizeof(struct { int:(-!!(e)); })))
#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __is_array(arr))
#define __is_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))

#if defined(XUTIL_CLANG) || defined(XUTIL_GCC)
#define likely(x)  __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
#else
#define likely(x)
#define unlikely(x)
#endif

#if defined (XUTIL_GCC) || defined (XUTIL_CLANG)
#define XUTIL_ALIGN(x) __attribute__((aligned(x)))
#elif defined (XUTIL_MSVC)
#define XUTIL_ALIGN(x) __declspec(align(x))
#else
#warning "XUTIL_ALIGN will not be defined as not known compiler is used."
#define XUTIL_ALIGN(x)
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#ifndef XUTIL_WIPE_TYPES

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#endif

typedef enum  {
	XUTIL_ERROR_OPENING_FILE,
	XUTIL_ERROR_READING_FILE,
	XUTIL_ERROR_GETTING_FILE_STATUS,
	XUTIL_ERROR_ALLOCATING_SPACE,
	XUTIL_ERROR_INVALID_PERM,
	XUTIL_DIRECTORY_EXISTS,
	__XUTIL_ERROR_LAST = 0,
} xutil_error;

typedef enum {
	XUTIL_PERM_USER,
	XUTIL_PERM_ROOT,
	XUTIL_PERM_ONLY_OWNER_AND_ROOT,
} xutil_perm;

#define KILOBYTE(x) ((x) / 1024)
#define MEGABYTE(x) ((x) / (1024 * 1024))
#define GIGABYTE(x) ((x) / (1024 * 1024 * 1024))
#define TERABYTE(x) ((x) / (1024 * 1024 * 1024 * 1024))

typedef struct {
	size_t capacity;
	size_t free;
	size_t available;
} xspace_info;

struct xutil_config {
	FILE *print_error_fd;
};

typedef struct {
	size_t capacity;
	size_t available;
} xmemory_info;


struct xutil_config xutil_config;

void xutil_init_config(void);
void xutil_config_change_print_err_fd(FILE *fd);
int  xutil_get_last_error();
void xutil_print_error(int errcode);
void xutil_print_last_error();
void xutil_write_error(int errcode, char **buf);

int xutil_is_root(void);
int xutil_get_num_cpu_core(void);
int xutil_get_num_cpu_core_avail(void);
int xutil_get_space_info(char *path, xspace_info *space);
int xutil_get_memory_info(xmemory_info *info);

int xutil_change_permission(char *path, xutil_perm perm);
int xutil_create_directory(char *path);
int xutil_create_file(char *path);

int xutil_delete_directory(char *path);
int xutil_delete_file(char *path);

int xutil_read_file(char *path, char **buf);
int xutil_write_file(char *path, char *contents, int mode);

int xutil_copy_file(char *source, char *dest);
int xutil_move_file(char *source, char *dest);

int xutil_is_debugger_attached(void);

int xutil_restart(void);
int xutil_poweroff(void);

int xutil_get_thread_stack_size(size_t *stack_size);
unsigned long int xutil_get_thread_id(void);
void xutil_init_threads(void);
void xutil_deinit_threads(void);

#endif /* XUTIL_H */

#if defined XUTIL_IMPLEMENTATION

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#ifdef XUTIL_UNIX
#include <pwd.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <linux/reboot.h>
#include <sys/syscall.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#ifdef XUTIL_WINDOWS
#include <windows.h>

#ifndef XUTIL_WINDOWS_DISABLE_PRAGMA_LINK
#pragma comment(lib, "user32.lib")

/* IF xutil_is_root IS NOT CALLED DO NOT DISABLE DEFINE THIS AS IT IS NOT USED IN OTHER FUNCTIONS */
#if defined(XUTIL_WINDOWS_ENABLE_LINK_ADVAPI)
#pragma comment(lib, "Advapi32.lib")
#endif

#endif /* ifndef XUTIL_WINDOWS_DISABLE_LINK_ADVAPI */
#endif /* XUTIL_WINDOWS*/

/**
 * This function should be called as the first thing before calling any other
 * function from xutil.h, altough not all functions are dependening on config
 * from this initilization some do and if this is not called program will
 * probably segfault.
 */
void xutil_init_config(void)
{
	xutil_config.print_error_fd = stderr;
}

/**
 * Change the global config for xutil_print_error messages's file descriptor used to print
 *
 * @param {FILE*} fd: File descriptor to print error messages,
 * set it either to stderr or stdout
 */
void xutil_config_change_print_err_fd(FILE *fd)
{
	xutil_config.print_error_fd = fd;
}

/**
 * Since different operating systems return different error codes it is not
 * possible to sort them properly, that's why this function is used whenever
 * errno or GetLastError(void) is set. The error code itself doesn't have any
 * use so use xutil_print_error(int errcode) or xutil_write_error(int errcode, char **buf)
 * to get a meaningfull error messaage.
 *
 * @return {int}: error code
 */
int xutil_get_last_error()
{
#if defined XUTIL_WINDOWS
	return GetLastError();
#elif defined XUTIL_UNIX
	return errno;
#endif
}

/**
 * Print errorcode to fd.
 *
 * @param {FILE*} fd: File descriptor to print output, should either be passed stdout or stderr
 * @apram {int} errcode: Error code retrieved from xutil_get_last_error.
 */
void xutil_print_error(int errcode)
{
#if defined XUTIL_UNIX
	fprintf(xutil_config.print_error_fd, "Error: %s\n", strerror(errno));
#elif defined XUTIL_WINDOWS
	LPSTR errormsg;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		       NULL, errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errormsg, 0, NULL);
	fprintf(xutil_config.print_error_fd, "Error: %s\n", errormsg);
	LocalFree(errormsg);
#endif
}

/**
 * Print errorcode retrieved from xutil_get_last error to fd.
 *
 * @param {FILE*} fd: File descriptor to print output, should either be passed stdout or stderr
 * @apram {int} errcode: Error code retrieved from xutil_get_last_error.
 */
void xutil_print_last_error()
{
#if defined XUTIL_UNIX
	fprintf(xutil_config.print_error_fd, "Error: %s\n", strerror(xutil_get_last_error()));
#elif defined XUTIL_WINDOWS
	LPSTR errormsg;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		       NULL, xutil_get_last_error(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errormsg, 0, NULL);
	fprintf(xutil_config.print_error_fd, "Error: %s\n", errormsg);
	LocalFree(errormsg);
#endif
}

/**
 * Write the error message into allocated string, then point buf to it.
 * The error message * is heap allocated so the programmer should free it explicitly.
 *
 * @param {int} errcode: Error code retrieved from xutil_get_last_error.
 * @param {char**} buf: Destinartion buffer to put the string.
 */
void xutil_write_error(int errcode, char **buf)
{
#if defined XUTIL_WINDOWS
	LPSTR errormsg;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		       NULL, errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errormsg, 0, NULL);
	*buf = errormsg;
#elif defined XUTIL_UNIX
	*buf = strerror(errcode);
#endif

}

/**
 * Check if the program has root or admnisitration privilages.
 *
 * FOR WINDOWS USERS DEFINE XUTIL_WINDOWS_ENABLE_LINK_ADVAPI for linking agains Advapi
 * library or XUTIL_WINDOWS_SUPRESS_LINK_ADVAPI_WARNING to supress the warning if you
 * are going to NOT link with pragma comment. Otherwise linker will fail!
 *
 * @return {enum xutil_perm}: Either XUTIL_PERM_USER or XUTIL_PERM_ROOT is returned
 */
int xutil_is_root(void)
{
#if defined XUTIL_UNIX
	if (geteuid() == 0)
		return 1;
	else
		return 0;
#elif defined XUTIL_WINDOWS
/*
 * XUTIL_WINDOWS_SUPRESS_LINK_ADVAPI_WARNING is added for those who do not want to link
 * it with pragma statements and will to on build stage
*/
#if defined(XUTIL_WINDOWS_ENABLE_LINK_ADVAPI) || defined(XUTIL_WINDOWS_SUPRESS_LINK_ADVAPI_WARNING)
	BOOL isroot = FALSE;
	HANDLE tk = NULL;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &tk))
	{
		TOKEN_ELEVATION elevation;
		DWORD size;

		if (GetTokenInformation(tk, TokenElevation, &elevation, sizeof(elevation), &size))
		{
			isroot = elevation.TokenIsElevated;
		}

		CloseHandle(tk);
	}

	return isroot;
#else
	return 0;
#endif

#endif
}

/**
 * Get number of CPU cores configured by the operating system
 */
int xutil_get_num_cpu_core(void)
{
#ifdef XUTIL_UNIX
	return get_nprocs_conf();
#elif defined XUTIL_WINDOWS
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
#endif
}

/**
 * TODO: I currently could not find the windows version of
 * get_nprocs in winapi, they seem to be the same call
 */

/**
 * Get number of CPU cores configured by the operating system NOT
 * including offline cores
 */
int xutil_get_num_cpu_core_avail(void)
{
#ifdef XUTIL_UNIX
	return get_nprocs();
#elif defined XUTIL_WINDOWS
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return sysinfo.dwNumberOfProcessors;
#endif
}

/**
 * Retrieve information about space on a disk
 *
 * @param {char*} path: Path to the disk to retrieve info
 * @param {xspace_info}: contains free, available and capacity info as size_t
 * @return {int}: On success 1 is returned, otherwise 0 is returned and errno is set
 * which can be printed with xutil_print_error(int error) or similar variants.
 */
int xutil_get_space_info(char *path, xspace_info *space)
{
#ifdef XUTIL_UNIX
	struct statvfs stat;
	statvfs(path, &stat);

	space->capacity = stat.f_frsize * stat.f_blocks;
	space->free = stat.f_frsize * stat.f_bfree;
	space->available = stat.f_frsize * stat.f_bavail;

	return 1;
#elif defined XUTIL_WINDOWS
	ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

	if (GetDiskFreeSpaceEx(path, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
	{
		space->available = (size_t)freeBytesAvailable.QuadPart;
		space->capacity = (size_t)totalNumberOfBytes.QuadPart;
		space->free = (size_t)totalNumberOfFreeBytes.QuadPart;
		return 1;
	}
	else {
		return 0;
	}
#else
	return 0;
#endif
}

/**
 * Get ram info from system
 * @param {xmemory_info}: Pointer to a struct which will receive capacity and avaliable ram in bytes
 * @return {int}: On succeed 1 is returned, otherwise 0 is returned and errno is set.
 */
int xutil_get_memory_info(xmemory_info *info)
{
#if defined XUTIL_UNIX
	long phpages = get_phys_pages();
	if (phpages == -1)
		return 0;

	long avphpages = get_avphys_pages();
	if (avphpages == -1)
		return 0;

	long pgsize = sysconf(_SC_PAGESIZE);
	if (pgsize == -1)
		return 0;

	info->capacity =  phpages * pgsize;
	info->available = avphpages * pgsize;

	return 1;
#elif defined XUTIL_WINDOWS
	MEMORYSTATUSEX memorystat;
	memorystat.dwLength = sizeof(memorystat);

	if (!GlobalMemoryStatusEx(&memorystat))
		return 0;

	info->capacity = (size_t)memorystat.ullTotalPhys;
	info->available = (size_t)memorystat.ullAvailPhys;
	return 1;
#endif
}

/**
 * Change the permission of a file or directory
 *
 * @param {char*} path: Path to the file or directory to change permissions
 * @param {xutil_perm} perm: Enum that will indicate the permission level to be set
 * @return {int}: On success 1 is returned, else errno * is returned which can be
 * printed with xutil_print_error() * or similar variants.
 */
int xutil_change_permission(char *path, xutil_perm perm)
{
#if defined XUTIL_UNIX
	int err;
	if (perm == XUTIL_PERM_USER) {
		const char* username = getlogin();
		if (username == NULL)
			return errno;

		struct passwd *pw = getpwnam(username);
		if (pw == NULL)
			return errno;

		uid_t userid = pw->pw_uid;

		err = chown(path, userid, userid);
		if (err == -1)
			return errno;
		return 1;
	}
	else if (perm == XUTIL_PERM_ROOT) {
		err = chown(path, 0, 0);
		if (err == -1) {
			rmdir(path);
			return errno;
		}
		else {
			return 1;
		}
	}
	else if (perm == XUTIL_PERM_ONLY_OWNER_AND_ROOT){
		err = chmod(path, 700);
		if (err == -1)
			return errno;
		return 1;
	}

	return 1;
#elif defined XUTIL_WINDOWS
	/* UNIMPLEMENTED UNTILL I FIGURE OUT A WAY TO USE WINAPI*/
	return 0;
#endif
}


/**
 * Create directory with given permissions
 *
 * @return {int}: Errno is returned which can be printed with
 * xutil_print_error(int error) or similar variants.
 */
int xutil_create_directory(char *path)
{
#ifdef XUTIL_UNIX
	struct stat st = {0};

	if (stat(path, &st) == -1) {
		mkdir(path, 0755);
		return 1;
	}
	else {
		return errno;
	}
#elif defined XUTIL_WINDOWS
	BOOL result = CreateDirectoryA(path, NULL);
	return !!result;
#endif
}

/**
 * Create directory
 *
 * @param {char*} path: Path to create the directory, name of the directory is included to path
 * @return {int}: On success 1 is returned, otherwise errno is returned which can be printed with
 * xutil_print_error(int error) or similar variants.
 */
int xutil_create_file(char *path)
{
#ifdef XUTIL_UNIX
	int fd = open(path, O_CREAT | S_IRUSR | S_IWUSR);
	if (fd == -1)
		return errno;

	int err = close(fd);
	if (err == -1)
		return errno;

	return 1;
#elif defined XUTIL_WINDOWS
	HANDLE fd = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fd != INVALID_HANDLE_VALUE) {
		CloseHandle(fd);
		return 1;
	}
	return 0;
#endif
}

/**
 * Delete directory
 *
 * @param {char*} path: Path to the directory to be deleted
 * @return {int}: 1 is returned on success, otherwise errno is returned which can be printed with
 * xutil_print_error(int error) or similar variants.
 */
int xutil_delete_directory(char *path)
{
#if defined XUTIL_UNIX
	if (rmdir(path) == -1)
		return errno;
	return 1;
#elif defined XUTIL_WINDOWS
	BOOL result = RemoveDirectoryA(path);
	return !!result;
#endif
}

/**
 * Delete file
 *
 * @param {char*} path: Path to the file to be deleted
 * @return {int}: 1 is returned on success, otherwise errno is returned which can be printed with
 * xutil_print_error(int error) or similar variants.
 */
int xutil_delete_file(char *path)
{
#if defined XUTIL_UNIX
	if (remove(path) == -1)
		return errno;
	return 1;
#elif defined XUTIL_WINDOWS
	BOOL result = DeleteFileA(path);
	return !!result;
#endif
}

/**
 * Read file into buffer
 *
 * @param {char*} path: Path to the file to be read
 * @param {char**} buf: Double pointer to destination for contents
 * @return if function succeedes it returns 1, otherwise one of
 *	XUTIL_ERROR_GETTING_FILE_STATUS
 *	XUTIL_ERROR_GETTING_FILE_STATUS
 *	XUTIL_ERROR_ALLOCATING_SPACE
 *	XUTIL_ERROR_READING_FILE
 * error codes are returned.
 */
int xutil_read_file(char *path, char **buf)
{
#if defined XUTIL_UNIX
	int fd = open(path, O_RDONLY);
	if (fd == -1)
		return XUTIL_ERROR_OPENING_FILE;

	struct stat file_stat;
	if (fstat(fd, &file_stat) == -1) {
		close(fd);
		return XUTIL_ERROR_GETTING_FILE_STATUS;
	}

	off_t file_size = file_stat.st_size;

	char *buffer = (char *)malloc(file_size);
	if (buffer == NULL) {
		close(fd);
		return XUTIL_ERROR_ALLOCATING_SPACE;
	}

	ssize_t bytes_read = read(fd, buffer, file_size);
	if (bytes_read == -1) {
		close(fd);
		free(buffer);
		return XUTIL_ERROR_READING_FILE;
	}

	*buf = buffer;
	close(fd);
	return 1;
#elif defined XUTIL_WINDOWS
	HANDLE fd = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (fd == INVALID_HANDLE_VALUE) {
		return 0;
	}

	DWORD fileSize = GetFileSize(fd, NULL);
	if (fileSize == INVALID_FILE_SIZE) {
		CloseHandle(fd);
		return 0;
	}

	BYTE* buffer = (BYTE*)malloc(fileSize);
	if (buffer == NULL) {
		CloseHandle(fd);
		return 0;
	}

	DWORD bytesRead;
	BOOL result = ReadFile(fd, buffer, fileSize, &bytesRead, NULL);
	if (result == FALSE || bytesRead != fileSize) {
		free(buffer);
		CloseHandle(fd);
		return 0;
	}

	*buf = buffer;
	CloseHandle(fd);
	return 1;
#endif
}

int xutil_write_file(char* path, char* contents, int mode)
{

}

/**
 * Copy file from source to destination
 *
 * @param {char*} source: Path to the source file to be copied
 * @param {char*} dest: Destination to copy to
 * @return {int}: On success 1 is returned, on failure 0 is returned and
 * errorno is set.
 */
int xutil_copy_file(char* source, char* dest)
{
#if defined XUTIL_WINDOWS
	BOOL result = CopyFile(source, dest, FALSE);
	return !!result;
#elif defined XUTIL_UNIX
#endif
}

/**
 * Move file from source to desitnation
 *
 * @param {char*} source: Source path to file to be moved
 * @param {char*} dest: Destination path to move the file to
 * @return {int}: On success 1 is returned, on failure 0 is returned and
 * errorno is set.
 */
int xutil_move_file(char* source, char* dest)
{
#if defined XUTIL_WINDOWS
	BOOL result = MoveFile(source, dest);
	return !!result;
#elif defined XUTIL_UNIX
#endif
}

int xutil_is_debugger_attached(void)
{
#if defined XUTIL_UNIX
	char buf[4096];

	const int status_fd = open("/proc/self/status", O_RDONLY);
	if (status_fd == -1)
		return 0;

	const ssize_t num_read = read(status_fd, buf, sizeof(buf) - 1);
	close(status_fd);

	if (num_read <= 0)
		return 0;

	buf[num_read] = '\0';
	char tracer_pid_str[] = "TracerPid:";
	char* tracer_pid = strstr(buf, tracer_pid_str);
	if (!tracer_pid)
		return 0;

	for (char* characterPtr = tracer_pid + sizeof(tracer_pid_str) - 1; characterPtr <= buf + num_read; ++characterPtr) {
		if (isspace(*characterPtr))
			continue;
		else
			return isdigit(*characterPtr) != 0 && *characterPtr != '0';
	}

	return 0;
#elif defined XUTIL_WINDOWS
	return IsDebuggerPresent();

#endif
}

int xutil_restart(void)
{
#if defined XUTIL_UNIX
	if (reboot(RB_AUTOBOOT) == -0)
		return 1;
	else
		return xutil_get_last_error();
#elif defined XUTIL_WINDOWS

	BOOL result = ExitWindowsEx(EWX_REBOOT,  SHTDN_REASON_MINOR_OTHER);
	return !!result;
#endif
}

/**
 * Log out, power off the systme
 *
 * @return {int}: 1 is returned on success, else value from xutil_get_last_error(void) is
 * returned.
 */
int xutil_poweroff(void)
{
#if defined XUTIL_UNIX
	if(reboot(RB_POWER_OFF) == 0)
		return 1;
	else
		return xutil_get_last_error();
#elif defined XUTIL_WINDOWS
	BOOL result = ExitWindowsEx(EWX_SHUTDOWN, SHTDN_REASON_MINOR_OTHER);
	return !!result;
#endif
}

#if defined XUTIL_UNIX
static pthread_attr_t attr;
#endif

unsigned long int xutil_get_thread_id(void)
{
#ifdef XUTIL_WINDOWS
	return GetCurrentThreadId();
#elif defined XUTIL_UNIX
	return pthread_self();
#endif
}

/**
 * UNIX ONLY FUNCTION! This function does not exits on winapi, it states that:
 * The default size for the reserved and initially committed stack memory is specified in the executable file header.
 * As far as I can tell there is no way to get the default stack size for a thread, that is what you define it to be
 * in the beggining, so on windows this function will allways return 0 and fail.
 *
 * @param {size_t*} stack_size: Pointer to data that will recieve stack size in bytes
 * @return {int}: On success 1 is returned, otherwise 0 is returned
 *
*/
int xutil_get_thread_stack_size(size_t *stack_size)
{
#if defined XUTIL_UNIX
	size_t tmp;
	if (pthread_attr_getstacksize(&attr, &tmp) == 0) {
		*stack_size = tmp;
		return 1;
	}
	return 0;
#elif defined XUTIL_WINDOWS
	return 0;
#endif
}

/**
 * This function should be called before creating any threads.
 */
void xutil_init_threads(void)
{
#if defined XUTIL_UNIX
	size_t stacksize = 8388608;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setstacksize(&attr, stacksize);

#elif defined XUTIL_WINDOWS
#endif
}

/**
 * This function shoul be called to after the programmer created all the threads.
 * It deallocates thread attributes. If further thread creation is possible the
 * programmer should initialize the threads again with xutil_init_threads
 */
void xutil_deinit_threads(void)
{
#if defined XUTIL_UNIX
	pthread_attr_destroy(&attr);
#elif defined XUTIL_WINDOWS
#endif
}

#endif /* XUTIL_IMPLEMENTATION */
