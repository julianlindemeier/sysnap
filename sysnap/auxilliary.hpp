#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef SYSNAP_AUXILLIARY_HPP
#define SYSNAP_AUXILLIARY_HPP

namespace sysnap {
	/* * * * * * * * * * *
	 * Parsing Arguments *
	 * * * * * * * * * * */
	std::vector< std::pair<std::string, std::string> > ParseArguments(const char *argc[], int len_args_arr);
	std::string	GetCWD();

	/* * * * * * * *
	 * Math Stuff  *
	 * * * * * * * */
	int dec2oct(int _dec);

	/* * * * * * * *
	 * Timestamp_t *
	 * * * * * * * */
	struct Timestamp_t {
		int		day;
		int		month;
		int		year;
		int		hour;
		int		minute;
		int		second;
		char*	zone;
	};
	Timestamp_t GetLocalTime(std::time_t _time=0);
	std::string	GetTimeString(Timestamp_t _local_time);

	/* * * * * * * * * * *
	 * PermissionsFlag_t *
	 * * * * * * * * * * */
	struct PermissionsFlag_t {
		int owner;
		int group;
		int others;
	};
	PermissionsFlag_t	GetPermissionsFlag(int _perms_flag);
	std::string			GetPermissionsString(PermissionsFlag_t _perms_flag);

	/* * * * * * * *
	 * UNIX_FILE_t *
	 * * * * * * * */
	enum UNIX_FILE_t {
		REGULAR_FILE = 0,
		DIRECTORY,
		SYMLINK,
		BLOCK_CHAR_DEVICE,
		FIFO,
		SOCKET,
		UNKNOWN
	};
	UNIX_FILE_t	GetFileType(boost::filesystem::path _path);
	std::string	GetFileTypeString(UNIX_FILE_t _file_type);

	/* * * * * * * * * * * * *
	 * Not covered by boost  *
	 * * * * * * * * * * * * */
	//Boost does not yet support to acquire Owner/Group information, therefore fall back to the operating system:
	std::string GetOwner(boost::filesystem::path _path);
	std::string GetGroup(boost::filesystem::path _path);
	//Furthermore the size of a directory cannot easily be determined:
	unsigned long GetSize(boost::filesystem::path _path);

	/* * * * * * * * *
	 * Pretty Output *
	 * * * * * * * * */
	std::string ByteSuffix(float _num);
}

#endif
