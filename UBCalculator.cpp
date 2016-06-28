#include "UBCalculator.h"
#include <sstream>
#include "Lexer.h"
#include <stack>
#include <vector>
#include <iostream>

using namespace std;

void checkForErrors(Token &, stack<Token> &, stack<Token> &, vector<Token> &);

const std::map<char, char> UBCalculator::delimMatch = UBCalculator::createDelimMatch();
const  std::map<char, int> UBCalculator::precedenceMap = UBCalculator::createPrecedenceMap();
const std::string UBCalculator::openDelims = "([{";
const std::string UBCalculator::closeDelims = ")]}";


// Evaluate str 
void UBCalculator::setLine(std::string str) {

    //read each token for its value and put them in the approp stacks
    //evaluate the expression as either a result or an error
    //output the result or error

    //Needed: map, vector, stack

    ///make a lexer
    Lexer lex(str);
    ///tokenize lexer
    vector<Token> tokenVec = lex.tokenize();

    ///read each token for its value and put htem int he approp stacks
    stack<Token> numberStack;                //stack declarations
    stack<Token> opStack;

    for (size_t i = 0; i < tokenVec.size(); i++) {    //loop through vector and sort tokens by type

        bool tokenIsOperator = tokenVec[i].type == OPERATOR;
        bool tokenIsOpenDelim = (openDelims.find(tokenVec[i].value) != string::npos);
        bool tokenIsCloseDelim = (closeDelims.find(tokenVec[i].value) != string::npos);
        bool tokenIsNumber = tokenVec[i].type == NUMBER;
        bool tokenIsIdent = tokenVec[i].type == IDENT;
        Token thisToken = tokenVec[i];


        //IF TOKEN IS A NUMBER
        if(tokenIsNumber){

            numberStack.push(thisToken);
        }

        if(tokenIsOperator){
            if(opStack.empty()&&tokenIsCloseDelim){
                //Expression not well formed error goes here
            }
            else if(opStack.empty()&&!tokenIsCloseDelim){
                opStack.push(thisToken);
            }
            else if (opStack.top().value=="=" || (openDelims.find(opStack.top().value)!= string::npos)){
                opStack.push(thisToken);
            }
            else{
                while(precedenceMap.at(thisToken.value) <= precedenceMap.at(opStack.top().value)){

                }
            }


        }

        if(tokenIsIdent){

        }



    }


}

void checkForErrors(Token &thisToken, stack<Token> &numberStack, stack<Token> &opStack, vector<Token> &tokenVec){

    //INSERT ALL POSSIBLE ERROR CASES HERE


}