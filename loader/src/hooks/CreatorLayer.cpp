#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;

class $modify(CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) {
            return false;
        }

        if (auto menu = this->getChildByID("creator-buttons-menu")) {
            auto button = CCMenuItemSpriteExtra::create(
                CCSprite::create("GE_statsBtn.png"_spr),
                this,
                menu_selector(CreatorLayer::onBack)
            );

            button->setPosition(0.0f, 0.0f);
            button->setScale(.8f);

            menu->addChild(button);
        }

        return true;
    }
};