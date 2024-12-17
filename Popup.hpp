#ifndef POPUP_HPP
#define POPUP_HPP

#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace gdaddons {
    class Popup : public CCLayer {
    public:
        static Popup* create(const std::string& title, float width, float height) {
            auto ret = new Popup();
            if (ret && ret->init(title, width, height)) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }

        void show() {
            auto scene = CCDirector::sharedDirector()->getRunningScene();
            this->setZOrder(scene->getHighestChildZ() + 1);
            scene->addChild(this);

            auto contents = this->getChildByID("contents");
            if (!contents) {
                log::debug("Error: 'contents' node not found.");
                return;
            }

            contents->setScale(0);

            auto scaleTo = CCScaleTo::create(0.2f, 1.0f);
            auto bounce = CCEaseBackOut::create(scaleTo);
            contents->runAction(bounce);
        }

        bool init(const std::string& title, float width, float height) {
            if (!CCLayer::init()) {
                return false;
            }

            // Set the touch priority for the popup itself
            setTouchPriority(10);

            auto winSize = CCDirector::sharedDirector()->getWinSize();

            // Create a background layer
            auto fadeBG = CCLayerColor::create(ccc4(0, 0, 0, 150));
            fadeBG->setContentSize(winSize);
            fadeBG->setPosition({0, 0});
            this->addChild(fadeBG, 0);

            // Create the contents layer for the popup
            auto contents = CCLayer::create();
            contents->setContentSize({width, height});
            contents->setAnchorPoint(ccp(0.5f, 0.5f));
            contents->setPosition(ccp(winSize.width / 2, winSize.height / 2));
            contents->setID("contents");
            contents->ignoreAnchorPointForPosition(false);
            this->addChild(contents, 1);

            // Create the popup background
            auto bg = CCScale9Sprite::create("GJ_square01.png");
            bg->setContentSize({width, height});
            bg->setAnchorPoint(ccp(0.5f, 0.5f));
            bg->setPosition(ccp(width / 2, height / 2));
            contents->addChild(bg);

            // Title label
            auto titleLabel = CCLabelBMFont::create(title.c_str(), "bigFont.fnt");
            titleLabel->setAnchorPoint(ccp(0.5f, 1.0f));
            titleLabel->setPosition(ccp(width / 2, height - 10));
            contents->addChild(titleLabel);

            // Create the close button image and action
            auto closeBtnImage = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
            auto closeBtn = CCMenuItemSpriteExtra::create(
                closeBtnImage,
                this,
                menu_selector(Popup::onClose)
            );
            closeBtn->setPosition({width - 20, height - 20});  // Adjust position as needed

            // Create a menu and add the close button to it
            auto menu = CCMenu::createWithItem(closeBtn);
            menu->setPosition(CCPointZero);  // This ensures the menu itself has no offset
            contents->addChild(menu);

            // Set the touch priority for the close button menu
            menu->setHandlerPriority(-128); // This is the priority used by CCMenu (adjust as needed)

            return true;
        }

        void onClose(CCObject*) {
            this->removeFromParent();
        }
    };
}

#endif // POPUP_HPP
