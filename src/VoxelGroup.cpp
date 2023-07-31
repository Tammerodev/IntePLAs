#include "VoxelGroup.hpp"

std::pair<bool, sf::FloatRect> VoxelGroup::getOvelapWithRect(const sf::FloatRect &collider)
{
    for(auto &r : rects) {
        if(collider.intersects(r)) {
            sf::FloatRect rect2 = r;
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
    return {false, sf::FloatRect()};
}

std::pair<bool, sf::FloatRect> VoxelGroup::getOvelapWithRectY(const sf::FloatRect &collider)
{
    for(auto &r : rects) {
        if(collider.intersects(r)) {
            return {true, sf::FloatRect(0,0,0,collider.height + (collider.top) - r.top)};
        }   
    }
    return {false, sf::FloatRect()};
}


int VoxelGroup::load(std::string file, bool proced)
{

    const std::string path = file;
    if(!img.loadFromFile(path)) {
        perror("Could not load voxel map");
    }
    tex.loadFromFile(path);

    world_sx = img.getSize().x;
    world_sy = img.getSize().y;

    prndd(world_sx);

    for(int i=0; i<img.getSize().y; i++)
    {
        std::vector <Voxel> voxRow;
        for(int j=0; j<img.getSize().x; j++)
        {
            voxRow.push_back(Voxel());
        }
        grid.push_back(voxRow);
    }

    for (int y = 0;y < world_sy;y++) {
        for (int x = 0;x < world_sx;x++) {
            const sf::Color px = img.getPixel(x,y);
            getVoxelAt(x,y) = getValueFromCol(px, sf::Vector2i(x,y));
        }
    }

    prndd("Processing complete");
    
    // load only the vertex shader
    (shader.loadFromMemory(shader_vert, sf::Shader::Vertex));
    // load only the fragment shader
    (shader.loadFromMemory(shader_frag, sf::Shader::Fragment));
    // load both shaders
    (shader.loadFromMemory(shader_vert, shader_frag));

    return true;
}

void VoxelGroup::heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp)
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

    sf::Color currPixel = getImagePixelAt(x,y);

    uint64_t valR = vox.temp * 1; 
    if(valR >= 255) valR = 255;
    currPixel.r = valR;
    setImagePixelAt(x,y,currPixel);
}

void VoxelGroup::render(sf::RenderTarget &target, const sf::Vector2f &center)
{
    target.draw(spr);
}

void VoxelGroup::resetUsedFlag()
{
    for (int y = 0;y < world_sy;y++) {
        for (int x = 0;x < world_sx;x++) {
            getVoxelAt(x,y).used = false;
        }
    }
}

void VoxelGroup::update()
{
    tex.update(img);
    spr.setTexture(tex);

    spr.setPosition(physicsComponent.transform_position);
    spr.setRotation(physicsComponent.transform_rotation);
    physicsComponent.update();

    world_sx = img.getSize().x;
    world_sy = img.getSize().y;
}

void VoxelGroup::merge()
{
rects.clear();

for (int y = 0; y < img.getSize().y;y++) {
    for (int x = 0; x < img.getSize().x;x++) {
        if (getVoxelAt(x,y).value != 0) {
            sf::FloatRect r;   
            r.left = x;
            r.top = y;
            r.height = 1;
            r.width = 1;
            rects.push_back(r);
        }
    }
}
}

void VoxelGroup::hole(const sf::Vector2i &p, const uint32_t& intensity, bool force, const int64_t heat)
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

}

const Voxel VoxelGroup::getValueFromCol(const sf::Color &px, sf::Vector2i p)
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