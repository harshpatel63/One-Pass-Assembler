/*
*   Project : One Pass Assembler that generates object program.
*   
*   Group No : 4
*   
*   Name of members : Roshan Kumar (19CSE1027)
*                     Patel Harsh Rajesh (19CSE1019)
*
*   For instructions on how to use it, kindly refer the Readme.txt file.
*/

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<cstring>
#include<iomanip>
#include<algorithm>
using namespace std;

struct Optab
{
string mnemonic;
string opcode;
};

struct Node
{
    string notDefinedAddress;
    struct Node *link;
};


struct Symtab
{
    string symbol;
    string address;
    struct Node *link;
};


vector<string> assemblerDirectives = {"START", "END", "BYTE", "RESB", "WORD", "RESW"};
vector<Symtab> vSymtab;
vector<string> location;
vector<Optab> vOptab;
ostringstream objectProgram;
ostringstream preObjectProgram;
ostringstream lineObjProg;
ostringstream preLineObjProg;
string filename;
string name;
int startingAddress;
int endingAddress;
int firstExeAdd;
int lineStartAddress;
int objCodeCounter = 0;
bool firstExeAddBool = true;

void initOptab();
void printSplashScreen();
void onePassScan();
void printMainScreen();
void enterSymbolInColumn1( string temp , string value);
void enterSymbolInColumn3( string temp , string value);
void searchSymtabForColumn1(string symbol, string value);
void enterExtra(struct Node *link , string value , int i);
void moveToNextLine();
int searchSymtab(string temp);
int returnIndex( string temp);
bool isAssemblerDirective(string s);
string searchInOptab(string s);
string seachInSymtabForLoc( string temp);
string returnObjectCodeForAD(string str1, string str2);


int main(int argc, char *argv[])
{
    printSplashScreen();

    if(argc == 2)
    {
        filename = argv[1];
    }
    else
    {
        //If there isn't any input text file from the command line, it will consider input1.txt provided.
        cout<<endl<<"Your filename is not proper... Running the default file input1.txt\n"<<endl;
        filename = "input1.txt";
    }

    initOptab();

    onePassScan();

    printMainScreen();

    cin.get();

    return 0;
}



void moveToNextLine()
{
    if(preLineObjProg.str() == "")
    return;
    string temp5 = preLineObjProg.str();
    temp5.erase(remove(temp5.begin(), temp5.end(), '^'), temp5.end());
    int s = temp5.size()/2;
    lineObjProg<<"T^"<<setfill('0')<<setw(6)<<hex<<lineStartAddress<<"^"<<setfill('0')<<setw(2)<<hex<<s;
    lineObjProg<<preLineObjProg.str()<<"\n";
    preLineObjProg.str("");
    preLineObjProg.clear();
    preObjectProgram<<lineObjProg.str();
    lineObjProg.str("");
    lineObjProg.clear();
    objCodeCounter = 0;
}

bool isAssemblerDirective(string s)
{
    //Checks whether string s is an Assembler Directive or not.
    for(int i = 0; i<assemblerDirectives.size(); i++)
    {
        if(s == assemblerDirectives[i])
        return true;
    }
    return false;
}

void enterExtra(struct Node *link , string value , int i)
{
    //This Function creates link to the linked list used in symtab.
   if (link == NULL)
   {  
       struct Node *head ;
       head = new Node();
       head->notDefinedAddress =value;
       head->link = NULL;
       vSymtab[i].link = head;
       
   }
   else
   {   
       struct Node *temp = NULL;
       struct Node *newentry = NULL;
       
       temp = link;
      
       while (temp->link != NULL)
       {   
        temp = temp->link;
       }
      
       newentry = new Node();
       newentry->notDefinedAddress = value;
       newentry->link = NULL;
       temp->link = newentry;
   }
   
}

int searchSymtab( string temp)
{    
    int found = 0;

    for (int i = 0; i < vSymtab.size(); i++)
    {
        if(strcmp(vSymtab[i].symbol.c_str() , temp.c_str())==0)
        {    
           found = 1; 
           return 1;
        }
    }
    if (found == 0)
    {
       return 0;
    }
    
}

int returnIndex( string temp)
{
    for (int i = 0; i < vSymtab.size(); i++)
    {
        if(strcmp(vSymtab[i].symbol.c_str() , temp.c_str())==0)
        {    
           return i;
        }
    }
}

void enterSymbolInColumn1( string temp , string value)
{   
    struct Symtab s;
    s.symbol = temp;
    s.address = value;
    s.link = NULL;
    vSymtab.push_back(s);
}

void enterSymbolInColumn3( string temp , string value)
{   
    
    struct Symtab s;
    
  
        if(searchSymtab(temp))
        {  
        int i;
        i = returnIndex(temp);
        enterExtra(vSymtab[i].link , value , i);
        }
        else
        {  
           int i;
           s.symbol= temp;
           s.address = "*";
           s.link = NULL;
           vSymtab.push_back(s);
           i = returnIndex(temp);
           enterExtra( s.link , value , i);
          }
}
 
void searchSymtabForColumn1(string symbol, string value)
{   
    for (int i = 0; i < vSymtab.size(); i++)
    {
        if(strcmp(vSymtab[i].symbol.c_str() , symbol.c_str())==0)
        {    
            if( vSymtab[i].address == "*")
            {
                moveToNextLine();
                 struct Node *temp;
                 struct Node *next;
                 temp = vSymtab[i].link;
                 while (temp != NULL)
                 {  
                    preObjectProgram<<"T"<<"^00"<<temp->notDefinedAddress<<"^02^"<<value<<endl;
                    next= temp->link;
                    free(temp);
                    temp = next;
                 }
                 
                 vSymtab.erase( vSymtab.begin() + i);
                 enterSymbolInColumn1(symbol , value);
                 
            }
        }
    }
} 

void onePassScan()
{
    //The Prime function of the C++ program.
    //It scans the input file line by line and does the related job
    // of location incrementation, getting object code and many more.
    int loc;
    string line, col1, col2, col3;
    ifstream file(filename);
    bool afterAssemDir = false;
    
    if(file.is_open())
    {

        int tabPos[2];
        while(getline(file, line))
        {
            if(line[0] == '.')
                continue;
            bool firstTab = true;
            bool isIndexAdd =false;
            string opcode;
            string locFromSymtab;
            string ObjectCode;

            for(int i = 0; i<line.size(); i++)
            {
                if(line[i] == '\t' && firstTab == true)
                {
                firstTab = false;
                tabPos[0] = i;
                }
                else if(line[i] == '\t')
                    tabPos[1] = i;
            }

            col1 = line.substr(0,tabPos[0]);
            string lineReverse = line;
            reverse(lineReverse.begin(),lineReverse.end());
            ostringstream hexAdd;
            hexAdd<<hex<<loc;
            string push = hexAdd.str();
            if ( col2 != "START")
                location.push_back(push);
            if(lineReverse.substr(0,4) == "BUSR")
            {
                //We had to handle RSUB in this way only.
                //Since, the column 3 for RSUB doesn't existed.
                loc+=3;
                opcode = searchInOptab("RSUB");
                ObjectCode = opcode + "0000";
                preLineObjProg<<"^"<<ObjectCode;
                if(objCodeCounter == 0)
                    lineStartAddress = loc-3;
                objCodeCounter++;
                if(objCodeCounter == 10)
                    moveToNextLine();
                continue;
            }
            col2 = line.substr(tabPos[0] + 1, tabPos[1] - tabPos[0] - 1);
            col3 = line.substr(tabPos[1] + 1, line.size() - tabPos[1]);
            
            
               


            if(isAssemblerDirective(col2))
            {
                if(col2 == "START")
                    {
                        istringstream(col3)>>hex>>loc;
                        for(int i = col1.size();i<6;i++)
                        {
                            col1.append(" ");
                        }
                        name = col1;
                        startingAddress = loc;
                        lineStartAddress = loc;
                        continue;
                    }
                else if(col2 == "BYTE")
                {
                    afterAssemDir = true;
                    if(col3[0] == 'C')
                        loc += 3;
                    else
                        loc += 1;
                    string objCodeForByte = returnObjectCodeForAD(col2, col3);
                    preLineObjProg<<"^"<<objCodeForByte;
                }
                else if(col2 == "RESB")
                {
                    int temp;
                    istringstream(col3)>>temp;
                    loc += 1*temp;
                }
                else if(col2 == "RESW")
                {
                    afterAssemDir = true;
                    int temp;
                    istringstream(col3)>>temp;
                    loc += 3*temp;
                }
                else if(col2 == "WORD")
                {
                    loc+=3;
                    string objCodeForWord = returnObjectCodeForAD(col2, col3);
                    preLineObjProg<<"^"<<objCodeForWord;
                }
                else if(col2 == "END")
                    {
                        moveToNextLine();
                        loc+=3;
                        endingAddress = loc-3;
                        objectProgram<<"H^"<<name<<"^00"<<hex<<startingAddress<<"^00"<<hex<<endingAddress-startingAddress<<endl;
                        objectProgram<<preObjectProgram.str();
                        objectProgram<<"E^00"<<hex<<firstExeAdd;
                    }
                else
                    loc+=3;
            }
            else
            {  
                try
                {
                    //Error Handling for if the opcode is not mentioned in the optab.txt file.
                    opcode = searchInOptab(col2);
                }
                catch (int x)
                {
                    if(x == -1)
                    {
                        cout<<"WARNING: The mnemonic code "<<col2<<" is not present in optab."<<endl;
                        cout<<endl<<"Please add the opcode for the respective mnemonic code in optab.txt"<<endl;
                        exit(1);
                    }
                }
                if(firstExeAddBool)
                    {
                        firstExeAdd = loc;
                        firstExeAddBool = false;
                    }
                loc+=3;
                if(afterAssemDir)
                {
                    moveToNextLine();
                    afterAssemDir = false;
                }
            }
            
            
            
        
            
            if(col1[0] != ' ')
            { 
                 if (searchSymtab(col1))
                 {  
                    searchSymtabForColumn1( col1 , push);
                 }
                 else
                 {
                     enterSymbolInColumn1( col1 , push);
                 }
                 
            }

            if(!isAssemblerDirective(col2))
            {
                if(col3[col3.size()-1] == 'X' && col3[col3.size()-2] == ',')
                {
                    col3 = col3.substr(0,col3.size()-2);//BUFFER,X changes to BUFFER
                    isIndexAdd = true;
                }
                int temp;
                istringstream(push)>>hex>>temp;
                temp+=1;
                ostringstream t;
                t<<hex<<temp;
                enterSymbolInColumn3(col3 , t.str());
                locFromSymtab = seachInSymtabForLoc(col3);
                if (isIndexAdd)
                {
                    //Handling index addressing location incrementation
                    ostringstream s;
                    int temp;
                    istringstream(locFromSymtab)>>hex>>temp;
                    temp += 32768;
                    s<<hex<<temp;
                    ObjectCode = opcode + s.str();
                }
                else
                    ObjectCode = opcode + locFromSymtab;
                preLineObjProg<<"^"<<ObjectCode;
                if(objCodeCounter == 0)
                {
                    lineStartAddress = loc-3;
                }
                objCodeCounter++;
            }
            if(objCodeCounter == 10)
                {
                    moveToNextLine();
                }
        }

        file.close();
    }
}

<<<<<<< HEAD
=======

>>>>>>> fd8d6a81d3493eee5100a203f47388e7c7aae837
void initOptab()
{
    ifstream fn("optab.txt");
    string temp;
    if(fn.is_open())
    {
        int i = 0;
        while(fn>>temp)
        {
            struct Optab optab;   
            optab.mnemonic = temp;
            fn>>temp;
            optab.opcode = temp;
            vOptab.push_back(optab);
            i++;
            
        }
        fn.close();
    }
}
string searchInOptab(string s)
{
    for(int i = 0; i<vOptab.size(); i++)
    {
        if(strcmp( vOptab[i].mnemonic.c_str() , s.c_str())==0)
        {
            return vOptab[i].opcode;
        }
    }
    throw -1;
}
string seachInSymtabForLoc( string temp)
{
    for( int i=0; i<vSymtab.size() ;i++)
    {
        if((vSymtab[i].symbol  == temp) && (vSymtab[i].address !="*"))
        {
            return vSymtab[i].address;
        }
    }
    return "0000";
}
string returnObjectCodeForAD(string str1, string str2)
{
    if(str1 == "WORD")
    {
        int temp2;
        istringstream(str2)>>temp2;
        ostringstream temp3;
        temp3<<hex<<temp2;
        string temp4 = temp3.str();
        for(int i = temp4.size(); i<6 ; i++)
            temp4.insert(0,"0");
        return temp4;
    }
    else if(str1 == "BYTE")
    {
        if(str2[0] == 'C')
        {
            ostringstream os;
            string temp = str2.substr(2,str2.size()-3);
            for(int i = 0;i< temp.size();i++)
            {
                int t = temp[i];
                os<<hex<<t;
            }
            return os.str();
        }
        else if(str2[0] == 'X')
        {
            return str2.substr(2,str2.size()-3);
        }
    }
    return "000000";
}
void printSplashScreen()
{
    cout<<"\n\n\n\n\t\t\t\tNational Institute of Technology Goa"<<endl;
    cout<<"\n\n\n\n\t\t\t\tOne Pass Assembler that generates object program\n\n"<<endl;
    cout<<"\t\t\t\tCode by:\n"<<endl;
    cout<<"\t\t\t\tGroup No.: 4"<<endl;
    cout<<"\t\t\t\t1. Roshan Kumar - 19CSE1027"<<endl;
    cout<<"\t\t\t\t2. Patel Harsh Rajesh - 19CSE1019"<<endl;
    cout<<"\n\n\n\n\t\t\t\tPress Enter key to continue..."<<endl;
    cin.ignore();
    system("CLS");
}
void printMainScreen()
{
    int option;
    cout<<"\n\n\n\n\t\t\t\tEnter the desired option to get the output."<<endl;
    cout<<"\n\n\n\n\t\t\t\t1. Print the object program to screen"<<endl;
    cout<<"\t\t\t\t2. Store the object program into output.txt in the same directory"<<endl;
    cout<<"\t\t\t\t3. Print the object program to screen and also generate an output.txt file"<<endl;
    cout<<"\n\n\n\n\t\t\t\tNote : The screen output will contain ^ characters for better readability\n\t\t\t\t while the output file will not."<<endl;
    cin>>option;
    string final = objectProgram.str();
    transform(final.begin(),final.end(),final.begin(), ::toupper);
    fstream outputFile;
    string temp5 = final;
    switch(option)
    {
        case 1:
            cout<<endl<<final<<endl;
            cin.get();
        break;
        case 2:
            outputFile.open("output.txt",ios::out);
            if(!outputFile)
            {
                cout<<"File creation failed. Press enter to return to Main Screen"<<endl;
                cin.ignore();
                printMainScreen();
            }
            else
            {
                temp5.erase(remove(temp5.begin(), temp5.end(), '^'), temp5.end());
                outputFile<<temp5;
                cout<<"File with object program successfully created..."<<endl;
                cin.get();
            }
        break;
        case 3:
            outputFile.open("output.txt",ios::out);
            if(!outputFile)
            {
                cout<<"File creation failed.\n"<<endl;
            }
            else
            {
                temp5.erase(remove(temp5.begin(), temp5.end(), '^'), temp5.end());
                outputFile<<temp5;
                cout<<"\nFile with object program successfully created...\n"<<endl;
            }
            cout<<endl<<final<<endl;
            cin.get();
        break;
        default:
            cout<<"\n\n\n\n\t\t\t\tThe Option selected does not exist. Press Enter to re-enter.\n\n"<<endl;
            cin.ignore();
            system("CLS");
            printMainScreen();
        break;
    }
}