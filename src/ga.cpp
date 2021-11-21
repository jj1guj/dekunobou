#include "ga.hpp"
#if defined GA
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

//交叉
void intersection(float p1[param_size],float p2[param_size],int cur1,int cur2){
    int win_val[2];
    float c,c1[param_size],c2[param_size];
}
#endif