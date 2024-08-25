from flask import Flask, make_response, request, jsonify
from flask_cors import CORS
import logging
import subprocess
import os

api = Flask(__name__)
logging.basicConfig(level=logging.INFO)
CORS(api)

# エンジンからの指し手を送る
@api.route("/put", methods=["PUT"])
def put():
    body = request.json
    board = body["board"]
    turn = body["turn"]
    depth = body["depth"] if "depth" in body else None
    perfect_search_depth = body["perfect_search_depth"] if "perfect_search_depth" in body else None

    # ToDo リクエストのバリデーションを追加する

    # バイナリに実行権限を与える
    subprocess.run(["chmod", "u+x", "web/api/dekunobou"])

    # 実行コマンドを構成する
    turn = str(turn)
    cmd = ["web/api/dekunobou", "--mode", "web", "-b", board, "-t", turn]

    # 他にオプションが指定されていたら指定されたオプションを追加する
    if depth is not None:
        depth = str(depth)
        cmd += ["--depth", depth]
    if perfect_search_depth is not None:
        perfect_search_depth = str(perfect_search_depth)
        cmd += ["--perfect_search_depth", perfect_search_depth]

    cp = subprocess.run(cmd, encoding="utf-8", stdout=subprocess.PIPE)
    return jsonify({"move": str(int(cp.stdout))})


@api.route("/")
def index():
    return "I'm dekunobou, computer othello program!"


if __name__ == "__main__":
    api.run(host="0.0.0.0", port=int(
        os.environ.get("PORT", 5000)), debug=False)
