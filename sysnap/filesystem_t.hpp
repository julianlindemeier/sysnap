#include "filesystem_entry_t.hpp"
#include <ostream>
#include "path_t.hpp"

#ifndef SYSNAP_FILESYSTEM_HPP
#define SYSNAP_FILESYSTEM_HPP

namespace sysnap {
	class FileSystem_t {
	private:
		FileSystemEntry_t *system_m;

	public:
		FileSystem_t();
		~FileSystem_t();

		void Scan(Path_t _path);
		void Print(std::ostream &_out=std::cout);
		void ExportAsXML(Path_t _output_path);

	//TODO: change back to private!!!
	public:
		void _Insert_(FileSystemEntry_t _entry);
		void _Print_(FileSystemEntry_t _entry, std::ostream &_out);
		void _Scan_(boost::filesystem::path _dir_path);
	};
}

#endif
