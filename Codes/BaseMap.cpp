#include "BaseMap.h"

void BaseMap::draw(sf::RenderWindow& win) const
{
    for (const auto& plat : platforms)
    {
        win.draw(plat);
    }
}

void BaseMap::addPlatform(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color)
{
    sf::RectangleShape shape;
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(color);
    platforms.push_back(shape);
}

void BaseMap::load(int mapID) {
    platforms.clear();

    if (mapID == 1)
    {
        addPlatform({ 248 * 5.f, 8 * 5.f }, { 0.f, 0.f }, sf::Color(185, 122, 87)); 
        addPlatform({ 72 * 5.f, 8 * 5.f }, { 0.f, 8 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 72 * 5.f, 8 * 5.f }, { 104 * 5.f, 8 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 56 * 5.f }, { 0.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 40 * 5.f, 40 * 5.f }, { 32 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 40 * 5.f, 8 * 5.f }, { 104 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
		addPlatform({ 24 * 5.f, 24 * 5.f }, { 104 * 5.f, 24 * 5.f }, sf::Color(185, 122, 87));
		addPlatform({ 8 * 5.f, 16 * 5.f }, { 168 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 72 * 5.f }, { 0.f, 72 * 5.f }, sf::Color(188, 122, 87));
        addPlatform({ 232 * 5.f, 16 * 5.f }, { 0.f, 168 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 40 * 5.f, 24 * 5.f }, { 0.f, 144 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 152 * 5.f, 8 * 5.f }, { 80 * 5.f, 160 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 128 * 5.f, 8 * 5.f }, { 80 * 5.f, 152 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 24 * 5.f }, { 80 * 5.f, 128 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 64 * 5.f, 24 * 5.f }, { 144.f * 5.f, 128 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 32 * 5.f, 48 * 5.f }, { 144.f * 5.f, 80 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 184 * 5.f }, { 312.f * 5.f, 0.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 152 * 5.f }, { 304.f * 5.f, 0.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 144 * 5.f }, { 296.f * 5.f, 0.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 128 * 5.f }, { 288.f * 5.f, 0.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 80 * 5.f }, { 280.f * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 40 * 5.f }, { 272.f * 5.f, 56 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 24 * 5.f }, { 248.f * 5.f, 56 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 8 * 5.f }, { 272.f * 5.f, 32 * 5.f }, sf::Color(185, 122, 87));
    }
    else if (mapID == 2)
    {
        addPlatform({ 272 * 5.f, 8 * 5.f }, { 0.f, 0.f }, sf::Color(185, 122, 87));
        addPlatform({ 80 * 5.f, 24 * 5.f }, { 0.f, 8 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 72 * 5.f, 32 * 5.f }, { 0.f, 32 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 32 * 5.f }, { 0.f, 64 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 32 * 5.f }, { 112 * 5.f, 8 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 144 * 5.f, 8 * 5.f }, { 128 * 5.f, 8 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 40 * 5.f, 24 * 5.f }, { 160 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 32 * 5.f, 8 * 5.f }, { 168 * 5.f, 40 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 24 * 5.f }, { 168 * 5.f, 48 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 56 * 5.f, 8 * 5.f }, { 216 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 40 * 5.f, 24 * 5.f }, { 232 * 5.f, 24 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 32 * 5.f }, { 256 * 5.f, 48 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 16 * 5.f }, { 264 * 5.f, 80 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 64 * 5.f }, { 0.f, 96 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 128 * 5.f, 24 * 5.f }, { 0.f, 160 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 16 * 5.f }, { 112 * 5.f, 144 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 24 * 5.f }, { 160 * 5.f, 112 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 72 * 5.f }, { 168 * 5.f, 112 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 16 * 5.f }, { 192 * 5.f, 112 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 8 * 5.f }, { 192 * 5.f, 160 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 128 * 5.f, 16 * 5.f }, { 192 * 5.f, 168 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 96 * 5.f, 24 * 5.f }, { 224 * 5.f, 144 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 64 * 5.f, 24 * 5.f }, { 256 * 5.f, 120 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 120 * 5.f }, { 304 * 5.f, 0.f }, sf::Color(185, 122, 87));
    }
    else if (mapID == 3)
    {
		addPlatform({ 264 * 5.f, 16 * 5.f }, { 0.f, 0.f }, sf::Color(185, 122, 87));
        addPlatform({ 48 * 5.f, 16 * 5.f }, { 0.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 16 * 5.f }, { 0.f, 32 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 16 * 5.f }, { 120 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 80 * 5.f, 8 * 5.f }, { 136 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 16 * 5.f }, { 208 * 5.f, 24 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 8 * 5.f }, { 248 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 8 * 5.f }, { 256 * 5.f, 24 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 16 * 5.f }, { 120 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 104 * 5.f }, { 0.f, 48 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 32 * 5.f }, { 0.f, 152 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 56 * 5.f, 16 * 5.f }, { 16 * 5.f, 168 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 56 * 5.f }, { 296 * 5.f, 0.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 40 * 5.f }, { 304 * 5.f, 56 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 56 * 5.f }, { 312 * 5.f, 96 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 136 * 5.f, 24 * 5.f }, { 184 * 5.f, 160 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 24 * 5.f }, { 208 * 5.f, 136 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 40 * 5.f, 16 * 5.f }, { 232 * 5.f, 144 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 8 * 5.f }, { 304 * 5.f, 152 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 16 * 5.f }, { 72 * 5.f, 64 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 24 * 5.f }, { 88 * 5.f, 80 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 16 * 5.f }, { 128 * 5.f, 96 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 32 * 5.f, 24 * 5.f }, { 128 * 5.f, 112 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 16 * 5.f }, { 144 * 5.f, 136 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 8 * 5.f }, { 152 * 5.f, 152 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 24 * 5.f }, { 248 * 5.f, 64 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 24 * 5.f }, { 248 * 5.f, 88 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 8 * 5.f }, { 256 * 5.f, 112 * 5.f }, sf::Color(185, 122, 87));
    }
    else if (mapID == 4)
    {
        addPlatform({ 168 * 5.f, 16 * 5.f }, { 0.f, 0.f }, sf::Color(185, 122, 87));
        addPlatform({ 96 * 5.f, 8 * 5.f }, { 0.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 48 * 5.f, 8 * 5.f }, { 0.f, 24 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 40 * 5.f, 8 * 5.f }, { 0.f, 32 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 144 * 5.f }, { 0.f, 40 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 40 * 5.f }, { 16 * 5.f, 144 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 72 * 5.f }, { 16 * 5.f, 40 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 64 * 5.f, 24 * 5.f }, { 24 * 5.f, 160 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 24 * 5.f }, { 144 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 16 * 5.f }, { 160 * 5.f, 40 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 120 * 5.f, 16 * 5.f }, { 200 * 5.f, 0.f }, sf::Color(185, 122, 87));
        addPlatform({ 80 * 5.f, 8 * 5.f }, { 200 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 40 * 5.f, 8 * 5.f }, { 216 * 5.f, 24 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 32 * 5.f, 24 * 5.f }, { 224 * 5.f, 32 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 8 * 5.f }, { 224 * 5.f, 56 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 32 * 5.f }, { 232 * 5.f, 64 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 32 * 5.f, 24 * 5.f }, { 288 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 64 * 5.f }, { 304 * 5.f, 40 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 80 * 5.f }, { 312 * 5.f, 104 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 32 * 5.f, 8 * 5.f }, { 64 * 5.f, 72 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 16 * 5.f }, { 64 * 5.f, 80 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 32 * 5.f }, { 72 * 5.f, 96 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 16 * 5.f }, { 80 * 5.f, 112 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 8 * 5.f }, { 88 * 5.f, 112 * 5.f }, sf::Color(185, 122, 87));
    }
    else if (mapID == 5)
    {
        addPlatform({ 176 * 5.f, 8 * 5.f }, { 0.f, 0.f }, sf::Color(185, 122, 87));
        addPlatform({ 112 * 5.f, 16 * 5.f }, { 208 * 5.f, 0.f }, sf::Color(185, 122, 87));
        addPlatform({ 48 * 5.f, 8 * 5.f }, { 0.f, 8 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 8 * 5.f }, { 152 * 5.f, 8 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 32 * 5.f, 24 * 5.f }, { 0.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 64 * 5.f }, { 0.f, 40 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 80 * 5.f }, { 0.f, 104 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 24 * 5.f }, { 8 * 5.f, 160 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 8 * 5.f }, { 24 * 5.f, 152 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 176 * 5.f, 16 * 5.f }, { 32 * 5.f, 168 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 16 * 5.f }, { 120 * 5.f, 152 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 32 * 5.f, 8 * 5.f }, { 288 * 5.f, 16 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 16 * 5.f }, { 304 * 5.f, 24 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 144 * 5.f }, { 312 * 5.f, 40 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 16 * 5.f }, { 144 * 5.f, 48 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 8 * 5.f }, { 152 * 5.f, 64 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 48 * 5.f, 48 * 5.f }, { 160 * 5.f, 48 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 8 * 5.f }, { 184 * 5.f, 40 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 24 * 5.f, 8 * 5.f }, { 184 * 5.f, 96 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 16 * 5.f, 16 * 5.f }, { 184 * 5.f, 104 * 5.f }, sf::Color(185, 122, 87));
        addPlatform({ 8 * 5.f, 16 * 5.f }, { 192 * 5.f, 120 * 5.f }, sf::Color(185, 122, 87));
    }
}