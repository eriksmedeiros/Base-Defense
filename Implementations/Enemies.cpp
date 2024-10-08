#include "Enemies.hpp"

// Utility functions
sf::Vector2f generateRandomPosition(sf::RenderTarget &target, float margin = 0.f)
{
    int side = rand() % 4;
    float x, y;
    switch (side)
    {
    case 0: // Top
        x = static_cast<float>(rand() % static_cast<int>(target.getSize().x + 2 * margin)) - margin;
        y = -margin;
        break;
    case 1: // Right
        x = target.getSize().x + margin;
        y = static_cast<float>(rand() % static_cast<int>(target.getSize().y + 2 * margin)) - margin;
        break;
    case 2: // Bottom
        x = static_cast<float>(rand() % static_cast<int>(target.getSize().x + 2 * margin)) - margin;
        y = target.getSize().y + margin;
        break;
    case 3: // Left
        x = -margin;
        y = static_cast<float>(rand() % static_cast<int>(target.getSize().y + 2 * margin)) - margin;
        break;
    }
    sf::Vector2f position(x, y);
    return position;
}

// Init functions
void Enemies::initVariables(float enemySpeed)
{
    this->sprite = new sf::Sprite();
    this->enemySpeed = enemySpeed;
    this->shotInterval = 3.0f;
}

void Enemies::initSprite(const std::vector<const char*>& srcs, sf::RenderTarget &target)
{
    for (const auto& src : srcs){
        sf::Texture* texture = new sf::Texture();

        if (!texture->loadFromFile(src))
        {
        }

        this->textures.push_back(texture);
    }

    this->sprite->setTexture(*this->textures[0]);

    sf::Vector2f position = generateRandomPosition(target, 60.f);
    this->sprite->setPosition(position);
    sf::FloatRect bounds = this->sprite->getLocalBounds();
    this->sprite->setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Enemies::showCollisionBox(sf::RenderWindow &window)
{
    sf::FloatRect shape = getArea();
    sf::RectangleShape hitbox(sf::Vector2f(shape.width, shape.height));

    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(2);
    hitbox.setPosition(shape.left, shape.top);

    window.draw(hitbox);
}

// Constructors and Destructors
Enemies::Enemies(const std::vector<const char*>& srcs, sf::RenderWindow &window, Hero *hero, float enemySpeed)
{
    this->initVariables(enemySpeed);
    this->initSprite(srcs, window);
    this->hero = hero;
    this->shotClock.restart();
}

Enemies::~Enemies()
{
    for (auto& texture : this->textures){
        delete texture;
    }

    delete this->sprite;
}

// Public functions
void Enemies::update(sf::RenderWindow &window, float deltaTime)
{
    this->moveTowardsHero(deltaTime);
    this->updateWalkingAnimation(deltaTime);
}

void Enemies::render(sf::RenderWindow &window)
{
    window.draw(*this->sprite);
    //showCollisionBox(window);
}

void Enemies::moveTowardsHero(float deltaTime)
{
    sf::Vector2f heroPos = this->hero->getPosition();
    sf::Vector2f enemyPos = this->sprite->getPosition();
    sf::Vector2f direction = heroPos - enemyPos;

    float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
    if (length != 0)
    {
        direction /= length;
        this->sprite->move(direction * this->enemySpeed * deltaTime);
    }
}

void Enemies::updateWalkingAnimation(float deltaTime)
{
    animationTime += deltaTime;

    if (animationTime >= timePerFrame) {
        animationTime -= timePerFrame;

        currentTextureIndex = (currentTextureIndex + 1) % textures.size();

        sprite->setTexture(*textures[currentTextureIndex]);
    }
}

Shot *Enemies::shot(const std::string &src, const sf::Vector2f &destiny, Enemies *owner, float speed)
{
    return new Shot(src, this->sprite->getPosition(), destiny, owner, speed);
}

// Getters
sf::Clock &Enemies::getShotClock()
{
    return this->shotClock;
}

float Enemies::getShotInterval()
{
    return this->shotInterval;
}

sf::FloatRect Enemies::getArea()
{
    if (!this->sprite)
    {
        return sf::FloatRect();
    }

    float reductionFactor = 0.6f;

    sf::FloatRect originalBounds = this->sprite->getGlobalBounds();
    sf::FloatRect reducedBounds = originalBounds;
    reducedBounds.width *= reductionFactor;
    reducedBounds.height *= reductionFactor;

    reducedBounds.left += (originalBounds.width - reducedBounds.width) / 2.0f;
    reducedBounds.top += (originalBounds.height - reducedBounds.height) / 2.0f;

    return reducedBounds;
}
