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
            if(img.getPixel(x,y).a != 0) {
                grid[x][y].value = 1;
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

            // TODO Texture
            if(indexX < rects.size()) {
                rects.at(indexX).setTexture(world_tx);
                rects.at(indexX).setPosition(x, y);
                rects.at(indexX).setTextureRect(sf::IntRect(x,y,(float)x1 - (float)x, (float)y1 - (float)y));
            } else {
                sf::Sprite r;
                r.setTexture(world_tx);
                r.setPosition(x, y);
                r.setTextureRect(sf::IntRect(x,y,(float)x1 - (float)x, (float)y1 - (float)y));
                rects.push_back(r);
            }
            indexX++;
        }
    }
}
update();
}

void VoxelManager::hole(const sf::Vector2i &p, uint32_t intensity)
{
    for (int y = 0;y < gw;y++) {
        for (int x = 0;x < gl;x++) {
            if(grid[x][y].value == 0) continue;
            if(sqrt(pow(p.x - x, 2) + ((p.y - y)*(p.y - y))) - math::randFloat()*10 < intensity) {
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
