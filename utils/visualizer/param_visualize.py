#これを参考にした
#https://spcx8.hatenablog.com/entry/2018/10/09/070000
#https://tomosoft.jp/design/?p=29005
import cv2
import matplotlib
matplotlib.use("Agg")
from matplotlib import pyplot as plt
import numpy as np
import os
import struct
import tqdm
import yaml

with open("config.yaml") as f:
    config=yaml.safe_load(f)

file_path=config["param_dir"]
file_list=os.listdir(file_path)
file_list=[i for i in file_list if ".bin" in i]
file_num=len(os.listdir(file_path))

param_size=int(config["param_size"])
gene_num=int(config["gene_num"])

#写真保存用のパスが存在するか確認してないようなら作成する
if not os.path.exists(config["img_dir"]):
    os.mkdir(config["img_dir"])

#動画保存用のパスが存在するか確認してないようなら作成する
if not os.path.exists(config["mov_dir"]):
    os.mkdir(config["mov_dir"])

#動画の設定
mov_h=480
mov_w=640
fourcc=cv2.VideoWriter_fourcc("m","p","4","v")
video=cv2.VideoWriter(os.path.join(config["mov_dir"],"result.mp4"),
                                    fourcc,40.0,(mov_w,mov_h),True)

#世代を取得
#ファイル名は必ずhoge_[世代].csvになっている必要がある
L=[int(i.split("_")[1].split(".")[0]) for i in file_list]
L.sort()
print(L)

plt.figure()
for j in tqdm.tqdm(range(len(L))):
    i=L[j]
    filename=os.path.join(file_path,"out_{}.bin".format(i))
    

    # バイナリデータの読み込み
    with open(filename,"rb") as f:
        rawdata=f.read()
    
    data=[[struct.unpack_from("f",rawdata,4*(param_size*i+j))[0] for j in range(param_size)] for i in range(gene_num)]
    data=np.array(data)

    #ヒートマップの描画
    plt.clf()
    plt.imshow(data,interpolation="nearest",vmin=-1,vmax=1,cmap="jet",aspect=config["aspect"])
    plt.colorbar()
    plt.title("Generation: {}".format(i))
    plt.savefig(os.path.join(config["img_dir"],"out_{}.png".format(i)))

    #動画への出力
    img=cv2.imread(os.path.join(config["img_dir"],"out_{}.png".format(i)))
    video.write(img)
video.release()