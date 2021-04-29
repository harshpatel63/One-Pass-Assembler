#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

vector<string> assemblerDirectives = {"START", "END", "BYTE", "RESB", "WORD", "RESW"};

void onePassScan();
bool isAssemblerDirective(string s);

int main()
{
    onePassScan();
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
            cout<<isAssemblerDirective(col2)<<endl;
        }

        file.close();
    }



    
}