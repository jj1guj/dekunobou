function getRandomInt(min, max) {
  return Math.floor(Math.random() * (max - min) + min);
}

//1を黒石, 2を白石として処理を行う
class Board {
    constructor() {
        this.board = [];
        this.turn = false;//trueなら後手
        this.point;
        this.stone = [1, 2];
        this.flip_limit = [0, 0, 0, 0, 0, 0, 0, 0];
        this.di = [0, 0, -1, 1, -1, 1, -1, 1];
        this.dj = [-1, 1, 0, 0, 1, 1, -1, -1];

        this.init();
    }

    init() {
        for (var i = 0; i < 8; ++i) {
            let board_line = [];
            for (var j = 0; j < 8; ++j)board_line.push(0);
            this.board.push(board_line);
        }
        this.board[3][3] = 2;
        this.board[3][4] = 1;
        this.board[4][3] = 1;
        this.board[4][4] = 2;
        this.point = [2, 2]; //[先手の枚数, 後手の枚数]
    }

    push(move) {
        if (0 <= move && move <= 63) {
            var row = Math.floor(move / 8);
            var col = move % 8;
            if (this.board[row][col] != 0) return -1;

            var fliped = this.set_flip_limit(row, col);
            if (fliped == 0) return -1;

            //石を返す
            this.board[row][col] = this.stone[Number(this.turn)];

            for (var dir = 0; dir < 8; ++dir) {
                for (var i = 1; i < this.flip_limit[dir]; ++i) {
                    this.board[row + this.di[dir] * i][col + this.dj[dir] * i] = this.stone[Number(this.turn)];
                }
            }

            //着手後の石の枚数を計算
            this.point[Number(this.turn)] += fliped + 1;
            this.point[Number(!this.turn)] -= fliped;

            this.turn = (this.turn + 1) % 2;//手番を反転
            return 0;
        }
    }

    set_flip_limit(row, col) {
        //返せる石の枚数が返り値
        var flip_count = 0;
        //横左方向
        //this.flip_limit=[];
        //this.flip_limit.push(0);
        this.flip_limit[0] = 0;
        for (var i = 1; i <= col; ++i) {
            if (this.board[row][col - i] != this.stone[Number(!this.turn)]) {
                if (this.board[row][col - i] == this.stone[Number(this.turn)]) this.flip_limit[0] = i;
                break;
            }
        }
        if (this.flip_limit[0] > 1) flip_count += this.flip_limit[0] - 1;

        //横右方向
        //this.flip_limit.push(0);
        this.flip_limit[1] = 0;
        for (var i = 1; i <= 7 - col; ++i) {
            if (this.board[row][col + i] != this.stone[Number(!this.turn)]) {
                if (this.board[row][col + i] == this.stone[Number(this.turn)]) this.flip_limit[1] = i;
                break;
            }
        }
        if (this.flip_limit[1] > 1) flip_count += this.flip_limit[1] - 1;

        //縦上方向
        //this.flip_limit.push(0);
        this.flip_limit[2] = 0;
        for (var i = 1; i <= row; ++i) {
            if (this.board[row - i][col] != this.stone[Number(!this.turn)]) {
                if (this.board[row - i][col] == this.stone[Number(this.turn)]) this.flip_limit[2] = i;
                break;
            }
        }
        if (this.flip_limit[2] > 1) flip_count += this.flip_limit[2] - 1;

        //縦下方向
        //this.flip_limit.push(0);
        this.flip_limit[3] = 0;
        for (var i = 1; i <= 7 - row; ++i) {
            if (this.board[row + i][col] != this.stone[Number(!this.turn)]) {
                if (this.board[row + i][col] == this.stone[Number(this.turn)]) this.flip_limit[3] = i;
                break;
            }
        }
        if (this.flip_limit[3] > 1) flip_count += this.flip_limit[3] - 1;

        //右斜め上方向
        //this.flip_limit.push(0);
        this.flip_limit[4] = 0;
        for (var i = 1; i <= Math.min(row, 7 - col); ++i) {
            if (this.board[row - i][col + i] != this.stone[Number(!this.turn)]) {
                if (this.board[row - i][col + i] == this.stone[Number(this.turn)]) this.flip_limit[4] = i;
                break;
            }
        }
        if (this.flip_limit[4] > 1) flip_count += this.flip_limit[4] - 1;

        //右斜め下方向
        //this.flip_limit.push(0);
        this.flip_limit[5] = 0;
        for (var i = 1; i <= Math.min(7 - row, 7 - col); ++i) {
            if (this.board[row + i][col + i] != this.stone[Number(!this.turn)]) {
                if (this.board[row + i][col + i] == this.stone[Number(this.turn)]) this.flip_limit[5] = i;
                break;
            }
        }
        if (this.flip_limit[5] > 1) flip_count += this.flip_limit[5] - 1;

        //左斜め上方向
        //this.flip_limit.push(0);
        this.flip_limit[6] = 0;
        for (var i = 1; i <= Math.min(row, col); ++i) {
            if (this.board[row - i][col - i] != this.stone[Number(!this.turn)]) {
                if (this.board[row - i][col - i] == this.stone[Number(this.turn)]) this.flip_limit[6] = i;
                break;
            }
        }
        if (this.flip_limit[6] > 1) flip_count += this.flip_limit[6] - 1;

        //左斜め下方向
        //this.flip_limit.push(0);
        this.flip_limit[7] = 0;
        for (var i = 1; i <= Math.min(7 - row, col); ++i) {
            if (this.board[row + i][col - i] != this.stone[Number(!this.turn)]) {
                if (this.board[row + i][col - i] == this.stone[Number(this.turn)]) this.flip_limit[7] = i;
                break;
            }
        }
        if (this.flip_limit[7] > 1) flip_count += this.flip_limit[7] - 1;
        return flip_count;
    }
}

function board_to_str(board) {
    var board_str = ""
    for (var i = 0; i < 64; ++i)board_str += String(board.board[Math.floor(i / 8)][i % 8]);
    return board_str;
}

//終局かどうか判定
function is_gameover(board) {
    //0なら対局中, 1なら先手勝ち, 2なら後手勝ち, 3なら引き分け
    if (board.point[0] + board.point[1] == 64) {
        if (board.point[0] > board.point[1]) return 1;
        else if (board.point[0] < board.point[1]) return 2;
        else return 3;
    }

    //先手・後手ともに着手できなければ終局
    if (LegalMoveList(board).length == 0) {
        board.turn = !board.turn;
        if (LegalMoveList(board).length == 0) {
            if (board.point[0] > board.point[1]) return 1;
            else if (board.point[0] < board.point[1]) return 2;
            else return 3;
        }
        board.turn = !board.turn;
    }
    return 0;
}

//合法手のリストを生成
function LegalMoveList(board) {
    let movelist = [];
    for (var i = 0; i < 8; ++i)for (var j = 0; j < 8; ++j) {
        if (board.board[i][j] == 0) {
            board.set_flip_limit(i, j);
            for (var k = 0; k < 8; ++k) {
                if (board.flip_limit[k] > 1) {
                    movelist.push(8 * i + j);
                    break;
                }
            }
        }
    }
    return movelist;
}

//APIとの通信
var movebyAI;

// 勝敗の結果をDBに登録する
async function register_result(url, ai_turn_bool, board) {
    console.log("ai_level:", ai_level);
    // 先手と後手どちらの勝ちかを取得
    var winner = is_gameover(board);

    // ai_turn_boolはbooleanで渡される.
    // falseなら先手, trueなら後手
    // DBに登録する際は先手を1, 後手を2として登録する
    // これはis_gameover()の出力に準拠している
    var ai_turn;
    if (!ai_turn_bool) {
        ai_turn = 1;
    } else {
        ai_turn = 2;
    }

    // 先手の枚数と後手の枚数を取得
    var black_point = board.point[0];
    var white_point = board.point[1];

    // リクエストボディ
    const requestBody = {
        winner: winner,
        ai_turn: ai_turn,
        ai_level: ai_level,
        black_point: black_point,
        white_point: white_point
    };

    if (1 <= winner <= 3) {
        await fetch(url, {
            method: "POST",
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(requestBody)
        });
    }
}

// AIの通算成績を取得&表示する
async function get_results(url) {
    fetch(url).then(response => response.json()).then((data) => {
        document.getElementById("ai_win").textContent = data["ai_win"];
        document.getElementById("ai_lose").textContent = data["ai_lose"];
        document.getElementById("draw").textContent = data["draw"];
        document.getElementById("win_rate").textContent = data["win_rate"];
    });
}

// AIの通算成績を取得&表示する
async function get_results_by_level(url, ai_level) {
    // リクエストボディ
    const requestBody = {
        ai_level: ai_level,
    };

    fetch(url, {
        method: "POST",
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(requestBody)
    }).then(response => response.json()).then((data) => {
        document.getElementById("ai_win_by_level").textContent = data["ai_win"];
        document.getElementById("ai_lose_by_level").textContent = data["ai_lose"];
        document.getElementById("draw_by_level").textContent = data["draw"];
        document.getElementById("win_rate_by_level").textContent = data["win_rate"];
    });
}

async function get_func(url, board, turn, depth = 7, perfect_search_depth = 13) {

    console.log("depth:", depth, "perfect_search_depth:", perfect_search_depth);
    // リクエストボディ
    const requestBody = {
        board: board,
        turn: turn,
        depth: depth,
        perfect_search_depth: perfect_search_depth
    };

    try {
        // PUTリクエストを送信
        const response = await fetch(url, {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(requestBody)
        });

        // レスポンスがOKか確認
        if (response.ok) {
            const responseData = await response.json();

            // move プロパティの値を取り出して整数に変換
            if (responseData.hasOwnProperty('move')) {
                const move = parseInt(responseData.move, 10);
                return move; // move を返す
            } else {
                console.error('Response does not contain "move" property.');
                return null; // move プロパティがない場合は null を返す
            }
        } else {
            console.error('Error:', response.status, response.statusText);
            return null; // エラー時も null を返す
        }
    } catch (error) {
        console.error('Network Error:', error);
        return null; // ネットワークエラー時も null を返す
    }
}

// AIの通算成績の取得
get_results("/get_ai_result");

const ai_config = [
    {
        min_depth: 3,
        max_depth: 3,
        perfect_search_depth: 5
    },
    {
        min_depth: 6,
        max_depth: 8,
        perfect_search_depth: 13
    },
    {
        min_depth: 9,
        max_depth: 11,
        perfect_search_depth: 15
    }
]

var board = new Board();
game_started = false;
human_turn = false;
ai_level = 0;

// AIレベルの決定
// ページが読み込まれたときに実行
window.addEventListener('DOMContentLoaded', function() {
    // 初期表示を設定
    updateAiLevelDisplay();

    // すべてのAIレベルのラジオボタンを取得
    const aiLevelRadios = document.querySelectorAll('input[name="ai_level"]');

    // 各ラジオボタンにイベントリスナーを追加
    aiLevelRadios.forEach(function(radio) {
        radio.addEventListener('change', function() {
            updateAiLevelDisplay();
        });
    });
});

function updateAiLevelDisplay() {
    // AIレベルの決定
    if (document.select_level.ai_level[0].checked) {
        ai_level = 0;
    } else if (document.select_level.ai_level[1].checked) {
        ai_level = 1
    } else {
        ai_level = 2;
    }
    document.getElementById("ai_level").textContent = ai_level + 1
    // TODO: 選択したAIのレベルの通算戦績が出るようにする
    get_results_by_level("/get_ai_result_by_level", ai_level);
}

function game_start() {
    if (!game_started) {
        document.getElementById("ai_level1").disabled = true;
        document.getElementById("ai_level2").disabled = true;
        document.getElementById("ai_level3").disabled = true;

        // 手番の決定
        if (document.select_turn.human_turn[0].checked) {
            // 人間が先手
            human_turn = false;
        } else {
            // 人間が後手
            human_turn = true;
            depth = getRandomInt(ai_config[ai_level].min_depth, ai_config[ai_level].max_depth);
            perfect_search_depth = ai_config[ai_level].perfect_search_depth;
            get_func("/put", board_to_str(board), Number(board.turn), depth, perfect_search_depth).then(n => {
                move(n);
            })
        }
        document.getElementById("human_first").disabled = true;
        document.getElementById("human_second").disabled = true;

    }
    game_started = true;
}

function makeMove() {
    id = Number(this.getAttribute("id"));
    if (!LegalMoveList(board).includes(id) || board.turn != human_turn || !game_started) return;
    move(id);
}

function drawing(board) {
    var moves = LegalMoveList(board);
    for (var i = 0; i < 8; ++i) {
        for (var j = 0; j < 8; ++j) {
            document.getElementById(8 * i + j).innerHTML = "";
            if (board.board[i][j] == 1) {
                document.getElementById(8 * i + j).innerHTML = "<span class='stone_black'></span>";
            } else if (board.board[i][j] == 2) {
                document.getElementById(8 * i + j).innerHTML = "<span class='stone_white'></span>";
            }
            if (moves.includes(8 * i + j)) {
                document.getElementById(8 * i + j).innerHTML = "<span class='can_put'></span>";
            }
        }
    }
}

function move(id) {
    board.push(id);

    //着手後の盤面を表示
    drawing(board);

    //枚数の表示
    document.getElementById("point_black").textContent = String(board.point[0]);
    document.getElementById("point_white").textContent = String(board.point[1]);

    //手番の表示
    if (board.turn != human_turn) {
        document.getElementById("turn").textContent = "AI";
    } else {
        document.getElementById("turn").textContent = "人間";
    }

    //終局の判定
    if (is_gameover(board) != 0) {
        //alert(message[is_gameover(board)-1]);
        if (!human_turn) {
            // 人間が先手
            document.getElementById("result").textContent = message[is_gameover(board) - 1];
        } else {
            // 人間が後手
            document.getElementById("result").textContent = message[is_gameover(board) % 2];
        }

        // DBに結果を送信
        register_result("/post", !human_turn, board);
        return 0;
    }

    //パスの判定
    if (LegalMoveList(board).length == 0) {
        board.turn = !board.turn;

        //エンジン側がパスしたときは盤面と合法手を再描画
        drawing(board);
        //手番表示を更新
        if (board.turn == true) {
            document.getElementById("turn").textContent = "AI";
        } else {
            document.getElementById("turn").textContent = "人間";
        }
    }

    //エンジンに打たせる
    if (board.turn != human_turn/*false*/) {
        // 探索深さを決める
        depth = getRandomInt(ai_config[ai_level].min_depth, ai_config[ai_level].max_depth);
        perfect_search_depth = ai_config[ai_level].perfect_search_depth;
        get_func("/put", board_to_str(board), Number(board.turn), depth, perfect_search_depth).then(n => {
            move(n);
        })
    }
}

const message = ["人間の勝ち", "AIの勝ち", "引き分け"];

//盤面の生成
var table = document.createElement("table");
for (var i = 0; i < 8; ++i) {
    var tr = document.createElement('tr');
    for (var j = 0; j < 8; ++j) {
        var td = document.createElement('td');
        td.addEventListener('click', makeMove);
        td.setAttribute('id', 8 * i + j);
        tr.appendChild(td);
    }
    table.appendChild(tr);
}

document.getElementById('board').appendChild(table);

//合法手・石の表示
drawing(board);

//枚数の表示
document.getElementById("point_black").textContent = String(board.point[0]);
document.getElementById("point_white").textContent = String(board.point[1]);

//手番の表示
if (board.turn != human_turn) {
    document.getElementById("turn").textContent = "AI";
} else {
    document.getElementById("turn").textContent = "人間";
}

//ツイート
function tweet() {
    var dataText;
    const point_human = board.point[human_turn % 2];
    const point_ai = board.point[(human_turn + 1) % 2];
    if (is_gameover(board) == 0) {
        dataText = "でくのぼう -遺伝的アルゴリズムを使ったオセロAI-"
    } else if (is_gameover(board) == 3) {
        dataText = "でくのぼうに" + "対" + point_ai + "で引き分けました… でくのぼう -遺伝的アルゴリズムを使ったオセロAI-"
    } else if (is_gameover(board) == human_turn + 1) {
        dataText = "でくのぼうに" + point_human + "対" + point_ai + "で勝ちました!! でくのぼう -遺伝的アルゴリズムを使ったオセロAI-"
    } else {
        dataText = "でくのぼうに" + point_human + "対" + point_ai + "で負けました… でくのぼう -遺伝的アルゴリズムを使ったオセロAI-"
    }
    console.log(dataText);
    window.open("https://twitter.com/share?text="
        + dataText + "&url=https://dekunobou.jj1guj.net/" + "&hashtags=dekunobou");
}

// MFMを使用して終局図を添付する
function make_end_game_string() {
    const board_str = board_to_str(board);
    const black = "$[fg.color=000000 ●] "
    const white = "$[fg.color=FFFFFF ●] "
    const empty = "　 "
    var end_game_string = "$[x2 $[bg.color=14962e "

    for (var i = 0; i < 64; ++i) {
        if (board_str[i] == "1") {
            end_game_string += black
        } else if (board_str[i] == "2") {
            end_game_string += white
        } else {
            end_game_string += empty
        }
        if (i % 8 == 7 && i < 63) end_game_string += "\n"
    }

    end_game_string += "]]"
    return end_game_string
}

// Misskeyにノート
function misskey_note() {
    var dataText = ""
    const point_human = board.point[human_turn % 2];
    const point_ai = board.point[(human_turn + 1) % 2];
    if (is_gameover(board) > 0) {
        if (is_gameover(board) == 3) {
            dataText = "でくのぼうに" + point_human + "対" + point_ai + "で引き分けました… "
        } else if (is_gameover(board) == human_turn + 1) {
            dataText = "でくのぼうに" + point_human + "対" + point_ai + "で勝ちました!! "
        } else {
            dataText = "でくのぼうに" + point_human + "対" + point_ai + "で負けました… "
        }

        dataText += "でくのぼう -遺伝的アルゴリズムを使ったオセロAI- \nhttps://dekunobou.jj1guj.net\n#dekunobou\n\n\n"
        dataText += "終局図\n"

        if (human_turn) dataText += "先手: AI, 後手: 人間\n"
        else dataText += "先手: 人間, 後手: AI\n"
        // 終局していれば終局図を添付する
        dataText += make_end_game_string()
    } else {
        dataText = " でくのぼう -遺伝的アルゴリズムを使ったオセロAI- \nhttps://dekunobou.jj1guj.net\n#dekunobou"
    }

    return dataText
}
