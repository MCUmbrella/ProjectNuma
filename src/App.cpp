//
// Created by mcumbrella on 23-5-8.
//

#include "App.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "Session.h"
#include "Random.h"
#include "entity/Player.h"
#include "entity/enemy/Enemies.h"
#include "weapon/Weapon.h"
#include "weapon/player/PlayerWeapons.h"
#include "weapon/enemy/EnemyWeapons.h"
#include "CommonUtil.h"

map<string, Weapon*> weapons;
list<shared_ptr<Entity>> entities;
list<shared_ptr<Entity>> environment;
vector<UIComponent*> ui;

void App::startup(int argc, char** argv)
{
    cerr << endl << VERSION << endl
         << "Made by @MCUmbrella, licensed under MIT" << endl
         << "https://github.com/MCUmbrella/ProjectNuma" << endl << endl;
    string arg;
    for (int i = 1; i != argc; i++)
    {
        arg = argv[i];
        if (arg == "-h")
        {
            cerr << "\t'-h': Show this help message" << endl
                 << "\t'-d': Enable debug mode" << endl
                 << "\t'-f': Try to run the game in fullscreen" << endl
                 << "\t'-s': Force use software rendering" << endl;
            return;
        }
        else if (arg == "-d") flDebug = true;
        else if (arg == "-f") flFullscreen = true;
        else if (arg == "-s") flSoftwareRender = true;
    }

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "The game is coming up. Please wait");
    if (flDebug)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Debug mode enabled");
        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
    }
    r = new Random();
    session = new Session();
    renderManager = new RenderManager();
    soundManager = new SoundManager();
    renderManager->init();
    soundManager->init();
    // initialize weapons
    weapons.emplace("PlayerWeapon0", new PlayerWeapon0());
    weapons.emplace("PlayerWeapon1", new PlayerWeapon1());
    weapons.emplace("PlayerWeapon2", new PlayerWeapon2());
    weapons.emplace("PlayerWeapon3", new PlayerWeapon3());
    weapons.emplace("EnemyWeapon0", new EnemyWeapon0());
    weapons.emplace("EnemyWeapon1", new EnemyWeapon1());
    weapons.emplace("EnemyWeapon2", new EnemyWeapon2());
    // load session
    App::getInstance().getSession().load();
    // initialize player
    player = new Player();
    addEntity(player);
    player->maxHp = App::getInstance().getSession().hp;
    player->hp = App::getInstance().getSession().hp;
    // completed
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "The game is ready");
    state = STATE_MENU;
    mainLoop();
}

void App::shutdown()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "The game is coming down. Please wait");
    // shutdown sdl
    soundManager->shutdown();
    renderManager->shutdown();
    // save session
    App::getInstance().getSession().hp = player->maxHp;
    App::getInstance().getSession().save();
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Shutdown completed");
}

void App::doSDLEvents()
{
    // tick SDL events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "FORCE SHUTDOWN! (SDL_QUIT signal received)");
                shutdown();
                exit(0);
            }
            case SDL_KEYDOWN:
            {
                SDL_KeyboardEvent e = event.key;
                if (e.repeat == 0)
                    pressedKey[e.keysym.scancode] = true;
                break;
            }
            case SDL_KEYUP:
            {
                SDL_KeyboardEvent e = event.key;
                if (e.repeat == 0)
                    pressedKey[e.keysym.scancode] = false;
                break;
            }
            default:
                break;
        }
    }
}

void App::resetKeyState()
{
    memset(pressedKey, false, sizeof pressedKey); // reset keyboard
}

Weapon* App::getWeapon(const char* name)
{
    Weapon* w = weapons[name];
    if (w == null) throw runtime_error(string("Weapon not found: ") + name);
    return w;
}

Player* App::getPlayer()
{
    return player;
}

list<shared_ptr<Entity>> App::getEntities()
{
    return entities;
}

vector<UIComponent*> App::getUI()
{
    return ui;
}

UIComponent* App::addUIComponent(string name, SDL_Texture* texture, int x, int y, bool freeTextureOnDestruct)
{
    ui.emplace_back(new UIComponent(name, texture, x, y, freeTextureOnDestruct));
    return ui.at(ui.size() - 1);
}

bool App::removeUIComponent(const UIComponent* c)
{
    for (unsigned long i = 0; i != ui.size(); i++)
        if (ui[i] == c)
        {
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Remove UI %s", ui[i]->name.c_str());
            delete ui[i];
            ui.erase(ui.begin() + i);
            return true;
        }
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "UI %s not found", c->name.c_str());
    return false;
}

bool App::removeUIComponent(const char* name)
{
    for (unsigned long i = 0; i != ui.size(); i++)
        if (ui[i]->name == name)
        {
            delete ui[i];
            ui.erase(ui.begin() + i);
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Removed UI %s", name);
            return true;
        }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "UI %s not found", name);
    return false;
}

void App::clearAllUI()
{
    for (UIComponent* u : ui)
        delete u;
    ui.clear();
}

Entity* App::addEntity(Entity* e)
{
    if (e->type != ENTITY_TYPE_BULLET)SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Add entity %s", e->name.c_str());
    entities.emplace_back(e);
    if (e->onSpawn != null) e->onSpawn(e);
    return e;
}

void App::removeEntity(Entity* e, bool callOnDeath)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Remove entity %s", e->name.c_str());
    for (const shared_ptr<Entity>& ee : entities)
    {
        if (ee.get() == e)
        {
            if (callOnDeath && e->onDeath != null) e->onDeath(e);
            entities.remove(ee);
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entity removed");
            return;
        }
    }
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Entity not found");
}

void App::render()
{
    // refresh screen
    SDL_SetRenderDrawColor(renderer, 32, 32, 64, 255);
    SDL_RenderClear(renderer);
    // place entities
    for (const auto& e : entities)
    {
        if (!e->isDead) renderManager->placeTexture(e->texture, e->x, e->y, e->width, e->height);
    }
    // place ui components
    for (const auto& a : ui)
        renderManager->placeTexture(a->texture, a->x, a->y);
    SDL_RenderPresent(renderer);
}

bool App::showPrompt(const char* msg, bool showHints)
{
    const char* item[] = {"msgShadow", "msg", "yesShadow", "yes", "noShadow", "no"};
    SDL_Texture* textTexture = renderManager->textToTexture(msg, 255, 255, 0, 255, FONT_SIZE_XL);
    SDL_Texture* yes = renderManager->textToTexture("[SPACE] Proceed", 200, 200, 200, 255, FONT_SIZE_L);
    SDL_Texture* no = renderManager->textToTexture("[ESC] Cancel", 200, 200, 200, 255, FONT_SIZE_L);
    SDL_Texture* textTextureShadow = renderManager->textToTexture(msg, 0, 0, 0, 127, FONT_SIZE_XL);
    SDL_Texture* yesShadow = renderManager->textToTexture("[SPACE] Proceed", 0, 0, 0, 127, FONT_SIZE_L);
    SDL_Texture* noShadow = renderManager->textToTexture("[ESC] Cancel", 0, 0, 0, 127, FONT_SIZE_L);
    addUIComponent(item[0], textTextureShadow,
                   WINDOW_WIDTH / 2 - renderManager->getTextureWidth(textTexture) / 2 + 3, 403, true);
    addUIComponent(item[1], textTexture,
                   WINDOW_WIDTH / 2 - renderManager->getTextureWidth(textTexture) / 2, 400, true);
    addUIComponent(item[2], yesShadow,
                   WINDOW_WIDTH / 2 - renderManager->getTextureWidth(yes) / 2 + 3, showHints ? 463 : 1000, true);
    addUIComponent(item[3], yes,
                   WINDOW_WIDTH / 2 - renderManager->getTextureWidth(yes) / 2, showHints ? 460 : 1000, true);
    addUIComponent(item[4], noShadow,
                   WINDOW_WIDTH / 2 - renderManager->getTextureWidth(no) / 2 + 3, showHints ? 513 : 1000, true);
    addUIComponent(item[5], no,
                   WINDOW_WIDTH / 2 - renderManager->getTextureWidth(no) / 2, showHints ? 510 : 1000, true);
    soundManager->playSound("assets/projectnuma/sounds/misc/msg.wav");
    //TODO: darken the screen when displaying prompt
    for (;;)
    {
        resetKeyState();
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_RETURN] || pressedKey[SDL_SCANCODE_SPACE])
        {
            for (const char* s : item)
                removeUIComponent(s);
            resetKeyState();
            return true;
        }
        else if (pressedKey[SDL_SCANCODE_ESCAPE] || pressedKey[SDL_SCANCODE_BACKSPACE])
        {
            for (const char* s : item)
                removeUIComponent(s);
            resetKeyState();
            return false;
        }
        render();
        SDL_Delay(50);
    }
}

int App::showOptions(initializer_list<string> args, int x, int y, unsigned char alignMode, FontSize fontSize)
{
    if (args.size() == 0) return 0;
    alignMode = alignMode % 3;
    vector<SDL_Texture*> optionText;
    vector<SDL_Texture*> optionTextS;
    vector<UIComponent*> optionUI;
    for (const string& s : args) // generate text textures
    {
        optionText.emplace_back(renderManager->textToTexture(s, 255, 255, 255, 255, fontSize));
        optionTextS.emplace_back(renderManager->textToTexture(s, 0, 255, 0, 255, fontSize));
    }
    for (size_t i = 0; i != args.size(); i++) // generate text UIs
        optionUI.emplace_back(addUIComponent(
                string("option").append(to_string(i)), optionText[i],
                alignMode == 2 ? x - renderManager->getTextureWidth(optionText[i]) :
                alignMode == 1 ? x - renderManager->getTextureWidth(optionText[i]) / 2
                               : x,
                y + i * (fontSize == FONT_SIZE_XL ? 60 :
                         fontSize == FONT_SIZE_L ? 50 :
                         fontSize == FONT_SIZE_M ? 40 :
                         fontSize == FONT_SIZE_S ? 30 :
                         20),
                false)
        );
    function<void()> removeUI = [&]() {
        for (size_t i = 0; i != args.size(); i++)
            removeUIComponent(string("option").append(to_string(i)).c_str());
        for (auto t : optionText) SDL_DestroyTexture(t);
        for (auto t : optionTextS) SDL_DestroyTexture(t);
        resetKeyState();
    };
    soundManager->playSound("assets/projectnuma/sounds/misc/msg.wav");
    int selection = 0;
    for (;;)
    {
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_S] || pressedKey[SDL_SCANCODE_DOWN])
        {
            if (selection == args.size() - 1) selection = 0;
            else selection++;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_W] || pressedKey[SDL_SCANCODE_UP])
        {
            if (selection == 0) selection = args.size() - 1;
            else selection--;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_SPACE] || pressedKey[SDL_SCANCODE_RETURN])
        {
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Selected %d (%s)", selection, optionUI[selection]->name.c_str());
            removeUI();
            return selection;
        }

        // highlight the selected option
        for (size_t i = 0; i != args.size(); i++)
            optionUI[i]->setTexture(optionText[i], false);
        optionUI[selection]->setTexture(optionTextS[selection], false);

        render();
        SDL_Delay(50);
    }
}

void App::cleanupEntities()
{
    entities.remove_if(
            [](const shared_ptr<Entity>& e) { return e->isDead && e->type != ENTITY_TYPE_PLAYER; }
    );
}

void App::mainLoop()
{
    for (;;)
    {
        switch (state)
        {
            case STATE_STARTUP:
            {
                throw std::logic_error("App.state is not supposed to be STATE_STARTUP in mainLoop()");
            }
            case STATE_MENU:
            {
                doStateMenu();
                break;
            }
            case STATE_LEVELS:
            {
                doStateLevels();
                break;
            }
            case STATE_GAME:
            {
                Level defaultLevel = LevelUtil.loadLevel("assets/projectnuma/levels/infiniteNormal.level");
                doStateGame(&defaultLevel);
                break;
            }
            case STATE_HANGAR:
            {
                doStateHangar();
                break;
            }
            case STATE_SETTINGS:
            {
                doStateSettings();
                break;
            }
            case STATE_ABOUT:
            {
                doStateAbout();
                break;
            }
            case STATE_SHUTDOWN:
            {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Shutdown sequence initiated");
                shutdown();
                return;
            }
        }
    }
}

void App::doStateMenu()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Show main menu");
    if (bgm != soundManager->getMusic("assets/projectnuma/sounds/music/menu/0.ogg"))
        soundManager->setBGM("assets/projectnuma/sounds/music/menu/0.ogg");
    getPlayer()->setLocation(WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_Texture* logo = renderManager->getTexture("assets/projectnuma/textures/gui/title.png");

    SDL_Texture* item[] = {
            renderManager->textToTexture("START", 255, 255, 255, 255, FONT_SIZE_XL),
            renderManager->textToTexture("QUIT", 255, 255, 255, 255, FONT_SIZE_XL),
            renderManager->textToTexture("HANGAR", 255, 255, 255, 255, FONT_SIZE_XL),
            renderManager->textToTexture("SETTINGS", 255, 255, 255, 255, FONT_SIZE_XL),
            renderManager->textToTexture("ABOUT", 255, 255, 255, 255, FONT_SIZE_XL),
    };

    SDL_Texture* itemSelected[] = {
            renderManager->textToTexture("START", 0, 255, 0, 255, FONT_SIZE_XL),
            renderManager->textToTexture("QUIT", 0, 255, 0, 255, FONT_SIZE_XL),
            renderManager->textToTexture("HANGAR", 0, 255, 0, 255, FONT_SIZE_XL),
            renderManager->textToTexture("SETTINGS", 0, 255, 0, 255, FONT_SIZE_XL),
            renderManager->textToTexture("ABOUT", 0, 255, 0, 255, FONT_SIZE_XL),
    };

    addUIComponent("logo", logo, WINDOW_WIDTH / 2 - renderManager->getTextureWidth(logo) / 2, 150, false);

    UIComponent* startUI = addUIComponent("start", item[0],
                                          WINDOW_WIDTH / 2 - renderManager->getTextureWidth(item[0]) / 2, 450, true);
    UIComponent* quitUI = addUIComponent("quit", item[1],
                                         WINDOW_WIDTH / 2 - renderManager->getTextureWidth(item[1]) / 2, 510, true);
    UIComponent* hangarUI = addUIComponent("hangar", item[2],
                                           WINDOW_WIDTH / 2 - renderManager->getTextureWidth(item[2]) / 2, 570, true);
    UIComponent* settingsUI = addUIComponent("settings", item[3],
                                             WINDOW_WIDTH / 2 - renderManager->getTextureWidth(item[3]) / 2, 630, true);
    UIComponent* aboutUI = addUIComponent("about", item[4],
                                          WINDOW_WIDTH / 2 - renderManager->getTextureWidth(item[4]) / 2, 690, true);

    function<void()> removeUI = [&]() {
        clearAllUI();
        for (SDL_Texture* t : item)
            SDL_DestroyTexture(t);
        for (SDL_Texture* t : itemSelected)
            SDL_DestroyTexture(t);
        resetKeyState();
    };

    int selection = 0;
    for (;;)
    {
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_S] || pressedKey[SDL_SCANCODE_DOWN])
        {
            if (selection == 4) selection = 0;
            else selection++;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_W] || pressedKey[SDL_SCANCODE_UP])
        {
            if (selection == 0) selection = 4;
            else selection--;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_SPACE] || pressedKey[SDL_SCANCODE_RETURN])
        {
            switch (selection)
            {
                case 0: // start game
                {
                    removeUI();
                    state = STATE_GAME;
                    return;
                }
                case 1: // quit
                {
                    state = STATE_SHUTDOWN;
                    return;
                }
                case 2: // hangar
                {
                    removeUI();
                    state = STATE_HANGAR;
                    return;
                }
                case 3: // settings
                {
                    showPrompt("This feature has not been implemented yet", true);
                    break;
                }
                case 4: // about
                {
                    removeUI();
                    state = STATE_ABOUT;
                    return;
                }
            }
        }

        // highlight the selected option
        if (selection == 0) startUI->setTexture(itemSelected[0], false);
        else startUI->setTexture(item[0], false);
        if (selection == 1) quitUI->setTexture(itemSelected[1], false);
        else quitUI->setTexture(item[1], false);
        if (selection == 2) hangarUI->setTexture(itemSelected[2], false);
        else hangarUI->setTexture(item[2], false);
        if (selection == 3) settingsUI->setTexture(itemSelected[3], false);
        else settingsUI->setTexture(item[3], false);
        if (selection == 4) aboutUI->setTexture(itemSelected[4], false);
        else aboutUI->setTexture(item[4], false);

        render();
        SDL_Delay(50);
    }
}

void App::doStateLevels()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Show levels page");
}

void App::doStateGame(Level* level)
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Entering main game\n\tUsing level: %s", level->name.c_str());
    currentLevel = level;
    // initialize ui
    addUIComponent("level indicator", renderManager->textToTexture(level->name, 255, 255, 0, 200, FONT_SIZE_S), 10, 10, true);
    addUIComponent("weapon indicator", renderManager->textToTexture("A", 255, 255, 255, 255, FONT_SIZE_S), 10, 40, true);
    addUIComponent("hp indicator", renderManager->textToTexture("A", 255, 255, 255, 255, FONT_SIZE_S), 10, 70, true);
    addUIComponent("credit indicator", renderManager->textToTexture("A", 255, 255, 255, 255, FONT_SIZE_S), 10, 100, true);
    addUIComponent("life indicator", renderManager->textToTexture("A", 255, 255, 255, 255, FONT_SIZE_S), 10, 130, true);
    getPlayer()->life = level->playerLife;
    getPlayer()->hp = getPlayer()->maxHp;
    getPlayer()->isDead = false;
    getPlayer()->setLocation(100, WINDOW_HEIGHT / 2 - getPlayer()->height / 2);
    soundManager->setBGM("assets/projectnuma/sounds/music/game/0.ogg");
    for (unsigned long tick = 0, time0 = 0, time1 = 0; (time0 = CommonUtil.currentTimeNanos()); tick++)
    {
        assert(enemyCounter[0] >= 0 && enemyCounter[1] >= 0 && enemyCounter[2] >= 0 && enemyCounter[3] >= 0);
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_ESCAPE])
        {
            if (showPrompt("Return to the main menu?", true))
            {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Leaving main game");
                goto toMainMenu;
            }
            time0 = CommonUtil.currentTimeNanos();
        }
        // check level goal
        if (
                killboardLevel[0] >= level->killGoal[0] ||
                killboardLevel[1] >= level->killGoal[1] ||
                killboardLevel[2] >= level->killGoal[2] ||
                killboardLevel[3] >= level->killGoal[3]
                )
        {
            level->isPassed = true;
        }

        if (currentLevel->isPassed)
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Level passed: %s", level->name.c_str());
            if (level->id != 0)App::getInstance().getSession().unlockedLevels = MAX(level->id, App::getInstance().getSession().unlockedLevels);
            showPrompt("LEVEL PASSED!", false);
            goto toMainMenu;
        }
        if (currentLevel->isFailed)
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Level failed: %s", level->name.c_str());
            showPrompt("GAME OVER!", false);
            goto toMainMenu;
        }

        // randomly add enemies
        if (App::getInstance().getRandom().nextDouble(1) <= level->randomSpawnSpeed)
            switch (App::getInstance().getRandom().nextInt(4))
            {
                case 0:
                {
                    if (enemyCounter[0] < level->maxEnemiesInScreen[0] && App::getInstance().getRandom().nextDouble(1) <= level->enemySpawnRate[0])
                        addEntity((new Enemy0())->setLocation(WINDOW_WIDTH, App::getInstance().getRandom().nextInt(WINDOW_HEIGHT - 24)));
                    break;
                }
                case 1:
                {
                    if (enemyCounter[1] < level->maxEnemiesInScreen[1] && App::getInstance().getRandom().nextDouble(1) <= level->enemySpawnRate[1])
                        addEntity((new Enemy1())->setLocation(WINDOW_WIDTH, App::getInstance().getRandom().nextInt(WINDOW_HEIGHT - 32)));
                    break;
                }
                case 2:
                {
                    if (enemyCounter[2] < level->maxEnemiesInScreen[2] && App::getInstance().getRandom().nextDouble(1) <= level->enemySpawnRate[2])
                        addEntity((new Enemy2())->setLocation(WINDOW_WIDTH, App::getInstance().getRandom().nextInt(WINDOW_HEIGHT - 40)));
                    break;
                }
                case 3:
                {
                    if (enemyCounter[3] < level->maxEnemiesInScreen[3] && App::getInstance().getRandom().nextDouble(1) <= level->enemySpawnRate[3])
                        addEntity((new Enemy3())->setLocation(WINDOW_WIDTH, App::getInstance().getRandom().nextInt(WINDOW_HEIGHT - 160)));
                    break;
                }
            }
        // tick entities
        for (const shared_ptr<Entity>& e : entities)
        {
            if (!e->isDead)
                e->tick();
            else if (e->type == ENTITY_TYPE_PLAYER && e->isDead) e->beforeTick(e.get());
        }
        // tick UI
        for (const auto& it : ui)
            if (it->name == "weapon indicator")
                it->setTexture(
                        renderManager->textToTexture(string("Weapon: ").append(player->weapon->name).c_str(),
                                                     255, 255, 255, 200, FONT_SIZE_S), true);
            else if (it->name == "hp indicator")
                it->setTexture(
                        renderManager->textToTexture(string("HP: ").append(to_string(player->hp)).c_str(),
                                                     255 * (150 - player->invincibleTicks) / 150, player->hp > 0 ? 255 : 0,
                                                     player->hp > 0 ? 255 : 0, 200, FONT_SIZE_S), true);
            else if (it->name == "credit indicator")
                it->setTexture(
                        renderManager->textToTexture(string("Credit: ").append(to_string(App::getInstance().getSession().credit)).c_str(),
                                                     255, 255, 255, 200, FONT_SIZE_S), true);
            else if (it->name == "life indicator")
                it->setTexture(
                        renderManager->textToTexture(string("Life: ").append(to_string(getPlayer()->life)).c_str(),
                                                     255, player->life == 0 ? 0 : 255, player->life == 0 ? 0 : 255, 200,
                                                     FONT_SIZE_S), true);
        render();
        // clean up
        if (tick % 10 == 0)
            cleanupEntities();
        time1 = CommonUtil.currentTimeNanos();
        if (time1 - time0 > 16000000LU)
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                        "Can't keep up! Is the game overloaded or the system time has changed?\n\tA single tick took %lu ns", time1 - time0);
        SDL_Delay(16 - MIN(16LU, (time1 - time0) / 1000000LU));
    }
    toMainMenu:
    // remove ui
    clearAllUI();
    for (const auto& a : entities) // remove all entities
        if (a->type != ENTITY_TYPE_PLAYER)
            a->isDead = true;
    cleanupEntities();
    for (int i = 0; i != 4; i++) // move kill counts to total
    {
        App::getInstance().getSession().killboardTotal[i] += killboardLevel[i];
        killboardLevel[i] = 0;
    }
    memset(enemyCounter, 0, sizeof(enemyCounter));
    assert(entities.size() == 1);
    resetKeyState();
    currentLevel = null;
    state = STATE_MENU;
}

void App::doStateHangar()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Show hangar page");
    SDL_Texture* item[] = {
            renderManager->textToTexture("Shotgun (600CR)", 255, 255, 255, 255, FONT_SIZE_M),
            renderManager->textToTexture("Blaster (2000CR)", 255, 255, 255, 255, FONT_SIZE_M),
            renderManager->textToTexture("Laser cannon (2000CR)", 255, 255, 255, 255, FONT_SIZE_M),
            renderManager->textToTexture("+10 max HP (1880CR)", 255, 255, 255, 255, FONT_SIZE_M),
    };
    SDL_Texture* itemSelected[] = {
            renderManager->textToTexture("Shotgun (600CR)", 0, 255, 0, 255, FONT_SIZE_M),
            renderManager->textToTexture("Blaster (2000CR)", 0, 255, 0, 255, FONT_SIZE_M),
            renderManager->textToTexture("Laser cannon (2000CR)", 0, 255, 0, 255, FONT_SIZE_M),
            renderManager->textToTexture("+10 max HP (1880CR)", 0, 255, 0, 255, FONT_SIZE_M),
    };

    addUIComponent("hangar", renderManager->textToTexture("Hangar", 255, 255, 0, 255, FONT_SIZE_XL), 20, 20, true);
    addUIComponent("back", renderManager->textToTexture("[Esc] Go back", 255, 255, 255, 255, FONT_SIZE_L), 20, 850, true);
    UIComponent* w1ui = addUIComponent("w1", item[0], 50, 80, false);
    UIComponent* w2ui = addUIComponent("w2", item[1], 50, 120, false);
    UIComponent* w3ui = addUIComponent("w3", item[2], 50, 160, false);
    UIComponent* hpui = addUIComponent("hp", item[3], 50, 200, false);
    UIComponent* cr = addUIComponent("credit", renderManager->textToTexture("A", 255, 255, 255, 255, FONT_SIZE_L), 20, 800, true);

    int selection = 0;
    for (;;)
    {
        doSDLEvents();
        if (pressedKey[SDL_SCANCODE_ESCAPE])
        {
            clearAllUI();
            for (SDL_Texture* t : item)
                SDL_DestroyTexture(t);
            for (SDL_Texture* t : itemSelected)
                SDL_DestroyTexture(t);
            resetKeyState();
            state = STATE_MENU;
            return;
        }
        else if (pressedKey[SDL_SCANCODE_S] || pressedKey[SDL_SCANCODE_DOWN])
        {
            if (selection == 3) selection = 0;
            else selection++;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_W] || pressedKey[SDL_SCANCODE_UP])
        {
            if (selection == 0) selection = 3;
            else selection--;
            resetKeyState();
        }
        else if (pressedKey[SDL_SCANCODE_SPACE] || pressedKey[SDL_SCANCODE_RETURN])
        {
            switch (selection)
            {
                case 0: // buy Weapon1 with 600cr
                {
                    if (showPrompt("Purchase this weapon?", true))
                    {
                        if (App::getInstance().getSession().unlockedWeapons & 2) // check if this hasent been purchased before
                        {
                            showPrompt("You have already purchased this!", false);
                        }
                        else
                        {
                            if (App::getInstance().getSession().credit >= 600) // check if your money is enough
                            {
                                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Purchase weapon 1");
                                App::getInstance().getSession().unlockedWeapons = App::getInstance().getSession().unlockedWeapons | 2;
                                App::getInstance().getSession().credit -= 600;
                                player->currentWeapons.emplace_back(getWeapon("PlayerWeapon1"));
                                showPrompt("Purchase success!", false);
                            }
                            else showPrompt("You don't have enough credits!", false);

                        }
                    }
                    resetKeyState();
                    break;
                }
                case 1: // buy Weapon2 with 2000cr
                {
                    if (showPrompt("Purchase this weapon?", true))
                    {
                        if (App::getInstance().getSession().unlockedWeapons & 4)
                        {
                            showPrompt("You have already purchased this!", false);
                        }
                        else
                        {
                            if (App::getInstance().getSession().credit >= 2000)
                            {
                                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Purchase weapon 2");
                                App::getInstance().getSession().unlockedWeapons = App::getInstance().getSession().unlockedWeapons | 4;
                                App::getInstance().getSession().credit -= 2000;
                                player->currentWeapons.emplace_back(getWeapon("PlayerWeapon2"));
                                showPrompt("Purchase success!", false);
                            }
                            else showPrompt("You don't have enough credits!", false);

                        }

                    }
                    resetKeyState();
                    break;
                }
                case 2: // buy Weapon3 with 2000cr
                {
                    if (showPrompt("Purchase this weapon?", true))
                    {
                        if (App::getInstance().getSession().unlockedWeapons & 8)
                        {
                            showPrompt("You have already purchased this!", false);
                        }
                        else
                        {
                            if (App::getInstance().getSession().credit >= 2000)
                            {
                                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Purchase weapon 3");
                                App::getInstance().getSession().unlockedWeapons = App::getInstance().getSession().unlockedWeapons | 8;
                                App::getInstance().getSession().credit -= 2000;
                                player->currentWeapons.emplace_back(getWeapon("PlayerWeapon3"));
                                showPrompt("Purchase success!", false);
                            }
                            else showPrompt("You don't have enough credits!", false);
                        }
                    }
                    resetKeyState();
                    break;
                }
                case 3: // buy 10 max hp with 1880cr
                {
                    if (showPrompt("Purchase this upgrade?", true))
                    {
                        if (App::getInstance().getSession().credit >= 1880)
                        {
                            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Purchase 10 max HP");
                            App::getInstance().getSession().hp += 10;
                            player->maxHp += 10;
                            App::getInstance().getSession().credit -= 1880;
                            showPrompt("Purchase success!", false);
                        }
                        else showPrompt("You don't have enough credits!", false);
                    }
                    resetKeyState();
                    break;
                }
            }
        }

        // highlight the selected option
        if (selection == 0) w1ui->setTexture(itemSelected[0], false);
        else w1ui->setTexture(item[0], false);
        if (selection == 1) w2ui->setTexture(itemSelected[1], false);
        else w2ui->setTexture(item[1], false);
        if (selection == 2) w3ui->setTexture(itemSelected[2], false);
        else w3ui->setTexture(item[2], false);
        if (selection == 3) hpui->setTexture(itemSelected[3], false);
        else hpui->setTexture(item[3], false);

        cr->setTexture(
                renderManager->textToTexture(string("Credit: ").append(to_string(App::getInstance().getSession().credit)).c_str(),
                                             255, 255, 255, 255, FONT_SIZE_L), true);
        render();
        SDL_Delay(50);
    }
}

void App::doStateSettings()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Show settings page");
}

void App::doStateAbout()
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Show about page");
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%d",
                showOptions({"This", "feature", "has", "not", "been", "implemented", "yet"}, 200, 200, 0, FONT_SIZE_XL));
    state = STATE_MENU;
}

App& App::getInstance()
{
    static App instance;
    return instance;
}

Session& App::getSession()
{
    return *session;
}

Random& App::getRandom()
{
    return *r;
}
