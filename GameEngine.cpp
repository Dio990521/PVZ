#include "GameEngine.h"

void GameEngine::initWindow()
{
    std::ifstream ifs("Configs/window.ini");

    std::string title = "None";
    sf::VideoMode windowBounds(800, 600);
    unsigned frameRateLimit = 120;
    bool verticalSyncEnabled = false;
    if (ifs.is_open())
    {
        std::getline(ifs, title);
        ifs >> windowBounds.width >> windowBounds.height;
        ifs >> frameRateLimit;
        ifs >> verticalSyncEnabled;
    }
    ifs.close();

	this->window = new sf::RenderWindow(windowBounds, title);
    this->window->setFramerateLimit(frameRateLimit);
    this->window->setVerticalSyncEnabled(verticalSyncEnabled);
}

void GameEngine::initStates()
{
    this->states.push(new GameState(this->window));
}

GameEngine::GameEngine()
{
    this->initWindow();
    this->initStates();
}

GameEngine::~GameEngine() 
{
	delete this->window;

    while (!this->states.empty())
    {
        delete this->states.top();
        this->states.pop();
    }
        
}

void GameEngine::endApplication()
{
    std::cout << "End! fuck!" << std::endl;
}

/*Update the deltaTime with the time it takes to update and render one frame*/
void GameEngine::updateDeltaTime()
{
    this->deltaTime = this->deltaTimeClock.restart().asSeconds();
}

void GameEngine::updateSFMLEvents()
{
    while (this->window->pollEvent(this->sfEvent)) 
    {
        if (this->sfEvent.type == sf::Event::Closed)
            this->window->close();
    }
}

void GameEngine::update()
{
    this->updateSFMLEvents();

    if (!this->states.empty())
    {
        this->states.top()->update(this->deltaTime);
        if (this->states.top()->getQuit())
        {
            this->states.top()->exit();
            delete this->states.top();
            this->states.pop();
        }
    }
    else // Application end
    {
        this->endApplication();
        this->window->close();
    }
    

}

void GameEngine::render()
{
    this->window->clear();

    if (!this->states.empty()) 
        this->states.top()->render(this->window);

    this->window->display();
}

void GameEngine::run()
{
    while (this->window->isOpen())
    {
        this->updateDeltaTime();
        this->update();
        this->render();
    }
}
