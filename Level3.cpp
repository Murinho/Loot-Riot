#include "Escenas.h"
#include "AudioEngine.h"
#include <CCScheduler.h>
#include <iomanip>
#include <sstream>
#include <bits/stdc++.h>
#include "cocos2d.h"

USING_NS_CC;

static int currfil = 7;
static int currcol = 7;
static int moved = 1;
static int coingenamou = 1;
static int coins_in_grid = 0;
static int boxamou = 0;
static int boxgens = 1;
static Label* filalab;
static Label* columnalab;
static Label* timerr;
static Label* lives;
static Label* currmoney;
static Label* level_title;
static Sprite* player;

static int gameAudioProfile;
static int rows[14];
static int cols[14];
static bool newheart;
static Point heartcoords;
static int gridd[14][14];
static std::map <float, int> rowscords;
static std::map <float, int> colscords;

Scene* Level3::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));

    auto layer = Level3::create();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    scene->addChild(layer);

    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

static float randomizer(int minic, int maxic) {
    float num = rand() % (maxic - minic + 1) + minic;
    return num;
}

static void initGrid() {
    float stf = 282;
    float stc = 118;
    for (int i = 1; i <= 13; i++) {
        rows[i] = stf;
        cols[i] = stc;
        rowscords[stf] = i;
        colscords[stc] = i;
        stf -= 20.5;
        stc += 20.5;
    }
    for (int i = 1; i <= 13; i++) {
        for (int j = 1; j <= 13; j++) gridd[i][j] = 0;
    }
}

// on "init" you need to initialize your instance
bool Level3::init()
{
    srand(time(NULL));
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(Level3::menuCloseCallback, this));
    float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
    float y = origin.y + closeItem->getContentSize().height / 2;
    closeItem->setPosition(Point(x, y));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
  
    level_title = Label::createWithTTF("Loot Riot", "fonts/Marker Felt.ttf", 20);
    level_title->setPosition(120, 305);
    this->addChild(level_title, 3);
    timerr = Label::createWithTTF("Timer: ", "fonts/Marker Felt.ttf", 14);
    timerr->setPosition(230, 305);
    this->addChild(timerr, 3);
    //timerr->setVisible(false);
    lives = Label::createWithTTF("Vidas: 3 ", "fonts/Marker Felt.ttf", 14);
    lives->setPosition(300, 305);
    this->addChild(lives, 1);
    currmoney = Label::createWithTTF("$: 0", "fonts/Marker Felt.ttf", 14);
    currmoney->setPosition(365, 305);
    this->addChild(currmoney, 3);

    tempo = money = boxamou = 0;
    vidas = 3;
    currfil = currcol = 7;
    moved = coingenamou = boxgens = 1;
    newheart = false;
    initGrid();

    //this->schedule(CC_SCHEDULE_SELECTOR(Level2::timerMethod), 0.25f);


    gameAudioProfile = cocos2d::AudioEngine::INVALID_AUDIO_ID;

    if (gameAudioProfile == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
        gameAudioProfile = cocos2d::AudioEngine::play2d("music/jungle_music.mp3", true, 1.0f);
    }

    //Inicializar los sprites:
    player = Sprite::create("player_cross.png");
    player->setPosition(cols[7], rows[7]);
    player->setTag(10); //tag 10.
    auto fisica = PhysicsBody::createCircle(player->getContentSize().width / 2);
    fisica->setCategoryBitmask(0x01);
    fisica->setCollisionBitmask(0x02);
    fisica->setContactTestBitmask(0xFFFFFFFF);
    fisica->setDynamic(false);
    player->addComponent(fisica);
    this->addChild(player, 1);
    auto player_spin = RepeatForever::create(RotateBy::create(0.5, 180));
    player->runAction(player_spin);
    auto grid = Sprite::create("soil_grass_grid.png");
    grid->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(grid, 0);
    auto bamboos = Sprite::create("bamboos_grid.png");
    bamboos->setPosition(240,160);
    this->addChild(bamboos,2);

    this->schedule(CC_SCHEDULE_SELECTOR(Level3::timerMethod), 0.25f);

    //Create the ball destroyers:
    auto north_destroyer = Sprite::create("destroy_objects_hor.png");
    north_destroyer->setPosition(240, 350);
    north_destroyer->setTag(30); //tag 30.
    auto norfis = PhysicsBody::createBox(north_destroyer->getContentSize());
    norfis->setCategoryBitmask(0x01);
    norfis->setCollisionBitmask(0x02);
    norfis->setContactTestBitmask(0xFFFFFFFF);
    norfis->setDynamic(false);
    north_destroyer->addComponent(norfis);
    this->addChild(north_destroyer, 1);

    auto south_destroyer = Sprite::create("destroy_objects_hor.png");
    south_destroyer->setPosition(240, -20);
    south_destroyer->setTag(31); //tag 30.
    auto soufis = PhysicsBody::createBox(south_destroyer->getContentSize());
    soufis->setCategoryBitmask(0x01);
    soufis->setCollisionBitmask(0x02);
    soufis->setContactTestBitmask(0xFFFFFFFF);
    soufis->setDynamic(false);
    south_destroyer->addComponent(soufis);
    this->addChild(south_destroyer, 1);

    auto east_destroyer = Sprite::create("destroy_objects_vert.png");
    east_destroyer->setPosition(420, 160);
    east_destroyer->setTag(32);
    auto eastfis = PhysicsBody::createBox(east_destroyer->getContentSize());
    eastfis->setCategoryBitmask(0x01);
    eastfis->setCollisionBitmask(0x02);
    eastfis->setContactTestBitmask(0xFFFFFFFF);
    eastfis->setDynamic(false);
    east_destroyer->addComponent(eastfis);
    this->addChild(east_destroyer, 1);

    auto west_destroyer = Sprite::create("destroy_objects_vert.png");
    west_destroyer->setPosition(10, 160);
    west_destroyer->setTag(33);
    auto westfis = PhysicsBody::createBox(west_destroyer->getContentSize());
    westfis->setCategoryBitmask(0x01);
    westfis->setCollisionBitmask(0x02);
    westfis->setContactTestBitmask(0xFFFFFFFF);
    westfis->setDynamic(false);
    west_destroyer->addComponent(westfis);
    this->addChild(west_destroyer, 1);

    //Keys listeners:
    auto eventListener = EventListenerKeyboard::create();

    eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {

        Vec2 loc = event->getCurrentTarget()->getPosition();
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_A:
            if (currcol > 1) {
                //event->getCurrentTarget()->setPosition(loc.x - 20.5, loc.y);

                auto movleft = MoveBy::create(0.15, Point(-20.5, 0));
                player->runAction(movleft);
                currcol--;
                //columnalab->setString("Columna: " + std::to_string(currcol));
            }
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_D:
            if (currcol < 13) {
                //event->getCurrentTarget()->setPosition(loc.x + 20.5, loc.y);
                auto movright = MoveBy::create(0.15, Point(20.5, 0));
                player->runAction(movright);
                currcol++;

                //columnalab->setString("Columna: " + std::to_string(currcol));
            }
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_W:
            if (currfil > 1) {
                //event->getCurrentTarget()->setPosition(loc.x, loc.y + 20.5);
                auto movup = MoveBy::create(0.15, Point(0, 20.5));
                player->runAction(movup);
                currfil--;
                //filalab->setString("Fila: " + std::to_string(currfil));
            }
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_S:
            if (currfil < 13) {
                //event->getCurrentTarget()->setPosition(loc.x, loc.y - 20.5);
                auto movdown = MoveBy::create(0.15, Point(0, -20.5));
                player->runAction(movdown);
                currfil++;
                //filalab->setString("Fila: " + std::to_string(currfil));
            }
            break;
        }
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, player);


    //Contact listener:

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Level3::onContactBegin, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    return true;
}

//Insert images, fonts, and material to: C:\Users\Acer\Desktop\LootRiot\proj.win32\bin\LootRiot\Debug\Resources

void Level3::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void Level3::timerMethod(float dt) {
    tempo += dt;
    if (newheart) {
        heartGen();
    }
    if (tempo == 0.50 * moved) {
        spriteGen();
        moved++;
    }
    if (tempo == 2.00 * coingenamou) {
        coinGen();
        coingenamou++;
    }
    if (tempo == 4.00 * boxgens) {
        boxGen();
        boxgens++;
    }
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << tempo;
    std::string s = stream.str();
    timerr->setString("Timer: " + s);
}

void Level3::coinGen() {
    if (coins_in_grid < tope_coins_grid) {
        int strow, stcol;
        while (true) {
            strow = randomizer(1, 13);
            stcol = randomizer(1, 13);
            if (gridd[strow][stcol] == 0) break;
        }
        gridd[strow][stcol] = 1;
        auto coin = Sprite::create("norm_coin.png");
        coin->setPosition(cols[strow], rows[stcol]);
        auto coinfis = PhysicsBody::createCircle(coin->getContentSize().width / 2);
        coinfis->setCategoryBitmask(0x03);
        coinfis->setCollisionBitmask(0x01);
        coinfis->setContactTestBitmask(0xFFFFFFFF);
        coinfis->setDynamic(false);
        coin->addComponent(coinfis);
        coin->setTag(40);
        this->addChild(coin, 1);
        coins_in_grid++;

        auto sfx = cocos2d::AudioEngine::INVALID_AUDIO_ID;

        if (sfx == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
            sfx = cocos2d::AudioEngine::play2d("music/pop_item_sfx.mp3", false, 4.0f);
        }
    }
}

void Level3::spriteGen() {

    int startp = randomizer(1, 4);
    auto ball = Sprite::create("can_ball.png");
    auto rotacion = RepeatForever::create(RotateBy::create(1, 360));
    if (startp == 1) { //start from north
        auto action = MoveBy::create(2.25, Point(0, -400));
        int indx = randomizer(1, 13);
        ball->setPosition(cols[indx], 320);
        auto fisica = PhysicsBody::createCircle(ball->getContentSize().width / 2);
        fisica->setCategoryBitmask(0x02);
        fisica->setCollisionBitmask(0x01);
        fisica->setContactTestBitmask(0xFFFFFFFF);
        fisica->setDynamic(false);
        ball->addComponent(fisica);
        ball->setTag(20);
        this->addChild(ball, 1);
        ball->runAction(action);
    }
    else if (startp == 2) { //start from west
        auto action = MoveBy::create(2.5, Point(400, 0));
        int indx = randomizer(1, 13);
        ball->setPosition(100, rows[indx]);
        auto fisica = PhysicsBody::createCircle(ball->getContentSize().width / 2);
        fisica->setCategoryBitmask(0x02);
        fisica->setCollisionBitmask(0x01);
        fisica->setContactTestBitmask(0xFFFFFFFF);
        fisica->setDynamic(false);
        ball->addComponent(fisica);
        ball->setTag(20);
        this->addChild(ball, 1);
        ball->runAction(action);
    }
    else if (startp == 3) { //start from east
        auto action = MoveBy::create(2.5, Point(-400, 0));
        int indx = randomizer(1, 13);
        ball->setPosition(400, rows[indx]);
        auto fisica = PhysicsBody::createCircle(ball->getContentSize().width / 2);
        fisica->setCategoryBitmask(0x02);
        fisica->setCollisionBitmask(0x01);
        fisica->setContactTestBitmask(0xFFFFFFFF);
        fisica->setDynamic(false);
        ball->addComponent(fisica);
        ball->setTag(20);
        this->addChild(ball, 1);
        ball->runAction(action);
    }
    else if (startp == 4) { //start from south.
        auto action = MoveBy::create(2.5, Point(0, 400));
        int indx = randomizer(1, 13);
        ball->setPosition(cols[indx], 0);
        auto fisica = PhysicsBody::createCircle(ball->getContentSize().width / 2);
        fisica->setCategoryBitmask(0x02);
        fisica->setCollisionBitmask(0x01);
        fisica->setContactTestBitmask(0xFFFFFFFF);
        fisica->setDynamic(false);
        ball->addComponent(fisica);
        ball->setTag(20);
        this->addChild(ball, 1);
        ball->runAction(action);
    }
    ball->runAction(rotacion);
}


bool Level3::onContactBegin(PhysicsContact& contact)
{

    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB)
    {
        if (nodeA->getTag() == 10 && nodeB->getTag() == 20)
        {
            nodeB->removeFromParentAndCleanup(true);
            vidas--;
            lives->setString("Vidas: " + std::to_string(vidas));

            auto sfx = cocos2d::AudioEngine::INVALID_AUDIO_ID;

            if (sfx == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
                sfx = cocos2d::AudioEngine::play2d("music/player_hit_sfx.mp3", false, 300.0f);
            }
        }
        else if (nodeA->getTag() == 20 && nodeB->getTag() == 10)
        {
            vidas--;
            nodeA->removeFromParentAndCleanup(true);
            lives->setString("Vidas: " + std::to_string(vidas));
            auto sfx = cocos2d::AudioEngine::INVALID_AUDIO_ID;

            if (sfx == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
                sfx = cocos2d::AudioEngine::play2d("music/player_hit_sfx.mp3", false, 300.0f);
            }
        }
        else if (30 <= nodeA->getTag() && nodeA->getTag() <= 33)
        {
            nodeB->removeFromParentAndCleanup(true);
        }
        else if (30 <= nodeB->getTag() && nodeB->getTag() <= 33) {
            nodeA->removeFromParentAndCleanup(true);
        }
        else if (nodeA->getTag() == 40 && nodeB->getTag() == 10)
        {
            gridd[currfil][currcol] = 0;
            money++;
            currmoney->setString("$: " + std::to_string(money));
            nodeA->removeFromParentAndCleanup(true);
            coins_in_grid--;
        }
        else if (nodeB->getTag() == 40 && nodeA->getTag() == 10) {
            gridd[currfil][currcol] = 0;
            money++;
            currmoney->setString("$: " + std::to_string(money));
            nodeB->removeFromParentAndCleanup(true);
            coins_in_grid--;
            auto sfx = cocos2d::AudioEngine::INVALID_AUDIO_ID;

            if (sfx == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
                sfx = cocos2d::AudioEngine::play2d("music/coin_sfx.mp3", false, 1.0f);
            }
        }
        else if (nodeA->getTag() == 50 && nodeB->getTag() == 20) { //Box breaks.
            heartcoords = nodeA->getPosition();
            nodeA->removeFromParentAndCleanup(true);
            nodeB->removeFromParentAndCleanup(true);
            boxamou--;
            newheart = true;
        }
        else if (nodeA->getTag() == 20 && nodeB->getTag() == 50) {
            Point boxcoords = nodeB->getPosition();
            heartcoords = nodeB->getPosition();
            nodeB->removeFromParentAndCleanup(true);
            nodeA->removeFromParentAndCleanup(true);
            boxBreaks(boxcoords);

        }
        else if (nodeA->getTag() == 60 && nodeB->getTag() == 10) {
            nodeA->removeFromParentAndCleanup(true);
            vidas++;
            lives->setString("Vidas: " + std::to_string(vidas));
            //filalab->setString("HD");
            gridd[currfil][currcol] = 0;
            auto sfx = cocos2d::AudioEngine::INVALID_AUDIO_ID;

            if (sfx == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
                sfx = cocos2d::AudioEngine::play2d("music/extra_life_sfx.mp3", false, 4.0f);
            }
        }
        else if (nodeB->getTag() == 60 && nodeA->getTag() == 10) {
            nodeB->removeFromParentAndCleanup(true);
            vidas++;
            lives->setString("Vidas: " + std::to_string(vidas));
            //filalab->setString("HD");
            gridd[currfil][currcol] = 0;
            auto sfx = cocos2d::AudioEngine::INVALID_AUDIO_ID;

            if (sfx == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
                sfx = cocos2d::AudioEngine::play2d("music/extra_life_sfx.mp3", false, 4.0f);
            }
        }
        if (vidas == 0) {
            cocos2d::AudioEngine::stopAll();
            auto scene = MenuPrincipal::createScene();
            Director::getInstance()->replaceScene(scene);
        }
    }
    //bodies can collide
    return true;
}

void Level3::boxGen() {
    if (boxamou < tope_boxes) {
        int strow, stcol;
        while (true) {
            strow = randomizer(1, 13);
            stcol = randomizer(1, 13);
            if (gridd[strow][stcol] == 0) break;
        }
        gridd[strow][stcol] = 50;
        auto box = Sprite::create("crate.png");
        box->setPosition(cols[strow], rows[stcol]);
        auto boxfis = PhysicsBody::createCircle(box->getContentSize().width / 2);
        boxfis->setCategoryBitmask(0x01);
        boxfis->setCollisionBitmask(0x02);
        boxfis->setContactTestBitmask(0xFFFFFFFF);
        boxfis->setDynamic(false);
        box->addComponent(boxfis);
        box->setTag(50);
        this->addChild(box, 1);
        boxamou++;

        auto sfx = cocos2d::AudioEngine::INVALID_AUDIO_ID;

        if (sfx == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
            sfx = cocos2d::AudioEngine::play2d("music/pop_item_sfx.mp3", false, 4.0f);
        }
    }
}

void Level3::heartGen() {
    auto heart = Sprite::create("heart.png");
    gridd[rowscords[heartcoords.x]][rowscords[heartcoords.y]] = 1;
    heart->setPosition(heartcoords);
    heart->setTag(60);
    auto hfis = PhysicsBody::createCircle(heart->getContentSize().width / 3);
    hfis->setCategoryBitmask(0x03);
    hfis->setCollisionBitmask(0x01);
    hfis->setContactTestBitmask(0xFFFFFFFF);
    heart->addComponent(hfis);
    //filalab->setString("HC");
    newheart = false;
    this->addChild(heart, 1);
    auto pump = RepeatForever::create(Sequence::create(ScaleBy::create(1, 1.15), ScaleBy::create(1, 0.85), NULL));
    heart->runAction(pump);

    auto sfx = cocos2d::AudioEngine::INVALID_AUDIO_ID;

    if (sfx == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
        sfx = cocos2d::AudioEngine::play2d("music/pop_item_sfx.mp3", false, 4.0f);
    }
}

void Level3::boxBreaks(Point boxcoords) {
    gridd[rowscords[boxcoords.x]][colscords[boxcoords.y]] = 0;
    if (randomizer(1, 6) == 3) {
        newheart = true;
    }
    boxamou--;
    auto sfx = cocos2d::AudioEngine::INVALID_AUDIO_ID;

    if (sfx == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
        sfx = cocos2d::AudioEngine::play2d("music/box_breaks_sfx.mp3", false, 2.0f);
    }
}