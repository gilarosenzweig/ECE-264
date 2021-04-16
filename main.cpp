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
    struct Node* firstptr, lastptr;
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
            length = length +1;
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
            return;
        }
        else if (length ==1){
            stuff extract = firstptr -> getValue();
            firstptr = nullptr;
            lastptr = nullptr;
            length = length -1;
            return extract;
        }
        else {
            stuff extract = firstptr -> getValue();
            firstptr = firstptr -> getNext();
            length = length -1;
            return extract;
        }
    }
};

template <typename stuff> class stack {

private:
    string name;
public:
    stack(const string &name) : name(name) {
    }
    void push(stuff thing){
        this -> insertAtStart(thing);
    }
    stuff pop(){
        return this -> removeAtStart();
    }
};

template <typename stuff> class queue {
private:
    string name;
public:
    queue(const string &name) : name(name) {
    }
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
template <typename T> SimpleList<T>* getList(string name, list<SimpleList<T>*> setOfLists) {
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
            //cout << dataType;
            if (words[0] == "create"){
                if (checkName(words[1], nameList)){
                    outputfile << "ERROR: This name already exists!\n";
                }
                nameList.push_front(words[1]);
                if (words[2] == "stack"){
                    if (dataType == 'i'){
                        //push new stack into listSLi
                    }
                    else if (dataType == 'd'){
                        //push new stack into listSLd
                    }
                    else if (dataType == 's'){
                        //push new stack into listSLs
                    }
                }
                else if (words[2] == "queue"){
                    if (dataType == 'i'){
                        //push new queue into listSLi
                        new queue<int> (words[1]);
                    }
                    else if (dataType == 'd'){
                        //push new queue into SLd
                    }
                    else if (dataType == 's'){
                        // push new queue into listSLs
                    }
                }
            }
            else if (words[0] == "push"){

            }
            else if (words[0] == "pop"){

            }
        }
    }
}