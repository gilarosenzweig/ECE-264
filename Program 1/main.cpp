/* Gila Rosenzweig
 *
 * This program reads a user-designated text file for commands,
 * and, using the first word of the commands, creates lists of both stack and queue types, and can push and pop items from those lists.
 * Based on the second word in the command, the program determines the type of data stored in the list, as well as the name of that particular list.
 * The third word indicates either the type of list, or the value to be pushed onto the list.
 * An output file (name designated by user) keeps track of all processing commands, any errors that occur, and any values popped from the list.
 */


#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;
string useroutfile, userinfile;

//abstract base class for simpleList. stack and queue will inherit from this class
template <typename MyType> class simpleList {
 private:
    string name; //stores name of the list
    struct Node {
        MyType value; //the value stored in the node
        struct Node* next; //pointer to the next node
        Node (MyType item) {
            value = item;
            next = nullptr;
        }

        //getter for value (allows other functions to retrieve value)
        MyType getValue() const {
            return value;
        }
        //setter for value (allows other functions to set value)
        void setValue(MyType value) {
            Node::value = value;
        }
        //getter for next (allows other functions to retrieve next)
        Node *getNext() const {
            return next;
        }
        //setter for next (allows other functions to set next)
        void setNext(Node *next) {
            Node::next = next;
        }
    };
//defines variables for the length, pointer to first item, and pointer to last item of the list
protected:
    struct Node* firstptr;
    struct Node* lastptr;
    int length;

public:
    //constructor for a simple list: when a list is created, the input 'str' is the list name, the length is 0, the first and last pointers are null
    simpleList(string str) {
        firstptr = nullptr;
        lastptr = nullptr;
        length = 0;
        name = str;
    }
    virtual void push(MyType item) = 0; //this function will be defined in stack and queue
    virtual MyType pop() =0; //this function will be defined in stack and queue

    //getter for the name of the list (allows other functions to retrieve name)
    const string &getName() const {
        return name;
    }
    //getter for the length of the list (allows other functions to retrieve length)
    int getlength() const {
        return length;
    }
    //getter for the firstptr (allows other functions to retrieve firstptr)
    Node *getFirstptr() const {
        return firstptr;
    }
    //setter for the firstptr (allows other functions to set firstptr)
    void setFirstptr(Node *firstptr) {
        simpleList::firstptr = firstptr;
    }
    //checks if the list is empty
    bool isEmpty() {
        return length == 0;
    }
    //allows a new item to be inserted at the beginning of the simpleList
    void insertAtStart(MyType newNode){
        Node* insert = new Node(newNode);
        if (isEmpty()){
            //for an initially empty list, the firstptr and lastptr are reset to point to the inserted value
            firstptr = insert;
            lastptr = insert;
            length = length + 1;
        }
        else {
            //for a list that already contains members, the pointer of the new node points to the existing first item
            //the firstptr is reset to point to the inserted node
            insert -> setNext(firstptr);
            firstptr = insert;
            length = length + 1;
        }
    }
    //allows a new item (given in command) to be inserted to the end of the simpleList
    void insertAtEnd(MyType newNode){
        Node* insert = new Node(newNode);
        if (isEmpty()){
            //if the list is initially empty, firstptr and lastptr are reset to point to the inserted node
            lastptr = insert;
            firstptr = insert;
            length = length + 1;
        }
        else {
            //if the list already contains node(s): the pointer in the existing last node changes to point to the inserted node
            //lastptr is reset to be the pointer of the inserted node
            lastptr -> setNext(insert);
            lastptr = insert;
            length = length + 1;
        }
    }
    //to take a value off the beginning of the list:
    MyType removeAtStart() {
        if (isEmpty()){
            //if the list is empty, do nothing
            return 0;
        }
        else if (length ==1){ //if the list has only one item:
            MyType extract = firstptr -> getValue(); //the value of the firstptr is extracted and stored
            firstptr = nullptr; //the firstptr is reset to nullptr
            lastptr = nullptr; //the lastptr also reset to nullptr
            length = length - 1; //length is decremented
            return extract;
        }
        else { //if the length is >1
            MyType extract = firstptr -> getValue(); //extract the firstptr and store
            firstptr = firstptr -> getNext(); //reset the firstptr to the pointer of the next node
            length = length - 1; //length is decremented
            return extract;
        }
    }
};

//this stack class inherits its basic structure from the simpleList class
template <typename MyType> class stack: public simpleList<MyType> {
    using simpleList<MyType>::simpleList;
    public:
        //defines push and pop (pushed to the beginning of the stack, so it is the first item removed at command 'pop' (LIFO))
        void push(MyType newNode){
            this -> insertAtStart(newNode);
        }
        MyType pop(){
            return this -> removeAtStart();
        }
};

//this queue class inherits its basic structure from the simpleList class
template <typename MyType> class queue: public simpleList<MyType>{
    using simpleList<MyType>::simpleList;
    public:
        //defines push and pop (pushed to the end of the queue, so the first item in is the first item out (FIFO))
        void push(MyType newNode){
            this -> insertAtEnd(newNode);
        }
        MyType pop() {
            return this -> removeAtStart();
        }
};

//will iterate through the list of stacks and queues to find the list being pushed to/popped from
template <typename MyType> simpleList<MyType>* getList(string name, list<simpleList<MyType>*> setOfLists) {
    for (auto it = setOfLists.begin(); it != setOfLists.end(); it++) {
        if (((*it) -> getName()) == name)
            return *it;
    }
    return nullptr;
}

//checks the list of names of stacks and queues to see if a list of that name exists
bool checkName(string name, list<string> nameList){
    for (string n: nameList){
        if (n == name)
            return true;
    }
    return false;
}

//reads the command line from the input file, and stores the words in an array
void parse(string words[], ifstream &inputfile){
    inputfile >> words[0];
    if (words[0] == "pop")
        inputfile >> words[1];
    else {
        inputfile >> words[1];
        inputfile >> words[2];
    }
}

//gets name of input file from user
string getInFileName(){
    cout << "input file name: ";
    cin >> userinfile;
    //userinfile = userinfile + ".txt"; //could be used if user is not expected to write .txt
    return userinfile;
}
//gets name of output file from user
string getOutFileName(){
    cout << "output file name: ";
    cin >> useroutfile;
    //useroutfile = useroutfile + ".txt"; //could be used if user if not expected to write .txt
    return useroutfile;
}

//reads the input file and carries out the commands
void reader(){
    ifstream inputfile;
    ofstream outputfile;
    getInFileName();
    getOutFileName();
    string words[3];
    list < simpleList<int> * > listSLi; //list of all integer stacks and queues
    list < simpleList<double> * > listSLd; //list of all double stacks and queues
    list < simpleList<string> * > listSLs; //list of all string stacks and queues
    list <string> nameList; //list of names of all stacks and queues
    inputfile.open(userinfile);
    outputfile.open(useroutfile);
    if (inputfile.is_open()){
        while (inputfile.peek() != EOF){
            parse(words, inputfile);
            if (words[0] == "pop")
                outputfile << "PROCESSING COMMAND: " << words[0] << " " << words[1] << "\n";
            else
                outputfile << "PROCESSING COMMAND: " << words[0] << " " << words[1] << " " << words[2] << "\n";
            char dataType = words[1].at(0); //reads the first char of the list name, to determine the type of data it stores
            if (words[0] == "create"){
                /*if the first word of the command is create:
                 * check name lsit to ensure the name does not already exist
                 * if it doesnt, create a new lsit and push the name to the list of lists containing the same data type
                 */
                if (checkName(words[1], nameList)){
                    outputfile << "ERROR: This name already exists!\n";
                    continue;
                }
                nameList.push_front(words[1]);
                if (words[2] == "stack"){
                    if (dataType == 'i')
                        listSLi.push_front(new stack<int> (words[1]));
                    else if (dataType == 'd')
                        listSLd.push_front(new stack<double> (words[1]));
                    else if (dataType == 's')
                        listSLs.push_front(new stack<string> (words[1]));
                }
                else if (words[2] == "queue"){
                    if (dataType == 'i')
                        listSLi.push_front(new queue<int> (words[1]));
                    else if (dataType == 'd')
                        listSLd.push_front(new queue<double> (words[1]));
                    else if (dataType == 's')
                        listSLs.push_front(new queue<string> (words[1]));
                }
            }
            else if (words[0] == "push") {
                /*if the second word is push:
                 * check name list to ensure the list does exist
                 * push the third word
                 */
                if (!checkName(words[1], nameList))
                    outputfile << "ERROR: This name does not exist!\n";
                else if (checkName(words[1], nameList) && dataType =='i')
                    getList(words[1], listSLi)->push(stoi(words[2]));
                else if (dataType == 'd')
                    getList(words[1], listSLd) -> push(stod(words[2]));
                else if (dataType == 's')
                    getList(words[1], listSLs) -> push(words[2]);
            }
            else if (words[0] == "pop"){
                /*if the first word is pop:
                 * check that the list name exists, and that it is not empty
                 * pop and return the value
                 */
                if ( !checkName(words[1], nameList))
                    outputfile << "ERROR: This name does not exist!\n";
                else if (dataType == 'i'){
                    if (getList(words[1], listSLi)-> isEmpty())
                        outputfile << "ERROR: This list is empty!\n";
                     else
                         outputfile << "Value popped: " << getList(words[1], listSLi) -> pop() << "\n";
                }
                else if (dataType == 'd'){
                    if (getList(words[1], listSLd)-> isEmpty())
                        outputfile << "ERROR: This list is empty!\n";
                    else
                        outputfile << "Value popped: " << getList(words[1], listSLd) -> pop() << "\n";

                }
                else if (dataType == 's'){
                    if (getList(words[1], listSLs)-> isEmpty())
                        outputfile << "ERROR: This list is empty!\n";
                    else
                        outputfile << "Value popped: " << getList(words[1], listSLs) -> pop() << "\n";
                }
            }
        }
    }
}
int main() {
    reader();
}
