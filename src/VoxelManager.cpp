#include "VoxelManager.hpp"

std::pair<bool, sf::FloatRect> VoxelManager::getOvelapWithRect(const sf::FloatRect &collider)
{
    ChunkBounds bounds = ChunkBounds(
    collider.left / Chunk::sizeX - 3,
    collider.top / Chunk::sizeY - 3,
    collider.left / Chunk::sizeX + 3,
    collider.top / Chunk::sizeY + 3);

    for(uint32_t y = bounds.getArea().startY; y < bounds.getArea().endY; y++) {
    for(uint32_t x = bounds.getArea().startX; x < bounds.getArea().endX; x++) {
    for(auto &r : chIndexer.getChunkAt(x, y).rects) {
        if(collider.intersects(r.getGlobalBounds())) {
            sf::FloatRect rect2 = r.getGlobalBounds();
            sf::FloatRect rect1 = collider;

            float left = std::max(rect1.left, rect2.left);
            float top = std::max(rect1.top, rect2.top);
            float right = std::min(rect1.left + rect1.width, rect2.left + rect2.width);
            float bottom = std::min(rect1.top + rect1.height, rect2.top + rect2.height);

            float width = right - left;
            float height = bottom - top;

            return {true, sf::FloatRect(left, top, width, height)};
        }   
    }
    }
    }
    return {false, sf::FloatRect()};
}

std::pair<bool, sf::FloatRect> VoxelManager::getOvelapWithRectY(const sf::FloatRect &collider)
{
    ChunkBounds bounds = ChunkBounds(
        collider.left / Chunk::sizeX - 3,
        collider.top / Chunk::sizeY - 3,
        collider.left / Chunk::sizeX + 3,
        collider.top / Chunk::sizeY + 3);

    for(uint32_t y = bounds.getArea().startY; y < bounds.getArea().endY; y++) {
    for(uint32_t x = bounds.getArea().startX; x < bounds.getArea().endX; x++) {
    for(auto &r : chIndexer.getChunkAt(x, y).rects) {
        if(collider.intersects(r.getGlobalBounds())) {
            return {true, sf::FloatRect(0,0,0,collider.height + (collider.top) - r.getGlobalBounds().top)};
        }   
    }
    }
    }
    return {false, sf::FloatRect()};
}

std::pair<bool, sf::FloatRect> VoxelManager::getOvelapWithRectX(const sf::FloatRect &collider)
{
    ChunkBounds bounds = ChunkBounds(
        collider.left / Chunk::sizeX - 3,
        collider.top / Chunk::sizeY - 3,
        collider.left / Chunk::sizeX + 3,
        collider.top / Chunk::sizeY + 3);

    for(uint32_t y = bounds.getArea().startY; y < bounds.getArea().endY; y++) {
    for(uint32_t x = bounds.getArea().startX; x < bounds.getArea().endX; x++) {
    for(auto &r : chIndexer.getChunkAt(x, y).rects) {
        if(collider.intersects(r.getGlobalBounds())) {
            return {true, sf::FloatRect(0,0,collider.width + (collider.left) - r.getGlobalBounds().left, 0)};
        }   
    }
    }
    }
    return {false, sf::FloatRect()};
}

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

    mergeChunkBounds(bounds);

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
            hole(sf::Vector2i(x,y),100,true,2000);
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
    ChunkBounds draw_bounds((center.x / Chunk::sizeX) - 13, (center.y / Chunk::sizeY) - 13, 
                            (center.x / Chunk::sizeX) + 13, (center.y / Chunk::sizeY) + 13);
    ChunkArea draw_area = draw_bounds.getArea();

    for(int64_t y = draw_area.startY; y < draw_area.endY; y++) {
        for(int64_t x = draw_area.startX; x < draw_area.endX; x++) {
            chIndexer.getChunkAt(x, y).update();
            for(const auto &r : chIndexer.getChunkAt(x, y).rects)  {
               target.draw(r);
            }
        }
    }
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

    mergeChunkBounds(ChunkBounds((nextVoxelPos.x / Chunk::sizeX) - 2, (nextVoxelPos.y / Chunk::sizeX) - 2,
                                    (nextVoxelPos.x / Chunk::sizeY) + 2, (nextVoxelPos.y / Chunk::sizeY) + 2), true);
}

void VoxelManager::merge()
{


sf::Sprite r;

for(auto c : mergeChunks) {
    chIndexer.getChunkAt(c.x, c.y).rects.clear();

    r.setTexture(chIndexer.getChunkAt(c.x, c.y).tx);
    for (int y = c.y * Chunk::sizeY; y < (c.y * Chunk::sizeY) + Chunk::sizeY; y++) {
    for (int x = c.x * Chunk::sizeX; x < (c.x * Chunk::sizeX) + Chunk::sizeX; x++) {

        if (getVoxelAt(x,y).value != 0 && !getVoxelAt(x,y).used) {
            int x1 = x;
            int y1 = y;

            while (getVoxelAt(x1,y1).value != 0 && !getVoxelAt(x1,y1).used && y1 < (c.y * Chunk::sizeY) + Chunk::sizeY) {
                y1++;
            }
            
            for (int y2 = y; y2 <= y1; y2++) {
                for (int x2 = x; x2 <= x1; x2++) {
                    getVoxelAt(x,y2).used = true;
                }
            }
            
            x1++;
            y1++;
                    
            r.setPosition(x, y);
            r.setTextureRect(sf::IntRect(x - (c.x * Chunk::sizeX),y - (c.y * Chunk::sizeY),  x1 - x,  y1 -y));
            chIndexer.getChunkAt(c.x, c.y).rects.emplace_back(r);
        }
    }
    }
}


for(auto c : mergeChunks) {
    for (int y = c.y * Chunk::sizeY; y < (c.y * Chunk::sizeY) + Chunk::sizeY; y++) {
        for (int x = c.x * Chunk::sizeX; x < (c.x * Chunk::sizeX) + Chunk::sizeX; x++) {
            if(x < -world_snegx + 1) x = -world_snegx + 1;
            if(y < 0) y = 0;
            if(x > world_sx) x = world_sx - 1;
            if(y > world_sy) y = world_sy - 1;


            getVoxelAt(x, y).used = false;
        }
    }
}

mergeChunks.clear();
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

    mergeChunkBounds(ChunkBounds((p.x / Chunk::sizeX) - mergeChunkRadius, (p.y / Chunk::sizeX) - mergeChunkRadius,
                                 (p.x / Chunk::sizeY) + mergeChunkRadius, (p.y / Chunk::sizeY) + mergeChunkRadius));

}

bool VoxelManager::generate()
{
    procGen.generate(chIndexer, world_sx, world_sy);
    
    ChunkBounds bounds(0,0, chunks_x, chunks_y);
    mergeChunkBounds(bounds);

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
                setImagePixelAt(x,y,cimg.getPixel(x-p.x, y-p.y));
                getVoxelAt(x,y) = getValueFromCol(getImagePixelAt(x,y), sf::Vector2i(x,y));
            }
        }
    }

    mergeChunkBounds(ChunkBounds((p.x / Chunk::sizeX) - 2, (p.y / Chunk::sizeY) - 2,
                                 (p.x / Chunk::sizeX) + 2, (p.y / Chunk::sizeY) + 2));
}
