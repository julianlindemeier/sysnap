#include "filesystem_entry_t.hpp"

namespace sysnap {
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

	/* * * * * * * * * * *
	 * PermissionsFlag_t *
	 * * * * * * * * * * */
	PermissionsFlag_t GetPermissionsFlag(int _perms_flag) {
		PermissionsFlag_t ret_perm_flags;

		ret_perm_flags.owner	= (_perms_flag / 100) % 10;
		ret_perm_flags.group	= (_perms_flag /  10) % 10;
		ret_perm_flags.others	= (_perms_flag /   1) % 10;


		return ret_perm_flags;
	}

	/* * * * * * * * * * *
	 * FileSystemEntry_t *
	 * * * * * * * * * * */
	/*---> CONSTRUCTOR <---*/
	FileSystemEntry_t::FileSystemEntry_t() {
		this->name_m					= "";
		this->path_m					= "";

		this->date_created_m.day		= 0;
		this->date_created_m.month		= 0;
		this->date_created_m.year		= 0;
		this->date_created_m.hour		= 0;
		this->date_created_m.minute		= 0;
		this->date_created_m.second		= 0;
		this->date_created_m.zone		= NULL;

		this->date_modified_m.day		= 0;
		this->date_modified_m.month		= 0;
		this->date_modified_m.year		= 0;
		this->date_modified_m.hour		= 0;
		this->date_modified_m.minute	= 0;
		this->date_modified_m.second	= 0;
		this->date_modified_m.zone		= NULL;

		this->permissions_m.owner		= 0;
		this->permissions_m.group		= 0;
		this->permissions_m.others		= 0;

		this->owner_m					= "";
		this->group_m					= "";

		this->size_m					= 0;

		this->file_type_m				= REGULAR_FILE;

		this->content_m.clear();
	}

	/*---> SETTERS <---*/
	void FileSystemEntry_t::Name(std::string _name) {
		this->name_m = _name;
	}

	void FileSystemEntry_t::Path(std::string _path) {
		this->path_m = _path;
	}

	void FileSystemEntry_t::DateCreated(Timestamp_t _date_created) {
		this->date_created_m = _date_created;
	}

	void FileSystemEntry_t::DateCreated(int _day, int _month, int _year,
									  int _hour, int _minute, int _second,
									  char *_zone) {
		this->date_created_m.day 		= _day;
		this->date_created_m.month 		= _month;
		this->date_created_m.year 		= _year;
		this->date_created_m.hour 		= _hour;
		this->date_created_m.minute 	= _minute;
		this->date_created_m.second 	= _second;
		this->date_created_m.zone		= _zone;
	}

	void FileSystemEntry_t::DateModified(Timestamp_t _date_modified) {
		this->date_modified_m = _date_modified;
	}

	void FileSystemEntry_t::DateModified(int _day, int _month, int _year,
									   int _hour, int _minute, int _second,
									   char* _zone) {
		this->date_modified_m.day		= _day;
		this->date_modified_m.month		= _month;
		this->date_modified_m.year		= _year;
		this->date_modified_m.hour		= _hour;
		this->date_modified_m.minute	= _minute;
		this->date_modified_m.second	= _second;
		this->date_modified_m.zone		= _zone;
	}

	void FileSystemEntry_t::Permissions(PermissionsFlag_t _permissions) {
		this->permissions_m = _permissions;
	}

	void FileSystemEntry_t::Permissions(int _permissions) {
		this->permissions_m = sysnap::GetPermissionsFlag(_permissions);
	}

	void FileSystemEntry_t::Permissions(int _owner, int _group, int _others) {
		this->permissions_m.owner	= _owner;
		this->permissions_m.group	= _group;
		this->permissions_m.others	= _others;
	}

	void FileSystemEntry_t::Owner(std::string _owner) {
		this->owner_m = _owner;
	}

	void FileSystemEntry_t::Group(std::string _group) {
		this->group_m = _group;
	}

	void FileSystemEntry_t::Size(unsigned long _size) {
		this->size_m = _size;
	}

	void FileSystemEntry_t::FileType(UNIX_FILE_t _file_type) {
		this->file_type_m = _file_type;
	}


	void FileSystemEntry_t::InsertContent(FileSystemEntry_t _fs_entry) {
		this->content_m.push_back(_fs_entry);
	}

	/*---> GETTERS <---*/
	std::string						FileSystemEntry_t::Name() {
		return this->name_m;
	}

	std::string						FileSystemEntry_t::Path() {
		return this->path_m;
	}

	Timestamp_t						FileSystemEntry_t::DateCreated() {
		return this->date_created_m;
	}

	Timestamp_t						FileSystemEntry_t::DateModified() {
		return this->date_modified_m;
	}

	PermissionsFlag_t				FileSystemEntry_t::Permissions() {
		return this->permissions_m;
	}

	std::string						FileSystemEntry_t::Owner() {
		return this->owner_m;
	}

	std::string						FileSystemEntry_t::Group() {
		return this->group_m;
	}

	unsigned long					FileSystemEntry_t::Size() {
		return this->size_m;
	}

	UNIX_FILE_t						FileSystemEntry_t::FileType() {
		return this->file_type_m;
	}

	std::vector<FileSystemEntry_t>	FileSystemEntry_t::Content() {
		return this->content_m;
	}

	/*---> OPERATORS <---*/
	FileSystemEntry_t FileSystemEntry_t::operator[](int _index) {
		return this->content_m[_index];
	}

	FileSystemEntry_t FileSystemEntry_t::operator[](std::string _name) {
		for(std::vector<FileSystemEntry_t>::iterator iter = this->content_m.begin();
		iter != this->content_m.end();
		iter++) {
			if(iter->Name() == _name) {
				return *iter;
			}
		}

		FileSystemEntry_t EmptyEntry;

		return EmptyEntry;
	}
}
