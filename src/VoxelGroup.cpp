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

    voxelGroupID = VoxelGroupIDGenerator::getNextID();
    prndd(voxelGroupID);

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

    tex.update(img);
    spr.setTexture(tex);

    rigidBody.loadFromRectangle(spr.getGlobalBounds());
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
    rigidBody.debugRender(target);
}

void VoxelGroup::resetUsedFlag()
{
    for (uint64_t y = 0; y < world_sy; y++) {
        for (uint64_t x = 0;x < world_sx;x++) {
            //TODO !!!! TODO!!!! getVoxelAt(x,y).used = false;
        }
    }
}

void VoxelGroup::update(ChunkIndexer& world, const float dt)
{
    if(getDestroyed()) return;

    VoxelGroupIDGenerator::checkID(voxelGroupID);

    if(Controls::isPressed(sf::Keyboard::H))
        setVelocity({0.f, -5.f});
    if(Controls::isPressed(sf::Keyboard::G))
        setVelocity({-5.f, 1.f});

    tex.update(img);
    spr.setTexture(tex);

    rigidBody.setRect(spr.getGlobalBounds());
    rigidBody.calculatePoints(world);

    rigidBody.setSpritePoints(spr);

    world_sx = img.getSize().x;
    world_sy = img.getSize().y;
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


    sf::Vector2i p = pos - sf::Vector2i(spr.getPosition());
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

    //TODO: info.velocity = &.velocity;


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