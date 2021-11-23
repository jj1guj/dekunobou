# dekunobou
 Othello AI using GA

# Enviroments
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

4. Edit `scripts/mainb.js` line 220 like this
```javascript
220 const url="http://localhost:5000/put"
```

5. Run this command and open `index.html` by browser.
```bash
python3 api/app.py
```

# Bug report and etc.
If you find bugs, please take a issue.