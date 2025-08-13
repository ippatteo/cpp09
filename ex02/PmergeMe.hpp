#pragma once

#include <iostream>
#include <sstream>
#include <exception>
#include <iterator>
#include <vector>
#include <deque>
#include <sys/time.h>
#include <algorithm>
#include <utility>
#include <iomanip>
#include <ctime>
#include <cstddef>
#include <climits>


class PmergeMe
{
	public:
		std::vector<int> vectorContainer;
		std::deque<int> dequeContainer;
		PmergeMe(std::deque<int> dequeAlgorithm, std::vector<int> vectorAlgorithm);
		~PmergeMe();
		PmergeMe(const PmergeMe& obj);
		PmergeMe& operator=(const PmergeMe& obj);
		void fullContainers(char **args);
		static size_t jacobsthalNumber(unsigned int k);
		std::vector<int> mergeVector(std::vector<int> vec);
		std::deque<int> mergeDeque(std::deque<int> deq);
		int checkDuplicates();
		void printVectorContainer() const;
		void printDequeContainer() const;
		const std::vector<int>& getVectorContainer() const;
		const std::deque<int>& getDequeContainer() const;
};