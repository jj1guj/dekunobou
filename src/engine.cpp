#include "engine.hpp"
int nodes;
bool turn_p;

//αβ法による先読み
//α: 評価値の最小値
//β: 評価値の最大値
float alphabeta(Board board,float param[param_size],int depth,float alpha,float beta){
    //候補手の展開
    LegalMoveList moves(board);
    if(moves.size()==0){
        //手番を変えて展開
        board.push(-1);
        LegalMoveList moves2(board);
        //終局
        if(moves2.size()==0){
            ++nodes;
            return board.point[turn_p];
        }
        return alphabeta(board,param,depth-1,alpha,beta);
        moves=moves2;
    }

    float val;
    if(board.turn==turn_p)val=-inf;//エンジン側が手番のときは評価値の最大値を求める
    else val=inf;//相手が手番のときは評価値の最小値を求める
    //末端ノードのとき
    Board board_ref;
    if(depth<=0){
        for(int i=0;i<moves.size();++i){
            ++nodes;
            board_ref=board;
            board_ref.push(moves[i]);
            if(board.turn==turn_p){
                val=std::max(val,eval(board_ref,param));
                if(val>=beta)break;
                alpha=std::max(alpha,val);
            }else{
                val=std::min(val,eval(board_ref,param));
                if(val<=alpha)break;
                beta=std::min(beta,val);
            }
        }
        return val;
    }
    
    for(int i=0;i<moves.size();++i){
        //1手打つ
        board_ref=board;
        board_ref.push(moves[i]);
        if(board.turn==turn_p){
            val=std::max(val,alphabeta(board_ref,param,depth-1,alpha,beta));
            if(val>=beta)break;
            alpha=std::max(alpha,val);
        }else{
            val=std::min(val,alphabeta(board_ref,param,depth-1,alpha,beta));
            if(val<=alpha)break;
            beta=std::min(beta,val);
        }
    }
    return val;
}

int go(Board board,float param[param_size]){
    turn_p=board.turn;

    float val=-inf;
    LegalMoveList moves(board);
    //1手だけのときはその手を返す
    if(moves.size()==1)return moves[0];

    turn_p=board.turn;//エンジン側の手番を取得

    int BestMoves[64];
    int bestmoves_num;
    std::random_device rnd_select;
    
    bestmoves_num=0;
    float eval_ref;

    //現在の評価値を算出
    Board board_ref;

    //探索の優先順位付け
    float evals[64];
    int priority[64];
    bool selected[64];
    std::vector<float>evals_sort(moves.size());

#if not defined GA
    //5手読みの評価値を算出
    for(int i=0; i<moves.size();++i){
        board_ref=board;
        board_ref.push(moves[i]);
        evals[i]=alphabeta(board_ref,param,4,val,inf);
        evals_sort[i]=evals[i];
        val=std::max(evals[i],val);
        selected[i]=false;
    }

    //評価値の降順にソート
    std::sort(evals_sort.begin(),evals_sort.end(),std::greater<float>());
    
    for(int i=0;i<moves.size();++i){
        for(int j=0;j<moves.size();++j){
            if(!selected[j]&&evals_sort[i]==evals[j]){
                priority[i]=j;
                selected[j]=true;
                break;
            }
        }
    }
    #if defined Debug
        std::cout<<"priority: ";
        for(int i=0;i<moves.size();++i)std::cout<<priority[i]+1<<" ";
        std::cout<<std::endl;
        if(board.point[0]+board.point[1]>=49)std::cout<<"depth: "<<63-board.point[0]-board.point[1]<<std::endl;
    #endif
#else
    for(int i=0;i<moves.size();++i){
        priority[i]=i;
    }
#endif
    val=-inf;
    for(int i=0;i<moves.size();i++){
        //先読みしてみる
        //1手読みしたいなら深さを0に指定する
        board_ref=board;
        board_ref.push(moves[priority[i]]);
        
#if not defined GA
        //終盤20手で完全読み
        nodes=0;
        if(board.point[0]+board.point[1]>=60-perfect_search)eval_ref=alphabeta(board_ref,param,60,-inf,inf);
        else eval_ref=alphabeta(board_ref,param,6,val,inf);
    #if defined Debug
        std::cout<<priority[i]+1<<": "<<eval_ref<<" "<<nodes/1000<<"k"<<std::endl;
    #endif
#else
    eval_ref=alphabeta(board_ref,param,0,val,inf);
#endif
        if(eval_ref>val){
            bestmoves_num=0;
            BestMoves[bestmoves_num]=moves[priority[i]];
            ++bestmoves_num;
            val=eval_ref;
        }else if(eval_ref==val){
            BestMoves[bestmoves_num]=moves[priority[i]];
            ++bestmoves_num;
        }
    }
#if defined Debug
    //for debug
    std::cout<<"eval: "<<val<<std::endl;
#endif
    return BestMoves[rnd_select()%bestmoves_num];
}