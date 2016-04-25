/*
 * main.cpp
 * Created by Julian Lindemeier on 16.04.2016 19:53
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <termcolor/termcolor.hpp>
#include "sysnap/sysnap.hpp"

bool verbose = false;
bool print = false;
bool output = false;
bool compare = false;

int main(int argc, char const *argv[]) {
	//Default input path is the current working directory:
	sysnap::Path_t input_path = sysnap::GetCWD();
	sysnap::Path_t output_path;
	std::vector< std::pair<std::string, std::string> > args_table;

	//if no arguments are given, take the current directory
	args_table = sysnap::ParseArguments(argv, argc);

	{
		for(int i=0; i < args_table.size(); i++) {
			if(args_table[i].first == "path" || args_table[i].first == "p" || args_table[i].first.empty()) {
				input_path = args_table[i].second;
			}
			if(args_table[i].first == "print") {
				print = true;
			}
			if(args_table[i].first == "verbose" || args_table[i].first == "v") {
				verbose = true;
			}
			if(args_table[i].first == "output" || args_table[i].first == "o") {
				output = true;
				output_path = args_table[i].second;
			}
			if(args_table[i].first == "compare" || args_table[i].first == "c") {
				compare = true;
			}

			//TODO: Add further options here.
		}
	}

	//TODO: Add XML-Writer
	//TODO: Add XML-Reader

	sysnap::FileSystem_t mySystem1;
	sysnap::FileSystem_t mySystem2;

	mySystem1.Scan(input_path);
	std::getchar();
	mySystem2.Scan(input_path);

	if(print) {
		mySystem1.Print();
	}

	if(output) {
		mySystem1.ExportAsXML(output_path);
	}

	if(compare) {
		sysnap::ComparisonReport_t comparison_report;
		comparison_report =  sysnap::FileSystem_t::Compare(mySystem1, mySystem2);
		comparison_report.Print();
	}



	return 0;
}
