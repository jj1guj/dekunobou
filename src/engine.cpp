#include "engine.hpp"
long long nodes;
long long nodes_total=0;
bool turn_p;

void move_ordaring(LegalMoveList& moves,Board board,float param[param_size]){
    float evals[64];
    int priority[64];
    bool selected[64];
    std::vector<float>evals_sort(moves.size());
    Board board_ref;

    // 1手読みの評価値を算出
    for(int i=0;i<moves.size();++i){
        board_ref=board;
        board_ref.push(moves[i]);
        evals[i]=-eval(board_ref,param);
        evals_sort[i]=evals[i];
        selected[i]=false;
    }

    // 評価値の降順にソート
    std::sort(evals_sort.begin(),evals_sort.end(),std::greater<float>());

    for(int i=0;i<moves.size();++i){
        for(int j=0;j<moves.size();++j){
            if(!selected[j]&&evals_sort[i]==evals[j]){
                priority[i]=moves[j];
                selected[j]=true;
                break;
            }
        }
    }

    for(int i=0;i<moves.size();++i)moves[i]=priority[i];
}

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

//オセロAIの教科書をもとに実装
//https://note.com/nyanyan_cubetech/n/n210cf134b8b1?magazine_key=m54104c8d2f12
float nega_alpha(Board board,float param[param_size],int depth,bool passed,float alpha,float beta) {
    //末端ノードでは評価関数を呼ぶ
    if(depth==0){
        ++nodes;
        return eval(board,param);
    }

    LegalMoveList moves(board);
    move_ordaring(moves,board,param);

    float max_score=-inf;
    for(int i=0;i<moves.size();++i){
        Board board_ref=board;
        board_ref.push(moves[i]);
        float g=-nega_alpha(board_ref,param,depth-1,false,-beta,-alpha);
        if(g>=beta)return g;
        alpha=std::max(alpha,g);
        max_score=std::max(max_score,g);
    }

    if(max_score==-inf){
        if(passed){
            ++nodes;
            return eval(board,param);
        }
        board.push(-1);//手番を変えて探索する
        return -nega_alpha(board,param,depth,true,-beta,-alpha);
    }
    return max_score;
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
    float alpha=-inf,beta=inf;
    for(int i=0;i<moves.size();i++){
        //先読みしてみる
        //1手読みしたいなら深さを0に指定する
        board_ref=board;
        board_ref.push(moves[priority[i]]);
        
#if not defined GA
        //終盤20手で完全読み
        nodes=0;
        if(board.point[0]+board.point[1]>=60-perfect_search)eval_ref=alphabeta(board_ref,param,60,-inf,inf);
        // else eval_ref=alphabeta(board_ref,param,6,val,inf);
        else{
            eval_ref=-nega_alpha(board_ref,param,10,false,-beta,-alpha);
            if(alpha<eval_ref){
                alpha=eval_ref;
            }
        }
    #if defined Debug
        nodes_total+=nodes;
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
    std::cout<<"nodes: "<<nodes_total/1000<<"k"<<std::endl;
#endif
    return BestMoves[rnd_select()%bestmoves_num];
}