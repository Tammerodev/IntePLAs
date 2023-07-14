#include "VoxelManager.hpp"

std::pair<bool,float> VoxelManager::checkCollisionsWith(const sf::FloatRect &collider)
{
    if(locked()) {
        for(auto &r : rects_copy) {
            if(collider.intersects(r.getGlobalBounds())) {
                    return {true,r.getGlobalBounds().top - collider.top};
            }   
        }
        return {false,0.f};
    }
    for(auto &r : rects) {
        if(collider.intersects(r.getGlobalBounds())) {
                return {true,r.getGlobalBounds().top - collider.top};
        }   
    }
    return {false,0.f};
}

std::pair<bool,float> VoxelManager::checkCollisionsWithInv(const sf::FloatRect &collider)
{
    if(locked()) {
        for(auto &r : rects_copy) {
            if(collider.intersects(r.getGlobalBounds())) {
                return {true,(r.getGlobalBounds().top + r.getGlobalBounds().height)- collider.top};
            }   
        }
        return {false,0.f};
    }
    for(auto &r : rects) {
        if(collider.intersects(r.getGlobalBounds())) {
            return {true,(r.getGlobalBounds().top + r.getGlobalBounds().height)- collider.top};
        }   
    }
    return {false,0.f};
}

std::pair<bool,float> VoxelManager::checkCollisionsWithLeft(const sf::FloatRect &collider)
{
    if(locked()) {
        for(auto &r : rects_copy) {
            if(collider.intersects(r.getGlobalBounds())) {
                return {true, collider.left + collider.width - r.getGlobalBounds().left};
            }   
        }
        return {false,0.f};
    }
    for(auto &r : rects) {
        if(collider.intersects(r.getGlobalBounds())) {
            return {true, collider.left + collider.width - r.getGlobalBounds().left};
        }   
    }
    return {false,0.f};
}

std::pair<bool,float> VoxelManager::checkCollisionsWithRight(const sf::FloatRect &collider)
{
    if(locked()) {
        for(auto &r : rects_copy) {
            if(collider.intersects(r.getGlobalBounds())) {
                return {true,-(collider.left + collider.width - r.getGlobalBounds().left)};
            }   
        }
        return {false,0.f};
    }
    for(auto &r : rects) {
        if(collider.intersects(r.getGlobalBounds())) {
            return {true,-(collider.left + collider.width - r.getGlobalBounds().left)};
        }   
    }
    return {false,0.f};
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

    world_sx = gx * chunks_x;
    world_sy = gy * chunks_y;

    for (int y = 0;y < world_sy;y++) {
        for (int x = 0;x < world_sx;x++) {
            const sf::Color px = img.getPixel(x,y); // Short variable name, we are gonna use this A LOT
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
    merge();

    return true;
}

void VoxelManager::heatVoxelAt(const uint64_t x, const uint64_t y, const float temp)
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

void VoxelManager::render(sf::RenderTarget &target)
{
    if(locked()) {
        for(auto &r : rects_copy)  {
            target.draw(r);
        }
        return;
    }
    for(auto &r : rects)  {
        target.draw(r);
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
    world_sx = gx * chunks_x;
    world_sy = gy * chunks_y;
    world_tx.update(img);

    auto i = voxelsInNeedOfUpdate.begin();
    while (i != voxelsInNeedOfUpdate.end())
    {
        bool del = false;
        sf::Vector2i p = (*i);
        heatVoxelAt(p.x, p.y, -getVoxelAt(p.x,p.y).ambientDissipation);
        if(getVoxelAt(p.x,p.y).temp <= 0 || getVoxelAt(p.x,p.y).value == 0) {i = voxelsInNeedOfUpdate.erase(i); }
        else { ++i; }
    }
}

void VoxelManager::merge(bool useChunks)
{

long long indexX = 0;

sf::Sprite r;
r.setTexture(world_tx);
if(locked()) {

for (int y = 0;y < world_sy;y++) {
for (int x = 0;x < world_sx;x++) {
    if (getVoxelAt(x,y).value != 0 && !getVoxelAt(x,y).used) {
        int x1 = x;
        int y1 = y;
        int xc = x;

        while (getVoxelAt(x1,y1).value != 0 && !getVoxelAt(x1,y1).used) {
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
        if(indexX < rects.size()) {
            rects.at(indexX) = r;
        } else rects.push_back(r);

        indexX++;
    }
}
}

resetUsedFlag();
rects.erase(rects.begin() + indexX, rects.end());
return;
}
for (int y = 0;y < world_sy;y++) {
for (int x = 0;x < world_sx;x++) {
    if (getVoxelAt(x,y).value != 0 && !getVoxelAt(x,y).used) {
        int x1 = x;
        int y1 = y;
        int xc = x;

        while (getVoxelAt(x1,y1).value != 0 && !getVoxelAt(x1,y1).used) {
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
        if(indexX < rects_copy.size()) {
            rects_copy.at(indexX) = r;
        } else rects_copy.push_back(r);

        indexX++;
    }
}
}

resetUsedFlag();
rects_copy.erase(rects_copy.begin() + indexX, rects_copy.end());

rects = rects_copy;

}

void VoxelManager::hole(const sf::Vector2i &p, const uint32_t& intensity, bool force, const uint32_t heat)
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
            if(x >world_sx) break;
            if(getVoxelAt(x,y).value == 0) continue;
            const float distance = math::isqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y)));
            if(distance < intensity) {
                voxelsInNeedOfUpdate.push_back(sf::Vector2i(x,y));
                if(force) damageVoxelAt(x,y);
                heatVoxelAt(x,y, (intensity - distance)*heat);
            }
        }
    }
    merge();
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
        recativeVoxels.push_back(sf::Vector2i(p));
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
            if(x >world_sx) break;
            if(cimg.getPixel(x-p.x,y-p.y).a != 0) {
                img.setPixel(x,y,cimg.getPixel(x-p.x,y-p.y));
                getVoxelAt(x,y) = getValueFromCol(img.getPixel(x,y), sf::Vector2i(x,y));
            }

        }
    }
    merge();
}
