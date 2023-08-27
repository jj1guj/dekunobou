from flask import Flask,make_response,request,jsonify
from flask_cors import CORS
import logging
import subprocess
import os
import psycopg2
import yaml
from decimal import Decimal, ROUND_HALF_UP
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

# データベースの操作周り
def load_config():
    with open("dbconfig.yaml","r",encoding="utf-8") as f:
        config=yaml.safe_load(f)
    return config

# DBへの接続
def connect_to_db(config):
    username=config["username"]
    dbname=config["dbname"]
    password=config["password"]
    conn = psycopg2.connect("user={} dbname={} password={}".format(username,dbname,password))
    return conn

# DBに対局結果を登録する
@api.route("/post",methods=["POST"])
def register_result():
    winner=request.form["winner"]
    ai_turn=request.form["ai_turn"]
    black_point=request.form["black_point"]
    white_point=request.form["white_point"]
    # for debug
    api.logger.debug("winner: {}, ai_turn: {}, black_point: {}, white_point: {}"
                     .format(winner,ai_turn,black_point,white_point))
    api.logger.debug("{}, {}, {}, {}"
                     .format(type(winner),type(ai_turn),type(black_point),type(white_point)))
    
    # 念のため送信された結果に矛盾がないかチェックする
    config=load_config()
    conn=connect_to_db(config)
    with conn.cursor() as cur:
        cur.execute("insert into {} values({}, {}, {}, {})".format(config["tablename"],winner,ai_turn,black_point,white_point))
    conn.commit()
    return "0"

@api.route("/get_ai_result")
def get_ai_result():
    config=load_config()
    conn=connect_to_db(config)
    with conn.cursor() as cur:
        cur.execute("select count(*) from {} where {}.winner={}.ai_turn;".format(config["tablename"],config["tablename"],config["tablename"]))
        ai_win=cur.fetchall()[0][0]

        # デフォルト値で3は引き分けとしている
        cur.execute("select count(*) from {} where {}.winner!={}.ai_turn and {}.winner!=3;"
                    .format(config["tablename"],config["tablename"],config["tablename"],config["tablename"]))
        ai_lose=cur.fetchall()[0][0]

        # デフォルト値で3は引き分けとしている
        cur.execute("select count(*) from {} where {}.winner=3;".format(config["tablename"],config["tablename"]))
        draw=cur.fetchall()[0][0]
    
    # 勝率の計算
    win_rate=None
    # 総対局数
    battle_all_count=ai_win+ai_lose+draw
    
    # 総対局数が0でない場合のみ勝率を計算する
    if battle_all_count>0:
        win_rate=ai_win/battle_all_count*100
        win_rate=str(Decimal(str(win_rate)).quantize(Decimal("0.01"),rounding=ROUND_HALF_UP))
        
    return jsonify({"ai_win": ai_win, "ai_lose": ai_lose, "draw": draw, "win_rate": win_rate})

if __name__ == "__main__":
    api.run(host="0.0.0.0",port=int(os.environ.get("PORT",5000)),debug=False)