#pragma once
#include <SFML/Graphics.hpp>
#include "SoundFX.hpp"
#include "ExplosionInfo.hpp"
#include "ExplosionEffect.hpp"
#include "Shader.hpp"

class EffectOverlay {
public:
    int load() {
        for(int i = 0; i < PREALLOCATE_EFFECTS_COUNT; i++) {
            explosion_effects.at(i).load();
        }
        return true;
    }

    void render(sf::RenderTarget &targ) {
        for(auto &eff : explosion_effects) {
            eff.render(targ);
        }
    }

    void update(const sf::Vector2f &view_pos) {
        for(auto &eff : explosion_effects) {
            eff.update();
        }
    }

    void effect_explosion(const ExplosionInfo& exInfo) {
        for(auto &eff : explosion_effects) {
            if(!eff.getExploding()) {
                eff.explode(exInfo);
                break;
            }
        }
    }

private:

    std::array<ExplosionEffect, PREALLOCATE_EFFECTS_COUNT> explosion_effects;

};