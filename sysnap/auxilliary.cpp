#include "auxilliary.hpp"
#include <iostream>
#include <vector>
#include "filesystem_entry_t.hpp"

namespace sysnap {
	/* * * * * * * * * * *
	 * Parsing Arguments *
	 * * * * * * * * * * */
	std::vector< std::pair<std::string, std::string> > parse_args(std::vector<std::string> _args) {
		std::vector< std::pair<std::string, std::string> > args_table;
		int counter = 0;
		for(std::vector<std::string>::iterator iter = _args.begin();
		iter != _args.end();
		iter++, counter++) {
			if(iter[0][0] == '-') {
				//check if there are more elements within args than the current one.
				if(counter+1 < _args.size()) {
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

	/* * * * * * * *
	 * Timestamp_t *
	 * * * * * * * */
	Timestamp_t GetLocalTime() {
		std::time_t	t;
		std::tm*	local_time;
		Timestamp_t	ret_time;

		t = std::time(NULL);
		local_time = std::localtime(&t);

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

		ret_local_time_string = std::to_string(_local_time.day)  + "." + std::to_string(_local_time.month)  + "." + std::to_string(_local_time.year) + " "
							  + std::to_string(_local_time.hour) + ":" + std::to_string(_local_time.minute) + ":" + std::to_string(_local_time.second) + " ("
							  + _local_time.zone + ")";

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
			case NAMED_PIPE:
				ret_file_type_string = "Named Pipe";
				break;
			case SOCKET:
				ret_file_type_string = "Socket";
				break;
			case CHAR_BLOCK_DEVICE:
				ret_file_type_string = "Character/Block Device";
				break;
			default:
				ret_file_type_string = "Unknown File Type";
				break;
		}

		return ret_file_type_string;
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
