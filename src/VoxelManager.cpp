#include "VoxelManager.hpp"

// liquid is 5
// "neutral" is 3

int VoxelManager::load(std::string file)
{
    bool res = true;

    if(WorldSettings::worldConfigLoaded) {
        chunks_x = WorldSettings::createSizeX;
        chunks_y = WorldSettings::createSizeY;
    } else {
        prnerr("World could not load from JSON file!\n", "Using default size...\n");
        res = false;
    }

    chIndexer.init();

    ChunkBounds bounds = ChunkBounds(-chunks_negx, 0, chunks_x, chunks_y);

    sf::Clock timer;

    const bool create_new_world = file == "Create new world";

    const std::string path = StorageSettings::save_path + file;

    utils::path = path + "/";

    for(int64_t y = bounds.getArea().startY; y < bounds.getArea().endY; y++) {
        for(int64_t x = bounds.getArea().startX; x < bounds.getArea().endX; x++) {
            std::string finalPath = utils::path + utils::getPath(x, y);

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

    timer.restart();

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
            hole(sf::Vector2i(x,y), elm::lithiumExplosion, true, elm::lithiumExplosionTemp);
        } else if(val == VoxelValues::SODIUM) {
            hole(sf::Vector2i(x,y),elm::sodiumExplosion,true,elm::sodiumExplosionTemp);
        } else if(val == VoxelValues::NITROGLYCERIN) {

            hole(sf::Vector2i(x,y),elm::nitroglycerinExplosion,true,elm::nitroglycerinExplosionTemp);
            
        }
    }

    if(vox.value == VoxelValues::MAGNESIUM) {
        addElement(x, y, std::make_shared<Burning>(x,y));
    }

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

void VoxelManager::render(sf::RenderTarget &target, const sf::Vector2f &center)
{
    sf::FloatRect viewRect(target.getView().getCenter() - target.getView().getSize() / 2.f, target.getView().getSize());
    update_area = viewRect;


    const int draw_distx = viewRect.width / Chunk::sizeX;
    const int draw_disty = viewRect.height / Chunk::sizeY;

    ChunkBounds draw_bounds = ChunkBounds((center.x / Chunk::sizeX) - draw_distx, (center.y / Chunk::sizeY) - draw_disty, 
                            (center.x / Chunk::sizeX) + draw_distx, (center.y / Chunk::sizeY) + draw_disty);

    ChunkArea draw_area = draw_bounds.getArea();

    sf::Sprite spriteRend;

    for(int64_t y = draw_area.startY; y < draw_area.endY; y++) {
        for(int64_t x = draw_area.startX; x < draw_area.endX; x++) {
            if(chIndexer.getChunkAt(x, y).modified) {
                chIndexer.getChunkAt(x, y).update();
   
                if(Session::session == Session::Join)
                    voxelSpy.alertOfChunkModification(sf::Vector2i(x, y), chIndexer);
            }

            spriteRend.setTexture(chIndexer.getChunkAt(x, y).tx);  
            spriteRend.setPosition(x * Chunk::sizeX,y * Chunk::sizeY);

            target.draw(spriteRend);
        }
    }

    particleSimulation.render(target);
}

void VoxelManager::update(Player &player)
{   

    clientManager.update(chIndexer);
    chIndexer.update();
    simulationManager.updateAll(chIndexer);

    particleSimulation.update(1.0f, player.getPhysicsComponent().transform_position);
    auto &particles = particleSimulation.getParticlesList();

    debug_globals::particle_count = particles.size();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) {
        ChunkBounds bounds = ChunkBounds(0, 0, chunks_x, chunks_y);

        for(int64_t y = bounds.getArea().startY; y < bounds.getArea().endY; y++) {
            for(int64_t x = bounds.getArea().startX; x < bounds.getArea().endX; x++) {
                chIndexer.getChunkAt(x, y).unLoad();
            }
        }

        malloc_trim(0);
    }

    for (auto it = particles.begin(); it != particles.end();) {
        auto& p = *it;

        const sf::Vector2i position = sf::Vector2i(*p);
        const Voxel &voxel = chIndexer.boundGetVoxelAt(position.x, position.y);
        const bool collision = voxel.value != 0;
        const bool remove = p->energy <= 0;

        if(collision) {
            p->collide();
        }

        if(p->getType() == Particle::ParticleType::FireParticle) {
            sf::Vector2i v = sf::Vector2i(*p);
            chIndexer.boundVector(v);

            sf::Vector2i vp = sf::Vector2i(p->prev_position);
            chIndexer.boundVector(vp);

            if(doesLineContainMaterial(vp, v)) {
                holeRayCast(v, 10, false, 20);
                p->energy = 0;
            }
        }

        // Fission 
        if(p->getType() == Particle::ParticleType::Neutron && 
            elm::getInfoFromType(voxel.value).value == VoxelValues::URANIUM235) {

            heatVoxelAt(position.x, position.y, 100);

            const sf::Vector2i contactPos = getPositionOnContacy(position, VoxelValues::URANIUM235);

            if(contactPos != sf::Vector2i(0, 0)) {
                heatVoxelAt(contactPos.x, contactPos.y, 100);

                if(chIndexer.boundGetVoxelAt(position.x, position.y).temp > elm::getInfoFromType(VoxelValues::URANIUM235).max_temp) {
                    holeRayCast(position, 250, true, 30000);
                }   
            }
        }
        if (remove) {
            it = particles.erase(it);
        } else {
            ++it;
        }
    }

    auto i = voxelsInNeedOfUpdate.begin();
    while (i != voxelsInNeedOfUpdate.end())
    {
        bool del = false;

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

    for(int64_t y = draw_area.startY; y < draw_area.endY; y++) {
        for(int64_t x = draw_area.startX; x < draw_area.endX; x++) {
            Chunk& chunk = chIndexer.getChunkAt(x, y);

            if(chunk.elements.empty()) chunk.needs_update = false;

            if(!chunk.needs_update) continue;


            auto e = chunk.elements.begin();
            while (e != chunk.elements.end())
            {
                e->get()->update(chIndexer);

                const sf::Vector2i chunk_p = e->get()->move_this_to_chunk(chIndexer);
                const auto turned_into = e->get()->turn_into();
                
                if(turned_into != nullptr) {
                    addElement(e->get()->x, e->get()->y, turned_into);
                }

                if(chunk_p != sf::Vector2i(x, y)) {

                    chIndexer.boundGetChunkAt(chunk_p.x, chunk_p.y).elements.push_back(*e);

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
    }

    PlayerGlobal::radiation_received = 0;
    
    auto rad = radioactive_elements.begin();
    while (rad != radioactive_elements.end())
    {

        rad->get()->update(chIndexer);

        const float distance = math::distance(
            sf::Vector2f(*rad->get()), sf::Vector2f(player.getPhysicsComponent().transform_position)
            );

        const float radiation_strength = rad->get()->getRadiationStrength();

        if(distance < radiation_strength) {
            
            const int radiation = radiation_strength - (int)distance;

            const bool particle = math::randIntInRange(0, radiation_strength) > distance;

            if(particle) {
                PlayerGlobal::radiation_received = radiation;
                
                if(radiation > PlayerGlobal::still_radioation) 
                    PlayerGlobal::still_radioation = radiation;

                if(math::randIntInRange(0, 10000) < 1) 
                    player.damage_radiation(1);
            }
        }

        if(rad->get()->clear())
            rad = radioactive_elements.erase(rad);
        else 
            ++rad;  
    }
}

void VoxelManager::hole(sf::Vector2i p, const uint32_t intensity, bool force, const int64_t heat)
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

void VoxelManager::holeRayCast(sf::Vector2i p, const uint32_t intensity, bool force, const int64_t heat)
{

    if(force) {
        explosionEffect(sf::Vector2f(p), intensity);
    }

    const int numRays = intensity * 30;
    const int rayLength = intensity;


    int endX = p.x - intensity;
    int endY = p.y - intensity;

    Raycast::RaycastInfo info(&chIndexer);
    info.start = p;
    info.voxelsInNeedOfUpdate = &voxelsInNeedOfUpdate;
    info.intensity = intensity;
    info.propability_of_material = 10;
    info.particle_simulation = &particleSimulation;
    info.temp = heat;

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
        VegetationInfo vegetationInfo;
        Biome &biome = procGen.getBiomeAtPosition(i, chIndexer);
        vegetationInfo = biome.getVegetationInfo();

        if(vegetationInfo.filepath == "") continue;
        if(!image.loadFromFile(vegetationInfo.filepath)) continue;

        const float h = procGen.getHeightOnMap(i);


        sf::IntRect sourceRect = sf::IntRect(vegetationInfo.width_of_part * math::randIntInRange(0, vegetationInfo.parts_on_x), 0,
                                            vegetationInfo.width_of_part,
                                            vegetationInfo.height_of_part);

        sf::Image selectedImage;
        selectedImage.create(sourceRect.width, sourceRect.height);

        for (int x = 0; x < sourceRect.width; ++x) {
            for (int y = 0; y < sourceRect.height; ++y) {
                selectedImage.setPixel(x, y, image.getPixel(sourceRect.left + x, sourceRect.top + y));
            }
        }

        if(math::randIntInRange(0, 100) < vegetationInfo.amount) {
            // Build with no collisions
            build_image(sf::Vector2i(i, (2048 - h) - sourceRect.height + vegetationInfo.offset_up), selectedImage, nullptr, sf::Vector2f(0.f, 0.f), false);
        }

    }

    loginf("Creating vegetation took ", timer.restart().asSeconds(), " seconds.");

    return true;
}


void VoxelManager::build_image(const sf::Vector2i &p, const sf::Image &cimg, std::list<VoxelGroup>* grp, const sf::Vector2f velocity, bool hasCollisions)
{
    if(grp != nullptr) {
        VoxelGroup object = VoxelGroup();
        object.load(cimg);

        object.getPhysicsComponent().transform_position = sf::Vector2f(p);

        object.getPhysicsComponent().velocity = velocity;

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
                chIndexer.setImagePixelAt(x,y,cimg.getPixel(x - p.x, y - p.y));
                chIndexer.getVoxelAt(x, y) = getHandleVoxel(chIndexer.getImagePixelAt(x,y), sf::Vector2i(x,y), true);
                chIndexer.getVoxelAt(x, y).hasCollision = hasCollisions;

            }
        }
    }
}
