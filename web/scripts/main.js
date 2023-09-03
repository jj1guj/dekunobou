//1を黒石, 2を白石として処理を行う
class Board{
    constructor(){
        this.board=[];
        this.turn=false;//trueなら後手
        this.point;
        this.stone= [1,2];
        this.flip_limit=[0,0,0,0,0,0,0,0];
        this.di=[0,0,-1,1,-1,1,-1,1];
        this.dj=[-1,1,0,0,1,1,-1,-1];

        this.init();
    }

    init(){
        for(var i=0;i<8;++i){
            let board_line=[];
            for(var j=0;j<8;++j)board_line.push(0);
            this.board.push(board_line);
        }
        this.board[3][3]=2;
        this.board[3][4]=1;
        this.board[4][3]=1;
        this.board[4][4]=2;
        this.point=[2,2]; //[先手の枚数, 後手の枚数]
    }

    push(move){
        if(0<=move&&move<=63){
            var row=Math.floor(move/8);
            var col=move%8;
            if(this.board[row][col]!=0)return -1;
            
            var fliped=this.set_flip_limit(row,col);
            if(fliped==0)return -1;

            //石を返す
            this.board[row][col]=this.stone[Number(this.turn)];

            for(var dir=0;dir<8;++dir){
                for(var i=1;i<this.flip_limit[dir];++i){
                    this.board[row+this.di[dir]*i][col+this.dj[dir]*i]=this.stone[Number(this.turn)];
                }
            }

            //着手後の石の枚数を計算
            this.point[Number(this.turn)]+=fliped+1;
            this.point[Number(!this.turn)]-=fliped;

            this.turn=(this.turn+1)%2;//手番を反転
            return 0;
        }
    }

    set_flip_limit(row,col){
        //返せる石の枚数が返り値
        var flip_count=0;
        //横左方向
        //this.flip_limit=[];
        //this.flip_limit.push(0);
        this.flip_limit[0]=0;
        for(var i=1;i<=col;++i){
            if(this.board[row][col-i]!=this.stone[Number(!this.turn)]){
                if(this.board[row][col-i]==this.stone[Number(this.turn)])this.flip_limit[0]=i;
                break;
            }
        }
        if(this.flip_limit[0]>1)flip_count+=this.flip_limit[0]-1;

        //横右方向
        //this.flip_limit.push(0);
        this.flip_limit[1]=0;
        for(var i=1;i<=7-col;++i){
            if(this.board[row][col+i]!=this.stone[Number(!this.turn)]){
                if(this.board[row][col+i]==this.stone[Number(this.turn)])this.flip_limit[1]=i;
                break;
            }
        }
        if(this.flip_limit[1]>1)flip_count+=this.flip_limit[1]-1;

        //縦上方向
        //this.flip_limit.push(0);
        this.flip_limit[2]=0;
        for(var i=1;i<=row;++i){
            if(this.board[row-i][col]!=this.stone[Number(!this.turn)]){
                if(this.board[row-i][col]==this.stone[Number(this.turn)])this.flip_limit[2]=i;
                break;
            }
        }
        if(this.flip_limit[2]>1)flip_count+=this.flip_limit[2]-1;

        //縦下方向
        //this.flip_limit.push(0);
        this.flip_limit[3]=0;
        for(var i=1;i<=7-row;++i){
            if(this.board[row+i][col]!=this.stone[Number(!this.turn)]){
                if(this.board[row+i][col]==this.stone[Number(this.turn)])this.flip_limit[3]=i;
                break;
            }
        }
        if(this.flip_limit[3]>1)flip_count+=this.flip_limit[3]-1;

        //右斜め上方向
        //this.flip_limit.push(0);
        this.flip_limit[4]=0;
        for(var i=1;i<=Math.min(row,7-col);++i){
            if(this.board[row-i][col+i]!=this.stone[Number(!this.turn)]){
                if(this.board[row-i][col+i]==this.stone[Number(this.turn)])this.flip_limit[4]=i;
                break;
            }
        }
        if(this.flip_limit[4]>1)flip_count+=this.flip_limit[4]-1;

        //右斜め下方向
        //this.flip_limit.push(0);
        this.flip_limit[5]=0;
        for(var i=1;i<=Math.min(7-row,7-col);++i){
            if(this.board[row+i][col+i]!=this.stone[Number(!this.turn)]){
                if(this.board[row+i][col+i]==this.stone[Number(this.turn)])this.flip_limit[5]=i;
                break;
            }
        }
        if(this.flip_limit[5]>1)flip_count+=this.flip_limit[5]-1;

        //左斜め上方向
        //this.flip_limit.push(0);
        this.flip_limit[6]=0;
        for(var i=1;i<=Math.min(row,col);++i){
            if(this.board[row-i][col-i]!=this.stone[Number(!this.turn)]){
                if(this.board[row-i][col-i]==this.stone[Number(this.turn)])this.flip_limit[6]=i;
                break;
            }
        }
        if(this.flip_limit[6]>1)flip_count+=this.flip_limit[6]-1;

        //左斜め下方向
        //this.flip_limit.push(0);
        this.flip_limit[7]=0;
        for(var i=1;i<=Math.min(7-row,col);++i){
            if(this.board[row+i][col-i]!=this.stone[Number(!this.turn)]){
                if(this.board[row+i][col-i]==this.stone[Number(this.turn)])this.flip_limit[7]=i;
                break;
            }
        }
        if(this.flip_limit[7]>1)flip_count+=this.flip_limit[7]-1;
        return flip_count;
    }
}

//終局かどうか判定
function is_gameover(board){
    //0なら対局中, 1なら先手勝ち, 2なら後手勝ち, 3なら引き分け
    if(board.point[0]+board.point[1]==64){
        if(board.point[0]>board.point[1])return 1;
        else if(board.point[0]<board.point[1])return 2;
        else return 3;
    }

    //先手・後手ともに着手できなければ終局
    if(LegalMoveList(board).length==0){
        board.turn=!board.turn;
        if(LegalMoveList(board).length==0){
            if(board.point[0]>board.point[1])return 1;
            else if(board.point[0]<board.point[1])return 2;
            else return 3;
        }
        board.turn=!board.turn;
    }
    return 0;
}

//合法手のリストを生成
function LegalMoveList(board){
    let movelist=[];
    for(var i=0;i<8;++i)for(var j=0;j<8;++j){
        if(board.board[i][j]==0){
            board.set_flip_limit(i,j);
            for(var k=0;k<8;++k){
                if(board.flip_limit[k]>1){
                    movelist.push(8*i+j);
                    break;
                }
            }
        }
    }
    return movelist;
}

//APIとの通信
var movebyAI;

async function get_func(url,board,turn){
    let formData=new FormData();
    //for Debug
    //formData.append('board',"0000000000000000000000000002100000012000000000000000000000000000");
    //formData.append('turn',"0")
    formData.append('board',board);
    formData.append('turn',turn);
    console.log(formData);
    
    //for Debug
    console.log(board);
    console.log(turn);
    return  fetch(url,{
        method:"PUT",
        body:formData,
    }).then(response=>response.json());
}

const url="https://dekunobou-api.herokuapp.com/";

var board=new Board();
game_started=false;
human_turn=false;
function game_start(){
    if(!game_started){
        if(document.select_turn.human_turn[0].checked){
            // 人間が先手
            human_turn=false;
        }else{
            // 人間が後手
            human_turn=true;
            board_str="";
            for(var i=0;i<64;++i)board_str+=String(board.board[Math.floor(i/8)][i%8]);
            get_func(url+"put",board_str,String(Number(board.turn))).then((response)=>{
                const n=Number(response);
                move(n);
            })
        }
        document.getElementById("human_first").disabled=true;
        document.getElementById("human_second").disabled=true;
    }
    game_started=true;
}

function makeMove(){
    id=Number(this.getAttribute("id"));
    if(!LegalMoveList(board).includes(id)||board.turn!=human_turn||!game_started)return;
    move(id);
}

function drawing(board){
    var moves=LegalMoveList(board);
    for(var i=0; i<8;++i){
        for(var j=0; j<8;++j){
            document.getElementById(8*i+j).innerHTML="";
            if(board.board[i][j]==1){
                document.getElementById(8*i+j).innerHTML="<span class='stone_black'></span>";
            }else if(board.board[i][j]==2){
                document.getElementById(8*i+j).innerHTML="<span class='stone_white'></span>";
            }
            if(moves.includes(8*i+j)){
                document.getElementById(8*i+j).innerHTML="<span class='can_put'></span>";
            }
        }
    }
}

function move(id){
    board.push(id);

    //着手後の盤面を表示
    drawing(board);

    //枚数の表示
    document.getElementById("point_black").textContent=String(board.point[0]);
    document.getElementById("point_white").textContent=String(board.point[1]);

    //手番の表示
    if(board.turn!=human_turn){
        document.getElementById("turn").textContent="AI";
    }else{
        document.getElementById("turn").textContent="人間";
    }

    //終局の判定
    if(is_gameover(board)!=0){
        //alert(message[is_gameover(board)-1]);
        if(!human_turn){
            // 人間が先手
            document.getElementById("result").textContent=message[is_gameover(board)-1];
        }else{
            // 人間が後手
            document.getElementById("result").textContent=message[is_gameover(board)%2];
        }
        return 0;
    }

    //パスの判定
    if(LegalMoveList(board).length==0){
        board.turn=!board.turn;

        //エンジン側がパスしたときは盤面と合法手を再描画
        drawing(board);
        //手番表示を更新
        if(board.turn==true){
            document.getElementById("turn").textContent="AI";
        }else{
            document.getElementById("turn").textContent="人間";
        }
    }

    //エンジンに打たせる
    if(board.turn!=human_turn/*false*/){
        //console.log("engine");
        board_str=""
        for(var i=0;i<64;++i)board_str+=String(board.board[Math.floor(i/8)][i%8]);
        get_func(url+"put",board_str,String(Number(board.turn))).then((response)=>{
            const n=Number(response);
            move(n);
        })
    }
}

const message=["人間の勝ち","AIの勝ち","引き分け"];

//盤面の生成
var table=document.createElement("table");
for(var i=0; i<8;++i){
    var tr=document.createElement('tr');
    for(var j=0;j<8;++j){
        var td=document.createElement('td');
        td.addEventListener('click',makeMove);
        td.setAttribute('id',8*i+j);
        tr.appendChild(td);
    }
    table.appendChild(tr);
}

document.getElementById('board').appendChild(table);

//合法手・石の表示
drawing(board);

//枚数の表示
document.getElementById("point_black").textContent=String(board.point[0]);
document.getElementById("point_white").textContent=String(board.point[1]);

//手番の表示
if(board.turn!=human_turn){
    document.getElementById("turn").textContent="AI";
}else{
    document.getElementById("turn").textContent="人間";
}

//ツイート
function tweet(){
    var dataText;
    const point_human = board.point[human_turn % 2];
    const point_ai = board.point[(human_turn + 1) % 2];
    if(is_gameover(board)==0){
        dataText ="でくのぼう -遺伝的アルゴリズムを使ったオセロAI-"
    }else if(is_gameover(board)==3){
        dataText = "でくのぼうに"+"対"+point_ai+"で引き分けました… でくのぼう -遺伝的アルゴリズムを使ったオセロAI-"
    }else if(is_gameover(board)==human_turn+1){
        dataText = "でくのぼうに"+point_human+"対"+point_ai+"で勝ちました!! でくのぼう -遺伝的アルゴリズムを使ったオセロAI-"
    }else{
        dataText = "でくのぼうに"+point_human+"対"+point_ai+"で負けました… でくのぼう -遺伝的アルゴリズムを使ったオセロAI-"
    }
    console.log(dataText);
    window.open("https://twitter.com/share?text="
                +dataText+"&url=https://dekunobou.jj1guj.net/"+"&hashtags=dekunobou");
}