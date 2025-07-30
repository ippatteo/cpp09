#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cctype>

RPN::RPN()
{
}

RPN::~RPN()
{
}

RPN::RPN(const RPN &o) : _stack(o._stack)
{
}

RPN &RPN::operator=(const RPN &o)
{
	if (this != &o) _stack = o._stack;
	return *this;
}

int RPN::evaluate(const std::string &expr)
{
	std::istringstream iss(expr);
	std::string token;
	while (iss >> token) 
	{
		if (token.size() == 1 && std::isdigit(token[0]))
			_stack.push(token[0] - '0');
		else if (token == "+" || token == "-" || token == "*" || token == "/") 
		{
			if (_stack.size() < 2)
				throw std::runtime_error("Error");
			int b = _stack.top(); 
			_stack.pop();
			int a = _stack.top();
			_stack.pop();
			int res;
			if (token == "+") res = a + b;
			else if (token == "-") res = a - b;
			else if (token == "*") res = a * b;
			else 
			{
				if (b == 0)
					throw std::runtime_error("Error");
				res = a / b;
			}
			_stack.push(res);
		} 
		else 
			throw std::runtime_error("Error");
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Error");
	return _stack.top();
}
