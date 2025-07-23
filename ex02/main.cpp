#include "PmergeMe.hpp"
#include <sys/time.h>
#include <iostream>
#include <iomanip>

// Funzione ausiliaria per calcolare il tempo trascorso in microsecondi
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
        // Creare l'oggetto PmergeMe e riempire i container dagli argomenti
        std::deque<int> emptyDeque;
        std::vector<int> emptyVector;
        PmergeMe sorter(emptyDeque, emptyVector);
        sorter.fullContainers(argv);

        // Controllare che non ci siano duplicati
        if (!sorter.checkDuplicates())
        {
            throw std::runtime_error("I numeri duplicati non sono ammessi.");
        }

        // Stampare lo stato iniziale dei container
        std::cout << "Before: ";
        sorter.printVectorContainer();

        // Prendere il tempo e ordinare il vector
        struct timeval startVec, endVec;
        gettimeofday(&startVec, NULL);
        
        std::vector<int> sortedVector = sorter.mergeVector(sorter.getVectorContainer());
        
        gettimeofday(&endVec, NULL);
        double timeVec = getTimeElapsed(startVec, endVec);

        // Stampare il vector ordinato
        std::cout << "After:  ";
        for (size_t i = 0; i < sortedVector.size(); ++i)
        {
            std::cout << sortedVector[i];
            if (i < sortedVector.size() - 1)
                std::cout << " ";
        }
        std::cout << std::endl;

        // Stampare il tempo impiegato per il vector
        std::cout << "Time to process a range of " << sortedVector.size()
                  << " elements with std::vector: " << std::fixed << std::setprecision(5) 
                  << timeVec << " us" << std::endl;

        // Prendere il tempo e ordinare il deque
        struct timeval startDeq, endDeq;
        gettimeofday(&startDeq, NULL);
        
        std::deque<int> sortedDeque = sorter.mergeDeque(sorter.getDequeContainer());
        
        gettimeofday(&endDeq, NULL);
        double timeDeq = getTimeElapsed(startDeq, endDeq);

        // Stampare il deque ordinato
        for (size_t i = 0; i < sortedDeque.size(); ++i)
        {
            std::cout << sortedDeque[i];
            if (i < sortedDeque.size() - 1)
                std::cout << " ";
        }
        std::cout << std::endl;

        // Stampare il tempo impiegato per il deque
        std::cout << "Time to process a range of " << sortedDeque.size()
                  << " elements with std::deque:  " << std::fixed << std::setprecision(5) 
                  << timeDeq << " us" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Errore: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

