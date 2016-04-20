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
		delete this->system_m;
	}

	void FileSystem_t::Scan(Path_t _path) {
		boost::filesystem::path	dir_path(_path.GetString());

		//Here comes the boost stuff!
		try {
			this->_Scan_(dir_path);

		} catch(boost::filesystem::filesystem_error& _err) {
			std::cerr << termcolor::blink << termcolor::bold << termcolor::red << "ERROR: " << termcolor::reset;
			std::cerr << termcolor::red << _err.what() << std::endl << termcolor::reset;
		}
	}

	void FileSystem_t::Print() {
		this->_Print_(*this->system_m);
		//std::cout << "Path of system_m" << this->system_m->Path() << "\n";
	}

	void FileSystem_t::ExportAsXML() {
		//TODO: Finish FileSystem_t::ExportAsXML()-function.
	}

	void FileSystem_t::_Insert_(FileSystemEntry_t _entry) {
		//Check if we don't have a root directory:
		if(this->system_m == NULL) {
			FileSystemEntry_t *new_entry = new FileSystemEntry_t;
			*new_entry = _entry;

			this->system_m = new_entry;
			return;
		}


		//In case the root directory is already saved, iterate through it's content:
		FileSystemEntry_t* dir_iter = this->system_m;

		for(int i = this->system_m->Path().Levels()+1; i < _entry.Path().Levels(); i++) {
			if((dir_iter = dir_iter->Find(_entry.Path()[i])) == NULL) {
				std::cout << "Could not find " << _entry.Path()[i] << "\n";
			}
		}

		//insert the entry into the desired directory.
		dir_iter->InsertContent(_entry);

		return;
	}

	void FileSystem_t::_Print_(FileSystemEntry_t _entry) {
		std::string level_indent = "";
		//Indent the line, to show where the entry belongs:
		for(int i=0; i < _entry.Path().Levels(); i++) {
			level_indent += "\t";
		}

		//std::cout << level_indent << "+------------------------------------------\n";
		std::cout << level_indent << "| Name:        " << _entry.Name() << "\n";
		//std::cout << level_indent << "| Path:        " << _entry.Path() << "\n";
		//std::cout << level_indent << "| Modified:    " << GetTimeString(_entry.DateModified()) << "\n";
		//std::cout << level_indent << "| Permissions: " << GetPermissionsString(_entry.Permissions()) << "\n";
		//std::cout << level_indent << "| Owner:       " << _entry.Owner() << "\n";
		//std::cout << level_indent << "| Group:       " << _entry.Group() << "\n";
		std::cout << level_indent << "| Size:        " << sysnap::ByteSuffix(_entry.Size()) << "\n";
		//std::cout << level_indent << "| File Type:   " << GetFileTypeString(_entry.FileType()) << "\n";
		std::cout << level_indent << "+------------------------------------------\n";

		if(!_entry.Empty()) {
			std::vector<FileSystemEntry_t> content;
			content = _entry.Content();

			for(std::vector<FileSystemEntry_t>::iterator iter = content.begin();
			iter != content.end();
			iter++) {
				this->_Print_(*iter);
			}
		}

		return;
	}

	void FileSystem_t::_Scan_(boost::filesystem::path _path) {
		FileSystemEntry_t		newEntry;
		boost::filesystem::file_status stats = boost::filesystem::status(_path);

		newEntry.Name(_path.filename().string());
		newEntry.Path(_path.parent_path().string());
		newEntry.DateModified(GetLocalTime(boost::filesystem::last_write_time(_path)));
		newEntry.Permissions(stats.permissions());
		newEntry.Owner(GetOwner(_path));
		newEntry.Group(GetGroup(_path));
		newEntry.Size(GetSize(_path));
		newEntry.FileType(sysnap::BoostFileType2UNIX_FILE_t(stats.type()));

		//this->_Print_(newEntry);
		this->_Insert_(newEntry);

		if(!boost::filesystem::is_directory(_path)) {
			return;
		}

		//Just a try...
		std::vector<boost::filesystem::path> paths;

		boost::filesystem::directory_iterator dir_iter_end;
		for(boost::filesystem::directory_iterator dir_iter(_path);
		dir_iter != dir_iter_end;
		++dir_iter) {
			paths.push_back(dir_iter->path());
		}

		for(std::vector<boost::filesystem::path>::iterator paths_iter = paths.begin();
		paths_iter != paths.end();
		paths_iter++) {
			this->_Scan_(*paths_iter);
		}
	}
}
