#include "VoxelManager.hpp"

bool VoxelManager::getPixelCollision(sf::Vector2i pos) {
    chIndexer.boundVector(pos);
    return getImagePixelAt(pos.x, pos.y).a != 0;
}


int VoxelManager::load(std::string file, bool proced)
{
    bool res = true;

    ChunkBounds bounds = ChunkBounds(-chunks_negx, 0, chunks_x, chunks_y);

    sf::Clock timer;

    for(int64_t y = bounds.getArea().startY; y < bounds.getArea().endY; y++) {
        for(int64_t x = bounds.getArea().startX; x < bounds.getArea().endX; x++) {
            chIndexer.getChunkAt(x, y).create();
        }
    }
    loginf("Multithreading not used : ", " Single threaded", "");

    loginf("Creating map took : ", timer.getElapsedTime().asSeconds(), ".");

    initVoxelMap();

    return res;
}

void VoxelManager::heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp)
{

    Voxel &vox = chIndexer.getVoxelAt(x,y);

    vox.temp += temp;
    if(vox.temp >= elm::getMaxTempFromType(vox.value)) {
        int val = vox.value;
        damageVoxelAt(x,y);
        if(val == elm::ValLithium) {
            hole(sf::Vector2i(x,y),elm::lithiumExplosion,true,elm::lithiumExplosionTemp);
        } else if(val == elm::ValSodium) {
            hole(sf::Vector2i(x,y),elm::sodiumExplosion,true,elm::sodiumExplosionTemp);
        } else if(val == elm::ValNitroglycerin) {
            hole(sf::Vector2i(x,y),elm::nitroglycerinExplosion,true,elm::nitroglycerinExplosionTemp);
        }
    }

    if(vox.value == elm::ValMagnesium) {
        burningVoxels.push_back(sf::Vector2i(x,y));
    }

    if(vox.temp <= 0) vox.temp = 0;

    sf::Color currPixel = getImagePixelAt(x,y);

    uint64_t valR = vox.temp * 1; 
    if(valR >= 255) valR = 255;
    currPixel.r = valR;
    if(valR > 300) {
        currPixel.g = 255;
        currPixel.b = 255;
    }

    setImagePixelAt(x,y,currPixel);
}

void VoxelManager::render(sf::RenderTarget &target, const sf::Vector2f &center)
{
    draw_bounds = ChunkBounds((center.x / Chunk::sizeX) - 6, (center.y / Chunk::sizeY) - 6, 
                            (center.x / Chunk::sizeX) + 6, (center.y / Chunk::sizeY) + 6);
    draw_area = draw_bounds.getArea();

    for(int64_t y = draw_area.startY; y < draw_area.endY; y++) {
        for(int64_t x = draw_area.startX; x < draw_area.endX; x++) {
            chIndexer.getChunkAt(x, y).update();

            spriteRend.setTexture(chIndexer.getChunkAt(x, y).tx);  
            spriteRend.setPosition(x * Chunk::sizeX,y * Chunk::sizeY);
            target.draw(spriteRend);

        }
    }
}

void VoxelManager::update()
{   
    chIndexer.updateWorldSize();

    auto i = voxelsInNeedOfUpdate.begin();
    while (i != voxelsInNeedOfUpdate.end())
    {
        bool del = false;
        heatVoxelAt(i->x, i->y, -elm::getAmbientDissipationFromType(chIndexer.getVoxelAt(i->x, i->y).value));

        if(chIndexer.getVoxelAt(i->x, i->y).temp <= 0 || chIndexer.getVoxelAt(i->x, i->y).value == 0) {i = voxelsInNeedOfUpdate.erase(i); }
        else { ++i; }

    }

    sf::Vector2i nextVoxelPos;

    bool step = false;

    auto v = burningVoxels.begin();
    while (v != burningVoxels.end())
    {
        int direction = math::randIntInRange(0, 3);

        nextVoxelPos = *v;
        
        if(step) {
            if(direction == 0) nextVoxelPos.x += 1;
            if(direction == 1) nextVoxelPos.x -= 1;
            if(direction == 2) nextVoxelPos.y += 1;
            if(direction == 3) nextVoxelPos.y -= 1;

            step = false;
        }

        bool found = false;
        //(std::find(burningVoxels.begin(), burningVoxels.end(), nextVoxelPos) != burningVoxels.end());

        if(!found) {

           *v = nextVoxelPos;
        }

        heatVoxelAt(nextVoxelPos.x, nextVoxelPos.y, 3);
        if(chIndexer.getVoxelAt(nextVoxelPos.x, nextVoxelPos.y).value == 0) {
            step = true;
            v = burningVoxels.erase(v);
        }
        else {
            ++v;
        }

    }

    auto r = reactiveVoxels.begin();
    while (r != reactiveVoxels.end())
    {
        if(chIndexer.getVoxelAt(r->x, r->y).value == elm::ValSodium) {
            if(isInContactWithVoxel(*r, elm::ValWater))
                heatVoxelAt(r->x, r->y, elm::sodiumExplosionTemp / 100);
        }

        if(chIndexer.getVoxelAt(r->x, r->y).value == elm::ValLithium) {
            if(isInContactWithVoxel(*r, elm::ValWater))
                heatVoxelAt(r->x, r->y, elm::lithiumExplosionTemp);
        }
        
        ++r;
    }

    auto e = elements.begin();
    while (e != elements.end())
    {
        e->get()->update(chIndexer);
        ++e;
    }

}

void VoxelManager::hole(sf::Vector2i p, const uint32_t intensity, bool force, const int64_t heat)
{
    if(force) {
        ExplosionInfo info;
        info.position = sf::Vector2f(p);
        info.strength = intensity;
        explosion_points.push_back(info);
    }

    chIndexer.boundVector(p);

    int yexcept = p.y - intensity;
    int xexcept = p.x - intensity;

    if(yexcept < 0) yexcept = 0;
    if(xexcept < 0) xexcept = 0;

    for (int y = yexcept;y < p.y + intensity;y++) {
        if(y > chIndexer.world_sy) break;

        for (int x = xexcept;x < p.x + intensity;x++) {
            if(x > chIndexer.world_sx) break;
            Voxel &voxel = chIndexer.getVoxelAt(x,y);
                 if(voxel.value == 0) continue;

            const float distance = math::isqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y)));

            if(distance < intensity) {
                sf::Vector2i v = sf::Vector2i(x, y);
                chIndexer.boundVector(v);


                voxelsInNeedOfUpdate.push_back(v);
                if(force) damageVoxelAt(v.x, v.y);
                heatVoxelAt(v.x, v.y, (intensity - distance)*heat);
            }
        }
    }

    int mergeChunkRadius = (intensity / Chunk::sizeX) + 3;
}

bool VoxelManager::generate()
{
    procGen.generate(chIndexer, chIndexer.world_sx, chIndexer.world_sy);
    
    ChunkBounds bounds(0,0, chunks_x, chunks_y);

    return true;
}

bool VoxelManager::generateVegetation()
{

    int ind = 0;

    sf::Image image;
    image.loadFromFile("res/img/Proc.png");

    for(auto h : procGen.heightMap1D) {

        sf::IntRect sourceRect = sf::IntRect(16 * math::randIntInRange(0, 7), 0, 16, 16);

        sf::Image selectedImage;
        selectedImage.create(sourceRect.width, sourceRect.height);

        for (int x = 0; x < sourceRect.width; ++x) {
            for (int y = 0; y < sourceRect.height; ++y) {
                selectedImage.setPixel(x, y, image.getPixel(sourceRect.left + x, sourceRect.top + y));
            }
        }

        if(math::randIntInRange(0, 20) < 5) {
            build_image(sf::Vector2i(ind, (2048 - h) - sourceRect.height + 6), selectedImage, nullptr);
        }

        ind++;
    }

    return true;
}


void VoxelManager::build_image(const sf::Vector2i &p, const sf::Image &cimg, std::list<VoxelGroup>* grp, float angle, float mag)
{
    if(grp != nullptr) {
        VoxelGroup object = VoxelGroup();
        object.load(cimg);

        object.getPhysicsComponent().transform_position = sf::Vector2f(p);

        float angleRadians = angle * static_cast<float>(M_PI) / 180.0f;
        object.getPhysicsComponent().velocity.x = mag * cos(angleRadians);
        object.getPhysicsComponent().velocity.y = mag * sin(angleRadians);

        grp->emplace_back(object);
        return;
    }

    for (int y = p.y;  y < p.y + cimg.getSize().y;  y++) {
        if(y >= chIndexer.world_sy) break;
        if(y < 0) break;
        for (int x = p.x;  x < p.x + cimg.getSize().x;  x++) {
            if(x >= chIndexer.world_sx) break;
            if(x < 0) break;

            if(cimg.getPixel(x-p.x,y-p.y).a != 0) {
                setImagePixelAt(x,y,cimg.getPixel(x - p.x, y - p.y));
                chIndexer.getVoxelAt(x,y) = getHandleVoxel(getImagePixelAt(x,y), sf::Vector2i(x,y), true);
            }
        }
    }
}
