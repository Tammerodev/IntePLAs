#include "VoxelManager.hpp"

int VoxelManager::load(std::string file, sf::Vector2f* player_pos)
{
    bool res = true;
    const bool create_new_world = (file == "Create new world");

    if(WorldSettings::worldConfigLoaded) {
        chunks_x = WorldSettings::createSizeX;
        chunks_y = WorldSettings::createSizeY;
    } else {
        prnerr("World could not load from JSON file!\n", "Using default size...\n");
        res = false;
    }

    const std::string path = StorageSettings::save_path + file;
    utils::path = path + "/";

    sf::Clock timer;

    std::filesystem::create_directories(utils::path + "voxel/");
    std::filesystem::create_directories(utils::path + "cache/");

    if(!create_new_world) {
        std::fstream worldSizeChunks;
        worldSizeChunks.open(utils::path + "info/size.txt");

        if(worldSizeChunks.is_open()) {
            worldSizeChunks >> chunks_x;
            worldSizeChunks >> chunks_y; 
        } else {
            prnerr("Could not open world size data file", "!");
        }
    }

    loginf("World size (chunks) x", worldSize::world_sx, ".");
    loginf("World size (chunks) y", worldSize::world_sy, ".");


    ChunkBounds bounds = ChunkBounds(-chunks_negx, 0, chunks_x, chunks_y);

    chIndexer.init();

    for(int64_t y = bounds.getArea().startY; y < bounds.getArea().endY; y++) {
        for(int64_t x = bounds.getArea().startX; x < bounds.getArea().endX; x++) {
            std::string finalPath = utils::path + "voxel/" + utils::getPath(x, y);

            chIndexer.getChunkAt(x, y).setPosition(sf::Vector2i(x, y));

            if(create_new_world)
                chIndexer.getChunkAt(x, y).create();
            else {
                const bool success = chIndexer.getChunkAt(x, y).load(finalPath.c_str());

                if(!success) {
                    chIndexer.getChunkAt(x, y).create();
                    res = false;
                }
            }

        }
    }


    loginf("Multithreading not used : ", " Single threaded", "");

    loginf("Creating map took : ", timer.getElapsedTime().asSeconds(), ".");
    chIndexer.update();
    simulationManager.load(chIndexer, player_pos);

    return res;
}

void VoxelManager::heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp)
{

    Voxel &vox = chIndexer.getVoxelAt(x,y);

    vox.temp += temp;
    if(vox.temp >= elm::getInfoFromType(vox.value).max_temp) {
        int val = vox.value;
        chIndexer.damageVoxelAt(x,y);
        if(val == VoxelValues::LITHIUM) {
            holeRayCast(sf::Vector2i(x,y), elm::lithiumExplosion, true, elm::lithiumExplosionTemp);
        } else if(val == VoxelValues::SODIUM) {
            holeRayCast(sf::Vector2i(x,y),elm::sodiumExplosion, true, elm::sodiumExplosionTemp);
        } else if(val == VoxelValues::NITROGLYCERIN) {
            holeRayCast(sf::Vector2i(x,y), elm::nitroglycerinExplosion, true, elm::nitroglycerinExplosionTemp);
        }
    }

    /*if(vox.value == VoxelValues::MAGNESIUM) {
        addElement(x, y, std::make_shared<Flammable>(x,y));
    }*/

    if(vox.temp <= 0) vox.temp = 0;

    sf::Color currPixel = chIndexer.getImagePixelAt(x,y);

    uint64_t valR = vox.temp * 1; 
    if(valR >= 255) valR = 255;
    currPixel.r = valR;
    if(valR > 300) {
        currPixel.g = 255;
        currPixel.b = 255;
    }

    chIndexer.setImagePixelAt(x,y,currPixel);
}

void VoxelManager::render(sf::RenderTarget &target, sf::RenderTarget& rtx, const sf::Vector2f &center)
{
    sf::FloatRect viewRect(target.getView().getCenter() - target.getView().getSize() / 2.f, target.getView().getSize());
    update_area = viewRect;


    const int draw_distx = viewRect.width / Chunk::sizeX;
    const int draw_disty = viewRect.height / Chunk::sizeY;

    ChunkBounds draw_bounds = ChunkBounds((center.x / Chunk::sizeX) - draw_distx, (center.y / Chunk::sizeY) - draw_disty, 
                            (center.x / Chunk::sizeX) + draw_distx, (center.y / Chunk::sizeY) + draw_disty);

    ChunkArea draw_area = draw_bounds.getArea();

    sf::Sprite spriteRend;

    sf::RectangleShape debug;
    debug.setSize(sf::Vector2f(Chunk::sizeX, Chunk::sizeY));
    debug.setFillColor(sf::Color(0,0,0,0));
    debug.setOutlineColor(sf::Color::Black);
    debug.setOutlineThickness(1);

    for(int64_t y = draw_area.startY; y < draw_area.endY; y++) {
        for(int64_t x = draw_area.startX; x < draw_area.endX; x++) {
            Chunk& chunk = chIndexer.boundGetChunkAt(x, y);

            if(chunk.modified) {
                chunk.update();
            }


            spriteRend.setTexture(chunk.tx);  
            spriteRend.setPosition(x * Chunk::sizeX,y * Chunk::sizeY);
            debug.setPosition(spriteRend.getPosition());
            
            target.draw(spriteRend);

            if(debug_globals::inDebugDisplayState && chunk.needs_update)
                target.draw(debug);
        }
    }

    particleSimulation.render(target);
}

void VoxelManager::update(Player &player, GameEventEnum::Event& gameEvent)
{   
    chIndexer.update();
    simulationManager.updateAll(chIndexer);

    fireEffectManager.update(particleSimulation);

    auto &particles = particleSimulation.getParticlesList();

    debug_globals::particle_count = particles.size();

    for (auto it = particles.begin(); it != particles.end();) {
        auto& p = *it;

        const sf::Vector2i position = sf::Vector2i(*p);
        const Voxel &voxel = chIndexer.boundGetVoxelAt(position.x, position.y);
        const bool collision = voxel.value != 0;
        const bool remove = p->energy <= 0;

        if(collision) {
            p->collide();

            if(p->getType() == Particle::ParticleType::RainParticle) {
                chIndexer.boundGetVoxelAt(position.x, position.y - 1).value = VoxelValues::WATER;
                addElement(position.x, position.y - 1, std::make_shared<Water>(position.x, position.y - 1));
            } 
            
            if(p->getType() == Particle::ParticleType::SnowParticle) {
                chIndexer.boundGetVoxelAt(position.x, position.y - 1).value = VoxelValues::SNOW;
                addElement(position.x, position.y - 1, std::make_shared<Snow>(position.x, position.y - 1));
            }
        }

        if(p->getType() == Particle::ParticleType::FireParticle) {
            sf::Vector2i v = sf::Vector2i(*p);
            chIndexer.boundVector(v);

            sf::Vector2i vp = sf::Vector2i(p->prev_position);
            chIndexer.boundVector(vp);

            if(chIndexer.doesLineContainMaterial(vp, v)) {
                holeRayCast(v, 10, false, 20);
                p->energy = 0;
            }
        }

        // Fission 
        else if(p->getType() == Particle::ParticleType::Neutron && 
            elm::getInfoFromType(voxel.value).value == VoxelValues::URANIUM235) {

            heatVoxelAt(position.x, position.y, 100);

            const sf::Vector2i contactPos = getPositionOnContacy(position, VoxelValues::URANIUM235);

            if(contactPos != sf::Vector2i(0, 0)) {
                chIndexer.boundHeatVoxelAt(contactPos.x, contactPos.y, 10);

                if(chIndexer.boundGetVoxelAt(position.x, position.y).temp > elm::getInfoFromType(VoxelValues::URANIUM235).max_temp) {
                    gameEvent = GameEventEnum::Event::Nuclear_Explosion;
                    EventGlobals::position = position;
                }   
            }
        }

        if (remove || p->remove()) {
            it = particles.erase(it);
        } else {
            ++it;
        }
    }

    particleSimulation.update(1.0f, player.getPhysicsComponent().transform_position);

    auto i = voxelsInNeedOfUpdate.begin();
    while (i != voxelsInNeedOfUpdate.end())
    {
        Voxel& voxel = chIndexer.getVoxelAt(i->x, i->y);

        if(voxel.temp > 0)
        // TODO Ambient Dissipation
            heatVoxelAt(i->x, i->y, -1);

        if(voxel.temp <= 0 || voxel.value == 0) {
            i = voxelsInNeedOfUpdate.erase(i); 
            i++;
        }
        else { 
            ++i; 
        }

    }


    auto r = reactiveVoxels.begin();
    while (r != reactiveVoxels.end())
    {
        if(chIndexer.getVoxelAt(r->x, r->y).value == VoxelValues::SODIUM) {
            // Sodium - Water reaction (heat produced)
            if(chIndexer.isInContactWithVoxel(*r, VoxelValues::WATER))
                heatVoxelAt(r->x, r->y, elm::sodiumExplosionTemp / 100);
        }

        if(chIndexer.getVoxelAt(r->x, r->y).value == VoxelValues::LITHIUM) {
            // Lithium - Water reaction (heat produced)
            if(chIndexer.isInContactWithVoxel(*r, VoxelValues::WATER)) {
                heatVoxelAt(r->x, r->y, elm::lithiumExplosionTemp);
            }

            // Lithium - Radium226 reaction (free neutrons)
            if(chIndexer.isInContactWithVoxel(*r, VoxelValues::RADIUM226)) {
                const sf::Vector2f position = sf::Vector2f(*r); 
                const sf::Vector2f velocity = sf::Vector2f(math::randFloat() - 0.5f, math::randFloat() - 0.5f); 

                std::shared_ptr<FreeNeutron> particle = std::make_shared<FreeNeutron>(position, velocity);

                particleSimulation.addParticle(particle);
            }
        }
        
        ++r;
    }

    const sf::Vector2i center = sf::Vector2i(player.getPhysicsComponent().transform_position);

    const int draw_distx = update_area.width / Chunk::sizeX;
    const int draw_disty = update_area.height / Chunk::sizeY;

    ChunkBounds draw_bounds = ChunkBounds((center.x / Chunk::sizeX) - draw_distx, (center.y / Chunk::sizeY) - draw_disty, 
                            (center.x / Chunk::sizeX) + draw_distx, (center.y / Chunk::sizeY) + draw_disty);

    ChunkArea draw_area = draw_bounds.getArea();

    sf::Sprite spriteRend;

#if USE_MULTITHREADING
    updateElementsMultithreaded(draw_area);
#else
    updateElementsNonMultithreaded(draw_area);
#endif
}

void VoxelManager::updateChunkElements(int x, int y, Chunk& chunk) {
    auto e = chunk.elements.begin();
    while (e != chunk.elements.end())
    {
        if(e->get() == nullptr) continue;
        e->get()->update(chIndexer);

        const sf::Vector2i &chunk_p = e->get()->move_this_to_chunk(chIndexer);
        const auto turned_into = e->get()->turn_into();
        
        if(turned_into != nullptr) {
            addElement(e->get()->x, e->get()->y, turned_into);
        }

        if(chunk_p != sf::Vector2i(x, y)) {

            chIndexer.boundGetChunkAt(chunk_p.x, chunk_p.y).elements.emplace_back(*e);

            chunk.needs_update = false;
            e = chunk.elements.erase(e);  
            continue;
        }

        if(e->get()->clear()) {
            e = chunk.elements.erase(e);  
        }
        else {
            ++e;  
        }
    }
}

void VoxelManager::updateElementsNonMultithreaded(ChunkArea& draw_area) {
    for(int64_t y = draw_area.startY; y < draw_area.endY; y++) {
        for(int64_t x = draw_area.startX; x < draw_area.endX; x++) {
            Chunk& chunk = chIndexer.boundGetChunkAt(x, y);

            if(!chunk.needs_update) continue;
            if(chunk.elements.empty()) chunk.needs_update = false;

            updateChunkElements(x, y, chunk);
        }
    }
}

void VoxelManager::updateElementsMultithreaded(ChunkArea& draw_area) {
    std::vector<std::future<void>> futures;

    for(int64_t y = draw_area.startY; y < draw_area.endY; y++) {
        // Creating async thread to loop x values
        futures.emplace_back(std::async(std::launch::deferred, [y, &draw_area, this]() {
            for(int64_t x = draw_area.startX; x < draw_area.endX; x++) {
                Chunk& chunk = chIndexer.boundGetChunkAt(x, y);

                if(!chunk.needs_update) continue;
                if(chunk.elements.empty()) chunk.needs_update = false;

                updateChunkElements(x, y, chunk);
            }
        }));

    }

    for(auto& future : futures) {
        future.wait();
    }
}

void VoxelManager::hole(sf::Vector2i p, const uint32_t intensity, bool force, const int64_t heat, const unsigned char collect_percent)
{
    if(force) {
        explosionEffect(sf::Vector2f(p), (int)intensity);
    }

    chIndexer.boundVector(p);

    int yexcept = p.y - (int)intensity;
    int xexcept = p.x - (int)intensity;

    if(yexcept < 0) yexcept = 0;
    if(xexcept < 0) xexcept = 0;

    for (int y = yexcept;y < p.y + (int)intensity;y++) {
        if(y > worldSize::world_sy) break;

        for (int x = xexcept;x < p.x + (int)intensity;x++) {

            sf::Vector2i v = sf::Vector2i(x, y);
                chIndexer.boundVector(v);
            
            Voxel &voxel = chIndexer.getVoxelAt(v.x, v.y);
                if(voxel.value == 0) continue;

            const float distance = std::sqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y)));

            if(distance < intensity) {
                voxelsInNeedOfUpdate.push_back(v);
                if(force) chIndexer.damageVoxelAt(v.x, v.y);
                heatVoxelAt(v.x, v.y, (intensity - distance)*heat);

                chIndexer.boundGetChunkAt(chIndexer.getChunkFromPos(v.x, v.y).x, chIndexer.getChunkFromPos(v.x, v.y).y).needs_update = true;


                if(math::randIntInRange(0, 100) < 5) {
                    launchDebrisParticle(p, chIndexer.getImagePixelAt(v.x, v.y));
                }
            }
        }
    }

}

void VoxelManager::holeRayCast(const sf::Vector2i& p, const uint32_t intensity, bool force, const int64_t heat, const unsigned char collect_percent, bool turnToAsh)
{

    if(force) {
        explosionEffect(sf::Vector2f(p), intensity);
    }

    int endX = p.x - intensity;
    int endY = p.y - intensity;

    Raycast::RaycastInfo info(&chIndexer);
    info.start = p;
    info.voxelsInNeedOfUpdate = &voxelsInNeedOfUpdate;
    info.intensity = intensity;
    info.propability_of_material = collect_percent;
    info.particle_simulation = &particleSimulation;
    info.temp = heat;

    info.turnToAsh = turnToAsh;

    for(;endX < p.x + (int)intensity; endX++) {
        info.end = sf::Vector2i(endX, endY);
        Raycast::castRayLine(info, force);
    }

    for(;endY < p.y + (int)intensity; endY++) {
        info.end = sf::Vector2i(endX, endY);
        Raycast::castRayLine(info, force);
    }

    for(;endX > p.x - (int)intensity; endX--) {
        info.end = sf::Vector2i(endX, endY);
        Raycast::castRayLine(info, force);
    }

    for(;endY > p.y - (int)intensity; endY--) {
        info.end = sf::Vector2i(endX, endY);
        Raycast::castRayLine(info, force);  
    }
}

void VoxelManager::singleRayCast(const sf::Vector2i &start, const sf::Vector2i &end, const uint32_t intensity, bool isLaser, const int64_t heat, const unsigned char collect_percent, bool turnToAsh)
{
    Raycast::RaycastInfo info(&chIndexer);
    info.start = start;
    info.end = end;

    info.voxelsInNeedOfUpdate = &voxelsInNeedOfUpdate;
    info.intensity = intensity;
    info.propability_of_material = collect_percent;
    info.particle_simulation = &particleSimulation;
    info.temp = heat;

    info.turnToAsh = turnToAsh;
    info.isLaser = isLaser;

    Raycast::castRayLine(info, true);


}

void VoxelManager::mine(sf::Vector2i p, const uint32_t intensity, int percent_gain)
{
    chIndexer.boundVector(p);

    int yexcept = p.y - intensity;
    int xexcept = p.x - intensity;

    if(yexcept < 0) yexcept = 0;
    if(xexcept < 0) xexcept = 0;

    for (int y = yexcept;y < p.y + (int)intensity;y++) {
        if(y > worldSize::world_sy) break;

        for (int x = xexcept;x < p.x + (int)intensity;x++) {

            sf::Vector2i v = sf::Vector2i(x, y);
                chIndexer.boundVector(v);
            
            Voxel &voxel = chIndexer.getVoxelAt(v.x, v.y);
                if(voxel.value == 0) continue;

            const float distance = std::sqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y)));

            if(distance < intensity) {
                chIndexer.damageVoxelAt(v.x, v.y);

                chIndexer.boundGetChunkAt(chIndexer.getChunkFromPos(v.x, v.y).x, chIndexer.getChunkFromPos(v.x, v.y).y).needs_update = true;

                if(math::randIntInRange(0, 100) < percent_gain) {
                    if(chIndexer.getVoxelAt(v.x, v.y).value != 0) {
                            particleSimulation.addParticle(
                                std::make_shared<PickableDebris>(
                                    sf::Vector2f(v), math::subVector(sf::Vector2f(v), sf::Vector2f(p)) / 10.f,
                                    chIndexer.getImagePixelAt(v.x, v.y),
                                    chIndexer.getVoxelAt(v.x, v.y).value,
                                    &chIndexer
                                    ));
                        }
                }
            }
        }
    }

}

bool VoxelManager::generate()
{
    procGen.generate(chIndexer, worldSize::world_sx, worldSize::world_sy);
    
    ChunkBounds bounds(0,0, chunks_x, chunks_y);

    return true;
}

bool VoxelManager::generateVegetation()
{
    sf::Clock timer;
    sf::Image image;

    for(int i = 0; i < worldSize::world_sx - 1; i++) {
        BiomeInfo info;
        Biome &biome = procGen.getBiomeAtPosition(i, chIndexer);
        info = *biome.getInfo();

        if(info.vegetationInfo.filepath == "") continue;
        if(!image.loadFromFile(info.vegetationInfo.filepath)) continue;

        const float h = procGen.getHeightOnMap(i);


        sf::IntRect sourceRect = sf::IntRect(info.vegetationInfo.width_of_part * math::randIntInRange(0, info.vegetationInfo.parts_on_x), 0,
                                            info.vegetationInfo.width_of_part,
                                            info.vegetationInfo.height_of_part);

        sf::Image selectedImage;
        selectedImage.create(sourceRect.width, sourceRect.height);

        for (int x = 0; x < sourceRect.width; ++x) {
            for (int y = 0; y < sourceRect.height; ++y) {
                selectedImage.setPixel(x, y, image.getPixel(sourceRect.left + x, sourceRect.top + y));
            }
        }

        if(math::randIntInRange(0, 100) < info.vegetationInfo.amount) {
            // Build with no collisions
            build_image(sf::Vector2i(i, (2048 - h) - sourceRect.height + info.vegetationInfo.offset_up), selectedImage, nullptr, sf::Vector2f(0.f, 0.f), false);
        }
    }

    loginf("Creating vegetation took ", timer.restart().asSeconds(), " seconds.");

    return true;
}


void VoxelManager::build_image(const sf::Vector2i &p, const sf::Image &cimg, std::list<std::shared_ptr<VoxelGroup>>* grp, const sf::Vector2f velocity, bool hasCollisions)
{
    if(grp != nullptr) {
        auto object = std::make_shared<VoxelGroup>();
        object->load(cimg);

        object->setPosition(sf::Vector2f(p));
        object->setVelocity(velocity);

        grp->emplace_back(object);
        return;
    }

    for (int y = p.y;  y < p.y + (int)cimg.getSize().y;  y++) {
        if(y >= worldSize::world_sy) break;
        if(y < 0) break;

        for (int x = p.x;  x < p.x + (int)cimg.getSize().x;  x++) {
            if(x >= worldSize::world_sx) break;
            if(x < 0) break;

            if(cimg.getPixel(x-p.x,y-p.y).a != 0) {

                x = chIndexer.getBoundedVector(sf::Vector2i(x, y)).x;
                y = chIndexer.getBoundedVector(sf::Vector2i(x, y)).y;


                // Use one materials
                chIndexer.setImagePixelAt(x,y,cimg.getPixel(x - p.x, y - p.y));
                chIndexer.getVoxelAt(x, y) = getHandleVoxel(chIndexer.getImagePixelAt(x,y), sf::Vector2i(x,y), true);
                chIndexer.getVoxelAt(x, y).hasCollision = hasCollisions;
            }
        }
    }
}
