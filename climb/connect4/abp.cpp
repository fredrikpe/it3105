


enum Disc {
    EMPTY;
    RED;
    BLUE;
}


struct Board {
    Disc cells[6][7];
}

struct GameState {
    int turn;
    struct Board;
};

s


cut(node, num){
    if(node.child.size()>num+1){
        for(j=num+1; j< node.child.size(); j++){
            erase(node.child[j]);
        }
    }
}

int alphabeta(node, depth, alpha, beta){
    if(depth == 0 || node.child.empty())
        return node.data;
    if(nod.type == MAX){
        for each child of node{
            temp=alphabeta(node.child[i], depth-1, alpha, beta);
            alpha=maxof(alpha, temp);
            if(beta<=alpha){
                cut(node, i);   //beta cut-off  
            }
        }
        return alpha;
    }
    else{
        for each child of node{
            temp=alphabeta(node.child[i], depth-1, alpha, beta);
            beta=minof(beta, temp);
            if(beta<=alpha){
                cut(node, i);   //alpha cut-off         
            }
        }
        return beta;
    }
}
