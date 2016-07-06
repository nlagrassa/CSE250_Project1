#include "UBCalculator.h"
#include <sstream>
#include "Lexer.h"
#include <stack>
#include <vector>
#include <iostream>

using namespace std;

void checkForErrors(Token &, stack<Token> &, stack<Token> &, vector<Token> &); //HAVE NOT DEFINED YET
float opCompute(float &, Token &, float &);
char* opAssignFloat(char *, float &, stack<Token> &, stack<Token> &);
void determineAndRunCorrectComputation(stack<Token> &, stack<float> &);
float returnVariableValue();//DON'T FORGET TO DEFINE THIS FUNCTION
float returnSingleFloatInput(); //SHOULD RETURN 3.2 IF USER INPUTS ONLY 3.2


const std::map<char, char> UBCalculator::delimMatch = UBCalculator::createDelimMatch();
const  std::map<char, int> UBCalculator::precedenceMap = UBCalculator::createPrecedenceMap();
const std::string UBCalculator::openDelims = "([{";
const std::string UBCalculator::closeDelims = ")]}";
const std::string computationOps = "+-*/";


// Evaluate str 
void UBCalculator::setLine(std::string str) {

    //read each token for its value and put them in the approp stacks
    //evaluate the expression as either a result or an error
    //output the result or error

    //Needed: map, vector, stack

    //don't forget to update .h file with all appropriate headers and function descriptions

    ///make a lexer
    Lexer lex(str);
    ///tokenize lexer
    vector<Token> tokenVec = lex.tokenize();

    ///read each token for its value and put htem int he approp stacks
    stack<float> numberStack;                //stack declarations
    stack<Token> opStack;
    stack<Token> assignmentStack;
    stack<Token> delimStack;


    //DO ERROR CHECKING/SYNTAX CHECKING HERE BEFORE ANY COMPUTATION IS DONE. EVERYTHING SHOULD BE CORRECT BY THE TIME WE START TO PUSH TO STACKS AND COMPUTER



    for (size_t i = 0; i < tokenVec.size(); i++) {    //loop through vector and sort tokens by type

        bool tokenIsOperator = tokenVec[i].type == OPERATOR;
        bool tokenIsOpenDelim = (openDelims.find(tokenVec[i].value) != string::npos);
        bool tokenIsCloseDelim = (closeDelims.find(tokenVec[i].value) != string::npos);
        bool tokenIsNumber = tokenVec[i].type == NUMBER;
        bool tokenIsIdent = tokenVec[i].type == IDENT;
        Token thisToken = tokenVec[i];



        //INSERT PRE-PROCESSING METHOD HERE; SHOULD TAKE A TOKEN VECTOR AND RETURN A TOKEN VECTOR IN POSTFIX. Could potentially be a void method.





        //If token contains a number, convert from string to float and push it into the number stack.
        if(tokenIsNumber){
            float tokNum = stof(thisToken.value,0);
            numberStack.push(tokNum);
        }

        //If token contains an operator, determine which operator it is and handle appropriately
        if(tokenIsOperator){
            //If the opStack is empty, push the operator token immediately
            if(opStack.empty()){
                opStack.push(thisToken);
            }
            //If the top of the opStack is "=", push the current op on top of it
                //IS THIS WHAT'S SUPPOSED TO HAPPEN?
            else if (opStack.top().value=="="){
                opStack.push(thisToken);
            }
            else{
                while(!opStack.empty() && (precedenceMap.at(thisToken.value.at(0)) <= precedenceMap.at(opStack.top().value.at(0)))){
                    //store top op in temp variable
                    //compute correctly (write method that decides which computation function to call)(void method)
                    //pop op that is in the way

                    //opStack.pop();

                    determineAndRunCorrectComputation(opStack, numberStack);
                }
                opStack.push(thisToken);
            }


        }

        if(tokenIsOpenDelim){
            opStack.push(thisToken);


        }

        if(tokenIsCloseDelim){
            char thisCloseDelim = thisToken.value.at(0);
            while(opStack.top().value.at(0)!= delimMatch.at(thisCloseDelim)){
                determineAndRunCorrectComputation(opStack, numberStack);
            }
            if(opStack.top().value.at(0)==delimMatch.at(thisCloseDelim)){
                opStack.pop();
            }
        }

        if(tokenIsIdent){

        }



    }

    //Once we reach the end of the input [expression] vector, compute the remainder of the expression, guided by the operators in the opStack
    while(!opStack.empty()){
        determineAndRunCorrectComputation(opStack, numberStack);
    }

    while(!assignmentStack.empty()){
        //define this after defining the assignment method

    }

    //PRINTS THE OUTPUT
    cout << numberStack.top() << endl;


}

void checkForErrors(Token &thisToken, stack<Token> &numberStack, stack<Token> &opStack, vector<Token> &tokenVec){

    //INSERT ALL POSSIBLE ERROR CASES HERE


}

void determineAndRunCorrectComputation(stack<Token> &opStack, stack<float> &numberStack){

    //string computationOps = "+-*/";
    //If the op is +,-,*, or /, temporarily store the two values to be computed and the operator and then compute.
    if(computationOps.find(opStack.top().value)!=string::npos){
        float secondFloat = numberStack.top();
        numberStack.pop();
        float firstFloat = numberStack.top();
        numberStack.pop();
        Token op = opStack.top();
        opStack.pop();
        numberStack.push(opCompute(firstFloat, op, secondFloat));

    }



    //WHAT SHOULD HAPPEN WHEN X=Y=5 IS THE INPUT? WHERE SHOULD Y GO?
    //HOW DOES THE GLOBAL MAP FOR VARIABLES WORK?
//    if(op.value.at(0)=='='){
//        opStack.pop();
//
//
//        //do assignmentCompute
//    }


}


float opCompute(float &firstFloat, Token &op, float &secondFloat){

    if(op.value.at(0)=='+'){
        return firstFloat + secondFloat;
    }
    if(op.value.at(0)=='-'){
        return firstFloat - secondFloat;
    }
    if(op.value.at(0)=='*'){
        return firstFloat * secondFloat;
    }
    if(op.value.at(0)=='/'){
        return firstFloat / secondFloat;
    }

    return secondFloat;
}

//note: 'identStack' could be either opStack or assignmentStack, depending on what is needed
char* opAssignFloat(stack<float> & numberStack, stack<Token> & identStack){

}