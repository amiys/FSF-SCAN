# FSF-SCAN
固有ジョセフソン接合における電流-電圧特性(I-V特性)の画像データ(jpg形式)から臨界電流(Ic)の位置を見つけて，原点からのピクセル量を出力及びファイルへ書き出す（複数画像データ入力も可能）

以下は使い方
-------------------------------------------------

手順
1.「pics」フォルダに所望の画像ファイル群を入れる
2.exeファイルを起動する
3.各種パラメータを設定する
4.始めと終わりの画像番号を入力すると処理が開始する

5.続けて実行する場合は1.または2.から始めても良いが
　最終的に書き出される「.data」形式のデータは上書きされるようになっているので，
　新しく書き出したいときは削除するか名前を変えること


仕様
・Icを平均値で出したい場合は mode は「a」にして，Icの最大値を出したい場合は mode は「m」にする

・画像パラメータの設定
　IPx…「1レンジあたりのピクセル量(電圧軸)」(※倍率100%にてHakarun.exeなどを使って測る→ペイントがお薦め)
　IPy…「1レンジあたりのピクセル量(電流軸)」(※倍率100%にてHakarun.exeなどを使って測る→ペイントがお薦め)
　Vrange…「1レンジあたりの電圧(単位は[mV])」　(例：50mV/div → 50)
　Irange…「1レンジあたりの電流(単位は[mA])」　(例：100uA/div → 0.1)
　
　Th_max…「全ての画像において変動のない抵抗状態(直線部分)の箇所の電圧(単位は[mV])」
　　　　　　(例：80mV以上からであれば直線の形状はどの画像も同じ場合 → 80)
　　　　　　(だいたいで構わないが低いほうが無難)

・「Picture Number(end)」にて「0」と入力すると「Picture Number(start)」の画像のみを処理する


注意
・軸の目盛から読み取る値と誤差を少なくしたいときは画像の傾きを補正すると良い
・手振れした画像が多く入っていると正しく計測できない時がある
・Icの遷移を写真と照らし合わせ，目視で確認すること
・誤差が大きい場合は手動で計測をする
・実行結果をすぐ消さずに見たい場合は「コマンドプロンプト」で起動する

-------------------------------------------------
