#include <string>
#include "path_t.hpp"

namespace sysnap {
	Path_t::Path_t() {

	}

	Path_t::Path_t(std::string _path) {
		this->path_m = this->_DecomposePath_(_path);
	}

	Path_t::~Path_t() {

	}

	void Path_t::Path(std::string _path) {
		this->path_m = this->_DecomposePath_(_path);
	}

	std::string Path_t::operator[](int _index) {
		return this->path_m[_index];
	}

	int Path_t::Levels() {
		if(this->path_m.empty())
			return -1;

		return this->path_m.size();
	}

	std::vector<std::string> Path_t::_DecomposePath_(std::string _path) {
		std::vector<std::string> ret_path_decomposed;

		//Put string in proper form to analyze, in case it's needed:
		if(_path[0] == '/') {
			_path = _path.substr(1);
		}

		if(_path[_path.size()-1] != '/') {
			_path += '/';
		}

		size_t pos = 0;
		std::string token;
		while ((pos = _path.find('/')) != std::string::npos) {
			token = _path.substr(0, pos);
			ret_path_decomposed.push_back(token);
			_path.erase(0, pos + 1);
		}

		return ret_path_decomposed;
	}
}
