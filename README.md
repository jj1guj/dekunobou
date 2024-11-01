# でくのぼう

遺伝的アルゴリズムを使用したオセロ AI
(English version is [below](https://github.com/jj1guj/dekunobou/blob/main/README.md#dekunobou))

# 動作環境

- Linux  
  注意!: このプログラムは Windows での動作を確認していません.  
  もし OS が Windows でしたら WSL の使用を推奨します.
- Rust

# 必要なライブラリ

- OpenMP

# ビルド

```
mkdir build && cd build
cmake ..
make
```

# 使い方

`ulimit -s unlimited`は web モードでは不要です.

```
ulimit -s unlimited
./dekunobou --mode <モード名> [options]
```

```
--mode <mode name>           - 実行するモード.
         ga                    - 遺伝的アルゴリズムによる評価関数生成.
         web                   - Webアプリケーション向けの実行機能.
  --help                       - ヘルプを表示.
  -d                           - デバッグモードの有効化.
  ga mode options:
    --out_path <output path>   - [Required] 評価関数の出力先ディレクトリ.
    -M <integer>               - 1世代あたりの交叉の実行回数. default: 100
    --match_genetic <integer>  - 交叉時の親と子の対局数. default: 30
    --thresh <number>          - 親を子で置き換える勝率の閾値 (0 ~ 1). default: 0.72
    --mutation_start <integer> - 突然変異を始める時間 (hour). default: 0
    --mutation_prob <integer>  - 突然変異の確率 (0 ~ 1). default: 1e-3
    --time_limit <integer>     - 遺伝的アルゴリズムを実行する時間 (hour). default: 36
    --thread <integer>         - 遺伝的アルゴリズムを実行するスレッド数 (1 ~ 実行環境のCPUのスレッド数). default: 実行環境のCPUのスレッド数
  web mode options:
    -b <string>                - [Required] 限局面. 黒石は '1', 白石は '2', 何も置いてなければ '0'. 盤の左上から右下にかけて左から順番に変換する
    -t <0 or 1>                - [Required] 限局面の手番. 0 が先手, 1 が後手.
    --depth <integer>          - 探索深さ. default: 9
```

# ブラウザでの遊び方

以下のコマンドを実行してください.

```
sudo docker compose up -d
```

ブラウザで`http://localhost:5000`を開くと遊ぶことができます.

# バグ報告など

バグ等ありましたら issue までよろしくおねがいします.

# dekunobou

Othello AI using GA

# Environments

- Linux  
  Caution: This program is not confirmed on Windows.
  If your OS is Windows, please use WSL.
- Rust

# Requirements

- OpenMP

# Build

```
mkdir build && cd build
cmake ..
make
```

# Usage

`ulimit -s unlimited` is not required in "web" mode.

```
ulimit -s unlimited
./dekunobou --mode <mode name> [options]
```

```
--mode <mode name>           - Mode name to run
         ga                    - Generate eval params by genetic algorithm.
         web                   - For web application's API mode.
  --help                       - Print this help.
  -d                           - Enable debug mode.
  ga mode options:
    --out_path <output path>   - [Required] Directory to which the generated parameters are output.
    -M <integer>               - Number of intersections in one generation. default: 100
    --match_genetic <integer>  - Number of games between parent and child at intersection. default: 30
    --thresh <number>          - Threshold for the winning rate of replacing a parent with a child(0 ~ 1). default: 0.72
    --mutation_start <integer> - Time to start mutation (hour). default: 0
    --mutation_prob <integer>  - Probability of mutation. default: 1e-3
    --time_limit <integer>     - Time to perform the genetic algorithm (hour). default: 36
    --thread <integer>         - Number of threads when performing genetic algorithm (1 ~ [Number of threads on your CPU]). default: [Number of threads on your CPU]
  web mode options:
    -b <string>                - [Required] Current board. Black is '1', white is '2', and none is '0'.
    -t <0 or 1>                - [Required] Current turn. 0 is Black, 1 is White.
    --depth <integer>          - Depth to search. default: 9
```

# How to play in the browser

Execute the following command.

```
sudo docker compose up -d
```

You can play by opening `http://localhost:5000` in your browser.

# Bug report and etc.

If you find bugs, please take a issue.
