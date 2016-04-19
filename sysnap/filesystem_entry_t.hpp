#include <pugixml.hpp>
#include <vector>
#include <ctime>
#include <iostream>
#include "path_t.hpp"

#ifndef SYSNAP_FILESYSTEM_ENTRY_HPP
#define SYSNAP_FILESYSTEM_ENTRY_HPP

namespace sysnap {
	struct Timestamp_t {
		int		day;
		int		month;
		int		year;
		int		hour;
		int		minute;
		int		second;
		char*	zone;
	};
	Timestamp_t GetLocalTime();
	std::string	GetTimeString(Timestamp_t _local_time);

	struct PermissionsFlag_t {
		int owner;
		int group;
		int others;
	};
	PermissionsFlag_t	GetPermissionsFlag(int _perms_flag);
	std::string			GetPermissionsString(PermissionsFlag_t _perms_flag);

	enum UNIX_FILE_t {
		REGULAR_FILE = 0,
		DIRECTORY,
		SYMLINK,
		NAMED_PIPE,
		SOCKET,
		CHAR_BLOCK_DEVICE
	};
	std::string	GetFileTypeString(UNIX_FILE_t _file_type);

	class FileSystemEntry_t {
	private:
		std::string 					name_m;
		Path_t							path_m;
		Timestamp_t						date_created_m;
		Timestamp_t						date_modified_m;
		PermissionsFlag_t				permissions_m;
		std::string						owner_m;
		std::string						group_m;
		unsigned long					size_m;
		UNIX_FILE_t						file_type_m;
		std::vector<FileSystemEntry_t>	content_m;

	public:
		FileSystemEntry_t();
		~FileSystemEntry_t() {}

		/*---> SETTERS <---*/
		void Name(std::string _name);
		void Path(std::string _path);
		void Path(Path_t _path);
		void DateCreated(Timestamp_t _date_created);
		void DateCreated(int _day, int _month, int _year,
						 int _hour, int _minute, int _second,
						 char* _zone);
		void DateModified(Timestamp_t _date_modified);
		void DateModified(int _day, int _month, int _year,
						  int _hour, int _minute, int _second,
						  char* _zone);
		void Permissions(PermissionsFlag_t _permissions);
		void Permissions(int _permissions);
		void Permissions(int _owner, int _group, int _others);
		void Owner(std::string _owner);
		void Group(std::string _group);
		void Size(unsigned long _size);
		void FileType(UNIX_FILE_t _file_type);
		void InsertContent(FileSystemEntry_t _fs_entry);

		/*---> GETTERS <---*/
		std::string						Name();
		Path_t							Path();
		Timestamp_t						DateCreated();
		Timestamp_t						DateModified();
		PermissionsFlag_t				Permissions();
		std::string						Owner();
		std::string						Group();
		unsigned long					Size();
		UNIX_FILE_t						FileType();
		std::vector<FileSystemEntry_t>	Content();
		FileSystemEntry_t*				Find(std::string);
		bool							Empty();

		/*---> OPERATORS <---*/
		FileSystemEntry_t operator[](int _index);
		FileSystemEntry_t& operator[](std::string _name);
	};
}


#endif
