/*
 * main.cpp
 * Created by Julian Lindemeier on 16.04.2016 19:53
 */

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <termcolor/termcolor.hpp>
#include <pugixml.hpp>
#include <vector>

#include "sysnap/sysnap.hpp"

//#define DEBUG


int main(int argc, char const *argv[]) {
	std::string path;
	std::vector<std::string> args;
	std::vector< std::pair<std::string, std::string> > args_table;

	//Copy arguments into args-vector:
	for(int i=1; i < argc; i++) {
		args.push_back(argv[i]);
	}

	//if no arguments are given, take the current directory
	args_table = sysnap::parse_args(args);

	if(args_table.empty()) {
		char* path_tmp = new char[FILENAME_MAX];

		if(!getcwd(path_tmp, FILENAME_MAX)) {
			std::cerr << "Error.\n";
			return 1;
		}

		path = path_tmp;

		delete[] path_tmp;
	} else {
		for(int i=0; i < args_table.size(); i++) {
			if(args_table[i].first == "path" || args_table[i].first == "p" || args_table[i].first.empty()) {
				path = args_table[i].second;
			}

			//TODO: Add further options here.
		}
	}

	//TODO: Add File system Analyzer
	//TODO: Add XML-Writer
	//TODO: Add XML-Support
	//TODO: Add Comparator

	sysnap::Timestamp_t date_created;
	date_created = sysnap::GetLocalTime();

	sysnap::FileSystemEntry_t my_system;
	sysnap::FileSystemEntry_t dir_lindebear;
	sysnap::FileSystemEntry_t dir_applications;
	sysnap::FileSystemEntry_t dir_desktop;
	sysnap::FileSystemEntry_t dir_documents;
	sysnap::FileSystemEntry_t file_myDoc_txt;

	sysnap::FileSystem_t mySystem;

	dir_lindebear.Name("lindebear");
	dir_lindebear.Path("/");
	dir_lindebear.DateCreated(date_created);
	dir_lindebear.DateModified();
	dir_lindebear.Permissions(755);
	dir_lindebear.Owner("lindebear");
	dir_lindebear.Group("staff");
	dir_lindebear.Size(12394871974);
	dir_lindebear.FileType(sysnap::DIRECTORY);

	dir_applications.Name("Applications");
	dir_applications.Path("/lindebear");
	dir_applications.DateCreated(date_created);
	dir_applications.DateModified();
	dir_applications.Permissions(755);
	dir_applications.Owner("lindebear");
	dir_applications.Group("staff");
	dir_applications.Size(31123);
	dir_applications.FileType(sysnap::DIRECTORY);

	dir_desktop.Name("Desktop");
	dir_desktop.Path("/lindebear");
	dir_desktop.DateCreated(date_created);
	dir_desktop.DateModified();
	dir_desktop.Permissions(755);
	dir_desktop.Owner("lindebear");
	dir_desktop.Group("staff");
	dir_desktop.Size(35134);
	dir_desktop.FileType(sysnap::DIRECTORY);

	dir_documents.Name("Documents");
	dir_documents.Path("/lindebear");
	dir_documents.DateCreated(date_created);
	dir_documents.DateModified();
	dir_documents.Permissions(755);
	dir_documents.Owner("lindebear");
	dir_documents.Group("staff");
	dir_documents.Size(4789475);
	dir_documents.FileType(sysnap::DIRECTORY);

	file_myDoc_txt.Name("myDoc.txt");
	file_myDoc_txt.Path("/lindebear/Documents");
	file_myDoc_txt.DateCreated(date_created);
	file_myDoc_txt.DateModified();
	file_myDoc_txt.Permissions(755);
	file_myDoc_txt.Owner("lindebear");
	file_myDoc_txt.Group("staff");
	file_myDoc_txt.Size(423);
	file_myDoc_txt.FileType(sysnap::REGULAR_FILE);

	mySystem._Insert_(dir_lindebear);
	mySystem._Insert_(dir_applications);
	mySystem._Insert_(dir_desktop);
	mySystem._Insert_(dir_documents);
	mySystem._Insert_(file_myDoc_txt);

	mySystem.Print();

	mySystem.Scan("/lindebear");

	return 0;
}
