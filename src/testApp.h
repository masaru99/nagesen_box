#pragma once

#include "ofMain.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxBox2d.h"
#include "ofxOsc.h"
#include "ofxQuadWarp.h"
#include "ofxJSONElement.h"


// OSCを受信するポート番号を設定
#define PORT 8999

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void nageru();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    void dumpOSC(ofxOscMessage m); //OSCメッセージを出力
    ofxBox2d box2d; //Box2Dのインスタンス
    vector <ofxBox2dCircle *> circles; //円形の物体のインスタンス
    //vector <ofxBox2dCircle>::iterator it;
    
    
    // 画像
    ofImage myImage; // お金の画像
    ofImage bgImage; // 背景画像
    ofImage bgYenImage; // 金額の背景画像
    ofImage manualImage; // マニュアル画像
    
    // OSCメッセージを受信するインスタンス
    ofxOscReceiver receiver;
    
    // 受信したマウス座標
    ofPoint remoteMouse;
    
    // 受信したマウスボタンの状態 ("up", "down")
    string mouseButtonState;
    
    // 音声
    ofSoundPlayer mySound; //ofSoundクラスをインスタンス化
    ofSoundPlayer sound1up; //ofSoundクラスをインスタンス化
    ofSoundPlayer levelup; //ofSoundクラスをインスタンス化
    
    // お金カウント
    int num;
    int up1Num = 0;
    bool up1 = true;
    
    // 文字
    ofTrueTypeFont font;
    //ofTrueTypeFont yen;

    // 円
    ofxTrueTypeFontUC yen;
    string yenString;
    ofPoint p1, p2, p3;
    ofRectangle rect1, rect2, rect3;
    vector<ofPath> characters;
    float postyen;
    
    
    // マニュアル
    ofxTrueTypeFontUC manual;
    string manualString;
    
    // 秒
    int time;
    int time2;
    int time3;
  
    // 投げ受付
    bool nage = false;
    
    // Json用
    ofxJSONElement response;

    // Json：枚数
    float count;
    string type;
    
    
    // Jsonからの銭枚数
    float etime;
    float rtime;
    
    // 秒
    int s;
    
    // プロジェクションマッピング
    ofxQuadWarp         warper;
    ofxQuadWarp         warper2;
    ofFbo               fbo;
    ofFbo               fbo2;
    ofPoint             points[ 10 ];
    ofPoint             points2[ 10 ];
    
    // wrap位置
    int x = 0;
    int y = 0;
    int x2 = 20;
    int y2 = 20;
};
