#投げ銭BOX for MacOSX
===========

投げ銭ができるアプリです。  
プレゼンなどでリアルタイムに反応することができ、  
すぐに評価をフィードバックすることができます。

「投げ銭コントローラー」と「投げ銭BOX」の二つからなり、  
両方用意しないと投げ銭ができません。  
現在コントローラーは以下の種類があります。  
	iPhone用アプリ（iOS7以上対応）  
	MacOSX用  
	PHP用  
	Python用（有志の方に作成頂きました）  


投げ銭BOXはMacOSX、Windowsの２つがあります。  

こちらからダウンロードできます → <http://www.exeweb.net/app/nagesen.php>


#動作環境
 * MacOS X or Windows  
 * コントローラーとBOXを必ず同じIP内にしてお使いください。  
 * ポートは「8999」を使用します。  


#更新履歴

 * ver.0.4  
 	投げ銭サーバーとの連携により、遠隔投げができるようになりました。
 	(wwwを介した投げ銭が可能に。)
 	サーバーは複数設置可能で、BOXを使用する際に選ぶようになっています。

 * ver.0.3  
 	コインの種類を増やしました。  
 	OSCの引数が変更になりました。以下のとおりです  
 		・100円　アドレス「/mouse/button」 引数「y100」  
 		・10円　 アドレス「/mouse/button」 引数「y10」  
 		・煽り円　アドレス「/mouse/button」 引数「aoriyen」  
 	引数「down」は非推奨となりました。（10円として反応はします）  
 	コインの大きさをver.0.1に戻しました。  

 * ver.0.2  
 	プロジェクションマッピング機能追加  
 	背景追加  
 	円の大きさを小さくする  
 	10円表示リセットと、累計金額リセットの２つの機能を追加  
 	調整モードを追加

 * ver.0.1
 	投げ銭アプリ開発。