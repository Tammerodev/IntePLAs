#include "Jetpack.hpp"

void Jetpack::update(World &world, const sf::Vector2f &mousePos, const sf::Vector2f &pos, const float dt, Player &player)  {
    isEnabled = Controls::use();
    thrust = Controls::useUI();

    gun_spr.setPosition(pos.x + 9, pos.y + 13);

    if(isEnabled) {
        if(thrust == 0) {
            if(player.getPhysicsComponent().velocity.y < 0.f)  
                player.getPhysicsComponent().velocity.y += 0.1;
            else if(player.getPhysicsComponent().velocity.y > 0.f)  
                player.getPhysicsComponent().velocity.y -= 0.1;
        }

        if(player.getPhysicsComponent().velocity.y > -3.5) {
            player.getPhysicsComponent().velocity.y -= .15 * dt;
        }

        if(SFX::rocket.getStatus() != sf::SoundSource::Status::Playing) {
            SFX::rocket.setVolume(100.f);
            SFX::rocket.play();
        }

        for(int i = 0; i < (int)-(player.getPhysicsComponent().velocity.y - 10); i++) {
            sf::Color particleColor = sf::Color::Red;
            particleColor.r += math::randIntInRange(-80, 0);
            particleColor.g += math::randIntInRange(0, 200);
            particleColor.b += math::randIntInRange(0, 100);

            sf::Vector2f vel = sf::Vector2f(
                math::randFloat() * player.getPhysicsComponent().velocity.y, math::randFloat() * 10.0
            );

            if(math::randIntInRange(0, 1) == 0) {
                vel.x = -vel.x;
            }

            std::shared_ptr<FireParticle> particle = std::make_shared<FireParticle>(
                gun_spr.getPosition(),
                vel,
                particleColor,
                math::randIntInRange(2, 4)
            );

            world.main_world.launchParticle(particle);

        }
    } else {
        const float volume = SFX::rocket.getVolume() - 2.f;
        if(SFX::rocket.getVolume() <= 2.f) {
            SFX::rocket.stop();
        } else {
            SFX::rocket.setVolume(volume);
        }
    }

    gun_spr.setTextureRect(
        sf::IntRect(
            sf::Vector2i(0, isEnabled * 16),
            sf::Vector2i(20, 16)
        )
    );
}
