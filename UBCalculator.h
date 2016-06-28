// ============================================================================ 
// Calculator.h
// ~~~~~~~
// author    : Tamal T. Biswas
// - a simple calculator class
// - The class can evaluate any infix expression. 
// ============================================================================ 

#ifndef _UBCALCULATOR_H_
#define _UBCALCULATOR_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include "Lexer.h"
#include "error_handling.h"



/**
 * -----------------------------------------------------------------------------
 * the Calculator class:
 * - take a string to be scanned
 * - scan for tokens and return one at a time
 * -----------------------------------------------------------------------------
 */
class UBCalculator {
public:
	// constructor
	UBCalculator(){};

	// a couple of modifiers
	void setLine(std::string);
private:
   static std::map<char, char> createDelimMatch()
	{
		std::map<char, char> m;
		m[')'] = '(';
		m['}'] = '{';
		m[']'] = '[';
		return m;
	}
	static std::map<char, int> createPrecedenceMap()
	{
		std::map<char, int> m;
		m['+'] = 0;
		m['-'] = 0;
		m['*'] = 1;
		m['/'] = 1;
		m['('] = -1;
		m['{'] = -1;
		m['['] = -1;

		return m;
	}

   const static std::map<char, int> precedenceMap;
	const static std::map<char, char> delimMatch;
	const static std::string openDelims; 
	const static std::string closeDelims;
};

#endif
