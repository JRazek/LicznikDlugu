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
    char value;
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
        BinaryNode(int id, Range * r){
            this->id = id;
            this->range = r;
        }
    };

    BinaryNode * root;

    int height;
    int firstFloorSize;
    vector< BinaryNode * > nodes;
    SegmentTree(string sum){
        float logVal = log2(sum.size());
        this->firstFloorSize = pow(2, (int)logVal + (bool)((int)logVal != logVal));
        this->height = log2(firstFloorSize) + 1;
        buildTree();
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
            getParent(i);
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
        BinaryNode * leftChild = getChild(nodeID, true);
        BinaryNode * rightChild = getChild(nodeID, false);

        if(range->min == parent->range->min && range->max == parent->range->max){
            return {parent};
        }


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
    int queries = stoi(args[1]);

    getline(cin, line);
    string internal = line;
    getline(cin, line);
    string external = line;

    string sum = addNumbers(internal, external);

    vector<Range *> intervals;

    bool first = true;
    for(int i = 0; i < sum.size(); i ++){
        if(sum[i] == '9'){
            if(first){
                intervals.push_back(new Range(i, 0, '9'));
                first = false;
            }
            intervals.back()->max = i;
        }else{
            first = true;
        }
    }
    SegmentTree segmentTree(sum);
    vector<SegmentTree::BinaryNode *> nodes = segmentTree.rangeQuery(new Range(0,4));
    return 0;
}
