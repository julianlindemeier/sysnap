#include "filesystem_entry_t.hpp"
#include <ostream>
#include "path_t.hpp"

#ifndef SYSNAP_FILESYSTEM_HPP
#define SYSNAP_FILESYSTEM_HPP

namespace sysnap {
	enum COMPARISON_TYPE_t {
		NAME_CHANGE = 0,
		FILE_MODIFIED,
		PERMISSIONS_CHANGED,
		OWNER_CHANGED,
		GROUP_CHANGED,
		SIZE_CHANGED,
		FILE_TYPE_CHANGED,
		ADDED,
		REMOVED
	};

	struct ComparisonResult {
		FileSystemEntry_t*				previous;
		FileSystemEntry_t*				current;
		std::vector<COMPARISON_TYPE_t>	type;
	};

	class FileSystem_t {
	public:
		Timestamp_t						snap_created_m;
		std::vector<FileSystemEntry_t*>	system_m;

	public:
		FileSystem_t();
		~FileSystem_t();

		void Scan(Path_t _path);
		void Print(std::ostream &_out=std::cout);
		void ExportAsXML(Path_t _output_path);

		static
		std::vector<ComparisonResult> Compare(FileSystem_t& _first, FileSystem_t& _second);

	private:
		void _Insert_(FileSystemEntry_t* _entry);
		void _Print_(FileSystemEntry_t* _entry, std::ostream &_out);
		void _Scan_(boost::filesystem::path _dir_path);

		static
		void _Compare_(FileSystemEntry_t* _frst, FileSystemEntry_t* _scnd);
	};
}

#endif
