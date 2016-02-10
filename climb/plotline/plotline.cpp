


void plotline(int x1, int y1, int x2, int y2){
    double dx = x2-x1;     // bruk datatypen double her eller casting
    double dy = y2-y1;    // i divisjonen under
    double a = dy / dx; // hvis dy og dx er int bruker du dy/(double)dx 
    double b = y1 - (a * x1);
    for (int x = x1; x <= x2; ++x){
        int y = round((a * x) + b); //husk avrunding
        cout << "(" << x << "," << y << ")" << endl;
    }
}
