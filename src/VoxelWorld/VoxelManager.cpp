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

int VoxelManager::load()
{
    prndd("Started loading map");

    
    const std::string path = "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/world/forest.png";
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
            if(px.a != 0) {
                getVoxelAt(x,y).value = 1;
                getValueFromCol(px, sf::Vector2i(x,y));

            } else {
                getVoxelAt(x,y).value = 0;
            }
        }
    }

    prndd("Loading map texture");
    world_tx.loadFromFile("res/world/forest.png");

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

void VoxelManager::hole(const sf::Vector2i &p, const uint32_t& intensity, bool recursive)
{
    ExplosionInfo info;
    info.position = sf::Vector2f(p);
    info.strength = intensity;
    explosion_points.push_back(info);

    int yexcept = p.y - intensity;
    int xexcept = p.x - intensity;

    if(yexcept < 0) yexcept = 0;
    if(xexcept < 0) xexcept = 0;

    for (int y = yexcept;y < p.y + intensity;y++) {
        if(p.y > world_sy) break;

        for (int x = xexcept;x < p.x + intensity;x++) {
            if(x >world_sx) break;
            if(getVoxelAt(x,y).value == 0) continue;
            if(math::isqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y))) < intensity) {
                if(recursive) {
                    switch (getVoxelAt(x,y).value) {
                        case 2:
                            // Recursive... I dont care
                            getVoxelAt(x,y).value = 0;
                            hole(p, 250, true);
                            return;
                        case 3:
                            // Recursive... I dont care
                            getVoxelAt(x,y).value = 0;
                            hole(p, 6, true);
                            return;
                        case 4: 
                            // Recursive... I dont care
                            getVoxelAt(x,y).strenght--;
                            if(getVoxelAt(x,y).strenght <= 0) getVoxelAt(x,y).value = 0;
                        break;
                        case 5:
                            // Recursive... I dont care
                            getVoxelAt(x,y).value = 0;
                            hole(p, 500, true);
                            return;
                        default:
                            getVoxelAt(x,y).value = 0;
                            break;
                    }
                }

            }
        }
    }
    merge();
}

void VoxelManager::build_image(const sf::Vector2i &p, const sf::Image &cimg)
{
    for (int y = p.y;  y < p.y + cimg.getSize().y;  y++) {
        if(p.y > world_sy) break;
        for (int x = p.x;  x < p.x + cimg.getSize().x;  x++) {
            if(x >world_sx) break;
            if(cimg.getPixel(x-p.x,y-p.y).a != 0) {
                getVoxelAt(x,y).value = 1;
                img.setPixel(x,y,cimg.getPixel(x-p.x,y-p.y));
                getValueFromCol(img.getPixel(x,y), sf::Vector2i(x,y));
            }

        }
    }
    merge();
}
