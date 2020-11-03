#include <iostream>

#include <math.h>
#include <vector>
using namespace std;
struct Range{
    int min, max;
    Range(int min, int max){
        this->min = min;
        this->max = max;
    }
};
struct SegmentTree{
    struct BinaryNode{
        BinaryNode * parent;

        BinaryNode * left;
        BinaryNode * right;
        Range * range;
        BinaryNode(Range * range){
            this->range = range;
        }
        BinaryNode(BinaryNode * left, BinaryNode * right, Range * range){
            this->left = left;
            this->right = right;
            this->range = range;
        }
        string value;
    };
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
                    bool first = !(j % 2);
                    BinaryNode * binaryNode = new BinaryNode(new Range(j,j));

                    if(first){
                        binaryNode->value = internal[j];
                    }else{
                        binaryNode->value = external[j];
                    }
                    floors.at(i).push_back(binaryNode);
                } else{
                    BinaryNode * leftChild = floors.at(i - 1).at(j * 2);
                    BinaryNode * rightChild = floors.at(i - 1).at(j * 2 + 1);
                    BinaryNode * binaryNode = new BinaryNode(leftChild, rightChild, new Range(leftChild->range->min, leftChild->range->max));


                    leftChild->parent = binaryNode;
                    rightChild->parent = binaryNode;

                    floors.at(i).push_back(binaryNode);
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
    SegmentTree segmentTree(internal, external);
    return 0;
}
