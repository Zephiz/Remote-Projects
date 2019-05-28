/*
* By - Mohamad Hafiz Bin Sulaiman (64854),
*      Mohd Zulhafiz Bin Rahim (64893),
*      Muhammad Abbas Salimi bin Ramli(64903),
*       & Mageleen binti Durin,(66546)
*
*  Our project is about QQ Bubble Tea that is a beverage kiosk.
*  Our solution is based on Stack implementation
*/

#include <iostream>
#include <string> // for string and getline
#include <cctype> // for isdigit
#include <ctime> // for time()
#include <cstdlib> // for rand() and srand()
#include <iomanip> //for setw, right, left
#include <windows.h> // for various function, ie: Sleep(), SetConsoleTextAttribute(), system()
#include <stdint.h> // for uint8_t type
#include <sstream> // to change date month from integer to string
#include <time.h> //for converting between various time and date formats.

using std::cout;
using std::string;
using std::cin;
using std::setw;
using std::left;
using std::right;
using std::getline;
int newline;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//all of these are for outputting colours
enum Colour { BLACK, BLUE, GREEN, AQUA, RED, PURPLE, YELLOW, WHITE, GRAY, LIGHT_BLUE,
            LIGHT_GREEN, LIGHT_AQUA, LIGHT_RED, LIGHT_PURPLE, LIGHT_YELLOW, BRIGHT_WHITE };

struct _colour_struct{
    uint8_t _colour_code;
};

inline _colour_struct setcolour (Colour colour_foregrnd, Colour colour_backgrnd =BLACK){
    uint8_t colour_code = colour_backgrnd;
    colour_code <<= 4;
    colour_code |= colour_foregrnd;
    return { colour_code };
}

namespace std{
    template<typename _CharT, typename _Traits>
    inline basic_ostream<_CharT, _Traits>&
    operator<<(basic_ostream<_CharT, _Traits>& __os, _colour_struct __col)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), __col._colour_code);
        return __os;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//this function is to disable and enable cursor in cmd 
void ShowConsoleCursor(bool showFlag){
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//this is to trim customer name (remove whitespaces)
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r "){  
    str.erase(0, str.find_first_not_of(chars)); // trim left
    str.erase(str.find_last_not_of(chars) + 1); // trim right

    /*trim in between and leave one whitespaces*/
    size_t pos;
    while( ( pos = str.find( "  " ) ) != std::string::npos )
    str = str.replace( pos, 2, " " );

    return str;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TeaLink{
    string teaInfo[5]; // teaInfo[0] = tea name, teaInfo[1] = tea type, teaInfo[2] = fruit flavour,
    int quantity;      // teaInfo[3] = milk, teaInfo[4] = add on
    double teaPrice;
    TeaLink *nextLink;
};

struct CustomerNode{
    int orderID;
    string customerName;
    int storeNo;
    bool teaTypeCheck[3]; // teaTypeCheck[0] to check for Earl Grey , teaTypeCheck[1] for Jasmine, teaTypeCheck[2] for Oolong
    double totalPrice;
    TeaLink *headLink; // inside the customer, we have a linked list of bubble tea
    CustomerNode *nextNode;
};

class Stack{
    
private:
    CustomerNode *m_headNode; // inside the stack, we have a linked list of customer
    static int s_currNodeNum; // to keep track the current number of customer in the stack // s_ for static naming
    int currLinkNum; // to keep track the current number of bubble tea for each customer

/////////////////////////////////////////
// to remove 'magic number', we use enums
enum teaType{
    EARL_GREY,
    JASMINE,
    OOLONG,
    GO_BACK_TYPE,
    MAX_TEATYPE
};

enum fruitFlavour{
    TARO,
    HONEYDEW,
    MANGO,
    LYCHEE,
    STRAWBERRY,
    GO_BACK_FRUIT,
    MAX_FRUIT_FLAVOUR
};

enum milk{
    YES_MILK,
    NO_MILK,
    GO_BACK_MILK,
    MAX_MILK
};

enum addOn{
    CHEWY_TAPIOCA_PEARL,
    ALOE_VERA,
    CHEWY_JELLY,
    GO_BACK_ADDON,
    MAX_ADDON
};

enum teaInfo{
    TEA_NAME,
    TEA_TYPE,
    FRUIT_FLAVOUR,
    MILK,
    ADD_ON,
    FULL_INFO
};

enum teaAction{
    POINTER_LEFT_BOUND = -1,
    INSERT_TEA_AFTER,
    CHANGE_TEA,
    DELETE_TEA,
    INSERT_TEA_HEAD = 2,
    CLOSE,
    INSERT_TEA_TAIL = 3,
    POINTER_RIGHT_BOUND
};

enum pointerPos{
    POINTER_UP_BOUND = -1,
    TEA_HEAD,
    TEA_TAIL,
    BACK = 1,
    RESET,
    CONFIRM,
    POINTER_DOWN_BOUND
};
/////////////////////////////////////////

public:
    static const string teaTypeList[4];
    static const double teaTypePrice[3];

    static const string fruitFlavourList[6];
    static const double fruitFlavourPrice[5];

    static const string milkList[3];
    static const double milkPrice[2];

    static const string addOnList[4];
    static const double addOnPrice[3];

    static const string menuList[4];
    static const string addOrderList[5];
    static const string teaChoiceList[4];
    static const string summaryList[3];
    static int getOptionsNum(const string &intro, const string *arrayToDislpay, const int numOfOptions, int staticVariable = 0);
    static void printMenu();

public:
    Stack()
    :m_headNode(NULL) // initialize headNode of customer to not pointing to anyrhing
    {
    }

    CustomerNode* nodeAtPos(int nodePos);             // this will return the customer that we are referring to
    TeaLink* linkAtPos(int nodePos, int linkPos);     // this will return the bubble tea that we are referring to

    CustomerNode* getNewNode(string name);            // to make a node to keep new customer data
    void push(string name);                           // to push a customer into the stack
    void pop();

    TeaLink* getNewLink(string teaInfo[], double teaPrice);     // to keep new tea data
    void insertLinkAtHead(string teaInfo[], double teaPrice);
    void insertLinkAtTail(string teaInfo[], double teaPrice);
    void insertLinkAfterPos(int linkPos, string teaInfo[], double teaPrice);

    void printAllCustomerReceipt();
    void printAllID();
    void printOneCustomerReceipt(CustomerNode *tempNode);

    void changeLinkAtPos(int linkPos, string teaInfo[], double teaPrice);  // change bubble tea data 
    void deleteLinkAtPos(int linkPos);                                     // delete bubble tea

    void viewSearchOrder();

    void orderIDSort();
    void teaTypeSort();

    void coutLinkEmpty(int pointer);                                    // cout when customer has no bubble tea
    void coutLink(int pointer, bool teaMenuPress, int teaMenuPointer);  // cout when customer have one or more

    void confirmTea(CustomerNode *tempNode);
    void customerChoice();                                              // the logic that handles customer choice
    string* setTeaInfo(string *teaInfo, int teaOptions, int fruitOptions, int milkOptions, int addOnOptions); // for preset
    double calculateTeaPrice(int teaOptions, int fruitOptions, int milkOptions, int addOnOptions); // for preset
    void pushTea(int pointer, int teaPointer); // to push bubble tea for that customer
    void choiceSort();  // ask the user what type of sort they want

    void presetData();

    static int getRandomID(int min, int max)
    {   
        static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);  // static used for efficiency, so we only calculate this value once
        // evenly distribute the random number across our range
        return static_cast<int>(rand() * fraction * (max - min + 1) + min);
    }
};

int Stack::s_currNodeNum = 0; // current customer number initialize to zero

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bubble tea choices and pricing
const string Stack::teaTypeList[4] = {"Earl Grey", "Jasmine", "Oolong", "Go back"};
const double Stack::teaTypePrice[3] = {4.30, 4.20, 4.50};

const string Stack::fruitFlavourList[6] = {"Taro", "Honeydew", "Mango", "Lychee", "Strawberry", "Go back"};
const double Stack::fruitFlavourPrice[5] = {4.30, 4.50, 4.40, 4.30, 4.40};

const string Stack::milkList[3] = {"Yes", "No", "Go back"};
const double Stack::milkPrice[2] = {1.00, 0.00};

const string Stack::addOnList[4] = {"Chewy Tapioca Pearl", "Aloe Vera", "Chewy Jelly", "Go back"};
const double Stack::addOnPrice[3] = {2.30, 2.00, 2.20};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu options
const string Stack::menuList[4] = {"Add Order", "View an Order", "Summary Report", "Exit"};
const string Stack::addOrderList[5] = {"Menu", "Add Order", "View an Order", "Summary Report", "Exit"};
const string Stack::teaChoiceList[4] = {"Add Tea Here", "Change Tea", "Delete Tea", "Close"};
const string Stack::summaryList[3] = {"Order ID", "Tea Type\n", "Go back"};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CustomerNode* Stack::nodeAtPos(int nodePos){ // return customer node using int node position
    CustomerNode* tempNode = m_headNode;
    int tempPos = 0;
    nodePos -= 1; // array start at 0, need to minus by one

    while(tempPos != nodePos && tempNode->nextNode != NULL){  // traverse to the position of node
        tempNode = tempNode->nextNode;
        tempPos++;
    }
    return tempNode;
}

TeaLink* Stack::linkAtPos(int nodePos, int linkPos){ // return tea link using int node position and int link position
    CustomerNode* tempNode = nodeAtPos(nodePos);
    TeaLink* tempLink = tempNode->headLink;
    int tempPos = 0;
    linkPos -= 1;

    while(tempPos != linkPos && tempLink->nextLink != NULL){
        tempLink = tempLink->nextLink;
        tempPos++;
    }
    return tempLink;
}

CustomerNode* Stack::getNewNode(string name){ // to create a new customer node with randomly generated id
    CustomerNode* newNode = new CustomerNode;

    /*to check whether the ID is unique*/
    CustomerNode* IDCheckNode = m_headNode;
    bool flag;
    do{
        flag = false;
        newNode->orderID = getRandomID(1000, 1100);
        while(IDCheckNode != NULL){
            if(IDCheckNode->orderID == newNode->orderID){   
                flag = true;
                break;
            }
            else{
                IDCheckNode = IDCheckNode->nextNode;
            }
        }
    }while(flag == true); // regenerate if the id has been taken

    newNode->customerName = name;
    newNode->storeNo = getRandomID(1, 9);
    for(int index = 0; index < 3; ++index){   
        newNode->teaTypeCheck[index] = false; // this is used for sorting tea type
    }
    newNode->totalPrice = 0.0;
    newNode->headLink = NULL;
    newNode->nextNode = NULL;
    ++s_currNodeNum;
    return newNode;
} 

void Stack::push(string name){ // to push one customer into the stack
    CustomerNode* newNode = getNewNode(name);

    if(m_headNode == NULL){
        m_headNode = newNode;
        return;
    }
    else{
        nodeAtPos(s_currNodeNum)->nextNode = newNode;
    }
}

void Stack::pop(){ // to pop one customer from the stack

    /*to delete all the customer's bubble tea*/
    while(currLinkNum > 0){
        delete(linkAtPos(s_currNodeNum,currLinkNum));
        --currLinkNum;
    }

    /*to delete a customer and fix the link*/
    if(s_currNodeNum == 1){
        delete(nodeAtPos(s_currNodeNum));
        m_headNode = NULL;
    }
    else if(s_currNodeNum > 1){
        delete(nodeAtPos(s_currNodeNum));
        nodeAtPos(s_currNodeNum-1)->nextNode = NULL;
    }
    --s_currNodeNum;
}

TeaLink* Stack::getNewLink(string teaInfo[], double teaPrice){ // to create a new bubble tea link
    TeaLink* newLink = new TeaLink;

    /*to copy all of the bubble tea details*/
    for(int index = 0; index < FULL_INFO; ++index){
        newLink->teaInfo[index] = teaInfo[index];
    }
    newLink->teaPrice = teaPrice;
    newLink->quantity = 1;
    newLink->nextLink = NULL;
    ++currLinkNum;
    return newLink;
}

void Stack::insertLinkAtHead(string teaInfo[], double teaPrice){ // insert bubble tea at head 
    TeaLink* newLink = getNewLink(teaInfo,teaPrice);
    CustomerNode* tempNode = nodeAtPos(s_currNodeNum);

    if(tempNode->headLink == NULL) {
        tempNode->headLink = newLink;
        return;
    }
    newLink->nextLink = tempNode->headLink; 
    tempNode->headLink = newLink;
}

void Stack::insertLinkAtTail(string teaInfo[], double teaPrice){ // insert bubble tea at tail
    TeaLink* newLink = getNewLink(teaInfo,teaPrice);
    linkAtPos(s_currNodeNum, currLinkNum)->nextLink = newLink;  // this will traverse to the last bubble tea
}

void Stack::insertLinkAfterPos(int linkPos, string teaInfo[], double teaPrice){ // insert bubble tea at position
    TeaLink* tempLink = linkAtPos(s_currNodeNum, linkPos);
    TeaLink* newLink = getNewLink(teaInfo,teaPrice);
    newLink->nextLink = tempLink->nextLink;
    tempLink->nextLink = newLink;
}


void Stack::printAllCustomerReceipt(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BRIGHT_WHITE);
    if(m_headNode == NULL){
        cout << "\n\tStack is Empty\n";
        return;
    }
    CustomerNode* tempNode = m_headNode;
    printf("\n\tCustomer List:\n");
    while(tempNode != NULL) {
        printOneCustomerReceipt(tempNode);
        tempNode = tempNode->nextNode;
        cout << "\n";
    }
}

void Stack::printAllID(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BRIGHT_WHITE);
    if(m_headNode == NULL){
        cout << "\n\tStack is Empty\n";
        return;
    }
    CustomerNode* tempNode = m_headNode;
    cout << "\tCustomer ID in Stack:\n\t";
    while(tempNode != NULL) {
        cout << tempNode->orderID;
        if(tempNode->nextNode != NULL){
            cout    << "->";
        }
        tempNode = tempNode->nextNode;
    }
    cout << "\n\n";
}

void Stack::printOneCustomerReceipt(CustomerNode *tempNode){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BRIGHT_WHITE);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    string months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    std::stringstream date;
    int index = 1;
    
    date << tm.tm_mday << " " << months[tm.tm_mon] << " " << (tm.tm_year + 1900);
    TeaLink* tempLink = tempNode->headLink;
    
    cout << "\tÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n";
    cout << "\t³ Order ID: " << setw(70) << left << tempNode->orderID;
    cout << "Date: " << setw(17) << right << date.str() << " ³\n";
    cout << "\t³ Customer Name: " << setw(65) << left << tempNode->customerName;
    cout << "Store No: S0" << tempNode->storeNo << "           ³\n";
    cout << "\t³ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ³\n";
    cout << "\t³ Item ³  Tea Type   ³ Quantity ³   Flavour    ³ Milk ³        Add-on         ³ Price/item ³  Price (RM)  ³\n";
	
    while(tempLink != NULL){
        cout << "\t³ÄÄÄÄÄÄ³ÄÄÄÄÄÄÄÄÄÄÄÄÄ³ÄÄÄÄÄÄÄÄÄÄ³ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ³ÄÄÄÄÄÄ³ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ³ÄÄÄÄÄÄÄÄÄÄÄÄ³ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ³\n";
        cout << "\t³ " << setw(2) << right << index++ << ".  ³  ";
        cout << setw(9) << left << tempLink->teaInfo[TEA_TYPE] << "  ³    ";
        cout << setw(2) << right << tempLink->quantity << "    ³  ";
        cout << setw(10) << left << tempLink->teaInfo[FRUIT_FLAVOUR] << "  ³ ";
        cout << setw(3) << left << tempLink->teaInfo[MILK] << "  ³  ";
        cout << setw(19) << left << tempLink->teaInfo[ADD_ON] << "  ³   ";
        cout << setw(4) << right << tempLink->teaPrice << "    ³  ";
        cout << setw(8) << right << (static_cast<double>(tempLink->quantity)*tempLink->teaPrice) << "    ³\n";
        tempLink = tempLink->nextLink;
    }
    cout << "\t³ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ³ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ³\n";
    cout << "\t³" << setw(90) << right << "TOTAL " << "³  ";
    cout << setw(8) << right << tempNode->totalPrice << "    ³\n";
    cout << "\tÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n";
}


void Stack::changeLinkAtPos(int linkPos, string teaInfo[], double teaPrice){ // change bubble tea details at position
    TeaLink* tempLink = linkAtPos(s_currNodeNum, linkPos);
    for(int index = 0; index < FULL_INFO; ++index){
        tempLink->teaInfo[index] = teaInfo[index];
    }
    tempLink->teaPrice = teaPrice;
    tempLink->quantity = 1;
}


void Stack::deleteLinkAtPos(int linkPos){ // delete bubble tea at position
    TeaLink *linkToBeDeleted = linkAtPos(s_currNodeNum,linkPos);

    if(linkPos == 1){ // at head
        nodeAtPos(s_currNodeNum)->headLink = linkToBeDeleted->nextLink;
        delete(linkToBeDeleted);
        linkToBeDeleted = NULL;
        --currLinkNum;
        return;
    }
    else if(linkToBeDeleted->nextLink == NULL){ // at tail
        TeaLink *linkBefore = linkAtPos(s_currNodeNum,linkPos-1);
        delete(linkToBeDeleted);
        linkBefore->nextLink = NULL;
        linkToBeDeleted = NULL;
        --currLinkNum;
        return;
    }
    /*anywhere else*/
    TeaLink *linkBefore = linkAtPos(s_currNodeNum,linkPos-1);
    linkBefore->nextLink = linkToBeDeleted->nextLink;
    delete(linkToBeDeleted);
    linkToBeDeleted = NULL;
    --currLinkNum;
}


void Stack::viewSearchOrder(){ // to view the order of a customer based on ID

    cin.ignore(32767,'\n');
    cin.clear();

    if(m_headNode == NULL){
        cout << "\tStack is Empty\n";
        return;
    }

    ShowConsoleCursor(true);
    bool flag = false; // check for non-digit
    string keyID;

    do{
        Stack::printMenu();
        printAllID();

        if (flag == true)
            cout << "\t(Please enter digit only)";
        
        flag = false;
        cout << "\n\tEnter ID to be searched: ";
        getline(cin,keyID);
        trim(keyID);

        // check every character of user input if there is alphabet, etc
        for(unsigned int index = 0; index < keyID.size(); ++index){
            if(!isdigit(keyID[index]))
                flag = true;
        }

    }while(flag == true);

    ShowConsoleCursor(false);
    int key = 0;
    if(keyID != ""){ // user enters nothing
        std::istringstream iss (keyID);
        iss >> key;
    }

    CustomerNode* searchNode = m_headNode;

    while(searchNode != NULL){
        if(searchNode->orderID == key){
            cout << "\n\tCustomer Info:\n";
            printOneCustomerReceipt(searchNode);
            flag = true;
            break;
        }
        else{
            searchNode = searchNode->nextNode;
        }
    }
    if(flag == false)
        printf("\t(Item not found)\n\n");

}


void Stack::orderIDSort(){ // sort algorithm by order ID
    
    cout << "\nAfter Sort Order by Order ID:\n";

    CustomerNode *node1 = m_headNode->nextNode;
    const int max_teaType = 3;

    while(node1 != NULL){
        int secID = node1->orderID;
        string secName = node1->customerName;
        int secNo = node1->storeNo;
        TeaLink *secLinkHead = node1->headLink;
        double secPrice = node1->totalPrice;
        bool secTypeCheck[3];
        for(int index = 0; index < max_teaType; ++index){
            secTypeCheck[index] = node1->teaTypeCheck[index];
        }

        CustomerNode *node2 = m_headNode;
        bool found = false;

        while(node2 != node1) {
            if(node2->orderID > node1->orderID && found == false) {
                secID = node2->orderID;
                secName = node2->customerName;
                secNo = node2->storeNo;
                secLinkHead = node2->headLink;
                secPrice = node2->totalPrice;
                for(int index = 0; index < max_teaType; ++index){
                    secTypeCheck[index] = node2->teaTypeCheck[index];
                }

                node2->orderID = node1->orderID;
                node2->customerName = node1->customerName;
                node2->storeNo = node1->storeNo;
                node2->headLink = node1->headLink;
                node2->totalPrice = node1->totalPrice;
                for(int index = 0; index < max_teaType; ++index){
                    node2->teaTypeCheck[index] = node1->teaTypeCheck[index];
                }

                found = true;
                node2 = node2->nextNode;
            } else {
                if(found == true) {
                    int tempID = secID;
                    string tempName = secName;
                    int tempNo = secNo;
                    TeaLink *tempLinkHead = secLinkHead;
                    double tempPrice = secPrice;
                    bool tempTypeCheck[3];
                    for(int index = 0; index < max_teaType; ++index){
                        tempTypeCheck[index] = secTypeCheck[index];
                    }

                    secID = node2->orderID;
                    secName = node2->customerName;
                    secNo = node2->storeNo;
                    secLinkHead = node2->headLink;
                    secPrice = node2->totalPrice;
                    for(int index = 0; index < max_teaType; ++index){
                        secTypeCheck[index] = node2->teaTypeCheck[index];
                    }

                    node2->orderID = tempID;
                    node2->customerName = tempName;
                    node2->storeNo = tempNo;
                    node2->headLink = tempLinkHead;
                    node2->totalPrice = tempPrice;
                    for(int index = 0; index < max_teaType; ++index){
                        node2->teaTypeCheck[index] = tempTypeCheck[index];
                    }
                }
                node2 = node2->nextNode;
            }
    }
    node2->orderID = secID;
    node2->customerName = secName;
    node2->storeNo = secNo;
    node2->headLink = secLinkHead;
    node2->totalPrice = secPrice;
    for(int index = 0; index < max_teaType; ++index){
        node2->teaTypeCheck[index] = secTypeCheck[index];
    }

    node1 = node1->nextNode;
    }
    printAllCustomerReceipt();
}

void Stack::teaTypeSort(){ // sort algorithm by tea type

    int teaTypeOptions = Stack::getOptionsNum("\tWhich tea type?\n\n", Stack::teaTypeList, OOLONG + 1);
    
    system("cls");
    cout << setcolour(BRIGHT_WHITE) << "\nBefore Sort Order:\n";
    printAllCustomerReceipt();
    
    if(m_headNode->nextNode != NULL){
        CustomerNode* currNode = m_headNode;
        CustomerNode* traverseNode = m_headNode;
        const int max_teaType = 3;

        while(currNode != NULL){
            while(traverseNode == currNode && traverseNode->teaTypeCheck[teaTypeOptions] == true){
                traverseNode = traverseNode->nextNode;
                currNode = traverseNode;
            }

            while(traverseNode->teaTypeCheck[teaTypeOptions] == false){
                traverseNode = traverseNode->nextNode;
                if(traverseNode == NULL){
                    break;
                }
            }

            if(traverseNode == NULL){
                break;
            }

            if(traverseNode->teaTypeCheck[teaTypeOptions] == true){
                int tempID = currNode->orderID;
                string tempName = currNode->customerName;
                int tempNo = currNode->storeNo;
                TeaLink *tempLinkHead = currNode->headLink;
                double tempPrice = currNode->totalPrice;
                bool tempTypeCheck[3];
                for(int index = 0; index < max_teaType; ++index){
                    tempTypeCheck[index] = currNode->teaTypeCheck[index];
                }

                currNode->orderID = traverseNode->orderID;
                currNode->customerName = traverseNode->customerName;
                currNode->storeNo = traverseNode->storeNo;
                currNode->headLink = traverseNode->headLink;
                currNode->totalPrice = traverseNode->totalPrice;
                for(int index = 0; index < max_teaType; ++index){
                    currNode->teaTypeCheck[index] = traverseNode->teaTypeCheck[index];
                }

                traverseNode->orderID = tempID;
                traverseNode->customerName = tempName;
                traverseNode->storeNo = tempNo;
                traverseNode->headLink = tempLinkHead;
                traverseNode->totalPrice = tempPrice;
                for(int index = 0; index < max_teaType; ++index){
                    traverseNode->teaTypeCheck[index] = tempTypeCheck[index];
                }
            }
            currNode = currNode->nextNode;
            traverseNode = currNode;
        }
    }
    cout << "\nAfter Sort Order by " << teaTypeList[teaTypeOptions] << "\n";
    printAllCustomerReceipt();
}


int Stack::getOptionsNum(const string &intro, const string *arrayToDisplay, const int numOfOptions, int staticVariable){ // to get the number of user selected option

    /* The scrolling logic for menu, options, etc */

    /*reset GetAsyncKeyState value*/
    (GetAsyncKeyState(VK_UP)&1);
    (GetAsyncKeyState(VK_DOWN)&1);
    (GetAsyncKeyState(VK_RETURN)&1); 
    Sleep(150);

    int pointer = staticVariable; // remember the last location of the arrow scrolling

    while(true){

        printMenu();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BRIGHT_WHITE);
        cout << intro;

        for (int index = 0; index < numOfOptions; ++index){
            if (index == pointer){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_YELLOW);
                cout << "\t-> " << arrayToDisplay[index] << '\n';
            }
            else{
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BRIGHT_WHITE);
                cout << "\t  " << arrayToDisplay[index] << '\n';
            }
        }

        while(true){
            if (GetAsyncKeyState(VK_UP) != 0){
                pointer -= 1;
                if (pointer == -1){ // out of bound
                    pointer = (numOfOptions - 1);
                }
                break;
            }
            else if (GetAsyncKeyState(VK_DOWN) != 0){
                pointer += 1;
                if (pointer == numOfOptions){ // out of bound
                    pointer = 0;
                }
                break;
            }
            else if (GetAsyncKeyState(VK_RETURN) != 0){ // user press enter
                Sleep(150);
                return pointer;
            }
        }
        Sleep(150);
    }
}

void Stack::printMenu(){  // This function is to display BubbleTea Menu *with colors*

    system("cls");
    cout << std::fixed <<std::setprecision(2);
    cout << setcolour(WHITE) <<"\n\n";
    cout << "\t                           ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\n";
    cout << "\tÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¹  QQ Bubble Tea  ÌÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\n";
    cout << "\tº                          ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼                            º\n";
    cout << "\tº                                                                         º\n";
    cout << "\tÌÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍËÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¹\n";

    cout << "\tº   "<< setcolour(LIGHT_AQUA) << "Tea Types" << setcolour(WHITE);
    cout << "    º            " << setcolour(GRAY) << teaTypeList[EARL_GREY];
    cout << "    " << setcolour(GREEN) << teaTypeList[JASMINE] << "    " << setcolour(BLUE) << teaTypeList[OOLONG];
    cout << setcolour(WHITE) << "              º\n\tº" << setcolour(LIGHT_AQUA) << "     ( RM )     ";
    cout << setcolour(WHITE) << "º              " << setcolour(GRAY) << teaTypePrice[EARL_GREY] <<  "        ";
    cout << setcolour(GREEN) << teaTypePrice[JASMINE] << "       " << setcolour(BLUE) << teaTypePrice[OOLONG] << "               " << setcolour(WHITE);

    cout << "º\n\tÌÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÎÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¹\n";
    cout << "\tº " << setcolour(LIGHT_RED) << "Fruit Flavours" << setcolour(WHITE);
    cout << " º    " << setcolour(LIGHT_PURPLE) << fruitFlavourList[TARO] << "    ";
    cout << setcolour(YELLOW) << fruitFlavourList[HONEYDEW] << "    ";
    cout << setcolour(LIGHT_YELLOW) << fruitFlavourList[MANGO] << "    ";
    cout << setcolour(WHITE) << fruitFlavourList[LYCHEE] << "    ";
    cout << setcolour(RED) << fruitFlavourList[STRAWBERRY] << setcolour(WHITE) << "   º\n\tº";
    cout << setcolour(LIGHT_RED) << "     ( RM )     " << setcolour(WHITE) << "º    ";
    cout << setcolour(LIGHT_PURPLE) << fruitFlavourPrice[TARO] << "      " << setcolour(YELLOW) << fruitFlavourPrice[HONEYDEW] << "      ";
    cout << setcolour(LIGHT_YELLOW) << fruitFlavourPrice[MANGO] << "      " << setcolour(WHITE) << fruitFlavourPrice[LYCHEE] << "        ";
    cout << setcolour(RED) << fruitFlavourPrice[STRAWBERRY] << "      " << setcolour(WHITE);

    cout << "º\n\tÌÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÎÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¹\n";
    cout << "\tº     " << setcolour(BRIGHT_WHITE) << "Add on" << setcolour(WHITE);
    cout << "     º    " << setcolour(PURPLE) << addOnList[CHEWY_TAPIOCA_PEARL]  << "    ";
    cout << setcolour(LIGHT_GREEN) << addOnList[ALOE_VERA] << "    ";
    cout << setcolour(AQUA) << addOnList[CHEWY_JELLY] << setcolour(WHITE) << "     º\n\tº";
    cout << setcolour(BRIGHT_WHITE) << "     ( RM )     " << setcolour(WHITE) << "º           ";
    cout << setcolour(PURPLE) << addOnPrice[CHEWY_TAPIOCA_PEARL] << "              ";
    cout << setcolour(LIGHT_GREEN) << addOnPrice[ALOE_VERA] << "          ";
    cout << setcolour(AQUA) << addOnPrice[CHEWY_JELLY] << "         " << setcolour(WHITE);

    cout  << "º\n\tÌÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÊÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¹\n";
    cout << "\tº                                                                         º\n";
    cout << "\tº                            Milk is optional                             º\n";
    cout << "\tº                               ( RM " << milkPrice[YES_MILK] << " )                               º\n";
    cout << "\tÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼\n";
    cout << setcolour(BRIGHT_WHITE);
}


void Stack::coutLinkEmpty(int pointer){ // cout option when there is no bubble tea for that customer
    const int numOfOptions = 2;
    for(int index = 0; index < numOfOptions; ++index){
        if(index == pointer){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_YELLOW);
            if(index == TEA_HEAD)
                cout << "\t-> " << "(Add Tea at Head)" << '\n';
            else
                cout << "\t-> " << "Go Back (delete)" << '\n';
        }
        else{
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BRIGHT_WHITE);
            if(index == TEA_HEAD)
                cout << "\t  " << "(Add Tea at Head)" << '\n';
            else
                cout << "\t  " << "Go Back (delete)" << '\n';
        }
    }
}

void Stack::coutLink(int pointer, bool teaMenuPress, int teaMenuPointer){ // cout option when there is bubble tea for that customer
    char leftArrow ,rightArrow ,space;
    leftArrow = 17;
    rightArrow = 16;
    space = ' ';
    TeaLink* tempLink = nodeAtPos(s_currNodeNum)->headLink;

    for(int index = 0; index < currLinkNum + POINTER_DOWN_BOUND; ++index){ //magic number
        if(index == pointer){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_YELLOW);
            if(index == TEA_HEAD)
                cout << "\t-> " << "(Add Tea at Head)" << '\n';
            else if(index == currLinkNum + TEA_TAIL)
                cout << "\t-> " << "(Add Tea at Tail)" << '\n';
            else if(index == currLinkNum + RESET)
                cout << "\n\t-> " << "Go Back (delete)" << '\n';
            else if(index == currLinkNum + CONFIRM)
                cout << "\t-> " << "Confirm" << '\n';
            else{
                if(teaMenuPress == false){
                    cout << "\t-> #"; 
                    cout << setw(2) << left << index << " " ;
                    cout << left << setw(58) << tempLink->teaInfo[TEA_NAME];
                    cout << ( tempLink->quantity == 1 ? space : leftArrow);
                    cout << setw(3) << right << tempLink->quantity;
                    cout << " ";
                    if(tempLink->quantity != 99){
                        cout << rightArrow;
                    }
                    cout << '\n';
                }
                else{
                    cout << "\t  #"; 
                    cout << setw(2) << left <<index << " ";
                    cout << left << setw(60) <<tempLink->teaInfo[TEA_NAME];
                    cout << setw(3) << right << tempLink->quantity << "\n";
                    cout << "\t    " << leftArrow << " " << teaChoiceList[teaMenuPointer]; 
                    cout << " " <<rightArrow << '\n';
                }
                tempLink = tempLink->nextLink;
            }
        }
        else{
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BRIGHT_WHITE);
            if(index == TEA_HEAD)
                cout << "\t  " << "(Add Tea at Head)" << '\n';
            else if(index == currLinkNum + TEA_TAIL)
                cout << "\t  " << "(Add Tea at Tail)" << '\n';
            else if(index == currLinkNum + RESET)
                cout << "\n\t  " << "Go Back (delete)" << '\n';
            else if(index == currLinkNum + CONFIRM)
                cout << "\t  " << "Confirm" << '\n';
            else {
                cout << "\t  #"; 
                cout << setw(2) << left <<index << " ";
                cout << left << setw(60) << tempLink->teaInfo[TEA_NAME];
                cout << setw(3) << right << tempLink->quantity << "\n";
                tempLink = tempLink->nextLink;
            }
        }
    }
}

void Stack::confirmTea(CustomerNode *tempNode){ // function when user confirm their order
    TeaLink* tempLink = tempNode->headLink;
    while(tempLink != NULL){
        tempNode->totalPrice += tempLink->teaPrice * tempLink->quantity;

        if(tempLink->teaInfo[TEA_TYPE] == "Earl Grey"){
            tempNode->teaTypeCheck[EARL_GREY] = true;
        }
        if(tempLink->teaInfo[TEA_TYPE] == "Jasmine"){
            tempNode->teaTypeCheck[JASMINE] = true;
        }
        if(tempLink->teaInfo[TEA_TYPE] == "Oolong"){
            tempNode->teaTypeCheck[OOLONG] = true;
        }
        tempLink = tempLink->nextLink;
    }
}

void Stack::customerChoice(){ // choice to add, delete, change, etc for bubble tea for that customer

    (GetAsyncKeyState(VK_RETURN)&1);
    Sleep(150);

    int pointer = 0;
    currLinkNum = 0;
    CustomerNode* tempNode = nodeAtPos(s_currNodeNum);

    string instruction = (string)"\t(Move using " + (char)24 + (char)25 + (char)27 + (char)26 + " key, select using enter key)\n";

    /*customer choice logic*/
    while(true){

        printMenu();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BRIGHT_WHITE);
        cout << instruction;
        cout << "\tCustomer ID: " << tempNode->orderID << '\n';
        cout << "\tCustomer Name: " << tempNode->customerName << "\n";
        cout << '\t' << std::right << setw(73) << "Quantity\n";

        if(tempNode->headLink == NULL){
            coutLinkEmpty(pointer);

            while(true){
                if (GetAsyncKeyState(VK_UP) != 0){
                    pointer -= 1;
                    if (pointer == POINTER_UP_BOUND){
                        pointer = BACK;
                    }
                    break;
                }
                else if (GetAsyncKeyState(VK_DOWN) != 0){
                    pointer += 1;
                    if (pointer == (POINTER_DOWN_BOUND - 2)){
                        pointer = TEA_HEAD;
                    }
                    break;
                }
                else if (GetAsyncKeyState(VK_RETURN) != 0){
                    Sleep(150);
                    if(pointer == TEA_HEAD){
                        pushTea(pointer,INSERT_TEA_HEAD);
                        break;
                    }
                    else{
                        pop();
                        cout << '\n';
                        printAllID();
                        return;
                    }
                }
            }
            Sleep(150);
        }
        else{
            static bool teaMenuPress = false;
            static int teaMenuPointer = 0;
            coutLink(pointer, teaMenuPress, teaMenuPointer);

            while(true){
                TeaLink* tempLink = linkAtPos(s_currNodeNum, pointer);

                if (GetAsyncKeyState(VK_UP) != 0){
                    pointer -= 1;
                    if (pointer == POINTER_UP_BOUND){
                        pointer = currLinkNum + CONFIRM;
                    }
                    if(teaMenuPress == true){
                        teaMenuPress = false;
                        teaMenuPointer = 0;
                    }
                    break;
                }
                else if (GetAsyncKeyState(VK_DOWN) != 0){
                    pointer += 1;
                    if (pointer == currLinkNum + POINTER_DOWN_BOUND){
                        pointer = TEA_HEAD;
                    }
                    if(teaMenuPress == true){
                        teaMenuPress = false;
                        teaMenuPointer = 0;
                    }
                    break;
                }
                else if((pointer > TEA_HEAD && pointer < currLinkNum + TEA_TAIL) && GetAsyncKeyState(VK_LEFT) != 0){
                    if(teaMenuPress == true){
                        teaMenuPointer -= 1;
                        if(teaMenuPointer == POINTER_LEFT_BOUND){
                            teaMenuPointer = CLOSE;
                        }
                    }
                    else if(tempLink->quantity > 1){
                        tempLink->quantity -= 1;
                    }
                    break;
                }
                else if ((pointer > TEA_HEAD && pointer < currLinkNum + TEA_TAIL) && GetAsyncKeyState(VK_RIGHT) != 0){
                    if(teaMenuPress == true){
                        teaMenuPointer += 1;
                        if(teaMenuPointer == POINTER_RIGHT_BOUND){
                            teaMenuPointer = INSERT_TEA_AFTER;
                        }
                    }
                    else if(tempLink->quantity < 99){
                        tempLink->quantity += 1;
                    }
                    break;
                }
                else if (GetAsyncKeyState(VK_RETURN) != 0){
                    Sleep(150);
                    if(pointer == TEA_HEAD){
                        pushTea(pointer,INSERT_TEA_HEAD);
                        break;
                    }
                    else if(pointer == currLinkNum + TEA_TAIL){
                        pushTea(pointer,INSERT_TEA_TAIL);
                        pointer += 1;
                        break;
                    }
                    else if(pointer == currLinkNum + RESET){
                        pop();
                        cout << '\n';
                        printAllID();
                        return;
                    }
                    else if(pointer == currLinkNum + CONFIRM){
                        
                        confirmTea(tempNode);
                        cout << setcolour(BRIGHT_WHITE) <<"\n\tCustomer Info:\n";
                        printOneCustomerReceipt(tempNode);
                        cout << '\n';
                        printAllID();
                        return;
                    }
                    else{
                        if(teaMenuPress == false)
                            teaMenuPress = true;
                        else{
                            if(teaMenuPointer == INSERT_TEA_AFTER){
                                pushTea(pointer,INSERT_TEA_AFTER);
                            }
                            else if(teaMenuPointer == CHANGE_TEA){
                                pushTea(pointer,CHANGE_TEA);
                            }
                            else if(teaMenuPointer == DELETE_TEA){
                                deleteLinkAtPos(pointer);
                            }
                            teaMenuPress = false;
                            teaMenuPointer = 0;
                        }
                        break;
                    }
                }
            }
            Sleep(150);
        }
    }
}

/*to set bubble tea data*/
string* Stack::setTeaInfo(string *teaInfo, int teaOptions, int fruitOptions, int milkOptions, int addOnOptions){

    teaInfo[TEA_NAME] =  teaTypeList[teaOptions] + " " + fruitFlavourList[fruitOptions] + " " +
                (milkOptions == NO_MILK ? "" : "Milk ") + "Tea" + " (with " + addOnList[addOnOptions] + ')';
    
    teaInfo[TEA_TYPE] = teaTypeList[teaOptions];
    teaInfo[FRUIT_FLAVOUR] = fruitFlavourList[fruitOptions];
    teaInfo[MILK] = milkList[milkOptions];
    teaInfo[ADD_ON] = addOnList[addOnOptions];
    return teaInfo;
}

/*to calculate the bubble tea price*/
double Stack::calculateTeaPrice(int teaOptions, int fruitOptions, int milkOptions, int addOnOptions){
    return teaTypePrice[teaOptions] + fruitFlavourPrice[fruitOptions] + milkPrice[milkOptions] + addOnPrice[addOnOptions];
}


void Stack::pushTea(int pointer,int teaMenuPointer){ // to add a bubble tea for that customer

    int teaOptions = 0;
    int fruitOptions;
    int milkOptions;
    int addOnOptions;

    /*remember arrow scrolling position logic*/
    do{
        fruitOptions = 0;
        teaOptions = Stack::getOptionsNum("\tChoose your tea:\n\n", Stack::teaTypeList, MAX_TEATYPE, teaOptions);

        if(teaOptions == GO_BACK_TYPE){
            return;
            }
        
        do{
            milkOptions = 0;
            fruitOptions = Stack::getOptionsNum("\tFruits are nutritious:\n\n", Stack::fruitFlavourList, MAX_FRUIT_FLAVOUR, fruitOptions);

            if(fruitOptions == GO_BACK_FRUIT){
                continue;
            }

            do{
                addOnOptions = 0;
                milkOptions = Stack::getOptionsNum("\tDo you want milk to go with your tea?\n\n", Stack::milkList, MAX_MILK, milkOptions);
    
                if(milkOptions == GO_BACK_MILK){
                    continue;
                    }
    
                addOnOptions = Stack::getOptionsNum("\tSome addons that you can add in your tea:\n\n", Stack::addOnList,MAX_ADDON, addOnOptions);
    
            } while((addOnOptions == GO_BACK_ADDON) && (milkOptions != GO_BACK_MILK));
    
        } while((milkOptions == GO_BACK_MILK) && (fruitOptions != GO_BACK_FRUIT));

    } while(fruitOptions == GO_BACK_FRUIT);

    string teaData[FULL_INFO];
    string *teaInfo = setTeaInfo(teaData,teaOptions,fruitOptions,milkOptions,addOnOptions);
    
    double teaPrice = calculateTeaPrice(teaOptions,fruitOptions,milkOptions,addOnOptions);

    if(teaMenuPointer == INSERT_TEA_AFTER)
        insertLinkAfterPos(pointer,teaInfo,teaPrice);
    else if(teaMenuPointer == CHANGE_TEA)
        changeLinkAtPos(pointer, teaInfo,teaPrice);
    else if(teaMenuPointer == INSERT_TEA_HEAD)
        insertLinkAtHead(teaInfo,teaPrice);
    else if(teaMenuPointer == INSERT_TEA_TAIL){
        insertLinkAtTail(teaInfo,teaPrice);
        --pointer;
    }

    if(teaMenuPointer != CHANGE_TEA)
        cout << "\n\t" << teaInfo[TEA_NAME]  << " is added at tea #"  << pointer+1 << "\n\t";
    else
        cout << "\n\t" << "Tea #" << pointer << " is changed to " << teaInfo[TEA_NAME] << "\n\t";

    system("pause");
}

void Stack::choiceSort(){ // user can choose two type of sort, ID or tea type sort

    const int MAX_SUMMARY_NUM = 3;

    if(m_headNode == NULL){
        printAllCustomerReceipt();
    }
    else{
        int summaryOptions = Stack::getOptionsNum("\tShow summary order sorted by:\n\n", Stack::summaryList, MAX_SUMMARY_NUM);
        
        switch(summaryOptions){

            case 0:{
                system("cls");
                cout << "\nBefore Sort Order:\n";
                printAllCustomerReceipt();
                orderIDSort();
            }
            break;
            case 1:{
                teaTypeSort();
            }
            break;
            case 2:{
                return;
            }
        }
    }
    cout << "\t";
    system("pause");
}

void Stack::presetData(){ // to set preset data

    string tea1Data[FULL_INFO];
    string *tea1Info = setTeaInfo(tea1Data,OOLONG,TARO,YES_MILK,CHEWY_JELLY);
    double tea1Price = calculateTeaPrice(OOLONG,TARO,YES_MILK,CHEWY_JELLY);

    string tea2Data[FULL_INFO];
    string *tea2Info = setTeaInfo(tea2Data,JASMINE,STRAWBERRY,NO_MILK,CHEWY_TAPIOCA_PEARL);
    double tea2Price = calculateTeaPrice(JASMINE,STRAWBERRY,NO_MILK,CHEWY_TAPIOCA_PEARL);

    string tea3Data[FULL_INFO];
    string *tea3Info = setTeaInfo(tea3Data,EARL_GREY,MANGO,NO_MILK,ALOE_VERA);
    double tea3Price = calculateTeaPrice(EARL_GREY,MANGO,NO_MILK,ALOE_VERA);

    string tea4Data[FULL_INFO];
    string *tea4Info = setTeaInfo(tea4Data,OOLONG,LYCHEE,NO_MILK,CHEWY_JELLY);
    double tea4Price = calculateTeaPrice(OOLONG,LYCHEE,NO_MILK,CHEWY_JELLY);

    string tea5Data[FULL_INFO];
    string *tea5Info = setTeaInfo(tea5Data,JASMINE,HONEYDEW,YES_MILK,CHEWY_JELLY);
    double tea5Price = calculateTeaPrice(JASMINE,HONEYDEW,YES_MILK,CHEWY_JELLY);

    push("Thanos");
    insertLinkAtHead(tea1Info,tea1Price);
    linkAtPos(s_currNodeNum,currLinkNum)->quantity = 2;

    insertLinkAtTail(tea1Info,tea1Price);

    insertLinkAtTail(tea1Info,tea1Price);
    linkAtPos(s_currNodeNum,currLinkNum)->quantity = 3;

    confirmTea(nodeAtPos(s_currNodeNum));


    push("Stanley");
    insertLinkAtHead(tea1Info,tea1Price);

    insertLinkAtTail(tea2Info,tea2Price);
    linkAtPos(s_currNodeNum,currLinkNum)->quantity = 4;

    insertLinkAtTail(tea3Info,tea3Price);
    linkAtPos(s_currNodeNum,currLinkNum)->quantity = 2;

    confirmTea(nodeAtPos(s_currNodeNum));


    push("Ciko");

    insertLinkAtHead(tea2Info,tea2Price);
    linkAtPos(s_currNodeNum,currLinkNum)->quantity = 99;

    insertLinkAtTail(tea5Info,tea5Price);

    insertLinkAtTail(tea4Info,tea4Price);
    linkAtPos(s_currNodeNum,currLinkNum)->quantity = 1;

    confirmTea(nodeAtPos(s_currNodeNum));


    push("Burger King Foot Lettuce");

    insertLinkAtHead(tea5Info,tea5Price);
    linkAtPos(s_currNodeNum,currLinkNum)->quantity = 69;

    insertLinkAtTail(tea1Info,tea1Price);
    linkAtPos(s_currNodeNum,currLinkNum)->quantity = 69;

    confirmTea(nodeAtPos(s_currNodeNum));


    push("I Congratulations");

    insertLinkAtHead(tea3Info,tea3Price);
    linkAtPos(s_currNodeNum,currLinkNum)->quantity = 6;

    confirmTea(nodeAtPos(s_currNodeNum));
}

BOOL gotoxy(const WORD x, const WORD y){
        COORD xy;
        xy.X = x;
        xy.Y = y;
        return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}

/*print welcome*/
void welcome(){
    unsigned int x,y;

    for(x = 11; x < 70; ++x){
        gotoxy(x,8);
        Sleep(30);
        cout << "Ä";
    }
    cout << "¿";

    for(x = 70; x > 8; --x){
        gotoxy(x,15);
        Sleep(30);
        cout<<" À ";
    }
	
    gotoxy(10,8);
    printf("Ú");
    for(y = 9; y < 15; ++y){
        gotoxy(10,y);
        Sleep(100);
        cout << "³";
    }
    
    gotoxy(70,15);
    printf("Ù");
    for(y = 14; y > 8; --y){
        gotoxy(70,y);
        Sleep(100);
        cout << "³";
    }

    for(x = 69; x > 10; --x){
        gotoxy(x,15);
        Sleep(30);
        cout << "Ä";
    }

    gotoxy(18,10);
    string firstString = "Welcome To  .  .  .";
    for(x = 0; x < firstString.size(); ++x){
        cout << firstString[x];
        Sleep(200);
    }
    Sleep(300);
	
    string secondString = " TEA-SERIES QQ BUBBLE TEA";
    for(x = 0; x < secondString.size(); ++x){
        cout << secondString[x];
        Sleep(100);
    }
	
    gotoxy(35,13);
    printf("loading...");
    gotoxy(30,13);
    for(int row = 1; row < 21; ++row)
    {
        Sleep(200);
        printf("%c",177);
    }
    gotoxy(36,12);
    cout << "Finished";
    gotoxy(25,19);
    system("pause");

}

void sans(){
    unsigned int x;
    unsigned int column;

    for(x = 15; x < 24; ++x){
        gotoxy(x,8);
        Sleep(30);
        cout << setcolour(BRIGHT_WHITE) << (char)219;
    }

    for(x = 13; x < 26; ++x){
        gotoxy(x,9);
        Sleep(30);
        cout << (char)219;
    }

    for(x = 13; x < 26; ++x){
        gotoxy(x,10);
        Sleep(30);
        cout << (char)219;
    }

    for(column = 11; column < 14; column++){
        for(x = 12; x < 27; ++x){
            gotoxy(x,column);
            Sleep(30);
            if((x > 13 && x < 17 && column > 11) || (x > 21 && x < 25 && column > 11) || (x == 19 && column == 13) )
                cout << setcolour(BLACK);
            else
                cout << setcolour(BRIGHT_WHITE);
            cout << (char)219;
        }
    }

    for(x = 13; x < 26; ++x){
        gotoxy(x,14);
        Sleep(30);
        if(x > 17 && x < 21)
            cout << setcolour(BLACK);
        else
            cout << setcolour(BRIGHT_WHITE);
        cout << (char)219;
    }

    for(x = 13; x < 26; ++x){
        gotoxy(x,15);
        Sleep(30);
        if(x == 14 || x == 24 )
            cout << setcolour(BLACK);
        else
            cout << setcolour(BRIGHT_WHITE);
        cout << (char)219;
    }

    for(x = 12; x < 27; ++x){
        gotoxy(x,16);
        Sleep(30);
        if(x > 13 && x < 25)
            cout << setcolour(BLACK);
        else
            cout << setcolour(BRIGHT_WHITE);
        cout << (char)219;
    }

    for(x = 12; x < 27; ++x){
        gotoxy(x,17);
        Sleep(30);
        if(x == 15 || x == 17 || x == 19 || x == 21 || x == 23 )
            cout << setcolour(BLACK);
        else
            cout << setcolour(BRIGHT_WHITE);
        cout << (char)219;
    }

    for(x = 14; x < 25; ++x){
        gotoxy(x,18);
        Sleep(30);
        if(x > 15 && x < 23)
            cout << setcolour(BLACK);
        else
            cout << setcolour(BRIGHT_WHITE);
        cout << (char)219;
    }

    for(x = 16; x < 23; ++x){
        gotoxy(x,19);
        Sleep(30);
        cout << (char)219;
    }

    gotoxy(15,10);
    string firstString = "Thank You";
    cout << setcolour(BLACK,BRIGHT_WHITE);
    for(x = 0; x < firstString.size(); ++x){
        cout << firstString[x];
        Sleep(200);
    }
    gotoxy(16,22);
    Sleep(300);
}

int main(){
	
    welcome();

    ShowConsoleCursor(false);
    srand(static_cast<unsigned int>(time(0))); // set initial seed value to system clock
    rand(); // If using Visual Studio, discard first random value

    Stack stack;

    // simply as an introduction to the user / guide
    string menuIntro = (string)"\t(Move using " + (char)24 + char(25) + " key, confirm using enter key)\n";
    string presetIntro =  menuIntro + "\t Do you want preset data? (5 orders)\n\n";

    // ask the user if they want preset data
    int presetOptions;
    presetOptions = Stack::getOptionsNum(presetIntro, Stack::milkList, 2);

    if(presetOptions == 0){
        stack.presetData();
    }
    cin.ignore(32767,'\n');
    cin.clear();

    while(true){

        static int menuOptions;
        menuOptions = Stack::getOptionsNum(menuIntro+"\n", Stack::menuList, 4, menuOptions);
        
        switch(menuOptions){
            
            case 0:{
                cin.ignore(32767,'\n');
                cin.clear();

                ShowConsoleCursor(true);
                string name;
                bool flag = false;
                
                do{
                    Stack::printMenu();
					
                    if (name.size() > 64)
                        cout << "\t(Name is too long, please enter again)";

                    if(name == "" && flag == true){
                        cout << "\t(Nothing is entered)";
                        flag = false;
                    }
                    cout <<"\n\n\t   Enter name: ";
                    getline(cin,name);
                    trim(name);
                    
                    if(name == ""){
                        flag = true;
                    }

                } while(name == ""||name.size() > 64);
                
                ShowConsoleCursor(false);
                stack.push(name);
                stack.customerChoice();
                cout << "\t";
                system("pause");
            }
            break;
            case 1:{
                stack.viewSearchOrder();
                cout << "\t";
                system("pause");
            }
            break;
            case 2:{
                stack.choiceSort();
            }
            break;
            case 3:{
                int exitOptions;
                exitOptions = Stack::getOptionsNum("\tDo you really wish to exit?\n\n", Stack::milkList, 2);

                if(exitOptions == 0){
                    cout << setcolour(BRIGHT_WHITE);
                    system("cls");
                    sans();
                    exit(0);
                }
            }
        }
    }
    return 0;
}

