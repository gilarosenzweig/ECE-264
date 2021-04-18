#include <iostream> //library for reading file names
#include <fstream> //library for reading files
#include <string> //library for string functionality
#include <list> //library for list functionality (used to track the created stacks/queues

using namespace std; //using the standard namespace. put here to avoid needing it throughout the code
string useroutfile, userinfile; //global variables for the user file names

template <typename MyType> class simpleList { //class for simpleList. it is the base class for the stack and queue classes
 private:
    struct Node { //this is the basic structure for simpleList
        //members of this sub-struct are public (by default)
        MyType value;
        struct Node* next;
        Node (MyType item) {
            value = item; //the value stored in the node
            next = nullptr; //pointer to the next node, initially null
        }

        MyType getValue() const { //getter for value
            return value;
        }

        void setValue(MyType value) { //setter for value
            Node::value = value;
        }

        Node *getNext() const { //getter for next
            return next;
        }

        void setNext(Node *next) { //setter for next
            Node::next = next;
        }
    };
protected: //protected variables are accessible by the classes that inherit from base class (stack and queue)
    struct Node* firstptr; //pointer to the first item in the list
    struct Node* lastptr; //pointer to the last item in the list
    int length; //stores the length of the list
    string name; //stores the name of the list
public:
    simpleList(string str) {
        firstptr = nullptr; //firstptr is initially pointing to nothing, because there are not yet any items in the list
        lastptr = nullptr; //same for lastptr
        length = 0; //length is at first 0, because nothing in the list
        name = str; //the name of the list is a string (str)
    }
    virtual void push(MyType item) = 0; //this function will be defined in stack and queue
    virtual MyType pop() =0; //this function will be defined in stack and queue

    const string &getName() const { //getter for the name of the list
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

    void insertAtStart(MyType newNode){
        Node* insert = new Node(newNode);
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
    void insertAtEnd(MyType newNode){
        Node* insert = new Node(newNode);
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
    MyType removeAtStart() {
        if (isEmpty()){
            return 0;
        }
        else if (length ==1){
            MyType extract = firstptr -> getValue();
            firstptr = nullptr;
            lastptr = nullptr;
            length = length - 1;
            return extract;
        }
        else {
            MyType extract = firstptr -> getValue();
            firstptr = firstptr -> getNext();
            length = length - 1;
            return extract;
        }
    }
};

template <typename MyType> class stack: public simpleList<MyType> {
    using simpleList<MyType>::simpleList;
    public:
        void push(MyType newNode){
            this -> insertAtStart(newNode);
        }
        MyType pop(){
            return this -> removeAtStart();
        }
};

template <typename MyType> class queue: public simpleList<MyType>{
    using simpleList<MyType>::simpleList;
    public:
        void push(MyType newNode){
            this -> insertAtEnd(newNode);
        }
        MyType pop() {
            return this -> removeAtStart();
        }
};

template <typename T> simpleList<T>* getList(string name, list<simpleList<T>*> setOfLists) {
    for (auto it = setOfLists.begin(); it != setOfLists.end(); it++) {
        if (((*it) -> getName()) == name)
            return *it;
    }
    return nullptr;
}

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
string getInFileName(){
    cout << "input file name: ";
    cin >> userinfile;
    //userinfile = userinfile + ".txt";
    return userinfile;
}

string getOutFileName(){
    cout << "output file name: ";
    cin >> useroutfile;
    //useroutfile = useroutfile + ".txt";
    return useroutfile;
}
void reader(){
    ifstream inputfile;
    ofstream outputfile;
    getInFileName();
    getOutFileName();
    string words[3];
    list < simpleList<int> * > listSLi; //all integer stacks and queues
    list < simpleList<double> * > listSLd; //all double stacks and queues
    list < simpleList<string> * > listSLs; //all string stacks and queues
    list <string> nameList; //list of all list names
    inputfile.open(userinfile);
    outputfile.open(useroutfile);
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
int main() {
    reader();
}