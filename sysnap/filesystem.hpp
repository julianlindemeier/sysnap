#include <pugixml.hpp>
#include <vector>
#include <ctime>
#include <iostream>

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

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

	struct PermissionsFlag_t {
		int owner;
		int group;
		int others;
	};
	PermissionsFlag_t GetPermissionsFlag(int _perms_flag);

	enum UNIX_FILE_t {
		REGULAR_FILE = 0,
		DIRECTORY,
		SYMLINK,
		NAMED_PIPE,
		SOCKET,
		CHAR_BLOCK_DEVICE
	};

	class Path {
	private:
		std::vector<std::string> path_m;

	public:
		Path();
		Path(std::string _path) {
			this->path_m = this->_DecomposePath_(_path);
		}
		~Path() {}

		void Print() {
			for(std::vector<std::string>::iterator iter = this->path_m.begin();
			iter != this->path_m.end();
			iter++) {
				std::cout << iter->data() << "\n";
			}
		}

	private:
		std::vector<std::string> _DecomposePath_(std::string _path) {
			std::vector<std::string> ret_path_decomposed;

			//Put string in proper form to analyze, in case it's needed:
			if(_path[0] == '/') {
				_path = _path.substr(1);
			}

			if(_path[_path.size()-1] != '/') {
				_path += '/';
			}

			size_t pos = 0;
			std::string token;
			while ((pos = _path.find('/')) != std::string::npos) {
    			token = _path.substr(0, pos);
				ret_path_decomposed.push_back(token);
    			_path.erase(0, pos + 1);
			}

			return ret_path_decomposed;
		}
	};

	class FileSystemEntry_t {
	private:
		std::string 					name_m;
		std::string						path_m;
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
		void InsertContent(FileSystemEntry_t _fs_entry, std::string _);

		/*---> GETTERS <---*/
		std::string						Name();
		std::string						Path();
		Timestamp_t						DateCreated();
		Timestamp_t						DateModified();
		PermissionsFlag_t				Permissions();
		std::string						Owner();
		std::string						Group();
		unsigned long					Size();
		UNIX_FILE_t						FileType();
		std::vector<FileSystemEntry_t>	Content();
	};

	class FileSystem_t {
	private:
		FileSystemEntry_t system_m;

	public:
		FileSystem_t();
		~FileSystem_t();

		void Scan();
		void Print();
		void ExportAsXML();
	};
}


#endif
