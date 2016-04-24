#include "auxilliary.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include "filesystem_entry_t.hpp"

namespace sysnap {
	/* * * * * * * * * * *
	 * Parsing Arguments *
	 * * * * * * * * * * */
	std::vector< std::pair<std::string, std::string> > ParseArguments(const char *argv[], int len_args_arr) {
		std::vector<std::string> args;
		std::vector< std::pair<std::string, std::string> > args_table;
		int counter = 0;

		//Copy arguments into args-vector:
		for(int i=1; i < len_args_arr; i++) {
			args.push_back(argv[i]);
		}

		for(std::vector<std::string>::iterator iter = args.begin();
		iter != args.end();
		iter++, counter++) {
			if(iter[0][0] == '-') {
				//check if there are more elements within args than the current one.
				if(counter+1 < args.size()) {
					//make sure the next element does NOT begin with a hyphen.
					if(iter[1][0] != '-') {
						//-<key> <value>
						std::pair<std::string, std::string> args_table_entry;

						args_table_entry.first  = iter[0].substr(1);
						args_table_entry.second = iter[1];

						args_table.push_back(args_table_entry);
						iter++;	counter++;

						continue;	//so the -<key> case is not executed!
					} else { }
				} else { }
				//-<key>
				std::pair<std::string, std::string> args_table_entry;

				args_table_entry.first  = iter[0].substr(1);
				args_table_entry.second = "";

				args_table.push_back(args_table_entry);
			} else {
				//<value>
				std::pair<std::string, std::string> args_table_entry;

				args_table_entry.first  = "";
				args_table_entry.second = iter[0];

				args_table.push_back(args_table_entry);
			}
		}

		return args_table;
	}
	std::string GetCWD() {
		std::string ret_string;

		char* path_tmp = new char[FILENAME_MAX];

		if(!getcwd(path_tmp, FILENAME_MAX)) {
			std::cerr << "Error.\n";
			exit(1);
		}

		ret_string = path_tmp;

		delete[] path_tmp;
		return ret_string;
	}
	/* * * * * * * *
	 * Math Stuff  *
	 * * * * * * * */
	int dec2oct(int _dec) {
		int oct = 0, digit = 0;

		while(_dec > 0) {
			oct += (_dec % 8) * pow(10, digit++);

			_dec /= 8;
		}

		return oct;
	}

	/* * * * * * * *
	 * Timestamp_t *
	 * * * * * * * */
	bool Timestamp_t::operator==(const Timestamp_t& _time) {
		if(this->day	== _time.day
		&& this->month	== _time.month
		&& this->year	== _time.year
		&& this->hour 	== _time.hour
		&& this->minute == _time.minute
		&& this->second	== _time.second) {
			return true;
		}
		return false;
	}
	bool Timestamp_t::operator!=(const Timestamp_t& _time) {
		if(this->day	!= _time.day
		|| this->month	!= _time.month
		|| this->year	!= _time.year
		|| this->hour 	!= _time.hour
		|| this->minute != _time.minute
		|| this->second	!= _time.second) {
			return true;
		}
		return false;
	}
	bool Timestamp_t::operator<(const Timestamp_t& _time) {
		if(this->year < _time.year) {
			return true;
		} else {
			if(this->month < _time.month) {
				return true;
			} else {
				if(this->day < _time.day) {
					return true;
				} else {
					if(this->hour < _time.hour) {
						return true;
					} else {
						if(this->minute < _time.minute) {
							return true;
						} else {
							if(this->second < _time.second) {
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}
	bool Timestamp_t::operator>(const Timestamp_t& _time) {
		if(this->year > _time.year) {
			return true;
		} else {
			if(this->month > _time.month) {
				return true;
			} else {
				if(this->day > _time.day) {
					return true;
				} else {
					if(this->hour > _time.hour) {
						return true;
					} else {
						if(this->minute > _time.minute) {
							return true;
						} else {
							if(this->second > _time.second) {
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}
	Timestamp_t GetLocalTime(std::time_t _time) {
		if(_time == 0) {
			_time = std::time(NULL);
		}
		std::tm*	local_time;
		Timestamp_t	ret_time;

		local_time = std::localtime(&_time);

		ret_time.day	= local_time->tm_mday;
		ret_time.month	= local_time->tm_mon;
		ret_time.year	= local_time->tm_year + 1900;
		ret_time.hour	= local_time->tm_hour;
		ret_time.minute	= local_time->tm_min;
		ret_time.second	= local_time->tm_sec;
		ret_time.zone	= local_time->tm_zone;

		return ret_time;
	}
	std::string	GetTimeString(Timestamp_t _local_time) {
		std::string ret_local_time_string;

		ret_local_time_string =  ((std::to_string(_local_time.day).size()    == 2) ? std::to_string(_local_time.day)    : "0" + std::to_string(_local_time.day))    + ".";
		ret_local_time_string += ((std::to_string(_local_time.month).size()  == 2) ? std::to_string(_local_time.month)  : "0" + std::to_string(_local_time.month))  + ".";
		ret_local_time_string +=   std::to_string(_local_time.year) + " ";
		ret_local_time_string += ((std::to_string(_local_time.hour).size()   == 2) ? std::to_string(_local_time.hour)   : "0" + std::to_string(_local_time.hour))   + ":";
		ret_local_time_string += ((std::to_string(_local_time.minute).size() == 2) ? std::to_string(_local_time.minute) : "0" + std::to_string(_local_time.minute)) + ":";
		ret_local_time_string += ((std::to_string(_local_time.second).size() == 2) ? std::to_string(_local_time.second) : "0" + std::to_string(_local_time.second)) + " ";
		ret_local_time_string += "(" + std::string(_local_time.zone) + ")";

		return ret_local_time_string;
	}

	/* * * * * * * * * * *
	 * PermissionsFlag_t *
	 * * * * * * * * * * */
	PermissionsFlag_t	GetPermissionsFlag(int _perms_flag) {
		PermissionsFlag_t ret_perm_flags;

		ret_perm_flags.owner	= (_perms_flag / 100) % 10;
		ret_perm_flags.group	= (_perms_flag /  10) % 10;
		ret_perm_flags.others	= (_perms_flag /   1) % 10;


		return ret_perm_flags;
	}
	bool PermissionsFlag_t::operator==(const PermissionsFlag_t& _perms) {
		if(this->owner	== _perms.owner
		&& this->group	== _perms.group
		&& this->others	== _perms.others) {
			return true;
		}

		return false;
	}
	bool PermissionsFlag_t::operator!=(const PermissionsFlag_t& _perms) {
		if(this->owner	!= _perms.owner
		|| this->group	!= _perms.group
		|| this->others	!= _perms.others) {
			return true;
		}

		return false;
	}
	std::string			GetPermissionsString(PermissionsFlag_t _perms_flag) {
		std::string ret_perms_string;

		switch(_perms_flag.owner) {
			case 0:
				ret_perms_string += "---";
				break;
			case 1:
				ret_perms_string += "--x";
				break;
			case 2:
				ret_perms_string += "-w-";
				break;
			case 3:
				ret_perms_string += "-wx";
				break;
			case 4:
				ret_perms_string += "r--";
				break;
			case 5:
				ret_perms_string += "r-x";
				break;
			case 6:
				ret_perms_string += "rw-";
				break;
			case 7:
				ret_perms_string += "rwx";
				break;
		}

		switch(_perms_flag.group) {
			case 0:
				ret_perms_string += "---";
				break;
			case 1:
				ret_perms_string += "--x";
				break;
			case 2:
				ret_perms_string += "-w-";
				break;
			case 3:
				ret_perms_string += "-wx";
				break;
			case 4:
				ret_perms_string += "r--";
				break;
			case 5:
				ret_perms_string += "r-x";
				break;
			case 6:
				ret_perms_string += "rw-";
				break;
			case 7:
				ret_perms_string += "rwx";
				break;
		}

		switch(_perms_flag.others) {
			case 0:
				ret_perms_string += "---";
				break;
			case 1:
				ret_perms_string += "--x";
				break;
			case 2:
				ret_perms_string += "-w-";
				break;
			case 3:
				ret_perms_string += "-wx";
				break;
			case 4:
				ret_perms_string += "r--";
				break;
			case 5:
				ret_perms_string += "r-x";
				break;
			case 6:
				ret_perms_string += "rw-";
				break;
			case 7:
				ret_perms_string += "rwx";
				break;
		}

		return ret_perms_string;
	}

	/* * * * * * * *
	 * UNIX_FILE_t *
	 * * * * * * * */
 	UNIX_FILE_t	GetFileType(boost::filesystem::path _path) {
		UNIX_FILE_t 	ret_file_type;

		if(boost::filesystem::is_directory(_path)) {
			ret_file_type = DIRECTORY;
		}
		if(boost::filesystem::is_regular_file(_path)) {
			ret_file_type = REGULAR_FILE;
		}
		if(boost::filesystem::is_symlink(_path)) {
			ret_file_type = SYMLINK;
		}
		if(boost::filesystem::is_other(_path)) {
			ret_file_type = UNKNOWN;
		}
/*
		switch(_boost_file_type) {
			case boost::filesystem::regular_file:
				ret_file_type = REGULAR_FILE;
				break;
			case boost::filesystem::directory_file:
				ret_file_type = DIRECTORY;
				break;
			case boost::filesystem::symlink_file:
				ret_file_type = SYMLINK;
				break;
			case boost::filesystem::block_file:
				ret_file_type = BLOCK_DEVICE;
				break;
			case boost::filesystem::character_file:
				ret_file_type = CHARACTER_DEVICE;
				break;
			case boost::filesystem::fifo_file:
				ret_file_type = FIFO;
				break;
			case boost::filesystem::socket_file:
				ret_file_type = SOCKET;
				break;
			default:
				ret_file_type = UNKNOWN;
				break;
		}
*/
		return ret_file_type;
	}

	std::string	GetFileTypeString(UNIX_FILE_t _file_type) {
		std::string ret_file_type_string;

		switch(_file_type) {
			case REGULAR_FILE:
				ret_file_type_string = "Regular File";
				break;
			case DIRECTORY:
				ret_file_type_string = "Directory";
				break;
			case SYMLINK:
				ret_file_type_string = "Symlink";
				break;
			case BLOCK_CHAR_DEVICE:
				ret_file_type_string = "Block/Character Device";
				break;
			case FIFO:
				ret_file_type_string = "FIFO File";
				break;
			case SOCKET:
				ret_file_type_string = "Socket";
				break;
			default:
				ret_file_type_string = "Unknown File Type";
				break;
		}

		return ret_file_type_string;
	}

	/* * * * * * * * * * * * *
	 * Not covered by boost  *
	 * * * * * * * * * * * * */
	std::string GetOwner(boost::filesystem::path _path) {
		struct ::stat 			info;
		::stat(_path.string().c_str(), &info);
		struct ::passwd *pw = getpwuid(info.st_uid);

		return std::string(pw->pw_name);
	}
	std::string GetGroup(boost::filesystem::path _path) {
		struct ::stat 			info;
		::stat(_path.string().c_str(), &info);

		struct ::group  *gr = getgrgid(info.st_gid);

		return std::string(gr->gr_name);
	}
	unsigned long GetSize(boost::filesystem::path _path) {
		unsigned long ret_size = 0;

		if(boost::filesystem::is_regular_file(_path)) {
			ret_size = boost::filesystem::file_size(_path);
		}
		if(boost::filesystem::is_symlink(_path)) {
			ret_size = boost::filesystem::read_symlink(_path).size();
		}
		if( boost::filesystem::is_directory(_path)
		&& !boost::filesystem::is_other(_path)) {
    		for(boost::filesystem::recursive_directory_iterator rcrsv_iter(_path);
        	rcrsv_iter != boost::filesystem::recursive_directory_iterator();
        	++rcrsv_iter) {
				if(boost::filesystem::is_symlink(rcrsv_iter->path())) {
					ret_size += boost::filesystem::read_symlink(rcrsv_iter->path()).size();
					continue;
				}
        		if(boost::filesystem::is_regular_file(rcrsv_iter->path())) {
            		ret_size += boost::filesystem::file_size(*rcrsv_iter);
				}
    		}
		}

		return ret_size;
	}

	unsigned long GetiNode(boost::filesystem::path _path) {
		struct ::stat info;
		::stat(_path.string().c_str(), &info);

		return info.st_ino;
	}

	/* * * * * * * * *
	 * Pretty Output *
	 * * * * * * * * */
	std::string ByteSuffix(float _num) {
		int suffix = 0;
		while(_num > 1023.0f) {
			_num /= 1024.0f;
			suffix++;
		}

		std::string ret_string = std::to_string(_num);

		switch(suffix) {
			case 1:
				ret_string += " K";
				break;
			case 2:
				ret_string += " M";
				break;
			case 3:
				ret_string += " G";
				break;
			case 4:
				ret_string += " T";
				break;
			default:
				ret_string += " ";
				break;
		}

		ret_string += "Bytes";

		return ret_string;
	}
}

#ifdef DEBUG
	std::cout << std::setw(10) << "argument" << std::setw(10) << "|" << std::setw(10) << "value\n";
	std::cout << std::setfill('-') << std::setw(39) << "\n" << std::setfill(' ');
	for(int i=0; i < args_table.size(); i++) {
		std::cout << std::setw(10) << (!args_table.at(i).first.empty()? args_table.at(i).first : "-") << std::setw(10) << "|"
				  << std::setw(10) << (!args_table.at(i).second.empty()? args_table.at(i).second : "-") << "\n";
	}
#endif
