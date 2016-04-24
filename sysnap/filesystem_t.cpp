#include "filesystem_t.hpp"
#include "auxilliary.hpp"
#include "filesystem_entry_t.hpp"
#include "path_t.hpp"
#include "comparison_report_t.hpp"
#include <boost/filesystem.hpp>
#include <termcolor/termcolor.hpp>

extern bool verbose;

namespace sysnap {
	FileSystem_t::FileSystem_t() {

	}

	FileSystem_t::~FileSystem_t() {
		//Since every entry was allocated, call free on every entry:
		for(std::vector<FileSystemEntry_t*>::iterator del_iter = this->system_m.begin();
		del_iter != this->system_m.end();
		del_iter++) {
			//std::cout << "Deleting " << (*del_iter)->Name() << "\n";
			delete (*del_iter);
		}
	}

	void FileSystem_t::Scan(Path_t _path) {
		boost::filesystem::path	dir_path(_path.GetString());

		if(!boost::filesystem::exists(dir_path)	|| !boost::filesystem::is_directory(dir_path)) {
			std::cerr << termcolor::bold << termcolor::red << "[ERROR]: " << termcolor::reset;
			std::cerr << "Directory >" << dir_path.string() << "< does not exist!\n" << termcolor::reset;

			exit(1);
		}

		//Here comes the boost stuff!
		try {
			if(verbose) {
				std::cout << termcolor::bold << termcolor::cyan << "[INFO]: " << termcolor::reset;
				std::cout << "Scanning >" << dir_path.string() <<  "< ...\n" << termcolor::reset;
			}

			this->snap_created_m = GetLocalTime();
			this->_Scan_(dir_path);

			if(verbose) {
				std::cout << termcolor::bold << termcolor::cyan << "[INFO]: " << termcolor::reset;
				std::cout << "... complete.\n" << termcolor::reset;
			}

		} catch(boost::filesystem::filesystem_error& _err) {
			std::cerr << termcolor::bold << termcolor::red << "[ERROR]: " << termcolor::reset;
			std::cerr << _err.what() << std::endl << termcolor::reset;
		}
	}

	void FileSystem_t::Print(std::ostream &_out) {
		if(&_out != &std::cout) {
			if(verbose) {
				std::cout << termcolor::bold << termcolor::cyan << "[INFO]: " << termcolor::reset;
				std::cout << "Exporting as XML to file...\n" << termcolor::reset;
			}
			//If so, export file as XML, hence add xml-header
			_out << "<?xml version=\"1.0\"?>\n\n";
			_out << "<system>\n";
		}

		this->_Print_(this->system_m[0], _out);
		
		if(&_out != &std::cout) {
			_out << "</system>\n";

			if(verbose) {
				std::cout << termcolor::bold << termcolor::cyan << "[INFO]: " << termcolor::reset;
				std::cout << "...complete.\n" << termcolor::reset;
			}
		}
	}

	void FileSystem_t::ExportAsXML(Path_t _output_path) {
		std::ofstream output_file;
		output_file.open(_output_path.GetString());

		if(!output_file.is_open()) {
			std::cerr << termcolor::bold << termcolor::red << "[ERROR]: " << termcolor::reset;
			std::cerr << "Could not open/create file: " << _output_path.GetBoostPath().filename() << " in >"
										 << _output_path.GetBoostPath().parent_path().string() << "<!\n" << termcolor::reset;
			exit(1);
		} else if(verbose) {
			std::cout << termcolor::bold << termcolor::cyan << "[INFO]: " << termcolor::reset;
			std::cout << "Creating file: " << _output_path.GetBoostPath().filename() << " in >"
										 << _output_path.GetBoostPath().parent_path().string() << "<\n" << termcolor::reset;
		}

		this->Print(output_file);

		output_file.close();
	}

	ComparisonReport_t FileSystem_t::Compare(FileSystem_t& _first, FileSystem_t& _second) {
		if(verbose) {
			std::cout << termcolor::bold << termcolor::cyan << "[INFO]: " << termcolor::reset;
			std::cout << "Comparing Snapshot of " << _first.system_m[0]->Name() << " in "<< _first.system_m[0]->Path() << " from " << GetTimeString(_first.snap_created_m) << "\n"
			  << "               to Snapshot of " << _second.system_m[0]->Name() << " in "<< _second.system_m[0]->Path() << " from " << GetTimeString(_second.snap_created_m) << "\n"<< termcolor::reset;
		}

		ComparisonReport_t comparison_report;
		ComparisonResult_t new_comparison;

		for(std::vector<FileSystemEntry_t*>::iterator frst_cntnt_iter = _first.system_m.begin();
		frst_cntnt_iter != _first.system_m.end();
		frst_cntnt_iter++) {
			for(std::vector<FileSystemEntry_t*>::iterator scnd_cntnt_iter = _second.system_m.begin();
			scnd_cntnt_iter != _second.system_m.end();
			scnd_cntnt_iter++) {
				if((*frst_cntnt_iter)->iNode()		== (*scnd_cntnt_iter)->iNode()) {
					new_comparison.previous = *frst_cntnt_iter;
					new_comparison.current  = *scnd_cntnt_iter;
					bool change_occurred = false;

					if((*frst_cntnt_iter)->Name()			!= (*scnd_cntnt_iter)->Name()) {
						new_comparison.type.push_back(NAME_CHANGE);
						change_occurred = true;
					}
					if((*frst_cntnt_iter)->DateModified()	!= (*scnd_cntnt_iter)->DateModified()) {
						new_comparison.type.push_back(FILE_MODIFIED);
						change_occurred = true;
					}
					if((*frst_cntnt_iter)->Permissions()	!= (*scnd_cntnt_iter)->Permissions()) {
						new_comparison.type.push_back(PERMISSIONS_CHANGED);
						change_occurred = true;
					}
					if((*frst_cntnt_iter)->Owner()			!= (*scnd_cntnt_iter)->Owner()) {
						new_comparison.type.push_back(OWNER_CHANGED);
						change_occurred = true;
					}
					if((*frst_cntnt_iter)->Group()			!= (*scnd_cntnt_iter)->Group()) {
						new_comparison.type.push_back(GROUP_CHANGED);
						change_occurred = true;
					}
					if((*frst_cntnt_iter)->Size()			!= (*scnd_cntnt_iter)->Size()) {
						new_comparison.type.push_back(SIZE_CHANGED);
						change_occurred = true;
					}
					if((*frst_cntnt_iter)->FileType()		!= (*scnd_cntnt_iter)->FileType()) {
						//If the file Type changed to symlink, it's most likely not the same file, though the iNodes of both are the same,
						//we need to filter these cases:
						if((*frst_cntnt_iter)->FileType() == sysnap::SYMLINK || (*scnd_cntnt_iter)->FileType() == sysnap::SYMLINK) {
							new_comparison.type.clear();
							continue;
						}

						new_comparison.type.push_back(FILE_TYPE_CHANGED);
						change_occurred = true;
					}


					if(change_occurred) {
						comparison_report.Insert(new_comparison);
						new_comparison.type.clear();
					}
				}
			}
		}

		return comparison_report;
	}

	void FileSystem_t::_Insert_(FileSystemEntry_t* _entry) {
		FileSystemEntry_t *new_entry = new FileSystemEntry_t;

		//Check if we don't have a root directory:
		if(this->system_m.empty()) {
			new_entry = _entry;

			this->system_m.push_back(new_entry);
			return;
		}


		//In case the root directory is already saved, iterate through it's content:
		FileSystemEntry_t* dir_iter = this->system_m[0];

		for(int i = this->system_m[0]->Path().Levels()+1; i < _entry->Path().Levels(); i++) {
			if((dir_iter = dir_iter->Find(_entry->Path()[i])) == NULL) {
				std::cout << "Could not find " << _entry->Path()[i] << "\n";
			}
		}

		//insert the entry into the desired directory.
		dir_iter->InsertContent(_entry);
		this->system_m.push_back(_entry);

		return;
	}

	void FileSystem_t::_Print_(FileSystemEntry_t* _entry, std::ostream &_out) {
		std::string level_indent = (&_out != &std::cout) ? "\t" : "";

		//Indent the line, to show where the entry belongs:
		for(int i=0; i < (_entry->Path().Levels() - this->system_m[0]->Path().Levels()); i++) {
			level_indent += "\t";
		}

		//check if the _out adress is not the same as std::cout (hence if _out == std::cout):
		if(&_out != &std::cout) {
			_out << level_indent << "<iNode> " << _entry->iNode() << " </iNode>\n";
			_out << level_indent << "<name> " << _entry->Name() << " </name>\n";
			_out << level_indent << "<path> " << _entry->Path() << " </path>\n";
			_out << level_indent << "<modified>  \n";
			_out << level_indent << "\t<day> "    << _entry->DateModified().day    << " </day>\n";
			_out << level_indent << "\t<month> "  << _entry->DateModified().month  << " </month>\n";
			_out << level_indent << "\t<year> "   << _entry->DateModified().year   << " </year>\n";
			_out << level_indent << "\t<hour> "   << _entry->DateModified().hour   << " </hour>\n";
			_out << level_indent << "\t<minute> " << _entry->DateModified().minute << " </minute>\n";
			_out << level_indent << "\t<second> " << _entry->DateModified().second << " <second>\n";
			_out << level_indent << "</modified>\n";
			_out << level_indent << "<permissions> " << sysnap::dec2oct(_entry->Permissions().owner) *100
													  + sysnap::dec2oct(_entry->Permissions().group) *10
													  + sysnap::dec2oct(_entry->Permissions().others) << "</permissions>\n";
			_out << level_indent << "<owner> " << _entry->Owner() << " </owner>\n";
			_out << level_indent << "<group> " << _entry->Group() << " </group>\n";
			_out << level_indent << "<size> " << _entry->Size() << " </size>\n";
			_out << level_indent << "<file_type> " << GetFileTypeString(_entry->FileType()) << " </file_type>\n";
			_out << level_indent << "<content>\n";
		} else {
			//Otherwise print neatly to console:
			_out << level_indent << "iNode:       " << _entry->iNode() << "\n";
			_out << level_indent << "Name:        " << _entry->Name() << "\n";
			_out << level_indent << "Path:        " << _entry->Path() << "\n";
			_out << level_indent << "Modified:    " << GetTimeString(_entry->DateModified()) << "\n";
			_out << level_indent << "Permissions: " << GetPermissionsString(_entry->Permissions()) << "\n";
			_out << level_indent << "Owner:       " << _entry->Owner() << "\n";
			_out << level_indent << "Group:       " << _entry->Group() << "\n";
			_out << level_indent << "Size:        " << sysnap::ByteSuffix(_entry->Size()) << "\n";
			_out << level_indent << "File Type:   " << GetFileTypeString(_entry->FileType()) << "\n\n";
		}

		if(!_entry->Empty()) {
			std::vector<FileSystemEntry_t*> content;
			content = _entry->Content();

			for(std::vector<FileSystemEntry_t*>::iterator iter = content.begin();
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
		FileSystemEntry_t				*newEntry = new FileSystemEntry_t;
		boost::filesystem::file_status	stats = boost::filesystem::status(_path);

		newEntry->iNode(GetiNode(_path));
		newEntry->Name(_path.filename().string());
		newEntry->Path(_path.parent_path().string());
		newEntry->DateModified(GetLocalTime(boost::filesystem::last_write_time(_path)));
		newEntry->Permissions(stats.permissions());
		newEntry->Owner(GetOwner(_path));
		newEntry->Group(GetGroup(_path));
		newEntry->Size(GetSize(_path));
		newEntry->FileType(GetFileType(_path));

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

	void FileSystem_t::_Compare_(FileSystemEntry_t* _frst, FileSystemEntry_t* _scnd) {

	}
}
