#include "VoxelManager.hpp"

std::pair<bool,float> VoxelManager::checkCollisionsWith(const sf::FloatRect &collider)
{
    for(auto &r : rects) {
        if(collider.intersects(r.getGlobalBounds())) {
                return {true,r.getGlobalBounds().top - collider.top};
        }   
    }
    return {false,0.f};
}

std::pair<bool,float> VoxelManager::checkCollisionsWithInv(const sf::FloatRect &collider)
{
    for(auto &r : rects) {
        if(collider.intersects(r.getGlobalBounds())) {
            return {true,(r.getGlobalBounds().top + r.getGlobalBounds().height)- collider.top};
        }   
    }
    return {false,0.f};
}

std::pair<bool,float> VoxelManager::checkCollisionsWithLeft(const sf::FloatRect &collider)
{
    for(auto &r : rects) {
        if(collider.intersects(r.getGlobalBounds())) {
            return {true, collider.left + collider.width - r.getGlobalBounds().left};
        }   
    }
    return {false,0.f};
}

std::pair<bool,float> VoxelManager::checkCollisionsWithRight(const sf::FloatRect &collider)
{
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
    sf::Image img;
    if(!img.loadFromFile(path)) {
        perror("Could not load voxel map");
    }
    update();

    prndd("Updated map");

    prndd("Starting to process map");

    for (int y = 0;y < world_sy;y++) {
        for (int x = 0;x < world_sx;x++) {
            sf::Color px = img.getPixel(x,y); // Short variable name, we are gonna use this A LOT
            if(px.a != 0) {
                getVoxelAt(x,y).value = 1;
                     if(px.r == 34 && px.g == 196 && px.b == 34) getVoxelAt(x,y).value = 2;       // <-- Green explosibe thing
                else if(px.r == 204 && px.g == 223 && px.b == 223 ) getVoxelAt(x,y).value = 3;   // White explosive
                else if(px.r == 17 && px.g == 17 && px.b == 17 ) { getVoxelAt(x,y).value = 4; getVoxelAt(x,y).strenght = 5; }  // Stronk
                else if(px.r == 36 && px.g == 135 && px.b == 240) getVoxelAt(x,y).value = 5;       // <-- Nuclear bomb 36,135,240


            } else {
                getVoxelAt(x,y).value = 0;
            }
        }
    }

    prndd("Processing complete");

    world_tx.loadFromFile(path);
    world_spr.setTexture(world_tx);
    world_spr.setPosition(0,0);

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
}

void VoxelManager::merge()
{
update();

long long indexX = 0;
for (int y = 0;y < world_sy;y++) {
    for (int x = 0;x < world_sx;x++) {
        if (getVoxelAt(x,y).value != 0 && !getVoxelAt(x,y).used) {
            int x1 = x;
            int y1 = y;

            if (getVoxelAt(x1,y1).value != 0 && !getVoxelAt(x1,y1).used) {
                x1++;
            }
            x1--;
            int xc = x;

            while (getVoxelAt(x1,y1).value != 0 && !getVoxelAt(x1,y1).used) {
                for (int xx = x;
                    xx <= x1;
                    xx++) {
                    if (getVoxelAt(x1,y1).value == 0) goto out;
                }
                y1++;
            }
            out:

            y1--;

            for (int y2 = y;y2 <= y1;y2++) {
                for (int x2 = x;
                    x2 <= x1;
                    x2++) {
                    getVoxelAt(x2,y2).used = true;
                }
            }
            x1++;
            y1++;
                        
            sf::Sprite r;
            r.setTexture(world_tx);
            r.setPosition(x, y);
            r.setTextureRect(sf::IntRect(x,y,(float)x1 - (float)x, (float)y1 - (float)y));
            if(indexX < rects.size()) {
                rects.at(indexX) = r;
            } else rects.push_back(r);

            indexX++;
        }
    }
}
resetUsedFlag();
rects.erase(rects.begin() + indexX, rects.end());
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
                            hole(p, 6, false);
                            return;
                        case 4: 
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