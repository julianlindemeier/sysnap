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

	void FileSystem_t::Print(std::ostream &_out) {
		if(&_out != &std::cout) {
			//If so, export file as XML, hence add xml-header
			_out << "<?xml version=\"1.0\"?>\n\n";
			_out << "<system>\n";
		}
		this->_Print_(*this->system_m, _out);
		if(&_out != &std::cout) {
			_out << "</system>\n";
		}
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

	void FileSystem_t::_Print_(FileSystemEntry_t _entry, std::ostream &_out) {
		std::string level_indent = (&_out != &std::cout) ? "\t" : "";

		//Indent the line, to show where the entry belongs:
		for(int i=0; i < (_entry.Path().Levels() - this->system_m->Path().Levels()); i++) {
			level_indent += "\t";
		}

		//check if the _out adress is not the same as std::cout (hence if _out == std::cout):
		if(&_out != &std::cout) {
			_out << level_indent << "<name> " << _entry.Name() << " </name>\n";
			_out << level_indent << "<path> " << _entry.Path() << " </path>\n";
			_out << level_indent << "<modified>  \n";
			_out << level_indent << "\t<day> "    << _entry.DateModified().day    << " </day>\n";
			_out << level_indent << "\t<month> "  << _entry.DateModified().month  << " </month>\n";
			_out << level_indent << "\t<year> "   << _entry.DateModified().year   << " </year>\n";
			_out << level_indent << "\t<hour> "   << _entry.DateModified().hour   << " </hour>\n";
			_out << level_indent << "\t<minute> " << _entry.DateModified().minute << " </minute>\n";
			_out << level_indent << "\t<second> " << _entry.DateModified().second << " <second>\n";
			_out << level_indent << "</modified>\n";
			_out << level_indent << "<permissions> " << sysnap::dec2oct(_entry.Permissions().owner) *100
													  + sysnap::dec2oct(_entry.Permissions().group) *10
													  + sysnap::dec2oct(_entry.Permissions().others) << "</permissions>\n";
			_out << level_indent << "<owner> " << _entry.Owner() << " </owner>\n";
			_out << level_indent << "<group> " << _entry.Group() << " </group>\n";
			_out << level_indent << "<size> " << _entry.Size() << " </size>\n";
			_out << level_indent << "<file_type> " << GetFileTypeString(_entry.FileType()) << " </file_type>\n";
			_out << level_indent << "<content>\n";
		} else {
			//Otherwise print neatly to console:
			_out << level_indent << "Name:        " << _entry.Name() << "\n";
			_out << level_indent << "Path:        " << _entry.Path() << "\n";
			_out << level_indent << "Modified:    " << GetTimeString(_entry.DateModified()) << "\n";
			_out << level_indent << "Permissions " << GetPermissionsString(_entry.Permissions()) << "\n";
			_out << level_indent << "Owner       " << _entry.Owner() << "\n";
			_out << level_indent << "Group       " << _entry.Group() << "\n";
			_out << level_indent << "Size        " << sysnap::ByteSuffix(_entry.Size()) << "\n";
			_out << level_indent << "File Type   " << GetFileTypeString(_entry.FileType()) << "\n\n";
		}

		if(!_entry.Empty()) {
			std::vector<FileSystemEntry_t> content;
			content = _entry.Content();

			for(std::vector<FileSystemEntry_t>::iterator iter = content.begin();
			iter != content.end();
			iter++) {
				this->_Print_(*iter, _out);
			}
		}

		if(&_out != &std::cout) {
			_out << level_indent << "</content>\n";
		}

		return;
	}

	void FileSystem_t::_Scan_(boost::filesystem::path _path) {
		FileSystemEntry_t				newEntry;
		boost::filesystem::file_status	stats = boost::filesystem::status(_path);
		
		newEntry.Name(_path.filename().string());
		newEntry.Path(_path.parent_path().string());
		newEntry.DateModified(GetLocalTime(boost::filesystem::last_write_time(_path)));
		newEntry.Permissions(stats.permissions());
		newEntry.Owner(GetOwner(_path));
		newEntry.Group(GetGroup(_path));
		newEntry.Size(GetSize(_path));
		newEntry.FileType(GetFileType(_path));

		//this->_Print_(newEntry);
		this->_Insert_(newEntry);

		//If _path points to a non-directory file -> continue with the next one.
		if(!boost::filesystem::is_directory(_path)) {
			return;
		}

		//If _path points to a symlink -> Do not follow the symlink.
		if(boost::filesystem::is_symlink(_path)) {
			return;
		}

		//Just a try...
		std::vector<boost::filesystem::path> paths;

		boost::filesystem::directory_iterator dir_iter_end;
		for(boost::filesystem::directory_iterator dir_iter(_path);
		dir_iter != dir_iter_end;
		++dir_iter) {
			if(boost::filesystem::is_directory(dir_iter->path())
			|| boost::filesystem::is_regular_file(dir_iter->path())) {
				paths.push_back(dir_iter->path());
			}
		}

		for(std::vector<boost::filesystem::path>::iterator paths_iter = paths.begin();
		paths_iter != paths.end();
		paths_iter++) {
			this->_Scan_(*paths_iter);
		}
	}
}
