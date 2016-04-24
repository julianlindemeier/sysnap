#include <vector>

#ifndef COMPARISON_REPORT_T_HPP
#define COMPARISON_REPORT_T_HPP

namespace sysnap {
	//forward declaration of FileSystemEntry_t
	class FileSystemEntry_t;

	enum COMPARISON_TYPE_t {
		NAME_CHANGE = 0,
		FILE_MODIFIED,
		PERMISSIONS_CHANGED,
		OWNER_CHANGED,
		GROUP_CHANGED,
		SIZE_CHANGED,
		FILE_TYPE_CHANGED,
		ADDED,
		REMOVED
	};

	struct ComparisonResult_t {
		FileSystemEntry_t*				previous;
		FileSystemEntry_t*				current;
		std::vector<COMPARISON_TYPE_t>	type;
	};

	class ComparisonReport_t {
	private:
		std::vector<ComparisonResult_t> comparison_report_m;

	public:
		ComparisonReport_t();
		~ComparisonReport_t();

		void Insert(ComparisonResult_t& comp_result);
		void Print();
	};
}

#endif
