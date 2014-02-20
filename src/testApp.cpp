#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    //背景
    ofBackground(0, 0, 0);
    
    //画像の読み込み
    myImage.loadImage("10yen.png");
    
    //指定したポートで接続
    receiver.setup( PORT );
    //値を初期化
    mouseX = 0;
    mouseY = 0;
    mouseButtonState = "";
    
    //Box2Dの基本設定
    box2d.init(); //初期化
    box2d.registerGrabbing(); //物体をつかめるように設定
    box2d.setGravity(0, 1); //重力の設定(下方向に1)
    box2d.createBounds(0, 0, ofGetWidth(), ofGetHeight()); //枠を追加
    box2d.setFPS(15.0); //Box2Dの演算の更新頻度を設定
    box2d.setIterations(5, 5); //インタラクションの精度を設定
    
    // 音声
    mySound.loadSound("yen.mp3"); //サウンドファイルの読込み
    sound1up.loadSound("1up.wav"); //サウンドファイルの読込み
    levelup.loadSound("levelup.wav"); //サウンドファイルの読込み
    mySound.setLoop(false); //ループ再生をONに
    sound1up.setLoop(false); //ループ再生をONに
    levelup.setLoop(false); //ループ再生をONに
    
    // テキスト
    font.loadFont("verdana.ttf", 20);
    yen.loadFont("verdana.ttf", 15);
    num = 0;
    
    
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    //Box2Dの物理演算を実行
    box2d.update();
    
    // 現在順番待ちのOSCメッセージがある間は受信を続ける
    while(receiver.hasWaitingMessages()){
        
        //次のメッセージを取得
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        // メッセージが /mouse/position ならマウスの位置を取得
        if (m.getAddress() == "/mouse/position"){
            remoteMouse.x = m.getArgAsInt32(0);
            remoteMouse.y = m.getArgAsInt32(1);
            
        }
        // メッセージが /mouse/button ならマウスボタンの状態を取得
        else if (m.getAddress() == "/mouse/button"){
            mouseButtonState = m.getArgAsString(0) ;
        }
        
        //OSCメッセージをそのままコンソールに出力
        //dumpOSC(m);
        printf("test");
        
        
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    time++;
    
    //ofSetColor(31, 63, 255); //描画色の設定
    //ofCircle(mouseX, mouseY, 40); //マウスの現在位置を中心に円を描く
    //circlesに格納された全ての円を描画(イテレータ使用)
    for(vector <ofxBox2dCircle *>::iterator it = circles.begin(); it != circles.end(); ++it) {
        (*it)->draw();
    }
    
    //Box2Dで生成された図形を描画
    box2d.draw();
    
    //描画した円(ofxBox2dCircle)の数だけ、円を描画する
    /*
     for(int i=0; i<circles.size(); i++) {
     circles[i].draw();
     }
     */
    
    
    
    // マウスボタンが押されていたら
    if (mouseButtonState == "down") {
        
        //time2 = time;
        
        //if(mouseButtonState == "up"){
        
        //font.drawString(ofToString(ofGetMinutes()), 400, 50);
        
        
        //ofxBox2dCircle(円)クラスをインスタンス化
        ofxBox2dCircle* circle = new ofxBox2dCircle();
        
        ofPoint pos; //タッチした座標を格納するための変数
        pos.set(ofRandom(10, 300), 10); //タッチした場所を取得
        
        //半径を設定
        float r = ofRandom(10, 30);
        //物理パラメータを設定(重さ、反発力、摩擦力)
        circle->setPhysics(1.0, 0.8, 0.5);
        //マウスの位置に円を設定
        circle->setup(box2d.getWorld(), pos.x, pos.y, r);
        //生成した円をcirclesに追加
        circles.push_back(circle);
        
        //サウンド再生開始
        mySound.play();
        
        mouseButtonState = "up";
        
        // コイン枚数カウント
        num = num + 10;
        
        // 1up処理
        if((num % 100) == 0){
            sound1up.play();
        }
        // LevelUP処理
        if((num % 1000) == 0){
            levelup.play();
        }
        
        
        
        //画面をタッチすると新たに円(ofxBox2dCircle)を追加する
        /*
         ofxBox2dCircle c; //新規にofxBox2dCircleをインスタンス化
         c.setPhysics(1.0, 0.8, 0.3); //物理特性を設定
         */
        //Box2Dの世界に追加
        /*
         c.setup(box2d.getWorld(),
         pos.x,
         pos.y,
         ofRandom(10, 30));
         circles.push_back(c); //Vector、circlesに作成した円を追加
         mySound.play(); //サウンド再生開始
         num = num+10; // コイン枚数カウント
         */
        //}
        
    }
    
    // テキスト
    ofSetColor(255, 255, 255);
    font.drawString(ofToString(num), 50, 50);
    font.drawString("yen", 140, 50);
    //font.drawString(ofToString(time), 400, 50);
    
    
}

//--------------------------------------------------------------
void testApp::exit(){
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'z'){
        
        //circles配列の全ての要素を消去する(イテレータ使用)
        for(vector <ofxBox2dCircle *>::iterator it = circles.begin(); it != circles.end();){
            (*it)->destroyShape();
            delete *it;
            it = circles.erase(it);
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
    /*
     //画面をタッチすると新たに円(ofxBox2dCircle)を追加する
     ofPoint pos; //タッチした座標を格納するための変数
     pos.set(mouseX, mouseY); //タッチした場所を取得
     ofxBox2dCircle c; //新規にofxBox2dCircleをインスタンス化
     c.setPhysics(1.0, 0.8, 0.3); //物理特性を設定
     //Box2Dの世界に追加
     c.setup(box2d.getWorld(),
     pos.x,
     10,
     ofRandom(10, 30));
     circles.push_back(c); //Vector、circlesに作成した円を追加
     
     // 100個以上になったら先頭のofxBox2dCircleを削除
     if(circles.size() > 100){
     //vector <ofxBox2dCircle *>::iterator const it = circles.begin();
     
     //(*it)->destroy();
     //delete *it;
     //it = circles.erase(it);
     }
     */
    
    //ofxBox2dCircle(円)クラスをインスタンス化
    ofxBox2dCircle* circle = new ofxBox2dCircle();
    
    ofPoint pos; //タッチした座標を格納するための変数
    pos.set(mouseX, mouseY); //タッチした場所を取得
    
    //半径を設定
    float r = ofRandom(10, 30);
    //物理パラメータを設定(重さ、反発力、摩擦力)
    circle->setPhysics(1.0, 0.8, 0.5);
    //マウスの位置に円を設定
    circle->setup(box2d.getWorld(), pos.x, pos.y, r);
    //生成した円をcirclesに追加
    circles.push_back(circle);
    
    
    mySound.play(); //サウンド再生開始
    
    num = num+10; // コイン枚数カウント
    
    if((num % 100) == 0){
        sound1up.play();
    }
    // LevelUP処理
    if((num % 1000) == 0){
        levelup.play();
    }
    
    
    
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}