#include "Hero.hpp"

// Init funtions
void Hero::initVariables()
{
    this->texture = new sf::Texture();
    this->sprite = new sf::Sprite();
    this->destiny = sf::Vector2i(0, 0);
    this->speed = 200.f;
}

void Hero::initShotSound()
{
    if (!shotBuffer.loadFromFile("Assets/Sounds/heroShot.ogg"))
    {
    }
    this->shotSound.setBuffer(shotBuffer);
    this->shotSound.setVolume(50);
}

void Hero::initAttributes()
{
    this->life = new Attribute(100, 100);
    this->ammunition = new Attribute(40, 40);
}

void Hero::initSprite(const char *src, sf::RenderWindow &window)
{
    if (!this->texture->loadFromFile(src))
    {
    }
    this->sprite->setTexture(*this->texture);
    sf::FloatRect bounds = this->sprite->getLocalBounds();
    this->sprite->setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    float posX = window.getSize().x / 2.0;
    float posY = window.getSize().y / 2.0;
    this->sprite->setPosition(sf::Vector2f(posX, posY));
    this->destiny = sf::Vector2i(posX, posY);
}

void Hero::showCollisionBox(sf::RenderWindow &window)
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
Hero::Hero(const char *src, sf::RenderWindow &window)
{
    this->initVariables();
    this->initShotSound();
    this->initAttributes();
    this->initSprite(src, window);
}

Hero::~Hero()
{
    delete this->texture;
    delete this->sprite;
    delete this->life;
    delete this->ammunition;
}

// Getters and Setters
sf::Vector2f Hero::getMouseDirection(sf::RenderWindow &window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f heroPos = this->sprite->getPosition();
    sf::Vector2f direction = sf::Vector2f(mousePos.x, mousePos.y) - heroPos;
    return direction;
}

sf::Vector2f Hero::getDestinyDirection()
{
    sf::Vector2f heroPos = this->sprite->getPosition();
    sf::Vector2f direction = sf::Vector2f(this->destiny.x, this->destiny.y) - heroPos;

    float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude > 0.25f) // adaptation due to garbage generated by float
    {
        direction /= magnitude;
    }
    else
    {
        return sf::Vector2f(0.f, 0.f);
    }
    return direction;
}

sf::Vector2f Hero::getPosition()
{
    return this->sprite->getPosition();
}

int Hero::getLife()
{
    return this->life->points();
}

int Hero::getMaxLife()
{
    return this->life->maxPoints();
}

int Hero::getAmmunition()
{
    return this->ammunition->points();
}

int Hero::getMaxAmmunition()
{
    return this->ammunition->maxPoints();
}

void Hero::setDestiny(sf::Vector2i destiny)
{
    this->destiny = destiny;
}

sf::FloatRect Hero::getArea()
{
    sf::FloatRect original = this->sprite->getGlobalBounds();

    float scale = 0.4f;
    float newWidth = original.width * scale;
    float newHeight = original.height * scale;

    float newLeft = original.left + (original.width - newWidth) / 2;
    float newTop = original.top + (original.height - newHeight) / 2;

    sf::FloatRect area(newLeft, newTop, newWidth, newHeight);

    return area;
}

void Hero::rechargeAmmunition(int value)
{   
    this->ammunition->recharge(value);
}

void Hero::rechargeLife(int value)
{
    this->life->recharge(value);
}

void Hero::reset(sf::RenderWindow &window)
{
    this->life->recharge(this->life->maxPoints());
    this->ammunition->recharge(this->ammunition->maxPoints());

    sf::FloatRect bounds = this->sprite->getLocalBounds();
    this->sprite->setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    float posX = window.getSize().x / 2.0;
    float posY = window.getSize().y / 2.0;
    this->sprite->setPosition(sf::Vector2f(posX, posY));
    this->destiny = sf::Vector2i(posX, posY);
}

// Actions
Shot *Hero::shot(const std::string &src, const sf::Vector2f &destiny)
{
    if (this->getAmmunition() <= 0)
    {
        return nullptr;
    }

    Shot *shot = new Shot(src, this->sprite->getPosition(), destiny);
    this->shotSound.play();
    this->ammunition->consume(1);
    return shot;
}
void Hero::damage(int points)
{
    this->life->consume(points);
}

// Update
void Hero::updateRotation(sf::Vector2f direction)
{
    float angleRad = atan2(direction.y, direction.x);
    float angleDeg = (angleRad * (180.f / static_cast<float>(M_PI))) + 90.f;
    this->sprite->setRotation(angleDeg);
}

void Hero::updatePosition(sf::Vector2f direction, float deltaTime)
{
    this->sprite->move(direction * this->speed * deltaTime);
}

void Hero::update(sf::RenderWindow &window, float deltaTime)
{
    sf::Vector2f directionMouse = this->getMouseDirection(window);
    sf::Vector2f directionDestiny = this->getDestinyDirection();
    this->updateRotation(directionMouse);
    this->updatePosition(directionDestiny, deltaTime);

    // Collision
}

// Render
void Hero::render(sf::RenderWindow &window)
{
    window.draw(*this->sprite);
    //showCollisionBox(window);
}
