#include "UBCalculator.h"
#include <sstream>
#include "Lexer.h"
#include <stack>
#include <vector>
#include <iostream>

using namespace std;


const std::map<char, char> UBCalculator::delimMatch = UBCalculator::createDelimMatch();
const  std::map<char, int> UBCalculator::precedenceMap = UBCalculator::createPrecedenceMap();
const std::string UBCalculator::openDelims = "([{";
const std::string UBCalculator::closeDelims = ")]}";


// Evaluate str 
void UBCalculator::setLine(std::string str)
{
 
  //read each token for its value and put them in the approp stacks
  //evaluate the expression as either a result or an error
  //output the result or error

  //Needed: map, vector, stack

  ///make a lexer
  Lexer lex(str);
  ///tokenize lexer
  vector<Token> tokenVec = lex.tokenize();

  ///read each token for its value and put htem int he approp stacks
  stack<Token> valueStack;				//stack declarations
  stack<Token> opStack;
 
  for(size_t i=0; i<tokenVec.size();i++){	//loop through vector and sort tokens by type
     
    bool tokenIsOperator = tokenVec[i].type == OPERATOR;
    bool tokenIsOpenDelim = (openDelims.find(tokenVec[i].value)!=string::npos);
    bool tokenIsCloseDelim = (closeDelims.find(tokenVec[i].value)!=string::npos);
    bool tokenIsValue = tokenVec[i].type == NUMBER;

    if(tokenVec[i].type == OPERATOR || tokenVec[i].type == DELIM){
       
       

       

       if(opStack.empty()|| (openDelims.find(tokenVec[i].value)!=string::npos)){
      
       } 

       else{
         
       }
 
      // opStack.push(tokenVec[i]);

    }

    if(tokenVec[i].type == IDENT || tokenVec[i].type == NUMBER){

       valueStack.push(tokenVec[i]);

    } 

  }  

  cout << "Operators: " << opStack.size() << endl;
  cout << "Identifiers and values: " << valueStack.size() << endl; 



   return;
}
