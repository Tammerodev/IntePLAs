#include "VoxelManager.hpp"

std::pair<bool, sf::FloatRect> VoxelManager::getOvelapWithRect(const sf::FloatRect &collider)
{
    int x = collider.left / Chunk::sizeX;
    int y = collider.top / Chunk::sizeY;
    int tox = 1;
    int toy = 1;
    while(x < 1) x++;
    while(y < 1) y++;
    while(x + tox >= chunks_x) tox--;
    while(y + toy >= chunks_y) tox--;

    for(int i = -1; i < toy; i++) {
    for(int j = -1; j < tox; j++) {
    for(auto &r : grid[x + j][y + i].rects) {
        if(collider.intersects(r.getGlobalBounds())) {
            sf::FloatRect rect1 = r.getGlobalBounds();
            sf::FloatRect rect2 = collider;

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


int VoxelManager::load(std::string file)
{
    prndd("Started loading map");

    
    const std::string path = file;
    if(!img.loadFromFile(path)) {
        perror("Could not load voxel map");
    }

    prndd("Updated map");

    prndd("Starting to process map");

    world_sx = Chunk::sizeX * chunks_x;
    world_sy = Chunk::sizeY * chunks_y;

    for (int y = 0;y < world_sy;y++) {
        for (int x = 0;x < world_sx;x++) {
            if(x >= img.getSize().x) continue;
            if(y >= img.getSize().y) continue;

            const sf::Color px = img.getPixel(x,y);
            getVoxelAt(x,y) = getValueFromCol(px, sf::Vector2i(x,y));
        }
    }

    prndd("Loading map texture");
    world_tx.loadFromFile(path);

    prndd("Processing complete");
    
    // load only the vertex shader
    (shader.loadFromMemory(shader_vert, sf::Shader::Vertex));
    // load only the fragment shader
    (shader.loadFromMemory(shader_frag, sf::Shader::Fragment));
    // load both shaders
    (shader.loadFromMemory(shader_vert, shader_frag));

    prndd("Shaders loaded");

    prndd("Starting meshing...");

    mergeChunkBounds(ChunkBounds(0, 0, chunks_y, chunks_y));


    grid[5][5].unload();
    
    
    return true;
}

void VoxelManager::heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp)
{

    Voxel &vox = getVoxelAt(x,y);

    vox.temp += temp;
    if(vox.temp >= vox.maxTemp) {
        int val = vox.value;
        damageVoxelAt(x,y);
        if(val == 3) 
            hole(sf::Vector2i(x,y),100,true,2000);
    }

    if(vox.temp <= 0) vox.temp = 0;

    sf::Color currPixel = img.getPixel(x,y);

    uint64_t valR = vox.temp * 1; 
    if(valR >= 255) valR = 255;
    currPixel.r = valR;
    img.setPixel(x,y,currPixel);
}

void VoxelManager::render(sf::RenderTarget &target, const sf::Vector2f &center)
{
    ChunkBounds draw_bounds((center.x / Chunk::sizeX) - 8, (center.y / Chunk::sizeY) - 8, 
                            (center.x / Chunk::sizeX) + 8, (center.y / Chunk::sizeY) + 8);
    ChunkArea draw_area = draw_bounds.getArea();

    for(uint32_t y = draw_area.startY; y < draw_area.endY; y++) {
        for(uint32_t x = draw_area.startX; x < draw_area.endX; x++) {
            for(auto &r : grid[x][y].rects)  {
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
        sf::Vector2i p = (*i);
        heatVoxelAt(p.x, p.y, -getVoxelAt(p.x,p.y).ambientDissipation);
        if(getVoxelAt(p.x,p.y).temp <= 0 || getVoxelAt(p.x,p.y).value == 0) {i = voxelsInNeedOfUpdate.erase(i); }
        else { ++i; }

    }

    world_tx.update(img);
}

void VoxelManager::merge()
{
world_tx.update(img);
for(auto c : mergeChunks) {
    grid[c.x][c.y].rects.clear();
    sf::Sprite r;
    r.setTexture(world_tx);
    for (int y = c.y * Chunk::sizeY; y < (c.y * Chunk::sizeY) + Chunk::sizeY;y++) {
    for (int x = c.x * Chunk::sizeX; x < (c.x * Chunk::sizeX) + Chunk::sizeX;x++) {

        if (getVoxelAt(x,y).value != 0 && !getVoxelAt(x,y).used) {
            int x1 = x;
            int y1 = y;

            while (getVoxelAt(x1,y1).value != 0 && !getVoxelAt(x1,y1).used && y1 < (c.y * Chunk::sizeY) + Chunk::sizeY) {
                y1++;
            }


            for (int y2 = y;y2 <= y1;y2++) {
                for (int x2 = x;
                    x2 <= x1;
                    x2++) {
                    getVoxelAt(x2,y2).used = true;
                }
            }
            x1++;
            y1++;
                    
            r.setPosition(x, y);
            r.setTextureRect(sf::IntRect(x,y,x1 -x,y1 -y));
            grid[c.x][c.y].rects.push_back(r);
        }
    }
    }
}
for(auto c : mergeChunks) {
    for (int y = c.y * Chunk::sizeY; y < (c.y * Chunk::sizeY) + Chunk::sizeY;y++) {
        for (int x = c.x * Chunk::sizeX; x < (c.x * Chunk::sizeX) + Chunk::sizeX;x++) {
            getVoxelAt(x,y).used = false;
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
            if(getVoxelAt(x,y).value == 0) continue;
            const float distance = math::isqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y)));
            if(distance < intensity) {
                voxelsInNeedOfUpdate.push_back(sf::Vector2i(x,y));
                if(force) damageVoxelAt(x,y);
                heatVoxelAt(x,y, (intensity - distance)*heat);
            }
        }
    }

    mergeChunkBounds(ChunkBounds((p.x / Chunk::sizeX) - 2, (p.y / Chunk::sizeX) - 2,
                                 (p.x / Chunk::sizeY) + 2, (p.y / Chunk::sizeY) + 2));
}

const Voxel VoxelManager::getValueFromCol(const sf::Color &px, sf::Vector2i p)
{
    Voxel vox = Voxel();
    vox.value = px.a != 0;

    if(px == elm::Carbon) {
        vox.value = 2;
        vox.maxTemp = 3550;
        vox.strenght = 8;
    } else if(px == elm::Lithium) {
        vox.value = 3;
        vox.maxTemp = 180.5;
        vox.strenght = 2;
    } else if(px == elm::Magnesium) {
        vox.value = 4;
        vox.maxTemp = 650;
        vox.strenght = 10;
    } else if(px == elm::Sodium) {
        vox.value = 5;
        vox.maxTemp = 97.8;
        vox.strenght = 1;
    } else if(px == elm::Aluminium) {
        vox.value = 6;
        vox.maxTemp = 660;
        vox.strenght = 5;
    } else if(px == elm::Silicon) {
        vox.value = 7;
        vox.maxTemp = 1410;
        vox.strenght = 6;
    } else if(px == elm::Copper) {
        vox.value = 8;
        vox.maxTemp = 1085;
        vox.strenght = 10;
    } else if(px == elm::Titanium) {
        vox.value = 9;
        vox.maxTemp = 1668;
        vox.strenght = 100;
    }
    else if(px == elm::Lead) {
        vox.value = 10;
        vox.maxTemp = 327;
        vox.strenght = 3;
    }

    return vox;
}

void VoxelManager::build_image(const sf::Vector2i &p, const sf::Image &cimg)
{
    for (int y = p.y;  y < p.y + cimg.getSize().y;  y++) {
        if(p.y > world_sy) break;
        for (int x = p.x;  x < p.x + cimg.getSize().x;  x++) {
            if(x > world_sx) break;
            if(cimg.getPixel(x-p.x,y-p.y).a != 0) {
                img.setPixel(x,y,cimg.getPixel(x-p.x, y-p.y));
                getVoxelAt(x,y) = getValueFromCol(img.getPixel(x,y), sf::Vector2i(x,y));
            }
        }
    }
    mergeChunkBounds(ChunkBounds((p.x / Chunk::sizeX) - 2, (p.y / Chunk::sizeY) - 2,
                                 (p.x / Chunk::sizeX) + 2, (p.y / Chunk::sizeY) + 2));
}