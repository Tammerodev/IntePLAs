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

    for(unsigned int i = 0; i < img.getSize().y; i++)
    {
        std::vector <Voxel> voxRow;
        for(unsigned int j = 0; j < img.getSize().x; j++)
        {
            voxRow.push_back(Voxel());
        }
        grid.push_back(voxRow);
    }

    for (uint64_t y = 0; y < world_sy; y++) {
        for (uint64_t x = 0; x < world_sx; x++) {
            const sf::Color px = img.getPixel(x,y);
            getVoxelAt(x,y) = getValueFromCol(px, sf::Vector2i(x,y));
        }
    }

    physicsComponent.transform_origin = sf::Vector2f(tex.getSize().x / 2, tex.getSize().y / 2);
    return true;
}

void VoxelGroup::heatVoxelAt(int64_t x, int64_t y, int64_t temp)
{
    if(x <= 0) x = 0;
    if(y <= 0) y = 0;
    if(x >= (int)world_sx - 1) x = (int)world_sx - 1;
    if(y >= (int)world_sy - 1) y = (int)world_sy - 1;

    Voxel &vox = getVoxelAt(x,y);

    vox.temp += temp;
    if(vox.temp >= elm::getInfoFromType(vox.value).max_temp) {
        int val = vox.value;
        damageVoxelAt(x,y);
        
        if(val == VoxelValues::LITHIUM) 
            hole(sf::Vector2i(x,y), elm::lithiumExplosion, true, 2000);
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
    for (uint64_t y = 0; y < world_sy; y++) {
        for (uint64_t x = 0;x < world_sx;x++) {
            //TODO !!!! TODO!!!! getVoxelAt(x,y).used = false;
        }
    }
}

void VoxelGroup::update(const float dt)
{
    if(getDestroyed()) return;
    
    tex.update(img);
    spr.setTexture(tex);

    spr.setPosition(physicsComponent.transform_position);
    spr.setRotation(physicsComponent.transform_rotation);
    spr.setOrigin(physicsComponent.transform_origin);
    physicsComponent.update(dt);

    world_sx = img.getSize().x;
    world_sy = img.getSize().y;

    merge();
}

void VoxelGroup::merge()
{
    if(getDestroyed()) return;

    collisionTestPoints.clear();

    const std::array<const sf::Vector2i, 4> relativeTestPoints {
        sf::Vector2i(0, 0),
        sf::Vector2i(img.getSize().x, 0),
        sf::Vector2i(0, img.getSize().y),
        sf::Vector2i(img.getSize().x, img.getSize().y),

    };


    for(auto &relativeTestPosition : relativeTestPoints) {

        sf::Vector2i finalTestPoint {0,0};

        // We have to rotate the voxel position around physicscomponent.transform origin

        sf::Vector2f p = physicsComponent.transform_position + sf::Vector2f(relativeTestPosition.x, relativeTestPosition.y);

        float sine = sin(math::degreesToRadians(physicsComponent.transform_rotation));
        float cosine = cos(math::degreesToRadians(physicsComponent.transform_rotation));

        // Translate back to origin ( We dont have to do this becouse voxel index x and y are already at origin)
        float xx = relativeTestPosition.x - physicsComponent.transform_origin.x;
        float yy = relativeTestPosition.y - physicsComponent.transform_origin.y;

        finalTestPoint = sf::Vector2i(xx * cosine - yy * sine, xx * sine + yy * cosine);

        // Translate to world position
        finalTestPoint.x += physicsComponent.transform_position.x;
        finalTestPoint.y += physicsComponent.transform_position.y;

        collisionTestPoints.push_back(finalTestPoint);
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
    if(p.x >= (int)world_sx) p.x = (int)world_sx;
    if(p.y >= (int)world_sy) p.y = (int)world_sy;


    for (uint64_t y = 0;y < world_sy;y++) {
        for (uint64_t x = 0;x < world_sx;x++) {

            Voxel &voxel = getVoxelAt(x,y);
            const float distance = std::sqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y)));

            if(distance < intensity) {
                if(force) damageVoxelAt(x,y);
                heatVoxelAt(x,y, (intensity - distance)*heat);
            }
        }
    }

    const int numRays = intensity * 30;
    const int rayLength = intensity;


    int endX = pos.x - intensity;
    int endY = pos.y - intensity;

    Raycast::RaycastInfo info(nullptr);
    info.start = pos;
    info.intensity = intensity;

    info.world_sx = world_sx;
    info.world_sy = world_sy;

    info.velocity = &physicsComponent.velocity;


    for(;endX < pos.x + (int)intensity; endX++) {
        info.end = sf::Vector2i(endX, endY);
        Raycast::castRayLine(info);
    }

    for(;endY < pos.y + (int)intensity; endY++) {
        info.end = sf::Vector2i(endX, endY);
        Raycast::castRayLine(info);
    }

    for(;endX > pos.x - (int)intensity; endX--) {
        info.end = sf::Vector2i(endX, endY);
        Raycast::castRayLine(info);
    }

    for(;endY > pos.y - (int)intensity; endY--) {
        info.end = sf::Vector2i(endX, endY);
        Raycast::castRayLine(info);  
    }

}