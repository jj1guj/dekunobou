# でくのぼう
 遺伝的アルゴリズムを使用したオセロAI
 (English version is [below](https://github.com/jj1guj/dekunobou/blob/main/README.md#dekunobou))

# 動作環境
- Linux  
注意!: このプログラムはWindowsでの動作を確認していません.  
もしOSがWindowsでしたらWSLの使用を推奨します.

# 必要なライブラリ
- OpenMP

# 使い方
## 評価関数を生成する
1. `dekunobou.hpp`を開き, 15行目から18行目を以下のように編集する
```cpp
15 //#define Debug //デバッグモード
16 //#define Console //コンソールモードでの対局(default)
17 #define GA //遺伝的アルゴリズムによる評価関数生成
18 //#define API //https://jj1guj.net/dekunobou-web/ 向けのAPI用バイナリ
```

2. 以下のコマンドを実行してビルドする
```bash
mkdir build && cd build
cmake ..
make
```

3. 以下のコマンドを実行する
```bash
./dekunobou [スレッド数]
```

## でくのぼうとターミナル上で対戦する
1. `dekunobou.hpp`を開き, 15行目から18行目を以下のように編集する.
```cpp
15 //#define Debug //デバッグモード
16 #define Console //コンソールモードでの対局(default)
17 //#define GA //遺伝的アルゴリズムによる評価関数生成
18 //#define API //https://jj1guj.net/dekunobou-web/ 向けのAPI用バイナリ
```

2. 以下のコマンドを実行してビルドする
```bash
mkdir build && cd build
cmake ..
make
```

3. 以下のコマンドを実行する
```bash
./dekunobou
```
あとは表示されたメッセージに従って遊んでください.

## ブラウザででくのぼうと対戦する
1. `dekunobou.hpp`を開き, 15行目から18行目を以下のように編集する.
```cpp
15 //#define Debug //デバッグモード
16 //#define Console //コンソールモードでの対局(default)
17 //#define GA //遺伝的アルゴリズムによる評価関数生成
18 #define API //https://jj1guj.net/dekunobou-web/ 向けのAPI用バイナリ
```

2. 以下のコマンドを実行してビルドする
```bash
mkdir build && cd build
cmake ..
make
```

3. [dekunobou-web]("https://github.com/jj1guj/dekunobou-web") をcloneし, `api/`に2.でビルドしたバイナリをコピーする.
```bash
git clone https://github.com/jj1guj/dekunobou-web
cd dekunobou-web/api
cp [path-to-build]/dekunobou .
```

4. `scripts/main.js`を開き, 220行目を以下のように編集する.
```javascript
220 const url="http://localhost:5000/put"
```

5. dekunobou-web内で以下のコマンドを実行し, `index.html`をブラウザで開く
```bash
cd api
pip3 install -r requirements.txt
python3 app.py
```

# バグ報告など
バグ等ありましたらissueまでよろしくおねがいします.

# dekunobou
 Othello AI using GA

# Environments
- Linux  
Caution: This program is not confirmed on Windows.
If your OS is Windows, please use WSL.

# Requirements
- OpenMP

# Usage
## Generate evaluating function
1. Edit `dekunobou.hpp` from line 15 to line 18 like this.
```cpp
15 //#define Debug //デバッグモード
16 //#define Console //コンソールモードでの対局(default)
17 #define GA //遺伝的アルゴリズムによる評価関数生成
18 //#define API //https://jj1guj.net/dekunobou-web/ 向けのAPI用バイナリ
```

2. Run these comands and build dekunobou.
```bash
mkdir build && cd build
cmake ..
make
```

3. Run this command
```bash
./dekunobou [number of threads]
```

## Play with dekunobou via terminal
1. Edit `dekunobou.hpp` from line 15 to line 18 like this.
```cpp
15 //#define Debug //デバッグモード
16 #define Console //コンソールモードでの対局(default)
17 //#define GA //遺伝的アルゴリズムによる評価関数生成
18 //#define API //https://jj1guj.net/dekunobou-web/ 向けのAPI用バイナリ
```

2. Run these comands and build dekunobou.
```bash
mkdir build && cd build
cmake ..
make
```

3. Run this command
```bash
./dekunobou
```

## Play with dekunobou via Web browser
1. Edit `dekunobou.hpp` from line 15 to line 18 like this.
```cpp
15 //#define Debug //デバッグモード
16 //#define Console //コンソールモードでの対局(default)
17 //#define GA //遺伝的アルゴリズムによる評価関数生成
18 #define API //https://jj1guj.net/dekunobou-web/ 向けのAPI用バイナリ
```

2. Run these comands and build dekunobou.
```bash
mkdir build && cd build
cmake ..
make
```

3. Clone [dekunobou-web]("https://github.com/jj1guj/dekunobou-web") and copy binary (dekunobou) to `api/`
```bash
git clone https://github.com/jj1guj/dekunobou-web
cd dekunobou-web/api
cp [path-to-build]/dekunobou .
```

4. Edit `scripts/main.js` line 220 like this
```javascript
220 const url="http://localhost:5000/put"
```

5. Run this command and open `index.html` by browser.
```bash
cd api
pip3 install -r requirements.txt
python3 app.py
```

# Bug report and etc.
If you find bugs, please take a issue.
