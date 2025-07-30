#include "BitcoinExchange.hpp"

#include <fstream>
#include <sstream>
#include <cstdlib>

BitcoinExchange::BitcoinExchange() 
{
}

BitcoinExchange::~BitcoinExchange() 
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &o) : _db(o._db)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &o)
{
	if (this != &o) _db = o._db;
	return *this;
}

bool BitcoinExchange::isValidDate(const std::string &date) const
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return false;
	int y = atoi(date.substr(0, 4).c_str());
	int m = atoi(date.substr(5, 2).c_str());
	int d = atoi(date.substr(8, 2).c_str());
	if (m < 1 || m > 12 || d < 1 || d > 31 || y < 0)
		return false;
	return true;
}

bool BitcoinExchange::isValidValue(const std::string &val) const 
{
	std::istringstream ss(val);
	float f;
	ss >> f;
	if (ss.fail() || !ss.eof())
		return false;
	if (f < 0 || f > 1000)
		return false;
	return true;
}


/*
Legge csv e salva in mappa chiave=data, valore=cambio
*/
void BitcoinExchange::loadDatabase(const std::string &filename) 
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open database.");
	std::string line;
	std::getline(file, line); // salta header
	while (std::getline(file, line)) 
	{
		std::string date, value;
		std::stringstream ss(line);
		if (std::getline(ss, date, ',') && std::getline(ss, value)) 
			_db[date] = static_cast<float>(atof(value.c_str()));
	}
}


float BitcoinExchange::getClosestRate(const std::string &date) const
{
	/*
	// Controllo trattini 
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
	throw std::runtime_error("Error: invalid date format. Expected YYYY-MM-DD.");
	// anno mese e giorno numerici
	for (int i = 0; i < 4; ++i)
	if (!std::isdigit(date[i]))
	throw std::runtime_error("Error: invalid year format.");
	for (int i = 5; i < 7; ++i)
	if (!std::isdigit(date[i]))
	throw std::runtime_error("Error: invalid month format. Month must be 2 digits with padding (01-12).");
	for (int i = 8; i < 10; ++i)
	if (!std::isdigit(date[i]))
	throw std::runtime_error("Error: invalid day format. Day must be 2 digits with padding (01-31).");
	//controllo date
	std::string month = date.substr(5, 2);
	std::string day = date.substr(8, 2);
	int m = atoi(month.c_str());
	if (m < 1 || m > 12)
	throw std::runtime_error("Error: invalid month value. Must be between 01-12.");
	int d = atoi(day.c_str());
	if (d < 1 || d > 31)
	throw std::runtime_error("Error: invalid day value. Must be between 01-31.");
	if (m < 10 && month[0] != '0')
	throw std::runtime_error("Error: month must have zero padding (e.g., 01, 02, ..., 09).");
	if (d < 10 && day[0] != '0')  
	throw std::runtime_error("Error: day must have zero padding (e.g., 01, 02, ..., 09).");
	*/
	//data piu vicina
	std::map<std::string, float>::const_iterator it = _db.lower_bound(date); //funziona solo con chiave
	if (it == _db.end() || it->first != date)
	{
		if (it == _db.begin())
			throw std::runtime_error("No earlier date in database.");
		--it;
	}
	return it->second;
}

void BitcoinExchange::processInput(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}
	std::string line;
	std::getline(file, line); // skip header
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string date, val;
		if (!std::getline(ss, date, '|') || !std::getline(ss, val))
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		while (!date.empty() && date[date.size()-1] == ' ') date.erase(date.size()-1);
		while (!val.empty() && val[0] == ' ') val.erase(0,1);
		if (!isValidDate(date))
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (!isValidValue(val))
		{
			float tmp = atof(val.c_str());
			if (tmp < 0)
				std::cerr << "Error: not a positive number." << std::endl;
			else
				std::cerr << "Error: too large a number." << std::endl;
			continue;
		}
		float v = static_cast<float>(atof(val.c_str()));
		try
		{
			float rate = getClosestRate(date);
			std::cout << date << " => " << v << " = " << v * rate << std::endl;
		}
		catch (std::exception &e) 
		{
			std::cerr << e.what() << std::endl;
		}
	}
}
