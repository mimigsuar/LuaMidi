#include <iostream>
#include "luamidi.hpp"

int main(int argc, char **argv)
{
	if (argc < 2) { 
		std::cout << "luamidi <source file>" << std::endl;
		return -1;
	}

	LuaMidi lm; 

	lm.load_file(argv[1]);

	return 0;
}
