//
// Created by mcumbrella on 22-11-28.
//

#ifndef PROJECTNUMA_APP_H
#define PROJECTNUMA_APP_H

#include "GeneralDefinitions.h"
#include "UIComponent.h"
#include "LevelUtil.h"

/**
 * public class App
 */
class App
{
private:
    App() = default;

    ~App() = default;

    Session* session = null;
    Random* r = null;
    Player* player;

public:
    App(const App&) = delete;

    App& operator =(const App&) = delete;

    GameState state = STATE_STARTUP;
    bool pressedKey[1024]{};
    bool flDebug = false, flFullscreen = false, flSoftwareRender = false;
    int killboardLevel[4]{};
    int enemyCounter[4]{};
    Level* currentLevel = null;
    RenderManager* renderManager = null;
    SoundManager* soundManager = null;

    static App& getInstance();

    Session& getSession();

    Random& getRandom();

    /**
     * The codes to be executed when starting the game.
     */
    void startup(int argc, char** argv);

    /**
     * The codes to be executed when quitting the game.
     */
    void shutdown();

    /**
     * update SDL events.
     */
    void doSDLEvents();

    /**
     * Reset all keys' pressing state.
     */
    void resetKeyState();

    /**
     * Get the Weapon object by its registered name.
     * @param name The name of the weapon registration.
     * @return The pointer to the weapon object.
     * @throws runtime_error if theres no such weapon in the registered weapons list.
     */
    Weapon* getWeapon(const char* name);

    /**
     * Get the pointer to the player's Player object.
     */
    Player* getPlayer();

    /**
     * Get the list object that stores all the entities.
     */
    list<shared_ptr<Entity>> getEntities();

    /**
     * Get the list object that stores all the UI components.
     */
    vector<UIComponent*> getUI();

    /**
     * Add a UI component to the UI list.
     * @param name The name of the UI component.
     * @param texture The texture used by this UI.
     * @param x The X position of the component.
     * @param y The Y position of the component.
     * @param freeTextureOnDestruct Whether to free the texture when removing this component.
     * @return The pointer to the newly added UI component.
     */
    UIComponent* addUIComponent(string name, SDL_Texture* texture, int x, int y, bool freeTextureOnDestruct);

    bool removeUIComponent(const UIComponent* c); // not tested

    /**
     * Remove a UI component from the UI list by its name.
     * @param name The name of the UI component.
     * @return true if succeed, false otherwise.
     */
    bool removeUIComponent(const char* name);

    /**
     * Clear the UI list.
     */
    void clearAllUI();

    /**
     * Add an entity to the entities list.
     * @param e The pointer to the Entity object.
     * @return The pointer to the Entity object.
     */
    Entity* addEntity(Entity* e);

    void removeEntity(Entity* e, bool callOnDeath); //FIXME: crashes on call

    /**
     * Refresh the screen.
     */
    void render();

    /**
     * Show a prompt (with 'proceed' and 'cancel' options or not) on the screen.
     * @param msg The message to show.
     * @param showHints Whether to show "[SPACE] Proceed, [Esc] Cancel" under the message.
     * @return true if the prompt is proceeded, false if the prompt is cancelled.
     */
    bool showPrompt(const char* msg, bool showHints);

    /**
     * Show a list of options.
     * Press arrow keys, W, S to move between selections, and select one by pressing SPACE.
     * @param args The texts of the options.
     * @param x The X location of the start of the option texts.
     * @param y The Y location of the start of the option texts.
     * @param alignMode 0: left, 1: center align to X, 2: right align to X.
     * @param fontSize The size of the option texts.
     * @return The index of the selected option (start with 0).
     */
    int showOptions(initializer_list<string> args, int x, int y, unsigned char alignMode, FontSize fontSize);

    /**
     * Remove all the dead entities.
     */
    void cleanupEntities();

    void mainLoop();

    /**
     * Show main menu.
     */
    void doStateMenu();

    /**
     * Show level select page.
     */
    void doStateLevels(); //TODO

    /**
     * Enter the main game scene.
     * @param level The Level object used this time.
     */
    void doStateGame(Level* level);

    /**
     * Show the hangar page.
     */
    void doStateHangar();

    /**
     * Show the settings page.
     */
    void doStateSettings(); //TODO

    /**
     * Show the about page.
     */
    void doStateAbout(); //TODO
};

#endif //PROJECTNUMA_APP_H
