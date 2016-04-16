#include <iostream>
#include <vector>

#include "auxilliary.hpp"

namespace sysnap {
	std::vector< std::pair<std::string, std::string> > parse_args(std::vector<std::string> _args) {
		std::vector< std::pair<std::string, std::string> > args_table;
		int counter = 0;
		for(std::vector<std::string>::iterator iter = _args.begin();
		iter != _args.end();
		iter++, counter++) {
			if(iter[0][0] == '-') {
				//check if there are more elements within args than the current one.
				if(counter+1 < _args.size()) {
					//make sure the next element does NOT begin with a hyphen.
					if(iter[1][0] != '-') {
						//-<key> <value>
						std::pair<std::string, std::string> args_table_entry;

						args_table_entry.first  = iter[0].substr(1);
						args_table_entry.second = iter[1];

						args_table.push_back(args_table_entry);
						iter++;	counter++;

						continue;	//so the -<key> case is not executed!
					} else { }
				} else { }
				//-<key>
				std::pair<std::string, std::string> args_table_entry;

				args_table_entry.first  = iter[0].substr(1);
				args_table_entry.second = "";

				args_table.push_back(args_table_entry);
			} else {
				//<value>
				std::pair<std::string, std::string> args_table_entry;

				args_table_entry.first  = "";
				args_table_entry.second = iter[0];

				args_table.push_back(args_table_entry);
			}
		}

		return args_table;
	}
}

#ifdef DEBUG
	std::cout << std::setw(10) << "argument" << std::setw(10) << "|" << std::setw(10) << "value\n";
	std::cout << std::setfill('-') << std::setw(39) << "\n" << std::setfill(' ');
	for(int i=0; i < args_table.size(); i++) {
		std::cout << std::setw(10) << (!args_table.at(i).first.empty()? args_table.at(i).first : "-") << std::setw(10) << "|"
				  << std::setw(10) << (!args_table.at(i).second.empty()? args_table.at(i).second : "-") << "\n";
	}
#endif
