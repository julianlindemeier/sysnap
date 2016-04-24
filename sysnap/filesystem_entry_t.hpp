#include <pugixml.hpp>
#include <vector>
#include <ctime>
#include <iostream>
#include "path_t.hpp"
#include "auxilliary.hpp"

#ifndef SYSNAP_FILESYSTEM_ENTRY_HPP
#define SYSNAP_FILESYSTEM_ENTRY_HPP

namespace sysnap {
	class FileSystemEntry_t {
	private:
		unsigned long					iNode_m;
		std::string 					name_m;
		Path_t							path_m;
		Timestamp_t						date_modified_m;
		PermissionsFlag_t				permissions_m;
		std::string						owner_m;
		std::string						group_m;
		unsigned long					size_m;
		UNIX_FILE_t						file_type_m;
		std::vector<FileSystemEntry_t*>	content_m;

	public:
		FileSystemEntry_t();
		~FileSystemEntry_t();

		/*---> SETTERS <---*/
		void iNode(unsigned long _iNode);
		void Name(std::string _name);
		void Path(std::string _path);
		void Path(Path_t _path);
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
		void InsertContent(FileSystemEntry_t* _fs_entry);

		/*---> GETTERS <---*/
		unsigned long					iNode();
		std::string						Name();
		Path_t							Path();
		Timestamp_t						DateModified();
		PermissionsFlag_t				Permissions();
		std::string						Owner();
		std::string						Group();
		unsigned long					Size();
		UNIX_FILE_t						FileType();
		std::vector<FileSystemEntry_t*>	Content();
		FileSystemEntry_t*				Find(std::string _name);
		bool							Empty();

		/*---> OPERATORS <---*/
		FileSystemEntry_t& operator[](int _index);
		FileSystemEntry_t& operator[](std::string _name);

	//TODO: Change to private:
	public:
		bool operator<(FileSystemEntry_t& _fs) {return (this->iNode_m < _fs.iNode_m);}
		bool operator>(FileSystemEntry_t& _fs) {return (this->iNode_m > _fs.iNode_m);}
	};
}


#endif
