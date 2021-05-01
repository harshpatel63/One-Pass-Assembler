#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<cstring>
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

void onePassScan();
bool isAssemblerDirective(string s);
int searchSymtab(string temp);

void enterextra(struct Node *link , string value , int i);
int return_index( string temp);
void entersymbol_column1( string temp , string value);
void entersymbol_column3( string temp , string value);
void search_symtab_column1(string symbol, string value);
string searchInOptab(string s);
string seachInSymtabForLoc( string temp);
string returnObjectCodeForAD(string str1, string str2);


bool isAssemblerDirective(string s)
{
    for(int i = 0; i<assemblerDirectives.size(); i++)
    {
        if(s == assemblerDirectives[i])
        return true;
    }
    return false;
}

void enterextra(struct Node *link , string value , int i)
{
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

int return_index( string temp)
{
    for (int i = 0; i < vSymtab.size(); i++)
    {
        if(strcmp(vSymtab[i].symbol.c_str() , temp.c_str())==0)
        {    
           return i;
        }
    }
}

void entersymbol_column1( string temp , string value)
{   
    struct Symtab s;
    s.symbol = temp;
    s.address = value;
    s.link = NULL;
    vSymtab.push_back(s);
}

void entersymbol_column3( string temp , string value)
{   
    
    struct Symtab s;
    
  
        if(searchSymtab(temp))
        {  
        int i;
        i = return_index(temp);
        enterextra(vSymtab[i].link , value , i);
        }
        else
        {  
           int i;
           s.symbol= temp;
           s.address = "*";
           s.link = NULL;
           vSymtab.push_back(s);
           i = return_index(temp);
           enterextra( s.link , value , i);
          }
}
 
void search_symtab_column1(string symbol, string value)
{   
    //string c ="00";
    //string final;
     for (int i = 0; i < vSymtab.size(); i++)
    {
        if(strcmp(vSymtab[i].symbol.c_str() , symbol.c_str())==0)
        {    
            if( vSymtab[i].address == "*")
            {
                 struct Node *temp;
                 struct Node *next;
                 temp = vSymtab[i].link;
                 while (temp != NULL)
                 {  
                   // final = strcat( c , temp->opcode.c_str()) ;
                    cout<<"T"<<"^"<<temp->notDefinedAddress<<"^"<<value<<endl;
                    next= temp->link;
                    free(temp);
                    temp = next;
                 }
                 
                 vSymtab.erase( vSymtab.begin() + i);
                 entersymbol_column1(symbol , value);
                 
            }
        }
    }
} 

void onePassScan()
{
    int loc;
    string line, col1, col2, col3;
    ifstream file("input1.txt");
    
    if(file.is_open())
    {

        int tabPos[2];
        while(getline(file, line))
        {
            
            bool firstTab = true;
            bool isIndexAdd =false;
            string opcode;
            string locFromSymtab;
            string ObjectCode;
            if(line[0] == '.')
                continue;

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
            col2 = line.substr(tabPos[0] + 1, tabPos[1] - tabPos[0] - 1);
            col3 = line.substr(tabPos[1] + 1, line.size() - tabPos[1]);
            
            ostringstream hexAdd;
            hexAdd<<hex<<loc;
            string push = hexAdd.str();
            if ( col2 != "START")
            {
                location.push_back(push);
            }
               


            if(isAssemblerDirective(col2))
            {
                if(col2 == "START")
                    istringstream(col3)>>hex>>loc;
                else if(col2 == "BYTE")//Will have to handle C'EOF' separately here. We'll do it later.
                {
                    loc += 3;
                    string objCodeForByte = returnObjectCodeForAD(col2, col3);
                    cout<<objCodeForByte<<endl;
                }
                else if(col2 == "RESB")
                {
                    int temp;
                    istringstream(col3)>>temp;
                    loc += 1*temp;
                }
                else if(col2 == "RESW")
                {
                    int temp;
                    istringstream(col3)>>temp;
                    loc += 3*temp;
                }
                else if(col2 == "WORD")
                {
                    loc+=3;
                    string objCodeForWord = returnObjectCodeForAD(col2, col3);
                    cout<<objCodeForWord<<endl;
                }
                else
                    loc+=3;
            }
            else
            {  
                try
                {
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
                loc+=3;
            }
            if (col2 == "START")
            {
                continue;
            }
            
            
            
        
            
            if(col1[0] != ' ')
            { 
                 if (searchSymtab(col1))
                 {  
                    search_symtab_column1( col1 , push);
                 }
                 else
                 {
                     entersymbol_column1( col1 , push);
                 }
                 
            }

            if(!isAssemblerDirective(col2)) //It was col2 != "START" before
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
                entersymbol_column3(col3 , t.str());
                locFromSymtab = seachInSymtabForLoc(col3);
                if (isIndexAdd)
                {
                    //Handling index addressing location incrementation
                    ostringstream s;
                    int temp;
                    istringstream(locFromSymtab)>>hex>>temp;
                    temp *= 9;
                    s<<hex<<temp;
                    ObjectCode = opcode + s.str();
                }
                else
                    ObjectCode = opcode + locFromSymtab;
                cout<<ObjectCode<<endl;
            }
        }

        file.close();
    }
}
void printsymtab()
{
    for (int i = 0; i < vSymtab.size(); i++)
    {
        cout<<vSymtab[i].symbol<<" ";
        cout<<vSymtab[i].address<<endl;
    }
    
}

void printlinklist( )
{
    for(int i =0 ; i < vSymtab.size() ; i++)
    {
        if(vSymtab[i].address == "*")
        {   
            cout<<"\n link list for"<<vSymtab[i].symbol;
            struct Node *temp;
            temp = vSymtab[i].link;
            while (temp != NULL)
            {
                cout<<temp->notDefinedAddress<<" ";
                temp = temp->link;
            }
            
        }
    }
}
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
        for(int i = str2.size(); i<6 ; i++)
            str2.insert(0,"0");
        return str2;
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
int main()
{
    initOptab();

    onePassScan();

    
    // printsymtab();
   
    // printlinklist();
    return 0;
}

