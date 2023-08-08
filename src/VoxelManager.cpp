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
    for(auto &r : grid[x][y].rects) {
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
    for(auto &r : grid[x][y].rects) {
        if(collider.intersects(r.getGlobalBounds())) {
            return {true, sf::FloatRect(0,0,0,collider.height + (collider.top) - r.getGlobalBounds().top)};
        }   
    }
    }
    }
    return {false, sf::FloatRect()};
}


int VoxelManager::load(std::string file, bool proced)
{
    bool res = true;

    sf::Image img;

    world_sx = Chunk::sizeX * chunks_x;
    world_sy = Chunk::sizeY * chunks_y;

    img.create(world_sx, world_sy, sf::Color(0,0,0,0));

    prndd("WORLD X"); prndd(world_sx);
    prndd("WORLD Y"); prndd(world_sy);

    for (int y = 0;y < world_sy;y++) {
        for (int x = 0;x < world_sx;x++) {
            const sf::Color px = img.getPixel(x,y);
            getVoxelAt(x,y) = getValueFromCol(px, sf::Vector2i(x,y));
        }
    }
    
    // load only the vertex shader
    if(!shader.loadFromMemory(shader_vert, sf::Shader::Vertex)) res = false;
    // load only the fragment shader
    if(!shader.loadFromMemory(shader_frag, sf::Shader::Fragment)) res = false;
    // load both shaders
    if(!shader.loadFromMemory(shader_vert, shader_frag)) res = false;
    
    ChunkBounds bounds = ChunkBounds(0, 0, chunks_x, chunks_y);

    for(uint32_t y = bounds.getArea().startY; y < bounds.getArea().endY; y++) {
        for(uint32_t x = bounds.getArea().startX; x < bounds.getArea().endX; x++) {
            grid[x][y].create(img, sf::Vector2i(x,y));
        }
    }

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
        if(val == elm::ValLithium) 
            hole(sf::Vector2i(x,y),100,true,2000);
    }

    if(vox.temp <= 0) vox.temp = 0;

    sf::Color currPixel = getImagePixelAt(x,y);

    uint64_t valR = vox.temp * 1; 
    if(valR >= 255) valR = 255;
    currPixel.r = valR;
    setImagePixelAt(x,y,currPixel);
}

void VoxelManager::render(sf::RenderTarget &target, const sf::Vector2f &center)
{
    ChunkBounds draw_bounds((center.x / Chunk::sizeX) - 13, (center.y / Chunk::sizeY) - 13, 
                            (center.x / Chunk::sizeX) + 13, (center.y / Chunk::sizeY) + 13);
    ChunkArea draw_area = draw_bounds.getArea();

    for(uint32_t y = draw_area.startY; y < draw_area.endY; y++) {
        for(uint32_t x = draw_area.startX; x < draw_area.endX; x++) {
            grid[x][y].update();
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
        heatVoxelAt(p.x, p.y, -elm::getAmbientDissipationFromType(getVoxelAt(p.x,p.y).value));

        if(getVoxelAt(p.x,p.y).temp <= 0 || getVoxelAt(p.x,p.y).value == 0) {i = voxelsInNeedOfUpdate.erase(i); }
        else { ++i; }

    }


    merge();

}

void VoxelManager::merge()
{

for(auto c : mergeChunks) {
    grid[c.x][c.y].rects.clear();
    sf::Sprite r;
    r.setTexture(grid[c.x][c.y].tx);
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
            r.setTextureRect(sf::IntRect(x - (c.x * Chunk::sizeX),y - (c.y * Chunk::sizeY),  x1 - x,  y1 -y));
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

    mergeChunkBounds(ChunkBounds((p.x / Chunk::sizeX) - 2, (p.y / Chunk::sizeX) - 2,
                                 (p.x / Chunk::sizeY) + 2, (p.y / Chunk::sizeY) + 2));

}

bool VoxelManager::generate()
{
    std::array<sf::Color, 6> colr {
        sf::Color(50, 168, 82),
        elm::Carbon,
        elm::Carbon,
        elm::Lead,
        elm::Titanium,
        elm::Aluminium
    };

    const float val = math::randFloat() * 3;

    for(int x = 0; x <= world_sx - 10; x++ ) {
        const float fx = x / 400.0;
        hmap1D.push_back(abs(1000+((sin(2*fx) + sin(val * fx)) * 50.0)));
    }
    
    int ind = 0;
    for(auto h : hmap1D) {
        for(int i = world_sy - 1; i >= 2048 + h; i--) {
           // int offset = (rand() % 100 + 100);
           // int colorIndex = std::clamp(((i - (int)h) - offset) / 200, 0, (int)colr.size() - 1);
           // sf::Color col = colr.at(colorIndex);
            setImagePixelAt(ind, i, sf::Color::Red);
            getVoxelAt(ind, i) = getValueFromCol( sf::Color::Red, sf::Vector2i(ind, i));
        }
        ind++;
    }

    ChunkBounds bounds(0,0, chunks_x, chunks_y);
    

    mergeChunkBounds(bounds);

    return true;
}

bool VoxelManager::generateVegetation()
{
    sf::Image vege;
    if(!vege.loadFromFile("res/img/Proc.png")) return false;

    int ind = 0;
    for(auto h : hmap1D) {
        if(math::randIntInRange(0,100) > 50) {
            enum VegeType {
                Grass, Tree
            } vegType;
            
            sf::IntRect sourceRect; 

            if(math::randIntInRange(0, 1-50) < 1) {
                // Tree
                sourceRect = sf::IntRect(0, 20, 70, 94);
                vegType = Tree;
            } else {
                // Grass
                sourceRect = sf::IntRect(10 * math::randIntInRange(0,10), 0, 10, 10);
                vegType = Grass;
            }

            sf::Image extractedImage;
            extractedImage.create(sourceRect.width, sourceRect.height);
            
            for (int y = 0; y < sourceRect.height; ++y) {
                for (int x = 0; x < sourceRect.width; ++x) {   
                    sf::Color pixel = vege.getPixel(sourceRect.left + x, sourceRect.top + y);
                    extractedImage.setPixel(x, y, pixel);
                }
            }
            sf::Vector2i point;

            if(vegType == Grass)
                point = sf::Vector2i(ind, (2048 - h) - math::randIntInRange(0,6));
            else if(vegType == Tree)
                point = sf::Vector2i(ind, (2048 - h) - sourceRect.height);

            build_image(point, extractedImage, nullptr);
        }
        ind++;
    }
    return true;
}

const Voxel VoxelManager::getValueFromCol(const sf::Color &px, sf::Vector2i p)
{
    Voxel vox = Voxel();
    vox.value = px.a != 0;

    if(px == elm::Carbon) {
        vox.value = 2;
        vox.strenght = 254; // TODO change to 8
    } else if(px == elm::Lithium) {
        vox.value = 3;
        vox.strenght = 2;
    } else if(px == elm::Magnesium) {
        vox.value = 4;
        vox.strenght = 10;
    } else if(px == elm::Sodium) {
        vox.value = 5;
        vox.strenght = 1;
    } else if(px == elm::Aluminium) {
        vox.value = 6;
        vox.strenght = 5;
    } else if(px == elm::Silicon) {
        vox.value = 7;
        vox.strenght = 6;
    } else if(px == elm::Copper) {
        vox.value = 8;
        vox.strenght = 10;
    } else if(px == elm::Titanium) {
        vox.value = 9;
        vox.strenght = 100;
    }
    else if(px == elm::Lead) {
        vox.value = 10;
        vox.strenght = 3;
    }

    return vox;
}

void VoxelManager::build_image(const sf::Vector2i &p, const sf::Image &cimg, std::vector<VoxelGroup>* grp, float angle, float mag)
{
    if(grp != nullptr) {
        VoxelGroup object = VoxelGroup();
        object.load(cimg);
        object.getPhysicsComponent().transform_position = sf::Vector2f(p);

        float angleRadians = angle * static_cast<float>(M_PI) / 180.0f;
        object.getPhysicsComponent().velocity.x = mag * cos(angleRadians);
        object.getPhysicsComponent().velocity.y = mag * sin(angleRadians);

        grp->push_back(object);
        return;
    }
    for (int y = p.y;  y < p.y + cimg.getSize().y;  y++) {
        if(y >= world_sy) break;
        for (int x = p.x;  x < p.x + cimg.getSize().x;  x++) {
            if(x >= world_sx) break;
            if(cimg.getPixel(x-p.x,y-p.y).a != 0) {
                setImagePixelAt(x,y,cimg.getPixel(x-p.x, y-p.y));
                getVoxelAt(x,y) = getValueFromCol(getImagePixelAt(x,y), sf::Vector2i(x,y));
            }
        }
    }

    mergeChunkBounds(ChunkBounds((p.x / Chunk::sizeX) - 2, (p.y / Chunk::sizeY) - 2,
                                 (p.x / Chunk::sizeX) + 2, (p.y / Chunk::sizeY) + 2));
}
