#include "PmergeMe.hpp"

PmergeMe::PmergeMe(std::deque<int>deque, std::vector<int>vector) :
	vectorContainer(vector), dequeContainer(deque)
{
}

PmergeMe::~PmergeMe()
{}

PmergeMe::PmergeMe(const PmergeMe& obj) :
	vectorContainer(obj.vectorContainer), dequeContainer(obj.dequeContainer)
{}

PmergeMe& PmergeMe::operator=(const PmergeMe& obj)
{
	this->vectorContainer = obj.vectorContainer;
	this->dequeContainer = obj.dequeContainer;

	return *this;
}

void PmergeMe::fullContainers(char **args)
{
	for (int i = 1; args[i] != nullptr; i++)
	{
		std::string str(args[i]);
		
		// Check if string is empty
		if (str.empty())
			throw std::runtime_error("Empty argument");
			
		// Check for non-digit characters (except leading + sign)
		size_t start = 0;
		if (str[0] == '+')
			start = 1;
			
		if (start == str.length())
			throw std::runtime_error("Invalid number: only sign character");
			
		for (size_t j = start; j < str.length(); j++)
		{
			if (!std::isdigit(str[j]))
				throw std::runtime_error("Invalid character in number");
		}
		
		// Convert to integer
		std::stringstream ss(str);
		int number;
		ss >> number;
		
		// Check if conversion failed or if number is negative
		if (ss.fail() || number < 0)
			throw std::runtime_error("Invalid number or negative value");
			
		// Add to both containers
		vectorContainer.push_back(number);
		dequeContainer.push_back(number);
	}
}

size_t PmergeMe::jacobsthalNumber(unsigned int k)
{
	if (k == 0)
		return (1);
	if (k == 1)
		return (3);
	if (k == 2)
		return (5);
	if (k == 3)
		return (11);
	// Per evitare overflow, limitiamo k a valori ragionevoli
	if (k > 60) // 2^62 è vicino al limite di size_t (64 bit)
		return SIZE_MAX;
	
	// Calcolare 2^(k+2) in modo sicuro
	size_t power_of_2 = static_cast<size_t>(1) << (k + 2); //ogni shift è una potenza di due
	
	// Calcolare (-1)^k: se k è pari = 1, se k è dispari = -1
	int sign = (k % 2 == 0) ? 1 : -1;
	
	// Calcolare (2^(k+2) - (-1)^k) / 3
	size_t result;
	if (sign == 1)
		result = (power_of_2 - 1) / 3;
	else
		result = (power_of_2 + 1) / 3;
	return result;
}

std::vector<int> PmergeMe::mergeVector(std::vector<int> vec)
{
	if (vec.size() <= 1)
	return vec;
	
	std::vector<std::pair<int, int> > pairs;
	int oddNumber = -1;
	bool isOdd = false;
	
	// 1 crea coppie, handle dispari
	for (size_t i = 0; i < vec.size(); i += 2)
	{
		if (i + 1 < vec.size())
		{
			// Assicurarsi che il primo elemento della coppia sia il più grande
			if (vec[i] > vec[i + 1])
				pairs.push_back(std::make_pair(vec[i], vec[i + 1]));
			else
				pairs.push_back(std::make_pair(vec[i + 1], vec[i]));
		}
		else
		{
			oddNumber = vec[i];
			isOdd = true;
		}
	}
	
	//2 Ordinare ricorsivamente i primi elementi
	if (pairs.size() > 1)
	{
		std::vector<int> firsts;
		for (size_t i = 0; i < pairs.size(); ++i)
			firsts.push_back(pairs[i].first);
		firsts = mergeVector(firsts);
		
		// mette i riordinati in un nuovo vettore pairs
		std::vector<std::pair<int, int> > sortedPairs;
		for (size_t i = 0; i < firsts.size(); ++i)
		{
			for (size_t j = 0; j < pairs.size(); ++j)
			{
				if (pairs[j].first == firsts[i])
				{
					sortedPairs.push_back(pairs[j]);
					break;
				}
			}
		}
		pairs = sortedPairs; // Aggiorna la lista
	}
	// 3 Creare la main chain e attaccarci i pendant
	std::vector<int> mainChain;
	std::vector<int> pendant;
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		mainChain.push_back(pairs[i].first);
		pendant.push_back(pairs[i].second);
	}
	
// 4 Inserire i pendant usando la sequenza di Jacobsthal (qua so ca**i)
if (!pendant.empty())
{
	// Inserire il primo elemento pendant all'inizio "gratis" t0
	mainChain.insert(mainChain.begin(), pendant[0]);
	
	if (pendant.size() > 1)
	{
		std::vector<bool> inserted(pendant.size(), false); //inserted è un vector di bool, tiene traccia
		inserted[0] = true; // il primo è già stato inserito
		
		size_t k = 1;
		size_t insertedCount = 1;
		
		while (insertedCount < pendant.size())
		{
			// calcolare tk usando la formula di Jacobsthal tk = (2^(k+2) -(-1)^k) / 3
			size_t tk = jacobsthalNumber(k);
			
			
			if (tk > pendant.size()) //limita se va fuori size
				tk = pendant.size();
			if (tk <= insertedCount)  //evita loop
				break;
			// inserire gli elementi dall'ultimo al primo nell'intervallo
			for (size_t i = tk; i > insertedCount; --i)
			{
				if (i - 1 < pendant.size() && !inserted[i - 1])
				{
					std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pendant[i - 1]);
					mainChain.insert(pos, pendant[i - 1]);
					inserted[i - 1] = true;
				}
			}
			insertedCount = tk;
			k++;
		}
	}
}
// 5 inserire l'elemento dispari se presente
	if (isOdd)
	{
		std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), oddNumber);
		mainChain.insert(pos, oddNumber);
	}
	return mainChain;
}

std::deque<int> PmergeMe::mergeDeque(std::deque<int> deq)
{
	if (deq.size() <= 1)
		return deq;
	
	std::vector<std::pair<int, int> > pairs;
	int oddNumber = -1;
	bool isOdd = false;
	
	// Fase 1: Creare coppie e identificare l'eventuale numero dispari.
	for (size_t i = 0; i < deq.size(); i += 2)
	{
		if (i + 1 < deq.size())
		{
			if (deq[i] > deq[i + 1])
				pairs.push_back(std::make_pair(deq[i], deq[i + 1]));
			else
				pairs.push_back(std::make_pair(deq[i + 1], deq[i]));
		}
		else
		{
			oddNumber = deq[i];
			isOdd = true;
		}
	}
	
	// Fase 2: Ordinare ricorsivamente i primi elementi delle coppie.
	if (pairs.size() > 1)
	{
		std::deque<int> firsts;
		for (size_t i = 0; i < pairs.size(); ++i)
			firsts.push_back(pairs[i].first);
		
		firsts = mergeDeque(firsts);
		
		// Riorganizzare le coppie secondo l'ordine dei 'firsts'.
		std::vector<std::pair<int, int> > sortedPairs;
		for (size_t i = 0; i < firsts.size(); ++i)
		{
			for (size_t j = 0; j < pairs.size(); ++j)
			{
				if (pairs[j].first == firsts[i])
				{
					sortedPairs.push_back(pairs[j]);
					break;
				}
			}
		}
		pairs = sortedPairs;
	}
	std::deque<int> mainChain;
	std::deque<int> pendant;
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		mainChain.push_back(pairs[i].first);
		pendant.push_back(pairs[i].second);
	}
	if (!pendant.empty())
	{
		std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pendant[0]);
		mainChain.insert(pos, pendant[0]);
		
		if (pendant.size() > 1)
		{
			std::vector<bool> inserted(pendant.size(), false);
			inserted[0] = true;
			
			size_t k = 1;
			size_t insertedCount = 1;
			
			while (insertedCount < pendant.size())
			{
				size_t tk = jacobsthalNumber(k);
				if (tk > pendant.size())
					tk = pendant.size();
				if (tk <= insertedCount)
					break;
				
				for (size_t i = tk; i > insertedCount; --i)
				{
					if (i - 1 < pendant.size() && !inserted[i - 1])
					{
						std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pendant[i - 1]);
						mainChain.insert(pos, pendant[i - 1]);
						inserted[i - 1] = true;
					}
				}
				insertedCount = tk;
				k++;
			}
		}
	}
	if (isOdd)
	{
		std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), oddNumber);
		mainChain.insert(pos, oddNumber);
	}
	
	return mainChain;
}

bool PmergeMe::checkDuplicates()
{
	for (size_t i = 0; i < vectorContainer.size(); ++i)
	{
		for (size_t j = i + 1; j < vectorContainer.size(); ++j)
		{
			if (vectorContainer[i] == vectorContainer[j])
			{
				return false; 
			}
		}
	}
	return true; 
}

void PmergeMe::printVectorContainer() const
{
	for (size_t i = 0; i < vectorContainer.size(); ++i)
	{
		std::cout << vectorContainer[i];
		if (i < vectorContainer.size() - 1)
			std::cout << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::printDequeContainer() const
{
	for (size_t i = 0; i < dequeContainer.size(); ++i)
	{
		std::cout << dequeContainer[i];
		if (i < dequeContainer.size() - 1)
			std::cout << " ";
	}
	std::cout << std::endl;
}

const std::vector<int>& PmergeMe::getVectorContainer() const
{
	return vectorContainer;
}

const std::deque<int>& PmergeMe::getDequeContainer() const
{
	return dequeContainer;
}