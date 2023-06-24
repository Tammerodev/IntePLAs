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
            return {true,r.getGlobalBounds().left + r.getGlobalBounds().width - collider.left};
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
    const std::string path = "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/world/forest.png";
    sf::Image img;
    if(!img.loadFromFile(path)) {
        perror("Could not load voxel map");
    }

    for (int y = 0;y < gw;y++) {
        for (int x = 0;x < gl;x++) {
            sf::Color px = img.getPixel(x,y); // Short variable name, we are gonna use this A LOT
            if(px.a != 0) {
                grid[x][y].value = 1;
                if(px.r == 34 && px.g == 196 && px.b == 34) grid[x][y].value = 2;       // <-- Green explosibe thing
                else if(px.r == 204 && px.g == 223 && px.b == 223 ) grid[x][y].value = 3;
            } else {
                grid[x][y].value = 0;
            }
        }
    }

    world_tx.loadFromFile(path);
    world_spr.setTexture(world_tx);
    world_spr.setPosition(0,0);

    // load only the vertex shader
    (shader.loadFromMemory(shader_vert, sf::Shader::Vertex));
    // load only the fragment shader
    (shader.loadFromMemory(shader_frag, sf::Shader::Fragment));
    // load both shaders
    (shader.loadFromMemory(shader_vert, shader_frag));

    merge();

    return true;
}

void VoxelManager::render(sf::RenderTarget &target, sf::View &view)
{
    for(auto &r : rects)  {
        target.draw(r);
    }
}

void VoxelManager::update()
{
    for (int y = 0;y < gw;y++) {
        for (int x = 0;x < gl;x++) {
            grid[x][y].used = false;
        }
    }
}

void VoxelManager::merge()
{

long long indexX = 0;
for (int y = 0;y < gw;y++) {
    for (int x = 0;x < gl;x++) {
        if (grid[x][y].value != 0 && !grid[x][y].used) {
            int x1 = x;
            int y1 = y;

            if (grid[x1][y1].value != 0 && !grid[x1][y1].used) {
                x1++;
            }
            x1--;
            int xc = x;

            while (grid[x1][y1].value != 0 && !grid[x1][y1].used) {
                for (int xx = x;
                    xx <= x1;
                    xx++) {
                    if (grid[x1][y1].value == 0) goto out;
                }
                y1++;
            }
            out:

            y1--;

            for (int y2 = y;y2 <= y1;y2++) {
                for (int x2 = x;
                    x2 <= x1;
                    x2++) {
                    grid[x2][y2].used = true;
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
update();
rects.erase(rects.begin() + indexX, rects.end());
}

void VoxelManager::hole(const sf::Vector2i &p, const uint32_t& intensity)
{
    for (int y = p.y - intensity;y < p.y + intensity;y++) {
        for (int x = p.x - intensity;x < p.x + intensity;x++) {
            if(grid[x][y].value == 0) continue;
            else if(math::isqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y))) < intensity) {
                switch (grid[x][y].value)
                {
                case 2:
                    // Recursive... I dont care
                    grid[x][y].value = 0;
                    hole_not_recursive(p, 55);
                    break;
                case 3:
                    // Recursive... I dont care
                    grid[x][y].value = 0;
                    hole_not_recursive(p, 6);
                    return;
                default:
                    break;
                }
                
                grid[x][y].value = 0;
                
            }
        }
    }
    merge();
}

void VoxelManager::hole_not_recursive(const sf::Vector2i &p, const uint32_t& intensity)
{
    for (int y = p.y - intensity;y < p.y + intensity;y++) {
            for (int x = p.x - intensity;x < p.x + intensity;x++) {
            if(grid[x][y].value == 0) continue;
            if(math::isqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y))) < intensity) {
                grid[x][y].value = 0; 
            }
        }
    }
    merge();
}

void VoxelManager::showDebug()
{
    debug = true;
    for(auto &r : rects) {
        // TODO
    }
}

void VoxelManager::hideDebug()
{
    debug = false;
    for(auto &r : rects) {
        // TODO
    }
}
