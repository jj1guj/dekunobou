#include "board.hpp"
unsigned long long makeLegalBoard(unsigned long long board_player,unsigned long long board_opponent){
    //左右端の番人
    unsigned long long horizontalWatcher=board_opponent&0x7e7e7e7e7e7e7e7e;

    //上下端の番人
    unsigned long long verticalWatcher=board_opponent&0x00FFFFFFFFFFFF00;

    //全辺の番人
    unsigned long long allsideWatcher=board_opponent&0x007e7e7e7e7e7e00;

    //空きマスのみにビットが立っているボード
    unsigned long long blankBoard=~(board_player|board_opponent);

    unsigned long long tmp,legalBoard;

    //8方向で合法手があるかチェックする
    //左
    tmp=horizontalWatcher&(board_player<<1);
    for(int i=0;i<5;++i){
        tmp|=horizontalWatcher&(tmp<<1);
    }
    legalBoard=blankBoard&(tmp<<1);

    //右
    tmp=horizontalWatcher&(board_player>>1);
    for(int i=0;i<5;++i){
        tmp|=horizontalWatcher&(tmp>>1);
    }
    legalBoard|=blankBoard&(tmp>>1);

    //上
    tmp=verticalWatcher&(board_player<<8);
    for(int i=0;i<5;++i){
        tmp|=verticalWatcher&(tmp<<8);
    }
    legalBoard|=blankBoard&(tmp<<8);

    //下
    tmp=verticalWatcher&(board_player>>8);
    for(int i=0;i<5;++i){
        tmp|=verticalWatcher&(tmp>>8);
    }
    legalBoard|=blankBoard&(tmp>>8);

    //右斜め上
    tmp=allsideWatcher&(board_player<<7);
    for(int i=0;i<5;++i){
        tmp|=allsideWatcher&(tmp<<7);
    }
    legalBoard|=blankBoard&(tmp<<7);

    //左斜め上
    tmp=allsideWatcher&(board_player<<9);
    for(int i=0;i<5;++i){
        tmp|=allsideWatcher&(tmp<<9);
    }
    legalBoard|=blankBoard&(tmp<<9);

    //右斜め下
    tmp=allsideWatcher&(board_player>>9);
    for(int i=0;i<5;++i){
        tmp|=allsideWatcher&(tmp>>9);
    }
    legalBoard|=blankBoard&(tmp>>9);

    //左斜め下
    tmp=allsideWatcher&(board_player>>7);
    for(int i=0;i<5;++i){
        tmp|=allsideWatcher&(tmp>>7);
    }
    legalBoard|=blankBoard&(tmp>>7);

    return legalBoard;
}

unsigned long long transfer(unsigned long long id,int dir){
    if(dir==0){
        //上
        return (id << 8) & 0xffffffffffffff00;
    }else if(dir==1){
        //右上
        return (id << 7) & 0x7f7f7f7f7f7f7f00;
    }else if(dir==2){
        //右
        return (id >> 1) & 0x7f7f7f7f7f7f7f7f;
    }else if(dir==3){
        //右下
        return (id >> 9) & 0x007f7f7f7f7f7f7f;
    }else if(dir==4){
        //下
        return (id >> 8) & 0x00ffffffffffffff;
    }else if(dir==5){
        //左下
        return (id >> 7) & 0x00fefefefefefefe;
    }else if(dir==6){
        //左
        return (id << 1) & 0xfefefefefefefefe;
    }else if(dir==7){
        //左上
        return (id << 9) & 0xfefefefefefefe00;
    }
    return 0;
}

int bitcount(unsigned long long data){
    data=(data&0x5555555555555555)+((data&0xaaaaaaaaaaaaaaaa)>>1);//2桁ごとの1の数
    data=(data&0x3333333333333333)+((data&0xcccccccccccccccc)>>2);//4桁ごとの1の数
    data=(data&0xf0f0f0f0f0f0f0f)+((data&0xf0f0f0f0f0f0f0f0)>>4);//8桁ごとの1の数
    data=(data&0xff00ff00ff00ff)+((data&0xff00ff00ff00ff00)>>8);//16桁ごとの1の数
    data=(data&0xffff0000ffff)+((data&0xffff0000ffff0000)>>16);//32桁ごとの1の数
    data=(data&0xffffffff)+((data&0xffffffff00000000)>>32);//64桁の1の数
    return data;
}

//指定した座標に何色の石がおいてあるか返す
int Board::operator [](int i){
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
unsigned long long Board::idToBit(int id){
    unsigned long long mask=0x8000000000000000;
    unsigned long long x=id>>3,y=id&7;
    mask=mask>>y;
    mask=mask>>(x*8);
    return mask;
}

//着手できるか判定
bool Board::putable(unsigned long long id){
    unsigned long long legalBoard=makeLegalBoard(board_player,board_opponent);

    return (id&legalBoard)==id;
}

//着手
void Board::push(int id_int){
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
bool Board::is_pass(){
    unsigned long long playerLegalBoard=makeLegalBoard(board_player,board_opponent);
    unsigned long long opponentLegalBoard=makeLegalBoard(board_opponent,board_player);

    return (playerLegalBoard == 0x0000000000000000) && (opponentLegalBoard != 0x0000000000000000);
}

//終局の判定
bool Board::is_over(){
    unsigned long long playerLegalBoard=makeLegalBoard(board_player,board_opponent);
    unsigned long long opponentLegalBoard=makeLegalBoard(board_opponent,board_player);

    return (playerLegalBoard == 0x0000000000000000) && (opponentLegalBoard == 0x0000000000000000);
}

//手番交代
void Board::swapBoard(){
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