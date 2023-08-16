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


int VoxelGroup::load(const sf::Image &copy_img)
{
    img = copy_img;

    world_sx = img.getSize().x;
    world_sy = img.getSize().y;

    tex.create(world_sx,world_sy);

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

    return true;
}

void VoxelGroup::heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp)
{

    Voxel &vox = getVoxelAt(x,y);

    vox.temp += temp;
    if(vox.temp >= elm::getMaxTempFromType(vox.value)) {
        int val = vox.value;
        damageVoxelAt(x,y);
        
        if(val == elm::ValLithium) 
            hole(sf::Vector2i(x,y),elm::lithiumExplosion,true,2000);
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
    if(destroyed) return;

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
        if(getDestroyed()) return;
    

    tex.update(img);
    spr.setTexture(tex);

    physicsComponent.transform_origin = sf::Vector2f(tex.getSize().x / 2, tex.getSize().y / 2);

    spr.setPosition(physicsComponent.transform_position);
    spr.setRotation(physicsComponent.transform_rotation);
    spr.setOrigin(physicsComponent.transform_origin);
    physicsComponent.update();

    world_sx = img.getSize().x;
    world_sy = img.getSize().y;

    merge();
}

void VoxelGroup::merge()
{
if(getDestroyed()) return;

rects.clear();

for (int y = 0; y < img.getSize().y;y++) {
    for (int x = 0; x < img.getSize().x;x++) {
        if (getVoxelAt(x,y).value != 0) {
            Collider r;   

            // We have to rotate the voxel position around physicscomponent.transform origin

            sf::Vector2f p = physicsComponent.transform_position + sf::Vector2f(x,y);

            float sine = sin(math::degreesToRadians(physicsComponent.transform_rotation));
            float cosine = cos(math::degreesToRadians(physicsComponent.transform_rotation));

            // Translate back to origin ( We dont have to do this becouse voxel index x and y are already at origin)
            float xx = x - physicsComponent.transform_origin.x;
            float yy = y - physicsComponent.transform_origin.y;


            r.left = xx * cosine - yy * sine;
            r.top = xx * sine + yy * cosine;

            // Translate to world position
            r.left += physicsComponent.transform_position.x;
            r.top += physicsComponent.transform_position.y;


            r.height = 1;
            r.width = 1;
            rects.push_back(r);
        }
    }
}
}

void VoxelGroup::hole(const sf::Vector2i &pos, const uint32_t& intensity, bool force, const int64_t heat)
{
    if(getDestroyed()) return;

    if(force) {
        ExplosionInfo info;
        info.position = sf::Vector2f(pos);
        info.strength = intensity;
        explosion_points.push_back(info);
    }

    sf::Vector2i p = pos - sf::Vector2i(physicsComponent.transform_position);
    if(p.x <= 0) p.x = 0;
    if(p.y <= 0) p.y = 0;
    if(p.x >= world_sx) p.x = world_sx;
    if(p.y >= world_sy) p.y = world_sy;


    for (int y = 0;y < world_sy;y++) {
        for (int x = 0;x < world_sx;x++) {
            Voxel &voxel = getVoxelAt(x,y);
            const float distance = std::sqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y)));
            if(distance < intensity) {
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
        vox.strenght = 8;
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