#include "UBCalculator.h"
#include <sstream>
#include "Lexer.h"
#include <stack>
#include <vector>
#include <iostream>

using namespace std;

bool checkForErrors(vector<Token> &);
float opCompute(float &, Token &, float &);
char* opAssignFloat(char *, float &, stack<Token> &, stack<Token> &);
void determineAndRunCorrectComputation(stack<Token> &, stack<float> &);
float returnVariableValue();//DON'T FORGET TO DEFINE THIS FUNCTION
float returnSingleFloatInput(); //SHOULD RETURN 3.2 IF USER INPUTS ONLY 3.2
string syntaxError();
string variableUndefinedError(string);
string divideByZeroError();
string notWellFormedError();
string error();


const std::map<char, char> UBCalculator::delimMatch = UBCalculator::createDelimMatch();
const  std::map<char, int> UBCalculator::precedenceMap = UBCalculator::createPrecedenceMap();
const std::string UBCalculator::openDelims = "([{";
const std::string UBCalculator::closeDelims = ")]}";
const std::string computationOps = "+-*/";

std::map<string,float> identMap;


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
    stack<string> delimStack;


    //DO ERROR CHECKING/SYNTAX CHECKING HERE BEFORE ANY COMPUTATION IS DONE. EVERYTHING SHOULD BE CORRECT BY THE TIME WE START TO PUSH TO STACKS AND COMPUTER


    if(checkForErrors(tokenVec)){

        if(tokenVec.size()==1){
            cout << "There's only one item in here!" << endl;
        }

        else{

            for (size_t i = 0; i < tokenVec.size(); i++) {    //loop through vector and sort tokens by type

                bool tokenIsOperator = tokenVec[i].type == OPERATOR;
                bool tokenIsOpenDelim = (openDelims.find(tokenVec[i].value) != string::npos);
                bool tokenIsCloseDelim = (closeDelims.find(tokenVec[i].value) != string::npos);
                bool tokenIsNumber = tokenVec[i].type == NUMBER;
                bool tokenIsIdent = tokenVec[i].type == IDENT;
                Token thisToken = tokenVec[i];





                //If token contains a number, convert from string to float and push it into the number stack.
                if(tokenIsNumber){
                    float tokNum = stof(thisToken.value,0);
                    numberStack.push(tokNum);
                }

                //If token contains an operator, determine which operator it is and handle appropriately
                if(tokenIsOperator){
                    //If the opStack is empty, push the operator token immediately
                    if(opStack.empty()){

                        if(thisToken.value=="="){

                        }
                        else{
                            opStack.push(thisToken);
                        }

                    }
                        //If the top of the opStack is "=", push the current op on top of it
                        //IS THIS WHAT'S SUPPOSED TO HAPPEN?
                    else if (opStack.top().value=="="){

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
                    assignmentStack.push(thisToken);

                }



        cout << "Item " << i << ":" << endl;

        if(!numberStack.empty()){
            cout<< "Top of numStack: " << numberStack.top() << endl;
        }
        if(!opStack.empty()){
            cout << "Top of opStack: " << opStack.top().value << endl;
        }
        if(!assignmentStack.empty()){
            cout << "Top of assignmentStack: " << assignmentStack.top().value << endl;
        }



            }

            //Once we reach the end of the input [expression] vector, compute the remainder of the expression, guided by the operators in the opStack
            while(!opStack.empty()){
                determineAndRunCorrectComputation(opStack, numberStack);



        if(!numberStack.empty()){
            cout<< "Top of numStack: " << numberStack.top() << endl;
        }
        if(!opStack.empty()){
            cout << "Top of opStack: " << opStack.top().value << endl;
        }
        if(!assignmentStack.empty()){
            cout << "Top of assignmentStack: " << assignmentStack.top().value << endl;
        }
            }

            while(!assignmentStack.empty()){
                string key = assignmentStack.top().value;
                float value = numberStack.top();
                identMap[key] = value;
                assignmentStack.pop();
                cout << "Found! " << identMap[key] << endl;




        if(!numberStack.empty()){
            cout<< "Top of numStack: " << numberStack.top() << endl;
        }
        if(!opStack.empty()){
            cout << "Top of opStack: " << opStack.top().value << endl;
        }
        if(!assignmentStack.empty()){
            cout << "Top of assignmentStack: " << assignmentStack.top().value << endl;
        }

            }

            //PRINTS THE OUTPUT
            //cout << numberStack.top() << endl;

        }




    }





}

bool UBCalculator::checkForErrors(vector<Token> &tokenVec){
    //INSERT ALL POSSIBLE ERROR CASES HERE

    /*RULES:
     * 1. Assignments must happen on left and consecutively
     * 2. IDENTS must be assigned something or be defined already in identMap
     * 3. Deliminators must all match up correctly (use stack to see)
     * 4. ++++++ must simplify to '+'
     * 5. -- must simplify to '-'
     * 6. -3 must evaluate to (-3) in string form in the token
     * 7. Something about +-+-+-
     * 8. ...
    *
    */


    enum tokenOrder {
        base, ident, identEquals, number, openDelim
    };
    stack<char> openDelimStack;


    //Covers cases where the user inputs a single tokenized item
    if(tokenVec.size()==1){
        if(tokenVec[0].type==IDENT){

            if(identMap.find(tokenVec[0].value) == identMap.end()){
                cout << error() << endl;
                cout << variableUndefinedError(tokenVec[0].value) << endl;
                return false;
            }
            return true;

        }
        else if(tokenVec[0].type==NUMBER){
            return true;

        }
        else{
            cout << error() << endl;
            cout << syntaxError() << endl;
            return false;


        }
    }

    else{

        int state = base;

        for(int i=0; i<tokenVec.size(); i++){
            Token thisToken = tokenVec[i];

            switch(state){

                case base: {
                    if(thisToken.type==IDENT){
                        state = ident;
                        break;

                    }
                    else if(thisToken.type==NUMBER){
                        state = number;
                        break;

                    }
                    else if(thisToken.type==DELIM){
                        if(openDelims.find(thisToken.value.at(0))!= string::npos){
                            state = openDelim;
                            openDelimStack.push(thisToken.value.at(0));
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << syntaxError() << endl;
                            return false;
                        }
                    }
                    else if(thisToken.value=="+"){
                        state = add;
                        break;
                    }
                    else if (thisToken.value=="-"){
                        state = sub;
                        break;
                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        return false;
                    }
                }

                //First token is IDENT (100)
                case ident:{
                    if(thisToken.value == "="){
                        state = identEquals;
                        break;
                    }
                    else if(thisToken.value=="+"){
                        if(identMap.find(tokenVec[i-1].value)!=identMap.end()){
                            state = add;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << variableUndefinedError(thisToken.value) << endl;
                            return false;
                        }
                    }
                    else if(thisToken.value=="-"){
                        if(identMap.find(tokenVec[i-1].value)!=identMap.end()){
                            state = sub;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << variableUndefinedError(thisToken.value) << endl;
                            return false;
                        }
                    }
                    else if(thisToken.type==OPERATOR){
                        if(identMap.find(tokenVec[i-1].value)!=identMap.end()){
                            state = identOp;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << variableUndefinedError(thisToken.value) << endl;
                            return false;
                        }

                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        return false;
                    }

                }
                //**case 101 is for IDENT, '='
                case identEquals:{
                    if(thisToken.type==NUMBER){
                        state = equalsNumber;
                        break;
                    }
                    else if(thisToken.type==IDENT){
                        state = equalsIdent;
                        break;
                    }
                    else if(thisToken.value=="+"){
                        state = add;
                        break;

                    }
                    else if(thisToken.value == "-"){
                        state = sub;
                        break;

                    }
                    else if(thisToken.type==DELIM){
                        char thisDelim = thisToken.value.at(0);
                        if(openDelims.find(thisDelim)!= string::npos){
                            state = equalOpenDelim;
                            openDelimStack.push(thisToken.value.at(0));
                            break;

                        }
                        else{
                            if(!openDelimStack.empty()&&openDelimStack.top()==delimMatch.at(thisDelim)){
                                state = validCloseDelim;
                            }
                            else{
                                cout << error() << endl;
                                cout << notWellFormedError() << endl;
                                return false;
                            }

                        }
                    }
                    else{
                        cout << error()<<endl;
                        cout << syntaxError() <<endl;
                        return false;
                    }
                }







                //First token is NUMBER
                case number: {
                    if(thisToken.value=="="){               //= must come before operator condition because = is an operator
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        return false;
                    }
                    else if(thisToken.value=="+"){
                        state = add;
                        break;
                    }
                    else if(thisToken.value=="-"){
                        state = sub;
                        break;
                    }
                    else if(thisToken.type==OPERATOR){
                        state = numberOp;
                        break;
                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        return false;
                    }

                }








                //First token is openDELIM
                case openDelim: {
                    if(thisToken.type==IDENT){
                        //check to see if ident is defined, then do something
                        if(identMap.find(thisToken.value)!=identMap.end()){
                            state = ident;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << variableUndefinedError(thisToken.value) << endl;
                            return false;
                        }
                    }
                    else if(thisToken.type==NUMBER){
                        state = number;
                        break;
                    }
                    else if(openDelims.find(thisToken.value)!=string::npos){
                        openDelimStack.push(thisToken.value.at(0));
                        state = openDelim;
                        break;
                    }
                    else if(thisToken.value=="+"){
                        state = add;
                        break;
                    }
                    else if (thisToken.value=="-"){
                        state = sub;
                        break;
                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        return false;
                    }


                }






                //First token is OPERATOR
                case op: {

                }
                //**Case 401 covers IDENT, OP







                //True case (may not be needed)
                case 900: {

                }

                default:{
                    cout << "You probably forgot to define a called case!" << endl;
                    return false;
                }



            }



        }

    }

    return true;

    //EITHER HAVE SWITCH LOOK BACKWARDS FOR SYNTAX CHECKING OR HAVE A LIST OF CASES THAT ARE OKAY TO BE IN UPON ITERATOR COMPLETION


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

string error(){
    return "** ERROR **";
}
string syntaxError(){
    return "Syntax Error";
}
string variableUndefinedError(string var){
    return "Variable "+var+" undefined";
}
string divideByZeroError(){
    return "Divide by zero";
}
string notWellFormedError(){
    return "Syntax Error: Expression is not well formed";
}
