#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

void onePassScan();

int main()
{
    onePassScan();
    return 0;
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
            cout<<col1<<col2<<col3<<endl;
        }

        file.close();
    }



    
}