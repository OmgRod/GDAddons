#ifndef SWIPABLEBUTTON_HPP
#define SWIPABLEBUTTON_HPP

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace gdaddons {
    class SwipableButton : public CCLayer {
    public:
        static SwipableButton* create(CCSprite* buttonSprite) {
            auto ret = new SwipableButton();
            if (ret && ret->init(buttonSprite)) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }

        bool init(CCSprite* buttonSprite) {
            if (!CCLayer::init()) {
                return false;
            }

            // Enable touch handling
            setTouchEnabled(true);

            // Setting initial touch priority
            setTouchPriority(-1000);

            // Store the sprite and add it to the layer
            m_sprite = buttonSprite;
            m_sprite->setScale(0.4f);
            m_sprite->setPosition(getContentSize() / 2);
            addChild(m_sprite);

            // Set the size of the layer based on the sprite size
            setContentSize(m_sprite->getScaledContentSize());

            // Position the button at the center
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            setPosition(ccp(winSize.width / 2, winSize.height / 2));

            // Store the sprite's current position and scale
            m_startPos = getPosition();
            m_scale = getScale();

            return true;
        }

        void registerWithTouchDispatcher() {
            CCTouchDispatcher::get()->addTargetedDelegate(this, -9999999, true);
        }

        bool ccTouchBegan(CCTouch* touch, CCEvent* evt) {
            // If the button isn't visible, don't handle touch
            if (!isVisible()) return false;

            // Get the distance from the center of the button to the touch location
            diff = getPosition() - touch->getLocation();
            startPos = new CCPoint(touch->getLocation());

            // Check if the touch is within the bounds of the button
            if (getScaledContentSize().width / 2 < ccpDistance(m_sprite->getPosition(),
                convertToNodeSpace(touch->getLocation()))) {
                return false;
            }

            move = false;

            // Shrink the button when touched
            stopAllActions();
            runAction(CCEaseSineOut::create(CCScaleTo::create(0.3f, 0.8f)));
            return true;
        }

        void ccTouchEnded(CCTouch* touch, CCEvent* evt) {
            // Restore the button size when the touch ends
            stopAllActions();
            runAction(CCEaseSineOut::create(CCScaleTo::create(0.3f, 1.0f)));

            // If the button wasn't moved, trigger the onPress action
            if (!move) {
                onPress();
            }
        }

        void ccTouchMoved(CCTouch* touch, CCEvent* evt) {
            if (!move) {
                if (ccpDistance(*startPos, touch->getLocation()) > 3) {
                    move = true;
                }
            }

            if (move) {
                // Move the button with the touch
                auto pos = touch->getLocation() + diff;
                pos.x = std::clamp(pos.x, -getContentWidth() / 2, CCDirector::get()->getWinSize().width - getContentWidth() / 2);
                pos.y = std::clamp(pos.y, -getContentHeight() / 2, CCDirector::get()->getWinSize().height - getContentHeight() / 2);
                setPosition(pos);
            }
        }

        void onPress() {
            // Action when the button is pressed
            // Implement your functionality here, e.g., toggle a menu or perform an action
            FLAlertLayer::create("SwipableButton Pressed", "The button was pressed!", "OK")->show();
        }

    private:
        CCSprite* m_sprite = nullptr;   // The button's sprite
        CCPoint diff;                    // The difference between the button's position and touch
        CCPoint* startPos = nullptr;     // The initial touch position
        bool move = false;               // Flag to check if the button has been moved
        float m_scale = 1.0f;            // The button's scale factor
        CCPoint m_startPos;              // The button's start position
    };
}

#endif // SWIPABLEBUTTON_HPP
