#include "PmergeMe.hpp"
#include <sys/time.h>
#include <iostream>
#include <iomanip>

//micro secondi (secondi + micro) micro è solo la parte frazionaria
double getTimeElapsed(struct timeval start, struct timeval end)
{
	long seconds = end.tv_sec - start.tv_sec;
	long microseconds = end.tv_usec - start.tv_usec;
	return seconds * 1000000.0 + microseconds;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Errore: Fornire una sequenza di numeri come argomenti." << std::endl;
		return 1;
	}
	try
	{
		// crea l'oggetto PmergeMe e riempe
		std::deque<int> emptyDeque;
		std::vector<int> emptyVector;
		PmergeMe sorter(emptyDeque, emptyVector);
		sorter.fullContainers(argv);
		// controllo dup
		if (sorter.checkDuplicates() != -1)
		{
			std::cout << sorter.checkDuplicates() << ":";
			throw std::runtime_error("no duplicates allowed");
		}
		// stampa
		std::cout << "before: ";
		sorter.printVectorContainer();
		// prendere il tempo e ordinare il vector
		struct timeval startVec, endVec;
		gettimeofday(&startVec, NULL);
		std::vector<int> sortedVector = sorter.mergeVector(sorter.getVectorContainer());
		gettimeofday(&endVec, NULL);
		double timeVec = getTimeElapsed(startVec, endVec);
		// stampa il vector ordinato
		std::cout << "vector:  ";
		for (size_t i = 0; i < sortedVector.size(); ++i)
		{
			std::cout << sortedVector[i];
			if (i < sortedVector.size() - 1)
				std::cout << " ";
		}
		std::cout << std::endl;
		// tempo
		std::cout << "time to process a range of " << sortedVector.size()
				  << " elements with std::vector: " << std::fixed << std::setprecision(5)
				  << timeVec << " microseconds" << std::endl;
		// deque
		struct timeval startDeq, endDeq;
		gettimeofday(&startDeq, NULL);
		std::deque<int> sortedDeque = sorter.mergeDeque(sorter.getDequeContainer());
		gettimeofday(&endDeq, NULL);
		double timeDeq = getTimeElapsed(startDeq, endDeq);

		// stampa
		std::cout << "deque:  ";
		for (size_t i = 0; i < sortedDeque.size(); ++i)
		{
			std::cout << sortedDeque[i];
			if (i < sortedDeque.size() - 1)
				std::cout << " ";
		}
		std::cout << std::endl;
		// tempo
		std::cout << "time to process a range of " << sortedDeque.size()
				  << " elements with std::deque:  " << std::fixed << std::setprecision(5)
				  << timeDeq << " microseconds" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Errore: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

