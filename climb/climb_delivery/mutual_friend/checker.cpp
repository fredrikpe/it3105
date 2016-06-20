#include <stdlib.h>
#include <iostream>
#include <sstream>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "wrong number of arguments!" << endl;
        return -1;
    }
    stringstream ss;
    ss << "wdiff -3 " << argv[2] << " " << argv[3] << " > /dev/null";
    int retval = system(ss.str().c_str());
    if (retval != 0)
        return 1;
    return 0;
}
