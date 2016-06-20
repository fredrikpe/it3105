#include <iostream>
#include <bitset>
#include <string>
 
 
using namespace std;

bool newI(bool left, bool right, bool old)
{
    if (left) 
    {
        if (right || old)
            return false;
        return true;
    }
    else
    {
        if (right || old)
            return true;
        return false;
    }
}

int main()
{

    string s;
    cin >> s;

    int G;
    cin >> G;

    int size = s.size();
 
    bool *org = new bool[size];
    bool *tmp = new bool[size];

    for(int i = 0; i < size ; ++i)
        org[i] = (int)s[i] - 48;

    for(int j = 0; j < G; ++j)
    {
        //cout << s;
        for(int i = 0; i < size ; ++i)
        {
            if (i == 0)
                tmp[i] = newI(org[size - 1], org[1], org[i]); 
            else if (i == size - 1)
                tmp[i] = newI(org[i - 1], org[0], org[i]); 
            else
                tmp[i] = newI(org[i - 1], org[i + 1], org[i]); 
        }
        for (int i=0; i<size; i++)
        {
            org[i] = tmp[i];
        }
    }
    for (int i=0; i<size; i++)
    {
        if (org[i])
            cout << '1';
        else
            cout << '0';
    }
     
    cout << endl;
}



