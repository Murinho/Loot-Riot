#include "Escenas.h"
#include "AudioEngine.h"

USING_NS_CC;

static int gameAudioProfile;

Scene* MenuPrincipal::createScene()
{
    return MenuPrincipal::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
// on "init" you need to initialize your instance.
bool MenuPrincipal::init()
{
    srand(time(NULL));
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }
   
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(MenuPrincipal::menuCloseCallback, this));
    float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
    float y = origin.y + closeItem->getContentSize().height / 2;
    closeItem->setPosition(Point(x, y));

    this->addChild(closeItem);

    auto menu_backg = Sprite::create("menu_background.png");
    menu_backg->setPosition(240, 160);
    this->addChild(menu_backg, 0);

    auto menu_item1 = MenuItemFont::create("Play Level 1", CC_CALLBACK_1(MenuPrincipal::PlayLevel1, this));
    auto menu_item2 = MenuItemFont::create("Play Level 2", CC_CALLBACK_1(MenuPrincipal::PlayLevel2, this));
    auto menu_item3 = MenuItemFont::create("Play Level 3", CC_CALLBACK_1(MenuPrincipal::PlayLevel3, this));
    /*menu_item1->setPosition(Point(visibleSize.width / 1.3, (visibleSize.height / 5) * 4));
    menu_item2->setPosition(Point(visibleSize.width / 1.3, (visibleSize.height / 5)*3));
    menu_item3->setPosition(Point(visibleSize.width / 1.3, (visibleSize.height / 5)*2)); */
    auto title = Label::createWithTTF("Loot Riot", "fonts/Marker Felt.ttf", 64);
    title->setPosition(Point(visibleSize.width / 1.3, 270));
    title->setColor(Color3B(0, 0, 0));
    this->addChild(title, 1);
    menu_item1->setPosition(Point(visibleSize.width / 1.3, 180));
    menu_item1->setFontName("Marker Felt.ttf");
    menu_item1->setColor(Color3B(0, 0, 0));
    menu_item2->setPosition(Point(visibleSize.width / 1.3, 120));
    menu_item2->setFontName("Marker Felt.ttf"); 
    menu_item2->setColor(Color3B(0, 0, 0));
    menu_item3->setPosition(Point(visibleSize.width / 1.3, 60));
    menu_item3->setFontName("Marker Felt.ttf");
    menu_item3->setColor(Color3B(0, 0, 0));

    auto menu = Menu::create(menu_item1, menu_item2, menu_item3, NULL);
    menu->setPosition(Point(0, 0));
    this->addChild(menu, 1);

    gameAudioProfile = cocos2d::AudioEngine::INVALID_AUDIO_ID;

    if (gameAudioProfile == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
        gameAudioProfile = cocos2d::AudioEngine::play2d("music/menu_song.mp3", true, 0.05f);
    }

    return true;
}


void MenuPrincipal::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void MenuPrincipal::PlayLevel1(cocos2d::Ref* pSender) {
    CCLOG("Level 1");
    cocos2d::AudioEngine::stopAll();
    auto scene = Level1::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene, Color3B(0, 0, 0)));
}

void MenuPrincipal::PlayLevel2(cocos2d::Ref* pSender) {
    CCLOG("Level 2");
    cocos2d::AudioEngine::stopAll();
    auto scene = Level2::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene, Color3B(0, 0, 0)));
}

void MenuPrincipal::PlayLevel3(cocos2d::Ref* pSender) {
    CCLOG("Level 3");
    cocos2d::AudioEngine::stopAll();
    auto scene = Level3::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene, Color3B(0, 0, 0)));
}


