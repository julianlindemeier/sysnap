#include "filesystem_entry_t.hpp"
#include <ostream>
#include "path_t.hpp"
#include "comparison_report_t.hpp"

#ifndef SYSNAP_FILESYSTEM_HPP
#define SYSNAP_FILESYSTEM_HPP

namespace sysnap {
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
		ComparisonReport_t Compare(FileSystem_t& _first, FileSystem_t& _second);

	private:
		void _Insert_(FileSystemEntry_t* _entry);
		void _Print_(FileSystemEntry_t* _entry, std::ostream &_out);
		void _Scan_(boost::filesystem::path _dir_path);

		static
		void _Compare_(FileSystemEntry_t* _frst, FileSystemEntry_t* _scnd);
	};
}

#endif
