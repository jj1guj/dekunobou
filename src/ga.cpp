#include "ga.hpp"
#if defined GA

//評価関数のパラメータ関連
float params[N][param_size];
int memsize=sizeof(params[0]);
float param_black[param_size];
float param_white[param_size];

//交叉関連
int M=100;//1世代での交叉回数
int match_genetic=30;//交叉時の親と子の対局数
int thresh=0.55*match_genetic;//勝数がこの値を超えたら親を子で置き換える
float alpha=1e-2;//突然変異の確率
int result[3];//対戦結果を格納する
int win_impossible[1000];//i戦目時点で勝率0.55達成不可能ライン
double timelimit=7*3600;//遺伝的アルゴリズムを行う時間. 秒で指定

//交叉関連(並列化)
bool cur_used[N];//現在の世代で交叉中かどうか判定するフラグ

//交叉後の選抜関連
int win_count[N];//総当たり戦を行う際の各遺伝子の勝数を格納
int match_times=50;//総当たり戦での対局回数

//ファイル書き込み関連
std::string eval_output_path="eval.txt";//最終的に出力する評価関数のファイル名
std::string data_path="data";//遺伝子すべてを保存するディレクトリのパス
int out_interval=10;//遺伝子すべてを出力する間隔

std::random_device rnd;
void init_param(float params[param_size]){
    for(int i=0;i<param_size;++i){
        params[i]=2.0*(float)rnd()/0xffffffff-1.0;
    }
}

//遺伝的アルゴリズムで使用したパラメータすべてをCSVファイルから読み込む
int load_params(std::string filename,float params[N][param_size]){
    std::ifstream inputs(filename);
    std::string s;
    int i=0,j;
    if(inputs.fail()){
        std::cout<<"Failed to open file\n";
        return -1;
    }
    while(getline(inputs,s)){
        std::stringstream ss{s};
        std::string buf;
        j=0;
        while(std::getline(ss,buf,',')){
            params[i][j]=std::stof(buf);
            ++j;
        }
        ++i;
    }
    return 0;
}

//遺伝的アルゴリズムで生成したパラメータをすべてCSVファイルに格納する
void out_params(std::string path){
    std::ofstream output(path);
    for(int i=0;i<N;++i){
        for(int j=0;j<param_size;++j)output<<params[i][j]<<",";
        output<<std::endl;
    }
    output.close();
}

//交叉
void intersection(float p1[param_size],float p2[param_size],int cur1,int cur2){
    int win_val[2];
    //p1: 親1, p2: 親2
    //c1: 子1(p1ベースでp2と交叉した後のもの)
    //c2: 子2(p2ベースでp1と交叉した後のもの)
    float c,c1[param_size],c2[param_size];

    //M回交叉する
    for(int m=0;m<M;++m){
        //swapする区間を設定する
        //lからrの区間だけswapする
        //l<rが保証されている
        int l=rnd()%param_size;
        int r=rnd()%param_size;
        while(l==r)r=rnd()%param_size;
        if(r<l)std::swap(r,l);

        //lからrの区間をswapする
        for(int i=0;i<param_size;++i){
            if(l<=i && i<=r){
                c1[i]=p2[i];
                c2[i]=p1[i];
            }else{
                c1[i]=p1[i];
                c2[i]=p2[i];
            }

            //確率に応じて突然変異を行う
            if((float)rnd()/0xffffffff<=alpha){
                c=2.0*(float)rnd()/0xffffffff-1.0;
                while(c==c1[i])c=2.0*(float)rnd()/0xffffffff-1.0;
                c1[i]=c;
            }

            if((float)rnd()/0xffffffff<=alpha){
                c=2.0*(float)rnd()/0xffffffff-1.0;
                while(c==c1[i])c=2.0*(float)rnd()/0xffffffff-1.0;
                c1[i]=c;
            }
        }

        //子と親で対戦する
        win_val[0]=0;
        win_val[1]=0;
        for(int b=0;b<match_genetic;++b){
            if(win_impossible[b]<=win_val[0]&&win_val[0]<=thresh){
                if(play_engine(p1,c1)==1)++win_val[0];//親1が先手, 子1が後手
                if(play_engine(c1,p1)==0)++win_val[0];//子1が先手, 親1が後手
            }

            if(win_impossible[b]<=win_val[1]&&win_val[1]<=thresh){
                if(play_engine(p2,c2)==1)++win_val[1];//親1が先手, 子1が後手
                if(play_engine(c2,p2)==0)++win_val[1];//子1が先手, 親1が後手
            }
        }

        //子の勝数がしきい値を超えたら置き換える
        if(win_val[0]>thresh)memcpy(p1,c1,memsize);
        if(win_val[1]>thresh)memcpy(p2,c2,memsize);
    }

    //遺伝子をもとに戻す
    cur_used[cur1]=false;
    cur_used[cur2]=false;
    memcpy(params[cur1],p1,memsize);
    memcpy(params[cur2],p2,memsize);
}

void ga(int threads_num){
    //変数の初期化
    match_genetic/=2;//先後入れ替えて対局するので2で割る
    timelimit*=1000.0;//msecに変換
    match_times/=2;//先後両方行うので2で割る

    //パラメータの初期化
    for(int i=0;i<N;++i)init_param(params[i]);

    //thresh勝不可能ラインの前計算
    //(条件分岐を奇数局目でしか行わないのでその分だけ計算)
    for(int i=0;i<match_genetic/2;++i){
        win_impossible[i]=thresh+2*(i-match_genetic);
    }

    //並列化の準備
    int concurrency=std::min(omp_get_max_threads(),N/2);
    if(threads_num>0)concurrency=std::min(concurrency,threads_num);
    std::cout<<"Concurrency: "<<concurrency<<std::endl;

    float G[256][param_size];//交叉する遺伝子を格納. 256は多分なんとなくで決めてる気が
    int cursors[256];//交差する遺伝子のインデックスを格納
    for(int i=0;i<N;++i)cur_used[i]=false;

    //最初の重みを出力
    out_params(data_path+"/out_1.csv");

    //時間計測を開始
    std::chrono::system_clock::time_point start,end;
    start=std::chrono::system_clock::now();

    int itr=0;
    while(true){
        ++itr;
        std::cout<<"Generation: "<<itr<<std::endl;

        //ランダムにconcurrency×2個体選び出しコピーする
        for(int i=0;i<concurrency*2;++i){
            cursors[i]=rnd()%N;
            while(cur_used[cursors[i]])cursors[i]=rnd()%N;
            cur_used[cursors[i]]=true;

            memcpy(G[i],params[cursors[i]],memsize);
        }

        for(int i=0;i<concurrency*2;i+=2){
            std::cout<<cursors[i]<<"-"<<cursors[i+1]<<" ";
        }
        std::cout<<std::endl;

        //交叉を並列に実行
        #pragma omp parallel for num_threads(concurrency)
        for(int i=0;i<concurrency;++i){
            intersection(G[2*i],G[2*i+1],cursors[2*i],cursors[2*i+1]);
        }

        //今の遺伝子をファイルに出力
        if(itr%out_interval==0){
            out_params(data_path+"/out_"+std::to_string(itr)+".csv");
        }

        //10世代ごとに時間を計測し制限時間内か見る
        if(itr%10==0){
            end=std::chrono::system_clock::now();
            double elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
            std::cout<<itr<<" elapsed:"<<elapsed/1000<<" sec"<<std::endl<<std::endl;
            if(elapsed>timelimit)break;
        }
    }

    //1番最後の遺伝子をファイルに出力
    out_params(data_path+"/out_"+std::to_string(itr)+".csv");

    //総当たり戦を行い最終的に最も借り星の多いパラメータを出力
    int winner;
    for(int i=0;i<N;++i)win_count[i]=0;
    for(int i=0;i<N-1;++i){
        std::cout<<i<<std::endl;
        for(int j=i+1;j<N;++j){
            for(int k=0;k<match_times;++k){
                winner=play_engine(params[i],params[j]);
                if(winner==0)++win_count[i];
                if(winner==1)++win_count[j];

                winner=play_engine(params[j],params[i]);
                if(winner==0)++win_count[j];
                if(winner==1)++win_count[i];
            }
        }
    }

    int win_max=0;
    int best;
    for(int i=0;i<N;++i){
        if(win_count[i]>win_max){
            best=i;
            win_max=win_count[i];
        }
    }

    std::cout<<win_max<<"/"<<(N-1)*match_times*2<<" "<<(float)win_max/((N-1)*match_times*2)<<std::endl;
    //output to file
    std::ofstream eval_output(eval_output_path);
    for(int i=0;i<param_size;++i)eval_output<<params[best][i]<<std::endl;
    eval_output.close();
}
#endif