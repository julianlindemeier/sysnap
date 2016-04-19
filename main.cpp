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
#include <boost/filesystem.hpp>

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

	sysnap::FileSystem_t mySystem;

	mySystem.Scan(path);
	mySystem.Print();

	return 0;
}
