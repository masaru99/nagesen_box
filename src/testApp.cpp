#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    //背景
    ofSetBackgroundAuto(true);
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    //ofSetFullscreen(true);

    int w = 1300;
    int h = ofGetHeight() - 40;
    int w2 = 240;
    int h2 = 60;
    
    //画像の読み込み
    myImage.loadImage("10yen.png"); // お金
    bgImage.loadImage("bg.png"); // 背景
    bgYenImage.loadImage("yen_frame.png"); // 金額の背景
    manualImage.loadImage("manual.png"); // マニュアル
    
    //指定したポートで接続
    receiver.setup( PORT );
    //値を初期化
    mouseX = 0;
    mouseY = 0;
    mouseButtonState = "";
    
    //Box2Dの基本設定
    box2d.init(); //初期化
    box2d.registerGrabbing(); //物体をつかめるように設定
    box2d.setGravity(0, 1.2); // x, y方向の重力を設定(下方向に1)
    box2d.createBounds(0, 0, 1390, ofGetHeight()); //枠を追加
    box2d.setFPS(15); //Box2Dの演算の更新頻度を設定
    box2d.setIterations(5, 5); //インタラクションの精度を設定
    
    // 音声
    mySound.loadSound("yen.mp3"); //サウンドファイルの読込み
    sound1up.loadSound("1up.wav"); //サウンドファイルの読込み
    levelup.loadSound("levelup.wav"); //サウンドファイルの読込み
    mySound.setLoop(false); //ループ再生をOFFに
    sound1up.setLoop(false); //ループ再生をOFFに
    levelup.setLoop(false); //ループ再生をOFFに
    
    // テキスト
    font.loadFont("tare.ttf", 40);
    yen.loadFont("tare.ttf", 15, true, true);
    yenString = "円";
    //manual.loadFont("tare.ttf", 15, true, true);
    //manualString = "[p] -- プロジェクションマッピングモード<br>てすと";
    
    p1 = ofPoint(100, 200);
    p2 = ofPoint(200, 300);
    p3 = ofPoint(300, 400);
    
    
    num = 0;
    
    // BOX（実際の表示）
    fbo.allocate( 1390, ofGetHeight());
    
    // 金額（実際の表示）
    fbo2.allocate( w2, h2 );
    
    warper.setSourceRect( ofRectangle( 0, 0, w, h ) );
    warper.setTopLeftCornerPosition( ofPoint( x, y ) );
    warper.setTopRightCornerPosition( ofPoint( x + w, y ) );
    warper.setBottomLeftCornerPosition( ofPoint( x, y + h ) );
    warper.setBottomRightCornerPosition( ofPoint( x + w, y + h ) );
    warper.setup();
    
    warper2.setSourceRect( ofRectangle( 0, 0, w2, h2 ) );
    warper2.setTopLeftCornerPosition( ofPoint( x2, y2 ) );
    warper2.setTopRightCornerPosition( ofPoint( x2 + w2, y2 ) );
    warper2.setBottomLeftCornerPosition( ofPoint( x2, y2 + h2 ) );
    warper2.setBottomRightCornerPosition( ofPoint( x2 + w2, y2 + h2 ) );
    warper2.setup();
    
    
    // ラインを消す
    warper.toggleShow();
    warper2.toggleShow();

    //秒の取得
    s = ofGetSeconds();
    
    // jsonを読みにいく
    //if(time > 30) {
        bool result = response.open("http://www.exeweb.net/nagesen/server/rooms/fetch/1");
        if(!result) {
            //cout << "faild to get JSON data!" << endl;
        } else {
            cout << response.getRawString() << endl;
        }
    //}

}

//--------------------------------------------------------------
void testApp::update(){
    //etime = ofToInt(ofGetElapsedTimeMillis());
    //float etime2 = ofToFloat(etime);
    //ofToFloat
    if((ofGetElapsedTimeMillis() % 1000) <= 1) {
        rtime = 0;
    } else {
        rtime++;
    }


    //Box2Dの物理演算を実行
    box2d.update();
    
    // 現在順番待ちのOSCメッセージがある間は受信を続ける
    while(receiver.hasWaitingMessages()){
        
        //num = num+10;
        
        //次のメッセージを取得
        ofxOscMessage m;
        receiver.getNextMessage(&m);

        //dumpOSC(m);

        // メッセージが /mouse/position ならマウスの位置を取得
        if (m.getAddress() == "/mouse/position"){
            remoteMouse.x = m.getArgAsInt32(0);
            remoteMouse.y = m.getArgAsInt32(1);
        }
        
        // メッセージが /mouse/button ならマウスボタンの状態を取得
        else if (m.getAddress() == "/mouse/button"){
            mouseButtonState = m.getArgAsString(0) ;
        }
    }
}


//--------------------------------------------------------------
void testApp::draw(){
    
    time++;
    
    // wrapする
    fbo.begin();
    {
        bgImage.draw(0,0);
        bgImage.draw(1000,0);
        bgImage.draw(1000,1000);
        bgImage.draw(1000,1000);
        manualImage.draw(ofGetWidth() * 0.25, ofGetHeight() * 0.25);

        //circlesに格納された全ての円を描画(イテレータ使用)
        for(vector <ofxBox2dCircle *>::iterator it = circles.begin(); it != circles.end(); ++it) {
        (*it)->draw();
        }
        

        // 定期的に銭カウントをサーバーに見に行く
        if(rtime == 0){
            // サーバーに見に行く
            bool result = response.open("http://www.exeweb.net/nagesen/server/rooms/fetch/1");
            count = response["queues"]["count"].asFloat();
            
            // コイン投げる
            for(int i=0; i < count; i++){
                //cout << response["queues"]["coins"][i]["type"].asString() << endl;
                type = response["queues"]["coins"][i]["type"].asString();
                nageru();
            }
            // リセット
            response.open("http://www.exeweb.net/nagesen/server/rooms/reset/1");
            type = "";
        }
        
        // デバッグ
        ofDrawBitmapString(ofToString(count), 300 , 100);
        ofDrawBitmapString(ofToString(rtime), 300 , 140);
        // ofDrawBitmapString(ofToString(ofGetElapsedTimeMillis()), 300 , 140);
        // ofDrawBitmapString(ofToString(ofGetElapsedTimeMillis() % 3000), 300 , 160);

    }
    fbo.end();
    
    
    // 銭を投げる
    nageru();
    
    
        //======================== get our quad warp matrix.
        
        ofMatrix4x4 mat = warper.getMatrix();
        ofMatrix4x4 mat2 = warper2.getMatrix();

        //======================== use the matrix to transform our fbo.
        
        glPushMatrix();
        glMultMatrixf( mat.getPtr() );
        {
            fbo.draw( 0, 0 );
        }
        glPopMatrix();

    
        glPushMatrix();
        glMultMatrixf( mat2.getPtr() );
        {
            fbo2.draw( 0, 0 );
        }
        glPopMatrix();
    
        //======================== use the matrix to transform points.
        

        
    // wrapする
    fbo2.begin();
    {
        //ofBackground(0, 0, 0, 0.6);
        bgYenImage.draw(0,0);
        
        // テキスト
        ofSetColor(60, 30, 5);
        yen.drawString(yenString, 160, 50); // 円
        font.drawString(ofToString(num), 30, 50); // 数字
    }
    fbo2.end();

    // wrap描画
    warper2.draw();
    warper.draw();
}


//--------------------------------------------------------------
void testApp::nageru() {
    
    mouseButtonState = type;
    //cout << "start" << endl;
    //cout << type << endl;
    //cout << "end" << endl;
    // マウスボタンが押されていたら
    if (
        mouseButtonState == "y10" ||
        mouseButtonState == "y100" ||
        mouseButtonState == "aoriyen" ||
        mouseButtonState == "down"
        
    ) {
        if(nage == true){
            //ofxBox2dCircle(円)クラスをインスタンス化
            ofxBox2dCircle* circle = new ofxBox2dCircle();
            
            ofPoint pos; //タッチした座標を格納するための変数
            pos.set(ofRandom(10, 300), 10); //タッチした場所を取得
            
            //半径を設定
            float r = ofRandom(10, 30);
            //物理パラメータを設定(重さ、反発力、摩擦力)
            circle->setPhysics(3.0, 0.8, 0.8);
            
            //マウスの位置に円を設定
            //circle->setup(box2d.getWorld(), pos.x, pos.y, r);
            circle->setup(box2d.getWorld(), ofRandom(0, 1390), pos.y, r, false, mouseButtonState);
            
            //生成した円をcirclesに追加
            circles.push_back(circle);
            
            //サウンド再生開始
            mySound.play();
            
            // お金
            if(mouseButtonState == "y100") {
                postyen = 100;
            } else if(mouseButtonState == "y10") {
                postyen = 10;
            } else if(mouseButtonState == "aoriyen") {
                postyen = 0;
            } else {
                postyen = 10;
            } // if
            
            // コイン枚数カウント
            num = num + postyen;
            
            // 待ち受けに戻す
            mouseButtonState = "up";
            
            // 1up処理
            //up1Num += postyen;
            if((num % 1000) < 100){
                if(mouseButtonState != "aoriyen") {
                    if(up1){
                        if(num != 10){ sound1up.play(); }
                        up1 = false;
                    }
                }
            } else {
                up1 = true;
            }
        }
    }
}


//--------------------------------------------------------------
void testApp::exit(){
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    // プロジェクションマッピング用のライン
    if( key == 'p' ){
        warper.toggleShow();
        warper2.toggleShow();
    }
    // 画面フルスクリーン
    if( key == 'f' ){
        ofToggleFullscreen();
    }

    // 投げ受付
    if( key == 'n' ){
        if(nage == false){
            nage = true;
            manualImage.setAnchorPercent(5, 5);

        } else {
            nage = false;
            manualImage.setAnchorPercent(0.05, 0.05);

        }
    }
    
    // 見えているコインだけリセット
    if (key == 'x'){
        //circles配列の全ての要素を消去する(イテレータ使用)
        for(vector <ofxBox2dCircle *>::iterator it = circles.begin(); it != circles.end();){
            (*it)->destroyShape();
            delete *it;
            it = circles.erase(it);
        }
    }
    
    // 得点もリセット
    if (key == 'z'){
        //circles配列の全ての要素を消去する(イテレータ使用)
        for(vector <ofxBox2dCircle *>::iterator it = circles.begin(); it != circles.end();){
            (*it)->destroyShape();
            delete *it;
            it = circles.erase(it);
        }
        // 得点を0に
        num = 0;
    }
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
    
    // 銭を投げる
    mouseButtonState = "y10";
    nageru();
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