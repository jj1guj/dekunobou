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