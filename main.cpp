#include <iostream>
#include <cmath>
#include <stack>
#include<string>
#include <vector>
using namespace std;
using stringContainer = deque<int>;
struct Range{
    int min;
    int max;
    int value = -1;
    int lastUpdate;
    Range(int min, int max, char v){
        this->min = min;
        this->max = max;
        this->value = v;
    }
    Range(int min, int max){
        this->min = min;
        this->max = max;
    }
    static Range * commonPart(Range * r1, Range * r2){
        int min = r1->min > r2->min ? r1->min : r2->min;
        int max = r1->max < r2->max ? r1->max : r2->max;
        return new Range(min, max);
    }
};

string addNumbers(const string &num1, const string &num2){
    string result = "0" + num1;

    int nextLineNum = 0;
    int i = 0;
    while (i < num2.size()){
        int ind1 = result.size() - i - 1;
        int ind2 = num2.size() - i - 1;

        int newNum = (result[ind1] - '0') + (num2[ind2] - '0') + nextLineNum;
        nextLineNum = newNum / 10;
        newNum %= 10;
        result[ind1] = to_string(newNum)[0];
        i++;
    }
    result[0] = to_string(nextLineNum)[0];
    return result;
}
struct SegmentTree{
    struct BinaryNode{
        int id;
        Range * range;

        Range * stringBelonging = nullptr;
        BinaryNode(int id, Range * r){
            this->id = id;
            this->range = r;
        }
    };

    int debtLength;
    int height;
    int firstFloorSize;
    vector< BinaryNode * > nodes;
    SegmentTree(string d){
        this->debtLength = d.size();
        float logVal = log2(d.size());
        this->firstFloorSize = pow(2, (int)logVal + (bool)((int)logVal != logVal));
        this->height = log2(firstFloorSize) + 1;
        buildTree();
        preProcess(d);
    }
    void buildTree(){
        int nodesCount = pow(2, height) - 1;
        for(int i = 0; i < nodesCount; i ++){
            nodes.push_back(new BinaryNode(i, new Range(0,0)));
        }
        for(int i = 0 ; i < firstFloorSize; i ++){
            BinaryNode * n = nodes[i +  pow(2, height - 1) - 1];
            n->range->min = i;
            n->range->max = i;
        }
        for(int i = nodesCount - 1 - firstFloorSize; i >= 0; i --){
            BinaryNode * n = nodes[i];
            n->range->min = getChild(i, true)->range->min;
            n->range->max = getChild(i, false)->range->max;
        }
    }
    void preProcess(string debt){
        vector<BinaryNode *> firstFloor;
        for(int i = 0; i < firstFloorSize; i ++){
            int nodeID = i + pow(2, height - 1) - 1;
            BinaryNode * n = nodes[nodeID];
            int prevValue = debt[i - 1]-'0';
            int value = debt[i]-'0';
            if(i >= debtLength){
                value = 0;
            }
            if(i == 0 || prevValue != value){
                n->stringBelonging = new Range(n->range->min, n->range->max);
                n->stringBelonging->value = value;
            }else{
                n->stringBelonging = nodes[nodeID - 1]->stringBelonging;
                n->stringBelonging->max = n->range->max;
            }
            n->stringBelonging->lastUpdate = -1;
        }

        for(int i = 0; i < nodes.size() - firstFloorSize; i++){
            int nodeID = nodes.size() - firstFloorSize - i - 1;
            BinaryNode * n = nodes[nodeID];
            BinaryNode * leftChild = getChild(nodeID, true);
            BinaryNode * rightChild = getChild(nodeID, false);
            if(leftChild->stringBelonging != nullptr && rightChild->stringBelonging != nullptr) {
                if (leftChild->stringBelonging->value == rightChild->stringBelonging->value) {
                    Range *s = new Range(n->range->min, n->range->max);
                    s->value = leftChild->stringBelonging->value;
                    n->stringBelonging = s;
                    leftChild->stringBelonging = s;
                    rightChild->stringBelonging = s;
                    n->stringBelonging->lastUpdate = -1;
                }
            }else{
                n->stringBelonging = nullptr;
            }
        }
    }
    BinaryNode * getChild(int binaryNodeID, bool left){
        return nodes[(binaryNodeID) * 2 + !left + 1];
    }
    BinaryNode * getParent(int binaryNodeID){
        return nodes[(binaryNodeID - 1) / 2];
    }
    vector<BinaryNode *> rangeQuery(Range * range, int nodeID = 0){
        BinaryNode * parent = nodes[nodeID];

        if(range->min == parent->range->min && range->max == parent->range->max){
            return {parent};
        }

        BinaryNode * leftChild = getChild(nodeID, true);
        BinaryNode * rightChild = getChild(nodeID, false);



        Range * commonLeft = Range::commonPart(range, leftChild->range);
        Range * commonRight = Range::commonPart(range, rightChild->range);
        vector<BinaryNode *> result;
        if(commonLeft->min <= commonLeft->max){
            vector<BinaryNode *> resLeft = rangeQuery(commonLeft, leftChild->id);
            result.insert(result.begin(), resLeft.begin(), resLeft.end());
        }
        if(commonRight->min <= commonRight->max){
            vector<BinaryNode *> resRight = rangeQuery(commonRight, rightChild->id);
            result.insert(result.begin(), resRight.begin(), resRight.end());
        }
        return result;
    }
    int getLeafValue(const int digitNumber){
        const BinaryNode * node = nodes[digitNumber + pow(2, height - 1) - 1];
        int bestUpdate = -2;
        int value ;
        while(true){
            cout<<"";
            if(node->stringBelonging != nullptr && node->stringBelonging->lastUpdate > bestUpdate){
                bestUpdate = node->stringBelonging->lastUpdate;
                value = node->stringBelonging->value;
            }
            if(node == getParent(node->id)){
                break;
            }
            node = getParent(node->id);
        }
        return value;
    }
};

vector<string> split(string str, char divider = ' '){
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
    int queriesCount = stoi(args[1]);

    getline(cin, line);
    string internal = line;
    getline(cin, line);
    string external = line;

    string sum = addNumbers(internal, external);

    SegmentTree segmentTree(sum);

    int queryNum = 0;
    for(int i = 0; i < queriesCount; i++){
        getline(cin, line);
        args = split(line);
        char queryType = args[0][0];
        if(queryType == 'S'){
            int digitNum = sum.size() - (stoi(args[1]) - 1);
            cout<<segmentTree.getLeafValue(digitNum)<<"\n";
        }else{
            int digitNum = internal.size() - (stoi(args[1]) - 1);//same length for both
            int newFactorNum = stoi(args[2]);
            int delta;
            if(queryType == 'W'){
                delta = (newFactorNum - (internal[digitNum] - '0'));
                internal[digitNum] = newFactorNum;
            } else{
                delta = (newFactorNum - (external[digitNum] - '0'));
                external[digitNum] = newFactorNum;
            }
            int oldNum = segmentTree.getLeafValue(digitNum);

            int newNum = oldNum + delta;
            int nextLineNum = newNum / 10;
            newNum %= 10;
            queryNum ++;
        }
    }

    //segmentTree.rangeUpdate(new Range(0,7), 6, 1);
    //cout<<segmentTree.getLeafValue(3)<<"\n";
    return 0;
}
