from flask import Flask,make_response,request,jsonify
from flask_cors import CORS
import logging
import subprocess
import os

api=Flask(__name__)
logging.basicConfig(level=logging.INFO)
CORS(api)

#エンジンからの指し手を送る
@api.route("/put",methods=["PUT"])
def put():
    board=request.form["board"]
    turn=request.form["turn"]
    #バイナリに実行権限を与える
    subprocess.run(["bash", "permission.sh"])
    cp=subprocess.run(["./dekunobou",board,turn],encoding="utf-8",stdout=subprocess.PIPE)
    return str(int(cp.stdout))

@api.route("/")
def index():
    return "I'm dekunobou, computer othello program!"

if __name__ == "__main__":
    api.run(host="0.0.0.0",port=int(os.environ.get("PORT",5000)),debug=False)