#include "filesystem_t.hpp"
#include "filesystem_entry_t.hpp"
#include "path_t.hpp"

namespace sysnap {
	FileSystem_t::FileSystem_t() {

	}

	FileSystem_t::~FileSystem_t() {

	}

	void FileSystem_t::Scan(Path_t _path) {

	}

	void FileSystem_t::Print() {
		std::cout << "THIS FILESYSTEM (TEST):\n";

		this->_Print_(*this->system_m);
	}

	void FileSystem_t::ExportAsXML() {

	}

	void FileSystem_t::_Insert_(FileSystemEntry_t& _entry) {
		//Check if we don't have a root directory:
		if(this->system_m == NULL) {
			this->system_m = &_entry;
			return;
		}


		//In case the root directory is already saved, iterate through it's content:
		FileSystemEntry_t* dir_iter = this->system_m;
		for(int i=1; i < _entry.Path().Levels(); i++) {
			//if the directory is empty, we cannot dig any further.
			if(dir_iter->Empty()) {
				break;
			}
			//if the iterator points to an entry, which is not a directory, start over.
			if(dir_iter->FileType() != sysnap::DIRECTORY) {
				continue;
			}

			dir_iter = dir_iter->Find(_entry.Path()[i]);
		}

		//insert the entry into the desired directory.
		dir_iter->InsertContent(_entry);
	}

	void FileSystem_t::_Print_(FileSystemEntry_t _entry) {
		for(int i=0; i < _entry.Path().Levels(); i++) {
			std::cout << "\t";
		}


		std::cout << "Name: " << _entry.Name() << "  (" << _entry.Path() << ")\n";

		if(!_entry.Empty()) {
			for(int i = 0; i < _entry.Content().size(); i++) {
				this->_Print_(_entry[i]);
			}
		}

		return;
	}
}
