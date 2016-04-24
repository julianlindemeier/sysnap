/*
 * main.cpp
 * Created by Julian Lindemeier on 16.04.2016 19:53
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "sysnap/sysnap.hpp"

bool verbose = false;
bool print = false;
bool output = false;

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

			//TODO: Add further options here.
		}
	}

	//TODO: Add XML-Writer
	//TODO: Add XML-Reader
	//TODO: Add Comparator

	sysnap::FileSystem_t mySystem1;
	sysnap::FileSystem_t mySystem2;

	mySystem1.Scan(input_path);
	std::getchar();
	mySystem2.Scan(input_path);

	if(print) {
		mySystem1.Print();
		mySystem2.Print();
	}

	if(output) {
		mySystem1.ExportAsXML(output_path);
	}

	std::vector<sysnap::ComparisonResult> comparison_report;
	comparison_report =  sysnap::FileSystem_t::Compare(mySystem1, mySystem2);

	for(std::vector<sysnap::ComparisonResult>::iterator comp_iter = comparison_report.begin();
	comp_iter != comparison_report.end();
	comp_iter++) {
		for(std::vector<sysnap::COMPARISON_TYPE_t>::iterator comp_type_iter = comp_iter->type.begin();
		comp_type_iter != comp_iter->type.end();
		comp_type_iter++) {
			switch(*comp_type_iter) {
				case sysnap::NAME_CHANGE:
					std::cout << "Name of " << comp_iter->previous->Name() << " changed to " << comp_iter->current->Name() << "\n";
					break;
				case sysnap::FILE_MODIFIED:
					std::cout << "Contents of " << comp_iter->current->Name() << " were modified on " << sysnap::GetTimeString(comp_iter->current->DateModified()) << "\n";
					break;
				case sysnap::PERMISSIONS_CHANGED:
					std::cout << "Permissions of " << comp_iter->previous->Name() << " changed from " << sysnap::GetPermissionsString(comp_iter->previous->Permissions())
					 																			<< " to " << sysnap::GetPermissionsString(comp_iter->current->Permissions())  << "\n";
					break;
				case sysnap::OWNER_CHANGED:
					std::cout << "Owner of " << comp_iter->previous->Name() << " changed from " << comp_iter->previous->Owner()
																					  << " to " << comp_iter->current->Owner()  << "\n";
					break;
				case sysnap::GROUP_CHANGED:
					std::cout << "Group of " << comp_iter->previous->Name() << " changed from " << comp_iter->previous->Group()
																					  << " to " << comp_iter->current->Group()  << "\n";
					break;
				case sysnap::SIZE_CHANGED:
					std::cout << "Size of " << comp_iter->previous->Name() << " changed from " << sysnap::ByteSuffix(comp_iter->previous->Size())
																					 << " to " << sysnap::ByteSuffix(comp_iter->current->Size())  << "\n";
					break;
				case sysnap::FILE_TYPE_CHANGED:
					std::cout << "Type of " << comp_iter->previous->Name() << " changed from " << sysnap::GetFileTypeString(comp_iter->previous->FileType())
																					 << " to " << sysnap::GetFileTypeString(comp_iter->current->FileType())  << "\n";
					break;

				default:
					break;
			}
		}
		std::cout << "\n";
	}

	return 0;
}
