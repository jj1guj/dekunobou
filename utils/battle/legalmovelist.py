class LegalMoveList:
    def __init__(self,board):
        self.movelist=[]
        for i in range(8):
            for j in range(8):
                if board[8*i+j]==0:
                    _,flip_limit=board.set_flip_limit(i,j)
                    if max(flip_limit)>1:
                        self.movelist.append(8*i+j)
    
    def __len__(self):
        return len(self.movelist)

    def __getitem__(self,i):
        return self.movelist[i]
    
    def __repr__(self):
        return " ".join(map(str,self.movelist))