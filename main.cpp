/*
 * main.cpp
 * Created by Julian Lindemeier on 16.04.2016 19:53
 */

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <termcolor/termcolor.hpp>

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
	if(argc == 1) {
		char* path_tmp = new char[FILENAME_MAX];

		if(!getcwd(path_tmp, FILENAME_MAX)) {
			std::cerr << "Error.\n";
			return 1;
		}

		path = path_tmp;

		delete[] path_tmp;
	} else {
		args_table = sysnap::parse_args(args);

		for(int i=0; i < args_table.size(); i++) {
			if(args_table[i].first == "path" || args_table[i].first == "p" || args_table[i].first.empty()) {
				path = args_table[i].second;
			}

			//TODO: Add further options here.
		}
	}

	std::cout << "Path: " << path << "\n";

	return 0;
}
