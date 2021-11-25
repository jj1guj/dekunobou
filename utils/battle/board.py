# https://github.com/jj1guj/dekunobou-genetic/blob/master/othello.hpp をPython用に書き直した
# src/board.hpp, src/legalmovelist.hpp をPython向けにwrapperできたらそっちに置き換える予定
class Board:
    def __init__(self):
        self.board=[[0 for i in range(8)] for j in range(8)]
        self.board[3][3]=-1
        self.board[3][4]=1
        self.board[4][3]=1
        self.board[4][4]=-1
        self.point=[2,2]
        self.stone=[1,-1]
        self.turn=False

    def push(self,move):
        di=[0,0,-1,1,-1,1,-1,1]
        dj=[-1,1,0,0,1,1,-1,-1]
        if 0<=move and move<=63:
            row=move//8
            col=move%8
            if self.board[row][col]!=0:
                return -1
            
            flip_count,flip_limit=self.set_flip_limit(row,col)
            if flip_count==0:
                return -2
            
            #石を返す
            self.board[row][col]=self.stone[self.turn]
            for dir in range(8):
                for i in range(1,flip_limit[dir]):
                    self.board[row+di[dir]*i][col+dj[dir]*i]=self.stone[self.turn]
            
            #着手後の石の枚数を計算
            self.point[self.turn]+=flip_count+1
            self.point[not self.turn]-=flip_count
        self.turn=not self.turn #手番を反転
        return 0
    
    def set_flip_limit(self,row,col):
        flip_count=0
        flip_limit=[0 for i in range(8)]

        #横左方向
        for i in range(1,col+1):
            if self.board[row][col-i]!=self.stone[not self.turn]:
                if self.board[row][col-i]==self.stone[self.turn]:
                    flip_limit[0]=i
                break
        flip_count+=max(0,flip_limit[0]-1)

        #横右方向
        for i in range(1,8-col):
            if self.board[row][col+i]!=self.stone[not self.turn]:
                if self.board[row][col+i]==self.stone[self.turn]:
                    flip_limit[1]=i
                break
        flip_count+=max(0,flip_limit[1]-1)

        #縦上方向
        for i in range(1,row+1):
            if self.board[row-i][col]!=self.stone[not self.turn]:
                if self.board[row-i][col]==self.stone[self.turn]:
                    flip_limit[2]=i
                break
        flip_count+=max(0,flip_limit[2]-1)

        #縦下方向
        for i in range(1,8-row):
            if self.board[row+i][col]!=self.stone[not self.turn]:
                if self.board[row+i][col]==self.stone[self.turn]:
                    flip_limit[3]=i
                break
        flip_count+=max(0,flip_limit[3]-1)

        #右斜め上方向
        for i in range(1,min(row,7-col)+1):
            if self.board[row-i][col+i]!=self.stone[not self.turn]:
                if self.board[row-i][col+i]==self.stone[self.turn]:
                    flip_limit[4]=i
                break
        flip_count+=max(0,flip_limit[4]-1)

        #右斜め下方向
        for i in range(1,min(7-row,7-col)+1):
            if self.board[row+i][col+i]!=self.stone[not self.turn]:
                if self.board[row+i][col+i]==self.stone[self.turn]:
                    flip_limit[5]=i
                break
        flip_count+=max(0,flip_limit[5]-1)

        #左斜め上方向
        for i in range(1,min(row,col)+1):
            if self.board[row-i][col-i]!=self.stone[not self.turn]:
                if self.board[row-i][col-i]==self.stone[self.turn]:
                    flip_limit[6]=i
                break
        flip_count+=max(0,flip_limit[6]-1)

        #左斜め下方向
        for i in range(1,min(7-row,col)+1):
            if self.board[row+i][col-i]!=self.stone[not self.turn]:
                if self.board[row+i][col-i]==self.stone[self.turn]:
                    flip_limit[7]=i
                break
        flip_count+=max(0,flip_limit[7]-1)
        return flip_count,flip_limit

    def __getitem__(self,i):
        return self.board[i//8][i%8]

    def __repr__(self):
        out=""
        for i in range(8):
            for j in range(8):
                if self.board[i][j]==0:
                    out+="."
                elif self.board[i][j]==1:
                    out+="o"
                else:
                    out+="x"
            out+="\n"
        return out