#include <iostream>
#include <vector>

#ifndef SYSNAP_AUXILLIARY_HPP
#define SYSNAP_AUXILLIARY_HPP

namespace sysnap {
	/* * * * * * * * * * *
	 * Parsing Arguments *
	 * * * * * * * * * * */
	std::vector< std::pair<std::string, std::string> > parse_args(std::vector<std::string> _args);
}

#endif
