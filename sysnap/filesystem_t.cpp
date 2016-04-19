#include "filesystem_t.hpp"
#include "auxilliary.hpp"
#include "filesystem_entry_t.hpp"
#include "path_t.hpp"
#include <boost/filesystem.hpp>
#include <termcolor/termcolor.hpp>

namespace sysnap {
	FileSystem_t::FileSystem_t() {

	}

	FileSystem_t::~FileSystem_t() {

	}

	void FileSystem_t::Scan(std::string _path) {
		boost::filesystem::path	dir_path;
		FileSystemEntry_t		newEntry;

		//Here comes the boost stuff!
		try {
			if(boost::filesystem::exists(dir_path)) {
				if(boost::filesystem::is_directory(dir_path)) {
					
				} else {
					std::cerr << termcolor::blink << termcolor::bold << termcolor::red << "ERROR: " << termcolor::reset;
					std::cerr << termcolor::red << "Path not to directory! \n" << termcolor::reset;
				}
			} else {
				std::cerr << termcolor::blink << termcolor::bold << termcolor::red << "ERROR: " << termcolor::reset;
				std::cerr << termcolor::red << "Path does not exist! \n" << termcolor::reset;
			}
		} catch(boost::filesystem::filesystem_error& _err) {
			std::cerr << termcolor::blink << termcolor::bold << termcolor::red << "ERROR: " << termcolor::reset;
			std::cerr << termcolor::red << _err.what() << std::endl << termcolor::reset;
		}
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
		std::string level_indent = "";
		//Indent the line, to show where the entry belongs:
		for(int i=0; i < _entry.Path().Levels(); i++) {
			level_indent += "\t";
		}

		std::cout << level_indent << "-------------------------------------------\n";
		std::cout << level_indent << "Name:        " << _entry.Name() << "\n";
		std::cout << level_indent << "Path:        " << _entry.Path() << "\n";
		std::cout << level_indent << "Created:     " << GetTimeString(_entry.DateCreated()) << "\n";
		std::cout << level_indent << "Modified:    " << GetTimeString(_entry.DateModified()) << "\n";
		std::cout << level_indent << "Permissions: " << GetPermissionsString(_entry.Permissions()) << "\n";
		std::cout << level_indent << "Owner:       " << _entry.Owner() << "\n";
		std::cout << level_indent << "Group:       " << _entry.Group() << "\n";
		std::cout << level_indent << "File Type:   " << GetFileTypeString(_entry.FileType()) << "\n";
		std::cout << level_indent << "-------------------------------------------\n";

		if(!_entry.Empty()) {
			for(int i = 0; i < _entry.Content().size(); i++) {
				this->_Print_(_entry[i]);
			}
		}

		return;
	}
}
