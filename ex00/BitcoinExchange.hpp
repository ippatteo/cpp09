#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <string>

class BitcoinExchange
{
	private:
		std::map<std::string, float> _db; // chiave: data, valore: prezzo

	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void loadDatabase(const std::string &filename);
		void processInput(const std::string &filename);

	private:
		bool isValidDate(const std::string &date) const;
		bool isValidValue(const std::string &value) const;
		float getClosestRate(const std::string &date) const;
};

#endif
