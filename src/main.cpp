#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(ContadorPlayLayer, PlayLayer) {
	struct Fields {
		CCLabelBMFont* m_label = nullptr;
		int m_muertes = 0;
		bool m_contada = false;
	};

	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

		if (!Mod::get()->getSettingValue<bool>("mostrar-contador")) return true;

		auto label = CCLabelBMFont::create("Muertes: 0", "bigFont.fnt");
		label->setScale(0.5f);
		label->setAnchorPoint({0.f, 1.f});
		label->setPosition({8.f, CCDirector::get()->getWinSize().height - 8.f});
		label->setOpacity(static_cast<GLubyte>(Mod::get()->getSettingValue<int64_t>("opacidad")));
		label->setZOrder(100);
		this->addChild(label);

		m_fields->m_label = label;
		return true;
	}

	void destroyPlayer(PlayerObject* player, GameObject* object) {
		PlayLayer::destroyPlayer(player, object);

		// ignorar el objeto interno del anticheat
		if (object == m_anticheatSpike) return;

		auto f = m_fields.self();
		if (!f->m_label || f->m_contada) return;

		f->m_contada = true;
		f->m_muertes++;
		f->m_label->setString(fmt::format("Muertes: {}", f->m_muertes).c_str());
	}

	void resetLevel() {
		PlayLayer::resetLevel();
		m_fields->m_contada = false;
	}
};
