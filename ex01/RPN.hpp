#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN 
{
	private:
		std::stack<int> _stack; // contiene i valori temporanei

	public:
		RPN();
		RPN(const RPN &other);
		RPN &operator=(const RPN &other);
		~RPN();
		int evaluate(const std::string &expr);
};

#endif
