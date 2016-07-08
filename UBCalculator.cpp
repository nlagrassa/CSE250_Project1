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
    bool doneAssignment;




    if(checkForErrors(tokenVec)){

        if(tokenVec.size()==1){
            if(tokenVec[0].type==IDENT){
                float value = identMap.at(tokenVec[0].value);
                cout << value << endl;
            }
            else{
                cout << tokenVec[0].value << endl;
            }

        }

            //For cases where "-x" or "-5" is entered
        else if(tokenVec.size()==2 && tokenVec[0].value == "-" && (tokenVec[1].type==IDENT||tokenVec[1].type==NUMBER)){
            if(tokenVec[1].type==IDENT){
                float value = identMap.at(tokenVec[1].value);
                value = value*(-1);
                cout << value << endl;
            }
            else{
                float value = stof(tokenVec[1].value);
                value = value*(-1);
                cout << value << endl;
            }


        }

        else{


            numberStack.push(0);

            for (size_t i = 0; i < tokenVec.size(); i++) {    //loop through vector and sort tokens by type

                bool tokenIsOperator = tokenVec[i].type == OPERATOR;
                bool tokenIsOpenDelim = (openDelims.find(tokenVec[i].value) != string::npos);
                bool tokenIsCloseDelim = (closeDelims.find(tokenVec[i].value) != string::npos);
                bool tokenIsNumber = tokenVec[i].type == NUMBER;
                bool tokenIsIdent = tokenVec[i].type == IDENT;
                Token thisToken = tokenVec[i];

                doneAssignment=false;




                //If token contains a number, convert from string to float and push it into the number stack.
                if(tokenIsNumber){
                    float tokNum = stof(thisToken.value);
                    numberStack.push(tokNum);
                }

                else if(thisToken.value=="="){
                    assignmentStack.push(tokenVec[i-1]);
                }
                //If token contains an operator, determine which operator it is and handle appropriately
                else if(tokenIsOperator&&thisToken.value!="="){
                    //If the opStack is empty, push the operator token immediately
                    if(opStack.empty()){
                        opStack.push(thisToken);
                    }

                        //NEED TO ACCOUNT FOR OPERATIONS USING ONE OR TWO DEFINED IDENTS
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
                else if(tokenIsOpenDelim){
                    opStack.push(thisToken);
                }
                else if(tokenIsCloseDelim){
                    char thisCloseDelim = thisToken.value.at(0);
                    while(opStack.top().value.at(0)!= delimMatch.at(thisCloseDelim)){
                        determineAndRunCorrectComputation(opStack, numberStack);
                    }
                    if(opStack.top().value.at(0)==delimMatch.at(thisCloseDelim)){
                        opStack.pop();
                    }
                }
                else if(tokenIsIdent){
                    //check to see if var is being assigned or used for computation
                    if((i<tokenVec.size()-1) && (tokenVec[i+1].value=="=")){
                        assignmentStack.push(thisToken);
                    }
                    else{
                        if(identMap.find(thisToken.value)!=identMap.end()){

                            float value = identMap.at(thisToken.value);

                            numberStack.push(value);
                        }
                        else{
                            assignmentStack.push(thisToken);
                        }
                    }

                }

            }

            //Once we reach the end of the input [expression] vector, compute the remainder of the expression, guided by the operators in the opStack
            while(!opStack.empty()){
                determineAndRunCorrectComputation(opStack, numberStack);
            }

            while(!assignmentStack.empty()){
                string key = assignmentStack.top().value;
                float value = numberStack.top();
                identMap[key] = value;
                assignmentStack.pop();
                doneAssignment=true;

            }

            //PRINTS THE OUTPUT
            if(!doneAssignment){
                cout << numberStack.top() << endl;
            }


        }




    }





}

bool UBCalculator::checkForErrors(vector<Token> &tokenVec){



    enum tokenOrder {
        base, initialIdent, identEquals, number, openDelim, multOrDiv, add, sub, definedIdent, closeDelim, leadingAdd, leadingSub
    };
    stack<char> openDelimStack;
    stack<size_t> removeStack;

    int state2 = 0;
    size_t indexAfterEquals = 0;
    for(size_t i=0; i<tokenVec.size();i++){
        Token thisToken = tokenVec[i];

        switch(state2) {

            case 0: {
                if (thisToken.type == IDENT) {
                    state2 = 1;
                    break;
                }
                else{
                    state2=0;
                    break;
                }
            }

            case 1:{
                if(thisToken.value=="="){
                    state2 = 2;
                    break;
                }
                else{
                    indexAfterEquals = i;
                    state2 = 10;
                    break;
                }
            }

            case 2: {
                if(thisToken.type== IDENT){
                    state2=1;
                    break;
                }
                else{
                    state2 = 10;
                    indexAfterEquals = i;
                    break;
                }


            }
            case 10:{
                state2 = 10;
                break;
            }
        }
    }

    for(size_t i=indexAfterEquals; i<tokenVec.size();i++){
        if(tokenVec[i].type==IDENT && (identMap.find(tokenVec[i].value)!=identMap.end())){
            float value = identMap.at(tokenVec[i].value);
            tokenVec[i].type = NUMBER;

            stringstream s;
            string str;
            s << value;
            s >> str;
            tokenVec[i].value = str;

        }
    }


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

        for(size_t i=0; i<tokenVec.size(); i++){
            Token thisToken = tokenVec[i];

            switch(state){

                case base: {
                    if(thisToken.type==IDENT){
                        state = initialIdent;
                        break;

                    }
                    else if(thisToken.type==NUMBER){
                        state = number;
                        break;

                    }
                    else if(thisToken.type==DELIM){
                        if(openDelims.find(thisToken.value)!= string::npos){
                            state = openDelim;
                            openDelimStack.push(thisToken.value.at(0));
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << syntaxError() << endl;
                            cout << "Case base" << endl;
                            return false;
                        }
                    }
                    else if(thisToken.value=="+"){
                        state = leadingAdd;
                        break;
                    }
                    else if (thisToken.value=="-"){
                        state = leadingSub;
                        break;
                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        cout << "Case base" << endl;
                        return false;
                    }
                }

                //First token is IDENT (100)
                case initialIdent:{
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
                            cout << variableUndefinedError(tokenVec[i-1].value) << endl;
                            cout << "Case initialIdent" << endl;
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
                            cout << variableUndefinedError(tokenVec[i-1].value) << endl;
                            cout << "Case initialIdent" << endl;
                            return false;
                        }
                    }
                    else if(thisToken.value=="*"||thisToken.value=="/"){
                        if(identMap.find(tokenVec[i-1].value)!=identMap.end()){
                            state = multOrDiv;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << variableUndefinedError(tokenVec[i-1].value) << endl;
                            cout << "Case initialIdent" << endl;
                            return false;
                        }

                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        cout << "Case initialIdent" << endl;
                        return false;
                    }

                }
                //**case 101 is for IDENT, '='
                case identEquals:{
                    if(thisToken.type==NUMBER){
                        state = number;
                        break;
                    }
                    else if(thisToken.type==IDENT){

                        //if we're at the end of the vector
                        if(i==(tokenVec.size()-1)){
                            if(identMap.find(thisToken.value)==identMap.end()){
                                cout << error() << endl;
                                cout << variableUndefinedError(thisToken.value) << endl;
                                cout << "Case initialEquals" << endl;
                                return false;
                            }
                            else{
                                //state = definedIdent;
                                //break;
                                return true;
                            }
                        }
                        else{
                            if(tokenVec[i+1].value=="="){
                                state = identEquals;
                                break;
                            }
                            else if(identMap.find(thisToken.value)==identMap.end()){
                                cout << error() << endl;
                                cout << variableUndefinedError(thisToken.value) << endl;
                                cout << "Case initialEquals" << endl;
                                return false;
                            }
                            else{
                                cout << error() << endl;
                                cout << syntaxError() << endl;
                                cout << "Case initialEquals" << endl;
                                return false;
                            }
                        }

                    }
                    else if(thisToken.type==DELIM){
                        if(openDelims.find(thisToken.value)!= string::npos){
                            state = openDelim;
                            openDelimStack.push(thisToken.value.at(0));
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << notWellFormedError() << endl;
                            cout << "Case initialEquals" << endl;
                            return false;
                        }

                    }
                    else{
                        cout << error()<<endl;
                        cout << syntaxError() <<endl;
                        cout << "Case initialEquals" << endl;
                        return false;
                    }
                }

                case definedIdent:{
                    if(thisToken.value=="+"){
                        state = add;
                        break;
                    }
                    else if(thisToken.value=="-"){
                        state = sub;
                        break;
                    }
                    else if(thisToken.type==DELIM && closeDelims.find(thisToken.value)!=string::npos){
                        if(openDelimStack.empty()){
                            cout << error() << endl;
                            cout << syntaxError() << endl;
                            cout << "Case definedIdent" << endl;
                            return false;
                        }
                        else if(openDelimStack.top()==delimMatch.at(thisToken.value.at(0))){
                            state = closeDelim;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << notWellFormedError() << endl;
                            cout << "Case definedIdent" << endl;
                            return false;
                        }

                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        cout << "Case definedIdent" << endl;
                        return false;
                    }

                }









                //First token is NUMBER
                case number: {
                    if(thisToken.value=="="){               //= must come before operator condition because = is an operator
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        cout << "Case number (1st)" << endl;
                        cout << "Prev tokValue: " << tokenVec[i-1].value << endl;
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
                    else if(thisToken.value=="*"||thisToken.value=="/"){
                        state = multOrDiv;
                        break;
                    }
                    else if(closeDelims.find(thisToken.value)!=string::npos){
                        if(!openDelimStack.empty() && openDelimStack.top()==delimMatch.at(thisToken.value.at(0))){
                            state = closeDelim;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << notWellFormedError() << endl;
                            cout << "Case number" << endl;
                            return false;
                        }
                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        cout << "Case number (2nd)"  << endl;
                        return false;
                    }

                }








                //First token is openDELIM
                case openDelim: {
                    if(thisToken.type==IDENT){
                        //check to see if ident is defined, then do something
                        if(identMap.find(thisToken.value)!=identMap.end()){
                            state = definedIdent;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << variableUndefinedError(thisToken.value) << endl;
                            cout << "Case openDelim" << endl;
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
                        cout << "Case openDelim" << endl;
                        return false;
                    }
                }

                case closeDelim: {
                    if(thisToken.value=="+"){
                        state = add;
                        break;
                    }
                    else if(thisToken.value=="-"){
                        state = sub;
                        break;
                    }
                    else if(thisToken.value=="*"||thisToken.value=="/"){
                        state = multOrDiv;
                        break;
                    }
                    else if (thisToken.type==DELIM){
                        if(closeDelims.find(thisToken.value)!=string::npos){
                            state = closeDelim;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << notWellFormedError()<<endl;
                            cout << "Case closeDelim" << endl;
                            return false;
                        }
                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        cout << "Case closeDelim" << endl;
                        return false;
                    }
                }






                //previous token was * or /
                case multOrDiv: {
                    if(thisToken.type==DELIM){
                        if(openDelims.find(thisToken.value) != string::npos){
                            openDelimStack.push(thisToken.value.at(0));
                            state = openDelim;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << notWellFormedError() << endl;
                            cout << "Case multOrDiv" << endl;
                            return false;
                        }
                    }
                    else if(thisToken.type==IDENT){
                        if(identMap.find(thisToken.value) == identMap.end()){
                            cout << error() << endl;
                            cout << variableUndefinedError(thisToken.value) << endl;
                            cout << "Case multOrDiv" << endl;
                            return false;
                        }
                        else{
                            state = definedIdent;
                            break;
                        }
                    }
                    else if(thisToken.type==NUMBER){
                        state = number;
                        break;
                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        cout << "Case multOrDiv" << endl;
                        return false;
                    }

                }

                case leadingAdd:{
                    if(thisToken.value=="+"){
                        removeStack.push(i-1);
                        state = leadingAdd;
                        break;
                    }
                    else if(thisToken.value=="-"){
                        removeStack.push(i-1);
                        state = leadingSub;
                        break;
                    }
                    else if(thisToken.type==NUMBER){
                        removeStack.push(i-1);
                        state = number;
                        break;
                    }
                    else if(thisToken.type==DELIM){
                        if(openDelims.find(thisToken.value)!=string::npos){
                            //could have the + removed from the vector here, but not necessary as long as adding 0 to numberStack works
                            state = openDelim;
                            openDelimStack.push(thisToken.value.at(0));
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << notWellFormedError() << endl;
                            cout << "Case leadingAdd" << endl;
                            return false;
                        }
                    }
                    else if(thisToken.type==IDENT){
                        if(identMap.find(thisToken.value)!=identMap.end()){
                            removeStack.push(i-1);
                            state = definedIdent;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << variableUndefinedError(thisToken.value) << endl;
                            cout << "Case leadingAdd" << endl;
                            return false;
                        }
                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        cout << "Case leadingAdd" << endl;
                        return false;
                    }


                }
                case leadingSub: {
                    if (thisToken.value == "+") {
                        removeStack.push(i - 1);
                        tokenVec[i].value = "-";
                        state = leadingSub;
                        break;
                    }
                    else if (thisToken.value == "-") {
                        removeStack.push(i - 1);
                        tokenVec[i].value = "+";
                        state = leadingAdd;
                        break;
                    }
                    else if (thisToken.type == NUMBER) {
                        removeStack.push(i - 1);

                        float valueFloat = stof(thisToken.value);
                        valueFloat = (valueFloat * (-1));
                        //MAY NOT AUTOMATICALLY CONVERT FLOAT TO STRING, MAY HAVE TO USE STRINGSTREAM FIRST
                        stringstream s;
                        string str;

                        s << valueFloat;
                        s >> str;
                        tokenVec[i].value = str;

                        state = number;
                        break;
                    }
                    else if (thisToken.type == DELIM) {
                        if (openDelims.find(thisToken.value) != string::npos) {
                            state = openDelim;
                            openDelimStack.push(thisToken.value.at(0));
                            break;
                        }
                        else {
                            cout << error() << endl;
                            cout << notWellFormedError() << endl;
                            cout << "Case leadingSub" << endl;
                            return false;
                        }
                    }
                    else if (thisToken.type == IDENT) {
                        if (identMap.find(thisToken.value) != identMap.end()) {
                            //removeStack.push(i-1);
                            //Don't want "-" removed because calc should print 0-x
                            state = definedIdent;
                            break;
                        }
                        else {
                            cout << error() << endl;
                            cout << variableUndefinedError(thisToken.value) << endl;
                            cout << "Case leadingSub" << endl;
                            return false;
                        }
                    }
                    else {
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        cout << "Case leadingSub" << endl;
                        return false;
                    }

                }
//                case negateParens:{
//
//                }
                case add:{
                    if(thisToken.value=="+"){
                        removeStack.push(i-1);
                        state = add;
                        break;
                    }
                    else if(thisToken.value=="-"){
                        removeStack.push(i-1);
                        state = sub;
                        break;
                    }
                    else if(thisToken.type==IDENT){
                        if(identMap.find(thisToken.value)==identMap.end()){
                            cout << error() << endl;
                            cout << variableUndefinedError(thisToken.value) << endl;
                            cout << "Case add" << endl;
                            return false;
                        }
                        else{
                            state = definedIdent;
                            break;
                        }
                    }
                    else if(thisToken.type==DELIM){
                        if(openDelims.find(thisToken.value)!=string::npos){
                            openDelimStack.push(thisToken.value.at(0));
                            state = openDelim;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << syntaxError() << endl;
                            cout << "Case add" << endl;
                            return false;
                        }
                    }
                    else if(thisToken.type==NUMBER){
                        state = number;
                        break;
                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        cout << "Case add" << endl;
                        return  false;
                    }

                }

                case sub:{
                    if(thisToken.value=="+"){
                        removeStack.push(i-1);
                        tokenVec[i].value = "-";
                        state = sub;
                        break;
                    }
                    else if(thisToken.value=="-"){
                        removeStack.push(i-1);
                        tokenVec[i].value = "+";
                        state = add;
                        break;
                    }
                    else if(thisToken.type==IDENT){
                        if(identMap.find(thisToken.value)==identMap.end()){
                            cout << error() << endl;
                            cout << variableUndefinedError(thisToken.value) << endl;
                            cout << "Case sub" << endl;
                            return false;
                        }
                        else{
                            state = definedIdent;
                            break;
                        }
                    }
                    else if(thisToken.type==DELIM){
                        if(openDelims.find(thisToken.value)!=string::npos){
                            openDelimStack.push(thisToken.value.at(0));
                            state = openDelim;
                            break;
                        }
                        else{
                            cout << error() << endl;
                            cout << syntaxError() << endl;
                            cout << "Case sub" << endl;
                            return false;
                        }
                    }
                    else if(thisToken.type==NUMBER){
                        state = number;
                        break;
                    }
                    else{
                        cout << error() << endl;
                        cout << syntaxError() << endl;
                        cout << "Case sub" << endl;
                        return  false;
                    }
                }
            }
        }


        while(!removeStack.empty()){
            tokenVec.erase(tokenVec.begin()+removeStack.top());
            removeStack.pop();
        }

        cout << "You reached the end of pre-processing!" << endl;
        for(size_t i = 0; i< tokenVec.size();i++){
            cout << "Item "<< i << ": " << tokenVec[i].value << endl;
        }


        bool success = (state==initialIdent || state==number || state==definedIdent || state==closeDelims);
        if(success){
            return true;
        }
        else{
            return false;
        }

    }




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
        if(op.value=="/"&&secondFloat==0){
            throw DBZ_Exception();

        }
        else{
            numberStack.push(opCompute(firstFloat, op, secondFloat));
        }

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
