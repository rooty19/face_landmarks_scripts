# face_landmarks_scripts
- convertpts.cpp ... 学習済みデータをバイナリ化するもの, Liblinearのload_model_binとの互換性は無い(load_modelの機能限定版?), エンディアンについては考慮していないので沼る可能性はあり
- thinout.sh ... ランドマーク数を落とす(68->x)スクリプト, 68点の番号は https://ibug.doc.ic.ac.uk/media/uploads/images/300-w/figure_1_68.jpg に対応している(学習を走らせたりエラーを算出する際にはスクリプトのメッセージに従うこと)
- genlist.sh ... = ls -1 hoge/*.pts >> huga(つまりいらない子)
