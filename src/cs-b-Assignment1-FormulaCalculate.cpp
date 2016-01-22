#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"
#include "stack.h"
#include "tokenscanner.h"

using namespace std;

bool isDigitVariale(string &token){
    return (token[0] >= '0' && token[0] <= '9' || isalpha(token[0]));//result;
}

bool isOperatorFunction(string &token, Map<string, int> &priorityExpressionsMap){

    for (string key: priorityExpressionsMap){
        if (priorityExpressionsMap.get(token) == 0) break;
        if (token == key) return true;
    }
    return false;
}
int getPrioriti(string &token, Map<string, int> &priorityExpressionsMap){
    cout<<"= "<<token<<" ="<<"prioriti = "<< priorityExpressionsMap.get(token)<<endl;
    return priorityExpressionsMap.get(token);
}

string addScopeToResultString(Stack<string> &stackOperators, Map<string, int> &expressionsMap){
    string result, tokenToDell;
    cout<<"addScopeToResultString"<<endl;
    string lastOperator;
    while (true){
        if (!stackOperators.isEmpty()) lastOperator = stackOperators.peek();
        if ( lastOperator[0] == '('){
            cout<<"Skope close find"<< "now to stack= "<<stackOperators<<endl;
            break;
        }
        result +=stackOperators.pop() + ' ';

    }

    tokenToDell = stackOperators.pop();
    return result;
}

void addOperatorToStackOrResult(string &token,
                                Map<string, int> &expressionsMap,
                                Stack<string> &stackOperators,
                                string &resultString){

    cout<<"addOperatorToStackOrResult"<<endl;
    string previousOperator;
    int operatorPriority = getPrioriti(token, expressionsMap);
    if (!stackOperators.isEmpty()) previousOperator = stackOperators.peek();

    if (stackOperators.isEmpty() || getPrioriti(previousOperator, expressionsMap) < operatorPriority){
        stackOperators.push(token);
        cout<<"operator < prioriti = now to stack= "<<stackOperators<<endl;
    } else {

        while (!stackOperators.isEmpty() && getPrioriti(previousOperator, expressionsMap) >= operatorPriority ) {
            cout<<"operator >= prioriti = now to stack= "<<stackOperators<<endl;

            if (!stackOperators.isEmpty()) resultString += stackOperators.pop() + ' ';
            if (!stackOperators.isEmpty()) previousOperator = stackOperators.peek();
        }


        cout<<"now to stack= "<<stackOperators<<endl;
    }

    operatorPriority = getPrioriti(token, expressionsMap);
    if (!stackOperators.isEmpty()) previousOperator = stackOperators.peek();

    if (stackOperators.isEmpty() || getPrioriti(previousOperator, expressionsMap) < operatorPriority){
        stackOperators.push(token);
        cout<<"operator < prioriti = now to stack= "<<stackOperators<<endl;
    }

    cout<<"token = "<<token<<" ||Stack = "<<stackOperators<<endl;

}

string preparationForParsing(string formula){
    string resultString;
    TokenScanner scanner(formula);
    string token;
    string lastToken = "=";
    int scopeCounter=0;
    for (int i=0; i < formula.size(); i++){
        token = scanner.nextToken();
        if (isalpha(token[0])){
            cout<<"Enter the value of the variable "<<token<<":"<<endl;
            resultString += getLine();
        }
        if (token[0] == '-' && lastToken[0] == '('){
            resultString += "0";
        }
        if (token[0] == '-' && lastToken[0] == '='){
            resultString += "0";
        }
        lastToken = token;
        if (!isalpha(token[0])) resultString += token;
        cout<<resultString<<endl;
        if (token[0] == '(' || token[0] == ')') scopeCounter++;
    }
    if (scopeCounter % 2 != 0){
        cout<<"You make a mistake when entering the expression. Lacking a brace."<<endl;
        return resultString = "";
    }
    return resultString;
}

string convertToReversePolishNotation(string formula){
    string resultString;
    Map<string, int> expressionsMap;

    expressionsMap.put("(", 0);
    expressionsMap.put(")", 0);
    expressionsMap.put("+", 1);
    expressionsMap.put("-", 1);
    expressionsMap.put("*", 2);
    expressionsMap.put("/", 2);

    cout<<expressionsMap<<endl;
    cout<<formula<<endl;
    char nextChar;
    string token;
    int operatorPriority = 0;
    class Stack<string> stackOperators;

    formula = preparationForParsing(formula);

    TokenScanner scanner(formula);
    for (int i=0; i < formula.size(); i++){
        //nextChar = scanner.getChar();

        token = scanner.nextToken();
        cout<<"work this token= "<<token<<endl;

        if(nextChar == '/0')
            break;

        if(isDigitVariale(token)) {
            resultString += token + ' ';
            continue;
        }

        if(isOperatorFunction(token, expressionsMap)){
            addOperatorToStackOrResult(token, expressionsMap, stackOperators, resultString);
            continue;
        }

        if (token[0] == '(') {
            stackOperators.push(token);
            cout<<"Scope find! '(' "<<endl;
            cout<<"now to stack= "<<stackOperators<<endl;
            continue;
        }
        if (token[0] == ')') {
            cout<<"Scope close! ')' "<<endl;
            cout<<"now to stack= "<<stackOperators<<endl;
            resultString += addScopeToResultString(stackOperators, expressionsMap);
            continue;
        }


        cout<<"result "<<resultString<<endl;
    }
    while (!stackOperators.isEmpty())
        resultString += stackOperators.pop() + ' ';



    cout<<resultString<<endl;
    return resultString;
}

double calculatePlus(string leftOperand, string rightOperand){
    return stringToDouble(leftOperand) + stringToDouble(rightOperand) ;
}

double calculateMinus(string leftOperand, string rightOperand){
    return stringToDouble(leftOperand) - stringToDouble(rightOperand) ;
}

double calculateMultiply(string leftOperand, string rightOperand){
    return stringToDouble(leftOperand) * stringToDouble(rightOperand) ;
}

double calculateShare(string leftOperand, string rightOperand){
    double result;
    if (stringToDouble(rightOperand) != 0){
        result = stringToDouble(leftOperand) / stringToDouble(rightOperand);}
    else {
        cout<<"An attempt to divide by zero!"<<endl;
        result = 0;
    }
    return result;
}
double calculateResult(string formula){
    double result = 0;
    TokenScanner scaner(formula);
    string token, lastToken;
    class Stack<string> operandStack;
    for (int i=0; i < formula.size(); i++){
        token = scaner.nextToken();
        if (isDigitVariale(token)){
            operandStack.push(token);
        }
        if (token == "+"){
            result = calculatePlus(operandStack.pop(), operandStack.pop());
            cout<<"+ = "<<result<<endl;
            operandStack.push(doubleToString(result));
        }
        if (token == "-"){
            result = calculateMinus(operandStack.pop(), operandStack.pop());
            operandStack.push(doubleToString(result));
            cout<<"- = "<<result<<endl;
        }
        if (token == "*"){
            result = calculateMultiply(operandStack.pop(), operandStack.pop());
            operandStack.push(doubleToString(result));
            cout<<"* = "<<result<<endl;
        }
        if (token == "/"){
            result = calculateShare(operandStack.pop(), operandStack.pop());
            operandStack.push(doubleToString(result));
            cout<<"/ = "<<result<<endl;
        }

    }
    return stringToDouble(operandStack.pop());
}
int main() {
    string formula = getLine("Enter you formula: ");
    formula = convertToReversePolishNotation(formula);
    //calculateResult(formula);
    double result = calculateResult(formula);
    cout<<"Result: "<<result;
    return 0;
}
