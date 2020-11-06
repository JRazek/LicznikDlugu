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
        int lastUpdate;
        DigitsInterval(Range * r){
            this->range = new Range(r->min, r->max);
        }
    };
    struct BinaryNode{
        int id;
        Range * range;

        DigitsInterval * stringBelonging = nullptr;
        BinaryNode(int id, Range * r){
            this->id = id;
            this->range = r;
        }
    };

    int debtLength;
    int height;
    int firstFloorSize;

    int queryNum = 0;

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
            firstFloor.push_back(n);
        }
        cout<<"";
    }
    BinaryNode * getChild(int binaryNodeID, bool left){
        return nodes[(binaryNodeID) * 2 + !left + 1];
    }
    BinaryNode * getParent(int binaryNodeID){
        return nodes[(binaryNodeID - 1) / 2];
    }
    vector<BinaryNode *> rangeQuery(Range * range, int nodeID = 0){
        if(range == nullptr){
            return vector<BinaryNode *>();
        }
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
        int bestUpdate = -10;
        int value ;
        while(true){
            cout<<"";
            //if error here - wrong data set!
            if(node->stringBelonging != nullptr && node->stringBelonging->lastUpdate >= bestUpdate){
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
    DigitsInterval * getBelongingSegment(int digitNum){
        if(digitNum < 0 || digitNum >= debtLength){
            return nullptr;
        }
        int nodeID = digitNum + pow(2, height - 1) - 1;
        BinaryNode * n = nodes[nodeID];
        DigitsInterval * curr = n->stringBelonging;//this must be not null
        while(true){
            if(n->stringBelonging != nullptr && n->stringBelonging->lastUpdate > curr->lastUpdate){
                curr = n->stringBelonging;
            }
            if(n == getParent(n->id)){
                break;
            }
            n = getParent(n->id);
        }
        return curr;
    }
    void updateSegment(Range * range, int value){
        if(value == -10){
            cout<<"";
        }
        if(value < 0 || value > 9){
            cout<<"ERROR!\n";
            return;
        }
        if(this->queryNum == 2816){
            cout<<"";
        }
        vector<BinaryNode *> nodes = rangeQuery(range);
        DigitsInterval * digitsInterval = new DigitsInterval(range);
        digitsInterval->value = value;
        digitsInterval->lastUpdate = this->queryNum;

        int leftOutNodeID = range->min - 1;
        int rightOutNodeID = range->max + 1;
        DigitsInterval * leftInterval = getBelongingSegment(leftOutNodeID);
        DigitsInterval * rightInterval = getBelongingSegment(rightOutNodeID);
        if(leftInterval != nullptr) {
            if(leftInterval->value == digitsInterval->value){
                digitsInterval->range->min = leftInterval->range->min;
            }else{
                DigitsInterval * newLeftInterval = new DigitsInterval(leftInterval->range);
                newLeftInterval->range->max = leftOutNodeID;
                newLeftInterval->lastUpdate = queryNum;
                newLeftInterval->value = leftInterval->value;
                vector<BinaryNode *> affected = rangeQuery(newLeftInterval->range);
                leftInterval = nullptr;
                delete (leftInterval);
                for(BinaryNode * n : affected){
                    n->stringBelonging = newLeftInterval;
                }
            }
        }
        if(rightInterval != nullptr) {
            if(rightInterval->value == digitsInterval->value){
                digitsInterval->range->max = rightInterval->range->max;
            }else{
                DigitsInterval * newRightInterval = new DigitsInterval(rightInterval->range);
                newRightInterval->range->min = rightOutNodeID;
                newRightInterval->lastUpdate = queryNum;
                newRightInterval->value = rightInterval->value;
                vector<BinaryNode *> affected = rangeQuery(newRightInterval->range);
                rightInterval = nullptr;
                delete (rightInterval);
                for(BinaryNode * n : affected){
                    n->stringBelonging = newRightInterval;
                }
            }
        }

        vector<BinaryNode *> affected = rangeQuery(digitsInterval->range);
        for(BinaryNode * n : affected){
            n->stringBelonging = digitsInterval;
        }
        this->queryNum ++;
    }
    string getFullNum(){
        string s = "";
        for(int i = 0; i < debtLength; i ++){
            s += to_string(getDigitValue(i));
        }
        return s;
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
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

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

    int changeNum = 0;
    int answer = 0;
    for(int i = 0; i < queriesCount; i++){
        getline(cin, line);
        args = split(line);
        char queryType = args[0][0];
        if(queryType == 'S'){
            int digitNum = sum.size() - (stoi(args[1]) - 1) - 1;
            cout << segmentTree.getDigitValue(digitNum) << "\n";
            answer ++;

            // string tmpAdded = addNumbers(internal, external);
            // string tmpTree = segmentTree.getFullNum();
        }else{
            int changedDigit = internal.size() - stoi(args[1]);
            int changedFor = stoi(args[2]);

            int delta;
            if(queryType == 'W'){
                delta = changedFor - (internal[changedDigit] -'0');
                internal[changedDigit] = to_string(changedFor)[0];
            }
            else{
                delta = changedFor - (external[changedDigit] -'0');
                external[changedDigit] = to_string(changedFor)[0];
            }

            int digitInSum = changedDigit + 1;

            int valueInSum = segmentTree.getDigitValue(digitInSum);
            int newSum = valueInSum + delta;


            SegmentTree::DigitsInterval * segmentBefore = segmentTree.getBelongingSegment(digitInSum - 1);
            if((segmentBefore->value == 9 && newSum > 9) || (segmentBefore->value == 0 && newSum < 0)){
                Range * range = Range::commonPart(segmentBefore->range, new Range(0, digitInSum - 1));//the holy fix is digitInSum - 1!
                int valueInRange;
                int valueBeforeRange = segmentTree.getDigitValue(range->min - 1);
                if(segmentBefore->value == 9 && newSum > 9 ){
                    valueInRange = 0;
                    valueBeforeRange += 1;
                    newSum -= 10;
                }
                if(segmentBefore->value == 0 && newSum < 0){
                    valueInRange = 9;
                    valueBeforeRange -= 1;
                    newSum += 10;
                }

                segmentTree.updateSegment(new Range(digitInSum, digitInSum), newSum);
                segmentTree.updateSegment(range, valueInRange);
                segmentTree.updateSegment(new Range(range->min - 1, range->min - 1), valueBeforeRange);
            }
            else{
                int nextLineAdd = 0;
                if(newSum > 9){
                    nextLineAdd = 1;
                    newSum -= 10;
                }
                if(newSum < 0){
                    nextLineAdd = -1;
                    newSum += 10;
                }
                int nextLineValue = segmentBefore->value + nextLineAdd;

                segmentTree.updateSegment(new Range(digitInSum, digitInSum), newSum);
                segmentTree.updateSegment(new Range(digitInSum - 1, digitInSum - 1), nextLineValue);

                if(newSum > 9 || newSum < 0){
                    cout<<"ERROR\n";
                }
                if(nextLineValue > 9 || nextLineValue < 0){
                    cout<<"ERROR\n";
                }
            }

            // cout<<changeNum<<" ";*/
            // segmentTree.showNum();
            // cout<<"";
            changeNum++;
        }
    }
    //segmentTree.showNum();
    /*
    segmentTree.showNum();
    segmentTree.updateSegment(new Range(7,9), 0);
    segmentTree.updateSegment(new Range(5,5), 6);
    segmentTree.updateSegment(new Range(7,9), 6);
    segmentTree.updateSegment(new Range(8,8), 1);
    segmentTree.updateSegment(new Range(1,8), 0);
    segmentTree.showNum();

    int num = 1;
    SegmentTree::DigitsInterval * d = segmentTree.getBelongingSegment(num);
    cout<<d->range->min<<" "<<d->range->max<<" "<<d->value<<"\n";*/
    return 0;
}
