#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
using namespace std;

struct Node
{
    string notDefinedAddress;
    struct Node *link;
};
typedef struct Node node;

struct Symtab
{
    string symbol;
    string address;
    struct Node *link;
};
typedef struct Symtab symtab;

vector<string> assemblerDirectives = {"START", "END", "BYTE", "RESB", "WORD", "RESW"};
vector<symtab> vSymtab;

void onePassScan();
bool isAssemblerDirective(string s);
bool searchSymtab(string temp);
void defineSymbol(string str, string add);

int main()
{
    onePassScan();
    //Added temporary code to print the symtab
    for(int i= 0 ; i<vSymtab.size(); i++)
    {
        cout<<vSymtab[i].symbol<<" : "<<vSymtab[i].address<<endl;
    }
    cout<<endl<<"Program ended"<<endl;
    return 0;
}

bool isAssemblerDirective(string s)
{
    for(int i = 0; i<assemblerDirectives.size(); i++)
    {
        if(s == assemblerDirectives[i])
        return true;
    }
    return false;
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
            
            if(col1[0] == '.')
                continue;
            else if(col1[0] != ' ')
            {
                ostringstream hexAdd;
                hexAdd<<hex<<loc;
                defineSymbol(col1, hexAdd.str());
            }

            if(isAssemblerDirective(col2))
            {
                if(col2 == "START")
                    loc = atoi(col3.c_str());
                else if(col2 == "BYTE")
                    loc += 1;
                else if(col2 == "RESB")
                    loc += 1*(atoi(col3.c_str()));
                else if(col2 == "RESW")
                    loc += 3*(atoi(col3.c_str()));
                else
                    loc+=3;
            }
            else
            {
                //searchinoptab(col2)
                loc+=3;
            }

                //Search symtab for col3
                // if not found -> insertSym(col3, *)
                // if found but address is * -> insert linked list
                // if found and address is not * -> get address
        }

        file.close();
    }
}

bool searchSymtab(string temp)
{
    for(int i = 0; i<vSymtab.size(); i++)
    {
        if(temp == vSymtab[i].symbol)
            return true;
    }
    return false;
}
void defineSymbol(string str, string add)
{
    if(searchSymtab(str))
    {
        //Delete linked list, respective object code to be generated and store the original address...
    }
    else
    {
        symtab s;
        s.symbol = str;
        s.address = add;
        s.link = 0;
        vSymtab.push_back(s);
    }
}