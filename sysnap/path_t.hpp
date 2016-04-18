#include <vector>

#ifndef SYSNAP_PATH_T_HPP
#define SYSNAP_PATH_T_HPP

namespace sysnap {
	struct Path_t {
	private:
		std::vector<std::string> path_m;

	public:
		Path_t();
		Path_t(std::string _path);
		~Path_t();

		void	Path(std::string _path);
		int		Levels();
		std::string operator[](int _index);

	private:
		std::vector<std::string> _DecomposePath_(std::string _path);
	};
}

#endif
