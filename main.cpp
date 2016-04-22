/*
 * main.cpp
 * Created by Julian Lindemeier on 16.04.2016 19:53
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "sysnap/sysnap.hpp"

//#define DEBUG

int main(int argc, char const *argv[]) {
	sysnap::Path_t input_path;
	bool print = false;
	bool output = false;
	sysnap::Path_t output_path;
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

		input_path = path_tmp;

		delete[] path_tmp;
	} else {
		for(int i=0; i < args_table.size(); i++) {
			if(args_table[i].first == "path" || args_table[i].first == "p" || args_table[i].first.empty()) {
				input_path = args_table[i].second;
			}
			if(args_table[i].first == "print") {
				print = true;
			}
			if(args_table[i].first == "output" || args_table[i].first == "o") {
				output = true;
				output_path = args_table[i].second;
			}

			//TODO: Add further options here.
		}
	}

	std::cout << "Scanning " << input_path << "\n";

	//TODO: Add XML-Writer
	//TODO: Add XML-Reader
	//TODO: Add Comparator

	sysnap::FileSystem_t mySystem;

	mySystem.Scan(input_path);
	if(print) {
		mySystem.Print();
	}
	
	if(output) {
		std::ofstream output_file;
		output_file.open(output_path.GetString());
		mySystem.Print(output_file);
		output_file.close();
	}

	sysnap::Path_t path1, path2;

	return 0;
}
