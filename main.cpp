#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <iterator>
#include <vector>
using namespace std;

template <typename stuff> class simpleList {
 private:
    struct Node {
        stuff value;
        struct Node* next;
        Node (stuff item) {
            value = item;
            next = nullptr;
        }

        stuff getValue() const {
            return value;
        }

        void setValue(stuff value) {
            Node::value = value;
        }

        Node *getNext() const {
            return next;
        }

        void setNext(Node *next) {
            Node::next = next;
        }
    };
protected:
    struct Node* firstptr;
    struct Node* lastptr;
    int length;
    string name;
public:
    simpleList(string thing) {
        firstptr = nullptr;
        lastptr = nullptr;
        length = 0;
        name = thing;
    }
    virtual void push(stuff item) = 0;
    virtual stuff pop() =0;

    const string &getName() const {
        return name;
    }

    int getlength() const {
        return length;
    }

    Node *getFirstptr() const {
        return firstptr;
    }

    void setFirstptr(Node *firstptr) {
        simpleList::firstptr = firstptr;
    }

    bool isEmpty() {
        return length == 0;
    }

    void insertAtStart(stuff thing){
        Node* insert = new Node(thing);
        if (isEmpty()){
            firstptr = insert;
            lastptr = insert;
            length = length + 1;
        }
        else {
            insert -> setNext(firstptr);
            firstptr = insert;
            length = length + 1;
        }
    }
    void insertAtEnd(stuff thing){
        Node* insert = new Node(thing);
        if (isEmpty()){
            lastptr = insert;
            firstptr = insert;
            length = length + 1;
        }
        else {
            lastptr -> setNext(insert);
            lastptr = insert;
            length = length + 1;
        }
    }
    stuff removeAtStart() {
        if (isEmpty()){
            return 0;
        }
        else if (length ==1){
            stuff extract = firstptr -> getValue();
            firstptr = nullptr;
            lastptr = nullptr;
            length = length - 1;
            return extract;
        }
        else {
            stuff extract = firstptr -> getValue();
            firstptr = firstptr -> getNext();
            length = length - 1;
            return extract;
        }
    }
};

template <typename stuff> class stack: public simpleList<stuff> {
    using simpleList<stuff>::simpleList;
    public:
        void push(stuff thing){
            this -> insertAtStart(thing);
        }
        stuff pop(){
            return this -> removeAtStart();
        }
};

template <typename stuff> class queue: public simpleList<stuff>{
    using simpleList<stuff>::simpleList;
    public:
        void push(stuff thing){
            this -> insertAtEnd(thing);
        }
        stuff pop() {
            return this -> removeAtStart();
        }
};

bool checkName(string name, list<string> nameList){
    for (string n: nameList){
        if (n == name){
            return true;
        }
    }
    return false;
}

void parse(string words[], ifstream &inputfile){
    inputfile >> words[0];
    if (words[0] == "pop"){
        inputfile >> words[1];
    }
    else {
        inputfile >> words[1];
        inputfile >> words[2];
    }
}
template <typename T> simpleList<T>* getList(string name, list<simpleList<T>*> setOfLists) {
    for (auto it = setOfLists.begin(); it != setOfLists.end(); it++) {
        if (((*it) -> getName()) == name)
            return *it;
    }
    return nullptr;
}

int main() {
    ifstream inputfile;
    ofstream outputfile;
    string words[3];
    list < simpleList<int> * > listSLi; //all integer stacks and queues
    list < simpleList<double> * > listSLd; //all double stacks and queues
    list < simpleList<string> * > listSLs; //all string stacks and queues
    list <string> nameList; //list of all list names
    inputfile.open("input.txt");
    outputfile.open("output.txt");
    if (inputfile.is_open()){
        while (inputfile.peek() != EOF){
            parse(words, inputfile);
            outputfile << "PROCESSING COMMAND: " << words[0] << " " << words[1] << " " << words[2] << "\n";
            char dataType = words[1].at(0);
            if (words[0] == "create"){
                if (checkName(words[1], nameList)){
                    outputfile << "ERROR: This name already exists!\n";
                    continue;
                }
                nameList.push_front(words[1]);
                if (words[2] == "stack"){
                    if (dataType == 'i'){
                        //push new stack into listSLi
                        listSLi.push_front(new stack<int> (words[1]));
                    }
                    else if (dataType == 'd'){
                        //push new stack into listSLd
                        listSLd.push_front(new stack<double> (words[1]));
                    }
                    else if (dataType == 's'){
                        //push new stack into listSLs
                        listSLs.push_front(new stack<string> (words[1]));
                    }
                }
                else if (words[2] == "queue"){
                    if (dataType == 'i'){
                        //push new queue into listSLi
                        listSLi.push_front(new queue<int> (words[1]));
                    }
                    else if (dataType == 'd'){
                        listSLd.push_front(new queue<double> (words[1])); //push new queue into SLd
                    }
                    else if (dataType == 's'){
                        listSLs.push_front(new queue<string> (words[1])); // push new queue into listSLs
                    }
                }
            }
            else if (words[0] == "push"){
                if ( !checkName(words[1], nameList)){
                    outputfile << "ERROR: This name does not exist!\n";
                    continue;
                }
                if (dataType =='i'){
                    getList(words[1], listSLi) -> push(stoi(words[2]));
                    continue;
                }
                else if (dataType == 'd'){
                    getList(words[1], listSLd) -> push(stod(words[2]));
                    continue;
                }
                else if (dataType == 's'){
                    getList(words[1], listSLs) -> push(words[2]);
                    continue;
                }
            }
            else if (words[0] == "pop"){
                if ( !checkName(words[1], nameList)){
                    outputfile << "ERROR: This name does not exist!\n";
                    continue;
                }
               if (dataType == 'i'){
                   if (getList(words[1], listSLi)-> isEmpty()){
                       outputfile << "ERROR: This list is empty!\n";
                       continue;
                   }
                   outputfile << "Value popped: " << getList(words[1], listSLi) -> pop() << "\n";
                   continue;
               }
               else if (dataType == 'd'){
                   if (getList(words[1], listSLd)-> isEmpty()){
                           outputfile << "ERROR: This list is empty!\n";
                           continue;
                   }
                   outputfile << "Value popped: " << getList(words[1], listSLd) -> pop() << "\n";
                   continue;
               }
               else if (dataType == 's'){
                   if (getList(words[1], listSLs)-> isEmpty()){
                       outputfile << "ERROR: This list is empty!\n";
                       continue;
                   }
                   outputfile << "Value popped: " << getList(words[1], listSLs) -> pop() << "\n";
                   continue;
               }
            }
        }
    }
}