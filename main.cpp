#include <iostream>
#include <cmath>
#include <stack>
#include<string>
#include <vector>
using namespace std;
struct Range{
    int min, max;
    Range(int min, int max){
        this->min = min;
        this->max = max;
    }
};
string merge2Num(string &num1, int multiplier1, string &num2, int multiplier2){
    //multiplier is the power of then that the number is multiplied number2 should be equal or smaller than 1
    int additionalZeros = multiplier1 - multiplier2;
    for(int i = 0; i < additionalZeros; i ++){
        num1 += "0";
    }
    stack<int> queue;
    queue.push(0);
    for (int i = 0; i < num2.size(); i ++) {
        int indexInNum1 = num1.size() - i - 1;
        int indexInNum2 = num2.size() - i - 1;
        int newNum = (num1[indexInNum1] -'0') + (num2[indexInNum2] -'0') + queue.top();
        queue.pop();
        queue.push(newNum / 10);
        newNum %= 10;
        num1[indexInNum1] = to_string(newNum)[0];
    }

    return num1;
}
struct SegmentTree{
    struct BinaryNode{
        BinaryNode * parent;

        BinaryNode * left;
        BinaryNode * right;

        int powerOf10;
        string value = "";

        BinaryNode(string v, int p){
            this->value = v;
            this->powerOf10 = p;
        }
        BinaryNode(BinaryNode * left, BinaryNode * right, string v, int p){
            this->left = left;
            this->right = right;
            this->value = v;
            this->powerOf10 = p;
        }
    };

    BinaryNode * root;

    string internal;
    string external;

    int height;
    vector< vector<BinaryNode *> > floors;

    SegmentTree(string in, string out){
        this->internal = in;
        this->external = out;
        float logResult = log2(in.size() + out.size());
        bool integer = ((int)logResult) == logResult ? true : false;
        this->height = (int)logResult + !integer + 1;
        preprocessSegmentTree();
    }
    void preprocessSegmentTree(){
        for(int i = 0 ; i < height; i ++){
            floors.push_back(vector<BinaryNode *>());
            int currFloorSize = pow(2, height - i - 1);
            for(int j = 0; j < currFloorSize; j ++){
                if(i == 0){
                    bool odd = (j % 2);
                    string value;
                    value = odd ? internal[j / 2] : external[j / 2];

                    BinaryNode * binaryNode = new BinaryNode(value, currFloorSize - j/2 -1);

                    floors.at(i).push_back(binaryNode);
                } else{
                    BinaryNode * leftChild = floors.at(i - 1).at(j * 2);
                    BinaryNode * rightChild = floors.at(i - 1).at(j * 2 + 1);

                    string mergedValue = merge2Num(leftChild->value, leftChild->powerOf10, rightChild->value, rightChild->powerOf10);
                    BinaryNode * binaryNode = new BinaryNode(leftChild, rightChild, mergedValue, leftChild->powerOf10);


                    leftChild->parent = binaryNode;
                    rightChild->parent = binaryNode;

                    floors.at(i).push_back(binaryNode);
                }
                if(i == height - 1){
                    this->root = floors.at(i)[0];
                }
            }
            cout<<"";
        }
    }
};

vector<string> split(string str, char divider){
    vector<string> result;

    string currWord = "";
    for(int i = 0; i < str.size(); i ++){
        currWord+=str[i];
        if(str[i] == divider || str.size()-1 == i){
            result.push_back(currWord);
            currWord = "";
        }
    }
    return result;
}

int main() {
    string line;
    getline(cin, line);
    vector<string> args = split(line, ' ');
    int numSize = stoi(args[0]);
    int queries = stoi(args[1]);

    getline(cin, line);
    string internal = line;
    getline(cin, line);
    string external = line;
   // SegmentTree segmentTree(internal, external);

   string s1 = "9";
   string s2 = "2";

    return 0;
}
