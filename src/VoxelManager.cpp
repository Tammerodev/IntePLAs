#include "VoxelManager.hpp"

bool VoxelManager::getPixelCollision(sf::Vector2i pos) {
    boundVector(pos);
    return getImagePixelAt(pos.x, pos.y).a != 0;
}


int VoxelManager::load(std::string file, bool proced)
{
    bool res = true;

    world_sx = Chunk::sizeX * chunks_x;
    world_sy = Chunk::sizeY * chunks_y;

    world_snegx = Chunk::sizeX * chunks_negx;
    world_snegy = Chunk::sizeY * chunks_negy; 

    // load only the vertex shader
    if(!shader.loadFromMemory(shader_vert, sf::Shader::Vertex)) res = false;
    // load only the fragment shader
    if(!shader.loadFromMemory(shader_frag, sf::Shader::Fragment)) res = false;
    // load both shaders
    if(!shader.loadFromMemory(shader_vert, shader_frag)) res = false;
    
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

    Voxel &vox = getVoxelAt(x,y);

    vox.temp += temp;
    if(vox.temp >= elm::getMaxTempFromType(vox.value)) {
        int val = vox.value;
        damageVoxelAt(x,y);
        if(val == elm::ValLithium) {
            hole(sf::Vector2i(x,y),elm::lithiumExplosion,true,elm::lithiumExplosionTemp);
        } else if(val == elm::ValSodium) {
            hole(sf::Vector2i(x,y),elm::sodiumExplosion,true,elm::sodiumExplosionTemp);
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
    ChunkBounds draw_bounds((center.x / Chunk::sizeX) - 6, (center.y / Chunk::sizeY) - 6, 
                            (center.x / Chunk::sizeX) + 6, (center.y / Chunk::sizeY) + 6);
    ChunkArea draw_area = draw_bounds.getArea();

    sf::Sprite spriteRend;

    sf::Clock timer;

    for(int64_t y = draw_area.startY; y < draw_area.endY; y++) {
        for(int64_t x = draw_area.startX; x < draw_area.endX; x++) {
            chIndexer.getChunkAt(x, y).update();

            spriteRend.setTexture(chIndexer.getChunkAt(x, y).tx);  
            spriteRend.setPosition(x * Chunk::sizeX,y * Chunk::sizeY);
            target.draw(spriteRend);

        }
    }

    prndd(timer.getElapsedTime().asMilliseconds());
}

void VoxelManager::resetUsedFlag()
{
    for (int y = 0;y < world_sy;y++) {
        for (int x = 0;x < world_sx;x++) {
            getVoxelAt(x,y).used = false;
        }
    }
}

void VoxelManager::update()
{
    world_sx = Chunk::sizeX * chunks_x;
    world_sy = Chunk::sizeY * chunks_y;

    auto i = voxelsInNeedOfUpdate.begin();
    while (i != voxelsInNeedOfUpdate.end())
    {
        bool del = false;
        heatVoxelAt(i->x, i->y, -elm::getAmbientDissipationFromType(getVoxelAt(i->x, i->y).value));

        if(getVoxelAt(i->x, i->y).temp <= 0 || getVoxelAt(i->x, i->y).value == 0) {i = voxelsInNeedOfUpdate.erase(i); }
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
        if(getVoxelAt(nextVoxelPos.x, nextVoxelPos.y).value == 0) {
            step = true;
            v = burningVoxels.erase(v);
        }
        else {
            ++v;
        }

    }

    sf::Vector2i lastPos = {0,0};

    auto f = fluidVoxels.begin();
    while (f != fluidVoxels.end())
    {
        sf::Color col = elm::Water;
        col.a += math::randIntInRange(0, 255 - elm::Water.a);

        boundSetImagePixelAt(f->x, f->y, col);
        getVoxelAt(f->x, f->y).value = elm::ValWater;

        sf::Vector2i nextWaterPos = *f;

        nextWaterPos.y++;
        if(getVoxelAt(nextWaterPos.x, nextWaterPos.y).value == 0) {
            boundSetImagePixelAt(f->x, f->y, sf::Color(0,0,0,0));
            getVoxelAt(f->x, f->y).value = 0;
            *f = nextWaterPos;
        } else {
            nextWaterPos.y--;
            int res = 0;

            if(getVoxelAt(nextWaterPos.x + 1, nextWaterPos.y).value == 0) {
                res = 1;
            }
            if(getVoxelAt(nextWaterPos.x - 1, nextWaterPos.y).value == 0) {
                res = -1;
            }
            if(getVoxelAt(nextWaterPos.x - 1, nextWaterPos.y).value == 0 && getVoxelAt(nextWaterPos.x + 1, nextWaterPos.y).value == 0) {
                res = math::randIntInRange(-1, 1);
            }

            nextWaterPos.x += res;

            if(getVoxelAt(nextWaterPos.x, nextWaterPos.y).value == 0) {
                boundSetImagePixelAt(f->x, f->y, sf::Color(0,0,0,0));
                getVoxelAt(f->x, f->y).value = 0;
                *f = nextWaterPos;
            }
        }

        getVoxelAt(f->x, f->y).value = elm::ValWater;
        boundSetImagePixelAt(f->x, f->y, col);

        lastPos = *f;


        ++f;
    }

    auto r = reactiveVoxels.begin();
    while (r != reactiveVoxels.end())
    {
        if(getVoxelAt(r->x, r->y).value == elm::ValSodium) {
            if(isInContactWithVoxel(*r, elm::ValWater))
                heatVoxelAt(r->x, r->y, elm::sodiumExplosionTemp / 100);
        }

        if(getVoxelAt(r->x, r->y).value == elm::ValLithium) {
            if(isInContactWithVoxel(*r, elm::ValWater))
                heatVoxelAt(r->x, r->y, elm::lithiumExplosionTemp);
        }
        
        ++r;
    }

}

void VoxelManager::hole(const sf::Vector2i &p, const uint32_t& intensity, bool force, const int64_t heat)
{
    if(force) {
        ExplosionInfo info;
        info.position = sf::Vector2f(p);
        info.strength = intensity;
        explosion_points.push_back(info);
    }

    int yexcept = p.y - intensity;
    int xexcept = p.x - intensity;

    if(yexcept < 0) yexcept = 0;
    if(xexcept < 0) xexcept = 0;

    for (int y = yexcept;y < p.y + intensity;y++) {
        if(p.y > world_sy) break;

        for (int x = xexcept;x < p.x + intensity;x++) {
            if(x > world_sx) break;
            Voxel &voxel = getVoxelAt(x,y);
                 if(voxel.value == 0) continue;

            const float distance = math::isqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y)));

            if(distance < intensity) {
                voxelsInNeedOfUpdate.push_back(sf::Vector2i(x,y));
                if(force) damageVoxelAt(x,y);
                heatVoxelAt(x,y, (intensity - distance)*heat);
            }
        }
    }

    int mergeChunkRadius = (intensity / Chunk::sizeX) + 3;
}

bool VoxelManager::generate()
{
    procGen.generate(chIndexer, world_sx, world_sy);
    
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
        if(y >= world_sy) break;
        if(y < 0) break;
        for (int x = p.x;  x < p.x + cimg.getSize().x;  x++) {
            if(x >= world_sx) break;
            if(x < 0) break;

            if(cimg.getPixel(x-p.x,y-p.y).a != 0) {
                setImagePixelAt(x,y,cimg.getPixel(x - p.x, y - p.y));
                getVoxelAt(x,y) = getHandleVoxel(getImagePixelAt(x,y), sf::Vector2i(x,y), true);
            }
        }
    }
}
