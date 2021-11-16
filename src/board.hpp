#pragma once
unsigned long long makeLegalBoard(unsigned long long board_player,unsigned long long board_opponent);

class Board{
    public:
    bool turn=false;//後手のときtrue
    int ply=1;//今何手目か
    int point[2]={2,2};
    unsigned long long board_player=0x0000000810000000;
    unsigned long long board_opponent=0x0000001008000000;
    unsigned long long board_black=0x0000000810000000;
    unsigned long long board_white=0x0000001008000000;

    //指定した座標に何色の石がおいてあるか返す
    int operator [](int i){
        i=63-i;
        if((board_black>>i)&1){
            return 1;
        }else if((board_white>>i)&1){
            return -1;
        }else{
            return 0;
        }
    }

    //座標をビットに変換
    unsigned long long idToBit(int id){
        unsigned long long mask=0x8000000000000000;
        unsigned long long x=id>>3,y=id&7;
        mask=mask>>y;
        mask=mask>>(x*8);
        return mask;
    }

    //着手できるか判定
    bool putable(unsigned long long id){
        unsigned long long legalBoard=makeLegalBoard(board_player,board_opponent);

        return (id&legalBoard)==id;
    }

    void push(int id_int){
        if(0<=id_int&&id_int<64){
            unsigned long long id=idToBit(id_int);
            unsigned long long rev=0;
            
            for(int i=0;i<8;++i){
                unsigned long long rev_=0;
                unsigned long long mask=transfer(id,i);
                
                while((mask!=0)&&((mask&board_opponent)!=0)){
                    rev_|=mask;
                    mask=transfer(mask,i);
                }

                if((mask&board_player)!=0){
                    rev|=rev_;
                }
            }

            //反転する
            board_player^=(id|rev);
            board_opponent^=rev;
        }

        //手番等の更新
        swapBoard();
        ++ply;
    }

    //パスの判定
    bool is_pass(){
        unsigned long long playerLegalBoard=makeLegalBoard(board_player,board_opponent);
        unsigned long long opponentLegalBoard=makeLegalBoard(board_opponent,board_player);

        return (playerLegalBoard == 0x0000000000000000) && (opponentLegalBoard != 0x0000000000000000);
    }

    //終局の判定
    bool is_over(){
        unsigned long long playerLegalBoard=makeLegalBoard(board_player,board_opponent);
        unsigned long long opponentLegalBoard=makeLegalBoard(board_opponent,board_player);

        return (playerLegalBoard == 0x0000000000000000) && (opponentLegalBoard == 0x0000000000000000);
    }

    //手番交代
    void swapBoard(){
        unsigned long long tmp=board_player;
        board_player=board_opponent;
        board_opponent=tmp;
        turn=!turn;

        if(turn){
            board_white=board_player;
            board_black=board_opponent;
        }else{
            board_black=board_player;
            board_white=board_opponent;
        }

        //枚数の更新
        point[0]=bitcount(board_black);
        point[1]=bitcount(board_white);
    }

    private:
    unsigned long long transfer(unsigned long long id,int dir);

    int bitcount(unsigned long long data);
};