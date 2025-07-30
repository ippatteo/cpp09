#include "BitcoinExchange.hpp"
#include <iostream>

int main(int argc, char **argv) 
{
	if (argc != 2) 
	{
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	try 
	{
		BitcoinExchange btc;
		btc.loadDatabase("data.csv");
		btc.processInput(argv[1]);
	} 
	catch (std::exception &e) 
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}

/*
txt
date | value
2011-01-03 | 3
2011-01-03 | 2
2011-01-03 | 1
2011-01-03 | 1.2
2011-01-09 | 1
2012-01-11 | -1
2001-42-42
2012-01-11 | 1
2012-01-11 | 2147483648

*/
