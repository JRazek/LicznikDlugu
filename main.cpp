#include <iostream>
#include <cmath>
#include<string>
#include <vector>
using namespace std;
struct Range{
    int min;
    int max;
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
    struct BinaryNode;
    struct DigitsInterval{
        Range * range;
        int value;
        bool hasValue = false;
        int lastUpdate;
        DigitsInterval(Range * r){
            this->range = new Range(r->min, r->max);
        }
    };
    struct BinaryNode{
        int id;
        Range * range;

        DigitsInterval * stringBelonging;
        BinaryNode(int id, Range * r){
            this->id = id;
            this->range = r;
        }
    };

    int debtLength;
    int height;
    int firstFloorSize;

    int lastUpdate = -1;

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
                n->stringBelonging = new DigitsInterval(n->range);
                n->stringBelonging->value = value;
            }else{
                n->stringBelonging = nodes[nodeID - 1]->stringBelonging;
                n->stringBelonging->range->max = n->range->max;
            }
            n->stringBelonging->lastUpdate = -1;
            n->stringBelonging->hasValue = true;
            firstFloor.push_back(n);
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
    int getDigitValue(const int digitNumber){
        const BinaryNode * node = nodes[digitNumber + pow(2, height - 1) - 1];
        int bestUpdate = -2;
        int value ;
        while(true){
            cout<<"";
            //if error here - wrong data set!
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
    const DigitsInterval * getBelongingSegment(int digitNum){
        int nodeID = digitNum + pow(2, height - 1) - 1;
        BinaryNode * n = nodes[nodeID];
        DigitsInterval * curr = n->stringBelonging;//this must be not null
        while(true){
            if(n->stringBelonging != nullptr && n->stringBelonging->lastUpdate >= curr->lastUpdate){
                curr = n->stringBelonging;
            }
            if(n == getParent(n->id)){
                break;
            }
            n = getParent(n->id);
        }
        return curr;
    }
    void updateSegment(Range * range, int value, int queryNum){
        vector<BinaryNode *> nodes = rangeQuery(range);
        DigitsInterval * digitsInterval = new DigitsInterval(range);
        digitsInterval->value = value;
        digitsInterval->lastUpdate = queryNum;
        for(auto n : nodes){
            if(n->range->min == range->min){
                int leftOutNumberInFloor = n->range->min - 1;
                vector<BinaryNode *> r = rangeQuery(new Range(leftOutNumberInFloor, leftOutNumberInFloor));
                if(r.size() == 1){
                    BinaryNode * leftOut = r[0];
                    if(leftOut->stringBelonging == n->stringBelonging){
                        Range * newLeftRange = Range::commonPart(leftOut->stringBelonging->range, new Range(0, leftOutNumberInFloor));
                        DigitsInterval * newLeftInterval = new DigitsInterval(newLeftRange);
                        newLeftInterval->lastUpdate = queryNum;
                        newLeftInterval->value = leftOut->stringBelonging->value;
                        vector<BinaryNode *> leftAffected = rangeQuery(newLeftRange);
                        for(auto l : leftAffected){
                            l->stringBelonging = newLeftInterval;
                        }
                    }
                }
            }
            if(n->range->max == range->max){
                int rightOutNumberInFloor = n->range->max + 1;
                vector<BinaryNode *> r = rangeQuery(new Range(rightOutNumberInFloor, rightOutNumberInFloor));
                if(r.size() == 1){
                    BinaryNode * rightOut = r[0];
                    if(rightOut->stringBelonging == n->stringBelonging){
                        Range * newRightRange = Range::commonPart(rightOut->stringBelonging->range, new Range(rightOut->stringBelonging->range->max + 1, firstFloorSize - 1));
                        DigitsInterval * newRightInterval = new DigitsInterval(newRightRange);
                        newRightInterval->lastUpdate = queryNum;
                        newRightInterval->value = rightOut->stringBelonging->value;
                        vector<BinaryNode *> rightAffected = rangeQuery(newRightRange);
                        for(auto l : rightAffected){
                            l->stringBelonging = newRightInterval;
                        }
                    }
                }
            }
            n->stringBelonging = digitsInterval;
        }
    }
    void showNum(){
        cout<<"\n";
        for(int i = 0; i < debtLength; i ++){
            cout<<getDigitValue(i);
        }
        cout<<"\n";
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
    for(int i = 0; i < sum.size(); i ++){
        if(sum[i] != '9'){
            cout<<"";
        }
    }
    SegmentTree segmentTree(sum);

    int queryNum = 0;
    for(int i = 0; i < queriesCount; i++){
        getline(cin, line);
        args = split(line);
        char queryType = args[0][0];
        if(queryType == 'S'){
            int digitNum = sum.size() - (stoi(args[1]) - 1);
            //cout << segmentTree.getDigitValue(digitNum) << "\n";
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
            int oldNum = segmentTree.getDigitValue(digitNum);

            int newNum = oldNum + delta;
            int nextLineAdd = newNum / 10;
            newNum %= 10;
            int nextLineNum = segmentTree.getDigitValue(digitNum - 1);

            queryNum ++;
        }
    }

    //segmentTree.rangeUpdate(new Range(0,7), 6, 1);
    const SegmentTree::DigitsInterval * interval = segmentTree.getBelongingSegment(3);
    cout<<interval->range->min<< " " << interval->range->max<<"\n";
    segmentTree.updateSegment(new Range(1, 5), 1, 2);

    interval = segmentTree.getBelongingSegment(5);
    cout<<interval->range->min<< " " << interval->range->max<<"\n";

    segmentTree.showNum();
    return 0;
}
