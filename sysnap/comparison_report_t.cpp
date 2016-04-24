#include "comparison_report_t.hpp"
#include "filesystem_t.hpp"
#include <iostream>
#include <iomanip>
#include <termcolor/termcolor.hpp>

extern bool verbose;

namespace sysnap {
	ComparisonReport_t::ComparisonReport_t() {

	}

	ComparisonReport_t::~ComparisonReport_t() {

	}

	void ComparisonReport_t::Insert(ComparisonResult_t& comp_result) {
		this->comparison_report_m.push_back(comp_result);
	}

	void ComparisonReport_t::Print() {
		if(this->comparison_report_m.empty()) {
			std::cout << "Nothing changed. \n";
		}

		for(std::vector<sysnap::ComparisonResult_t>::iterator comp_iter = this->comparison_report_m.begin();
		comp_iter != this->comparison_report_m.end();
		comp_iter++) {
			std::string level_indent;
			//Indent the line, to show where the entry belongs:
			for(int i=0; i < (comp_iter->previous->Path().Levels() - this->comparison_report_m[0].previous->Path().Levels()); i++) {
				level_indent += "\t";
			}
			std::cout << level_indent << "Changes to " << comp_iter->previous->Name() << ":\n";

			for(std::vector<sysnap::COMPARISON_TYPE_t>::iterator comp_type_iter = comp_iter->type.begin();
			comp_type_iter != comp_iter->type.end();
			comp_type_iter++) {
				switch(*comp_type_iter) {
					case sysnap::NAME_CHANGE:
						std::cout << level_indent << "- Name:        " << std::setw(20) << comp_iter->previous->Name()
											 << " -> " << std::setw(20) << comp_iter->current->Name() << "\n";
						break;
					case sysnap::FILE_MODIFIED:
						std::cout << level_indent << "- Contents:    " << std::setw(30) << sysnap::GetTimeString(comp_iter->current->DateModified()) << "\n";
						break;
					case sysnap::PERMISSIONS_CHANGED:
						std::cout << level_indent << "- Permissions: " << std::setw(20) << sysnap::GetPermissionsString(comp_iter->previous->Permissions())
						 					 << " -> " << std::setw(20) << sysnap::GetPermissionsString(comp_iter->current->Permissions())  << "\n";
						break;
					case sysnap::OWNER_CHANGED:
						std::cout << level_indent << "- Owner:       " << std::setw(20) << comp_iter->previous->Owner()
											 << " -> " << std::setw(20) << comp_iter->current->Owner()  << "\n";
						break;
					case sysnap::GROUP_CHANGED:
						std::cout << level_indent << "- Group:       " << std::setw(20) << comp_iter->previous->Group()
											 << " -> " << std::setw(20) << comp_iter->current->Group()  << "\n";
						break;
					case sysnap::SIZE_CHANGED:
						std::cout << level_indent << "- Size:        " << std::setw(20) << sysnap::ByteSuffix(comp_iter->previous->Size())
											 << " -> " << std::setw(20) << sysnap::ByteSuffix(comp_iter->current->Size())  << "\n";
						break;
					case sysnap::FILE_TYPE_CHANGED:
						std::cout << level_indent << "- Type:        " << std::setw(20) << sysnap::GetFileTypeString(comp_iter->previous->FileType())
											 << " -> " << std::setw(20) << sysnap::GetFileTypeString(comp_iter->current->FileType())  << "\n";
						break;

					default:
						break;
				}
			}
			std::cout << "\n";
		}
	}
}
