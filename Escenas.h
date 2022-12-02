#include "cocos2d.h"
#include <CCScheduler.h>

class MenuPrincipal : public cocos2d::Scene {
	public:
        static cocos2d::Scene* createScene();
        virtual bool init();
        void menuCloseCallback(cocos2d::Ref* pSender);
        void PlayLevel1(cocos2d::Ref* pSender);
        void PlayLevel2(cocos2d::Ref* pSender);
        void PlayLevel3(cocos2d::Ref* pSender);
        CREATE_FUNC(MenuPrincipal);
};

class Level1 : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(Level1);

    void timerMethod(float dt);
    void spriteGen();
    void coinGen();
    void boxGen();
    void heartGen();
    void boxBreaks(cocos2d::Point p);
    cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);
    cocos2d::Sprite* mole;
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    float tempo;
    int vidas;
    int money;
    const int tope_coins_grid = 3;
    const int tope_boxes = 3;
};

class Level2 : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(Level2);

    void timerMethod(float dt);
    void spriteGen();
    void coinGen();
    void boxGen();
    void heartGen();
    void boxBreaks(cocos2d::Point p);
    cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);
    cocos2d::Sprite* mole;
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    float tempo;
    int vidas;
    int money;
    const int tope_coins_grid = 3;
    const int tope_boxes = 3;
};

class Level3 : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(Level3);

    void timerMethod(float dt);
    void spriteGen();
    void coinGen();
    void boxGen();
    void heartGen();
    void boxBreaks(cocos2d::Point p);
    cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);
    cocos2d::Sprite* mole;
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    float tempo;
    int vidas;
    int money;
    const int tope_coins_grid = 4;
    const int tope_boxes = 4;
};

