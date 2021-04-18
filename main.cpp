#include <iostream> //library for reading file names
#include <fstream> //library for reading files
#include <string> //library for string functionality
#include <list> //library for list functionality (used to track the created stacks/queues

using namespace std; //using the standard namespace. put here to avoid needing it throughout the code
string useroutfile, userinfile; //global variables for the user file names

template <typename MyType> class simpleList { //class for simpleList. it is the base class for the stack and queue classes
 private: //private members cannot be accessed by classes that inherit from simpleList
    struct Node { //this is the basic structure for simpleList
        //members of this sub-struct are public (by default), so they are accessible by stack and queue
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
public: //public members can be accessed by the classes that inherit from simpleList (namely, stack and queue)
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

    int getlength() const { //getter for the length of the list
        return length;
    }

    Node *getFirstptr() const { //getter for the firstptr
        return firstptr;
    }

    void setFirstptr(Node *firstptr) { //setter for the firstptr
        simpleList::firstptr = firstptr;
    }

    bool isEmpty() { //checks if the list is empty
        return length == 0;
    }

    void insertAtStart(MyType newNode){ //allows a new item to be inserted at the beginning of the simpleList
        Node* insert = new Node(newNode);
        if (isEmpty()){ //if there is initially nothing in the list,
            firstptr = insert; //the fistptr points to the inserted item
            lastptr = insert; //the lastptr also points to the inserted item
            length = length + 1; //the length of the list in incremented by 1
        }
        else { //if there is already at least one item in the list:
            insert -> setNext(firstptr); //the pointer in the new node points to the existing first item
            firstptr = insert; //the firstptr is set to point to the inserted item (making the inserted item the first one)
            length = length + 1; //the length is incremented by 1
        }
    }
    void insertAtEnd(MyType newNode){ //allows a new item (given in command) to be inserted to the end of the simpleList
        Node* insert = new Node(newNode);
        if (isEmpty()){ //if the list is initially empty,
            lastptr = insert; //the lastptr points to the inserted item
            firstptr = insert; //the firstptr also points to the inserted item
            length = length + 1; //length is incremented by 1
        }
        else { //if the list already has at least one item:
            lastptr -> setNext(insert); //the 'next' pointer of the existing last node is changed to point to the inserted item
            lastptr = insert; //the lastptr is reset to be the 'next' pointer in the inserted node
            length = length + 1; //length is incremented by 1
        }
    }
    MyType removeAtStart() { //to take a value off the beginning of the list:
        if (isEmpty()){ //if the list is empty
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

template <typename MyType> class stack: public simpleList<MyType> { //this stack class inherits its basic structure from the simpleList class
    using simpleList<MyType>::simpleList;
    public: //defines push and pop (pushed to the beginning of the stack, so it is the first item removed at command 'pop' (LIFO)
        void push(MyType newNode){ //newNode is the item being inserted
            this -> insertAtStart(newNode);
        }
        MyType pop(){
            return this -> removeAtStart();
        }
};

template <typename MyType> class queue: public simpleList<MyType>{
    using simpleList<MyType>::simpleList;
    public: //defines push and pop (pushed to the end of the queue, so the first item in is the first item out (FIFO)
        void push(MyType newNode){ //newNode is the item being inserted
            this -> insertAtEnd(newNode);
        }
        MyType pop() {
            return this -> removeAtStart();
        }
};

template <typename T> simpleList<T>* getList(string name, list<simpleList<T>*> setOfLists) {
    for (auto it = setOfLists.begin(); it != setOfLists.end(); it++) { //will iterate through the list of stacks and queues
        // to find the list being pushed to/popped from
        if (((*it) -> getName()) == name)
            return *it; //when the list is found, the pointer to the list is returned
    }
    return nullptr; //if the list is not found, the nullptr is returned
}

bool checkName(string name, list<string> nameList){ //checks the list of names of stacks and queues to see if a list of that name exists
    for (string n: nameList){
        if (n == name){
            return true;
        }
    }
    return false;
}

void parse(string words[], ifstream &inputfile){ //reads the command line from the input file, and stores the words in an array
    inputfile >> words[0]; //reads the first word of the line
    if (words[0] == "pop"){ //if it is pop, it reads only one more word
        inputfile >> words[1];
    }
    else { //if the first word is create or push, it reads two more words
        inputfile >> words[1];
        inputfile >> words[2];
    }
}
string getInFileName(){ //gets the information for input file from user
    cout << "input file name: ";
    cin >> userinfile;
    //userinfile = userinfile + ".txt"; //could be used if user is not expected to write .txt
    return userinfile;
}

string getOutFileName(){ //gets information for output file from user
    cout << "output file name: ";
    cin >> useroutfile;
    //useroutfile = useroutfile + ".txt"; //could be used if user if not expected to write .txt
    return useroutfile;
}
void reader(){
    ifstream inputfile; //allows access to the input file
    ofstream outputfile; //allows access to output file
    getInFileName();
    getOutFileName();
    string words[3];
    list < simpleList<int> * > listSLi; //list of all integer stacks and queues
    list < simpleList<double> * > listSLd; //list of all double stacks and queues
    list < simpleList<string> * > listSLs; //list of all string stacks and queues
    list <string> nameList; //list of names of all stacks and queues
    inputfile.open(userinfile); //opens the input file using user info
    outputfile.open(useroutfile); //opens the output file using user info
    if (inputfile.is_open()){
        while (inputfile.peek() != EOF){ //as long as the file has not reached the end:
            parse(words, inputfile); //use the parse function to read a command line
            outputfile << "PROCESSING COMMAND: " << words[0] << " " << words[1] << " " << words[2] << "\n"; //write the processing status to the output file
            char dataType = words[1].at(0); //reads the first char of the list name, to determine the type of data it stores
            if (words[0] == "create"){ //if the first word of the command is create:
                if (checkName(words[1], nameList)){ //check the list of names to see if it exists already
                    outputfile << "ERROR: This name already exists!\n"; //if it does, print an error message to the output file
                    continue;
                }
                nameList.push_front(words[1]); //if its a new name, add it to the list of names
                if (words[2] == "stack"){ //if the third word is stack:
                    if (dataType == 'i') //and the type of data it stores is ints,
                        listSLi.push_front(new stack<int> (words[1])); //push new stack into listSLi
                    else if (dataType == 'd') //if the data it stores is doubles,
                        listSLd.push_front(new stack<double> (words[1])); //push new stack into listSLd
                    else if (dataType == 's')//if the data it stores is strings,
                        listSLs.push_front(new stack<string> (words[1])); //push new stack into listSLs
                }
                else if (words[2] == "queue"){ //if the third word is queue,
                    if (dataType == 'i') //if it stores ints
                        listSLi.push_front(new queue<int> (words[1]));//push new queue into listSLi
                    else if (dataType == 'd') //if it stores doubles,
                        listSLd.push_front(new queue<double> (words[1])); //push new queue into SLd
                    else if (dataType == 's') //if it stores strings,
                        listSLs.push_front(new queue<string> (words[1])); // push new queue into listSLs
                }
            }
            else if (words[0] == "push"){ //if the first word of the command is pop:
                if ( !checkName(words[1], nameList)){ //check the list of names: if it does not exist, print an error
                    outputfile << "ERROR: This name does not exist!\n";
                    continue;
                }
                else if (dataType =='i'){ //if it stores ints,
                    getList(words[1], listSLi) -> push(stoi(words[2])); //change the string stored in the third word to an integer and push it
                    continue;
                }
                else if (dataType == 'd'){ //if it stores doubles,
                    getList(words[1], listSLd) -> push(stod(words[2])); //change the string in the third word to a double and push it
                    continue;
                }
                else if (dataType == 's'){ //if it stores strings,
                    getList(words[1], listSLs) -> push(words[2]); //just push the string of the third word directly
                    continue;
                }
            }
            else if (words[0] == "pop"){ //if the command says to pop
                if ( !checkName(words[1], nameList)){ //check if the name exists, print an error if it does not
                    outputfile << "ERROR: This name does not exist!\n";
                    continue;
                }
                if (dataType == 'i'){ //if it stores integers,
                    if (getList(words[1], listSLi)-> isEmpty()){ //check if the list is empty
                        outputfile << "ERROR: This list is empty!\n"; //if it is, print an error
                        continue;
                    }
                    outputfile << "Value popped: " << getList(words[1], listSLi) -> pop() << "\n"; //otherwise, pop it and print the popped value
                    continue;
                }
                else if (dataType == 'd'){ //same process for lists of doubles
                    if (getList(words[1], listSLd)-> isEmpty()){
                        outputfile << "ERROR: This list is empty!\n";
                        continue;
                    }
                    outputfile << "Value popped: " << getList(words[1], listSLd) -> pop() << "\n";
                    continue;
                }
                else if (dataType == 's'){ //same process for lists of strings
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