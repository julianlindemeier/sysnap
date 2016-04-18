#include <ostream>
#include <vector>
#include <string>

#ifndef SYSNAP_PATH_T_HPP
#define SYSNAP_PATH_T_HPP

namespace sysnap {
	class Path_t {
	private:
		std::vector<std::string> path_m;

	public:
		Path_t();
		Path_t(std::string _path);
		~Path_t();

		void			Path(std::string _path);
		int				Levels();

		void			operator= (std::string _path);
		bool			operator==(const Path_t& _path);
		std::string		operator[](const int _index);

		friend
		std::ostream&	operator<<(std::ostream &_out, Path_t _path);

	private:
		std::vector<std::string> _DecomposePath_(std::string _path);
	};
}

#endif
