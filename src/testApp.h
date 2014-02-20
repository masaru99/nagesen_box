#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOsc.h"

// OSCを受信するポート番号を設定
#define PORT 8000

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
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
    ofImage myImage;
    
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
    
    // カウント
    int num;
    
    // 文字
    ofTrueTypeFont font;
    ofTrueTypeFont yen;
    
    // 秒
    int time;
    int time2;
    int time3;
    
};
