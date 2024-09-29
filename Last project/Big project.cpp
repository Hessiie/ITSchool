#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <cctype>

using namespace std;

class Item {
public:
    Item(string name, string effect) : name(name), effect(effect) {}
    string getName() const { return name; }
    string getEffect() const { return effect; }
private:
    string name;
    string effect;
};

class Inventory {
public:
    void addItem(const Item& item) {
        items.push_back(item);
        cout << item.getName() << " has been added to your inventory!\n";
    }

    void removeItem(const string& itemName) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->getName() == itemName) {
                items.erase(it);
                cout << itemName << " has been removed from your inventory!\n";
                return;
            }
        }
        cout << "Item not found in inventory.\n";
    }

    void displayItems() const {
        cout << "\nYour Inventory:\n";
        if (items.empty()) {
            cout << "The inventory is empty.\n";
            return;
        }
        for (const auto& item : items) {
            cout << " - " << item.getName() << ": " << item.getEffect() << "\n";
        }
    }

private:
    vector<Item> items;
};

class Character {
public:
    Character(string name, int health, int attackPower, int defense, int experience = 0, int level = 1)
        : name(name), health(health), maxHealth(health), attackPower(attackPower), defense(defense), experience(experience), level(level),
        poisoned(false), poisonTurns(0) {}

    string getName() const { return name; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return health; }
    int getAttackPower() const { return attackPower; }
    int getDefense() const { return defense; }
    int getExperience() const { return experience; }
    int getLevel() const { return level; }

    // Simple damage formula taking into account the amount of armour you have.
    void takeDamage(int damage) {
        int effectiveDamage = damage - defense;
        if (effectiveDamage < 0) effectiveDamage = 0;
        health -= effectiveDamage;
        if (health < 0) health = 0;
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= level * 100) {
            level++;
            experience = 0;
            maxHealth += 10;
            health = maxHealth;
            attackPower += 5;
            defense += 2;
            cout << "Congratulations! You leveled up to level " << level << "!\n";
        }
    }

    bool isAlive() const { return health > 0; }

    void displayStats() const {
        cout << "\n"
            << "====================================================\n"
            << "                 Character Stats                    \n"
            << "====================================================\n";

        cout << " Name         : " << getName() << "\n"
            << "----------------------------------------------------\n"
            << " Health       : " << getHealth() << " / " << getMaxHealth() << "\n"
            << " Attack Power : " << getAttackPower() << "\n"
            << " Defense      : " << getDefense() << "\n"
            << "----------------------------------------------------\n"
            << " Level        : " << getLevel() << "\n"
            << " Experience   : " << getExperience() << "\n"
            << "====================================================\n";
    }


    void applyPoison(int turns) {
        poisoned = true;
        poisonTurns = turns;
    }

    // Poison mechanic for one of the monsters.
    void processPoisonDamage() {
        if (poisoned && poisonTurns > 0) {
            int poisonDamage = 5;
            cout << "You are poisoned! You take " << poisonDamage << " damage.\n";
            takeDamage(poisonDamage);
            poisonTurns--;
            if (poisonTurns <= 0) {
                poisoned = false;
                cout << "The poison has worn off.\n";
            }
        }
    }

    bool isPoisoned() const {
        return poisoned;
    }

    void rest() {
        int restAmount = 70; // Amount of HP restored.
        heal(restAmount);
        cout << "You rest and restore " << restAmount << " health points. Your health is now " << health << ".\n";
    }

    void displayInventory() const {
        if (inventoryItems.empty()) {
            cout << "Your inventory is empty.\n";
        }
        else {
            cout << "Your Inventory:\n";
            for (const auto& item : inventoryItems) {
                cout << "- " << item.getName() << "\n";
            }
        }

        cout << "Press Enter to return to the menu...\n";
        cin.ignore(); // Clear any remaining input in the buffer
        cin.get();    // Wait for the user to press Enter
    }

    void addItemToInventory(const Item& item) {
        inventory.addItem(item);
    }

private:
    string name;
    int health;
    int maxHealth;
    int attackPower;
    int defense;
    int experience;
    int level;
    bool poisoned;
    int poisonTurns;
    Inventory inventory;
    vector<Item> inventoryItems;
};

void displaySlowText(const string& text, int delay = 50) {
    for (char c : text) {
        cout << c;
        cout.flush(); // Forces the output to appear character by character
        this_thread::sleep_for(chrono::milliseconds(delay)); // Delay between each character
    }
    cout << endl;
}

class Monster {
public:
    Monster(string type, int health, int attackPower, int defense, int experienceValue, string ability = "")
        : type(type), health(health), attackPower(attackPower), defense(defense), experienceValue(experienceValue), ability(ability) {}

    string getType() const { return type; }
    int getHealth() const { return health; }
    int getAttackPower() const { return attackPower; }
    int getDefense() const { return defense; }
    int getExperienceValue() const { return experienceValue; }
    string getAbility() const { return ability; }

    // Simple damage formula taking into account the amount of armour you have.
    void takeDamage(int damage) {
        int effectiveDamage = damage - defense;
        if (effectiveDamage < 0) effectiveDamage = 0;
        health -= effectiveDamage;
        if (health < 0) health = 0;
    }

    bool isAlive() const { return health > 0; }

    //Ability manager.
    void useAbility(Character& character) {
        if (ability == "Regeneration") {
            int healAmount = 10;
            health += healAmount;
            if (health > 80) {  // Assume 80 is the max health since only one of the monsters use this ability.
                health = 80;
            }
            cout << "The " << type << " uses " << ability << " and regenerates " << healAmount << " health!\n";
        }
        else if (ability == "Fire Breath") {
            int extraDamage = 15;
            cout << "The " << type << " uses " << ability << " and deals an extra " << extraDamage << " damage!\n";
            character.takeDamage(extraDamage);
        }
        else if (ability == "Ferocious Bite") {
            int extraDamage = 10;
            cout << "The " << type << " uses " << ability << " and bites viciously, dealing an extra " << extraDamage << " damage!\n";
            character.takeDamage(extraDamage);
        }
        else if (ability == "Poisoned Dagger") {
            int poisonTurns = 3; // Example: poison lasts 3 turns.
            cout << "The " << type << " uses " << ability << " and poisons you for " << poisonTurns << " turns!\n";
            character.applyPoison(poisonTurns);
        }
    }

private:
    string type;
    int health;
    int attackPower;
    int defense;
    int experienceValue;
    string ability;
};

void clearScreen(int delayMilliseconds = 1000) {
    // Delay before clearing
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMilliseconds));

#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Linux/Unix/MacOS
#endif
}

void delay(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

//Combat system.
void battle(Character& character, Monster& monster) {
    cout << "\nA wild " << monster.getType() << " appeared!\n";

    bool isPlayerTurn = true; //Tracks if it's the players turn or not.

    while (character.isAlive() && monster.isAlive()) {
        if (character.isPoisoned()) {
            character.processPoisonDamage();
        }

        // Player's turn
        if (isPlayerTurn) {
            int choice;
            cout << "\nYour turn:\n";
            cout << "1. Attack\n";
            cout << "2. Use Potion\n";
            cout << "Choose your action: ";
            cin >> choice;

            switch (choice) {
            case 1:
                monster.takeDamage(character.getAttackPower());
                cout << "You hit the " << monster.getType() << " for " << character.getAttackPower() << " damage!\n";
                break;
            case 2:
                character.heal(50);
                cout << "You use a potion and heal 50 HP!\n";
                break;
            default:
                cout << "Invalid choice. You lose your turn.\n";
                break;
            }

            // Check if the monster is still alive after the player's turn
            if (!monster.isAlive()) {
                cout << "You defeated the " << monster.getType() << "!\n";
                character.gainExperience(monster.getExperienceValue());
                cout << "You gained " << monster.getExperienceValue() << " experience points.\n";
                return;
            }

            isPlayerTurn = false; // Switch to monster's turn
        }
        else {
            // Monster's turn
            cout << "\n" << monster.getType() << "'s turn:\n";
            if (!monster.getAbility().empty()) {
                monster.useAbility(character);
            }
            character.takeDamage(monster.getAttackPower());
            cout << "The " << monster.getType() << " hit you for " << monster.getAttackPower() << " damage!\n";
            cout << "Your health is now " << character.getHealth() << "\n";

            // Check if the player is still alive after the monster's turn
            if (!character.isAlive()) {
                cout << "You have been defeated by the " << monster.getType() << "...\n";
                cout << "Game Over. You lost.\n";
                exit(0);
            }

            isPlayerTurn = true; // Switch back to player's turn
        }
    }
}

// Function to confirm quest choice
bool confirmQuest() {
    char confirmChoice;
    cout << "\nDo you want to proceed with this quest? (y/n): ";
    cin >> confirmChoice;
    confirmChoice = tolower(confirmChoice);

    return confirmChoice == 'y';
}

//Quest system.
void questForestBeast(Character& character) {
    displaySlowText("\nA village elder approaches you with a plea for help.");
    displaySlowText("Elder: \"Please, brave adventurer, a terrible beast has been terrorizing our village. Will you help us?\"");

    if (!confirmQuest()) {
        cout << "Returning to the main menu...\n";
        return;
    }

    Monster forestBeast("Forest Beast", 60, 18, 5, 70, "Ferocious Bite");

    displaySlowText("\nYou venture into the forest to confront the beast...");
    battle(character, forestBeast);

    if (character.isAlive()) {
        displaySlowText("\nElder: \"Thank you, hero! Our village is saved!\"");
        character.gainExperience(100);
        displaySlowText("You gained 100 experience points.");
        character.addItemToInventory(Item("Beast Slayer", "Increases attack power by 5"));
    }
    else {
        displaySlowText("Elder: \"Alas, our hopes are dashed...\"");
    }
}


void questCursedCave(Character& character) {
    displaySlowText("\nA wounded knight stumbles towards you.\n");
    displaySlowText("Knight: \"Adventurer, beware... A mighty dragon has taken refuge in the cursed cave. I could not defeat it. Can you?\"\n");

    if (!confirmQuest()) {
        cout << "Returning to the main menu...\n";
        return;
    }

    Monster caveDragon("Cave Dragon", 120, 25, 10, 150, "Fire Breath");

    cout << "\nYou steel yourself and enter the cursed cave...\n";
    battle(character, caveDragon);

    if (character.isAlive()) {
        displaySlowText("\nKnight: \"Incredible! You have slain the dragon!\"\n");
        character.gainExperience(200);
        displaySlowText("You gained 200 experience points.\n");
    }
    else {
        displaySlowText("Knight: \"Another brave soul lost to the dragon...\"\n");
    }
}

void questLostArtifact(Character& character) {
    displaySlowText("\nA mysterious figure cloaked in shadows approaches you.\n");
    displaySlowText("Mysterious Figure: \"A powerful artifact has been stolen by the goblin chief. Retrieve it, and I shall reward you handsomely.\"\n");

    if (!confirmQuest()) {
        cout << "Returning to the main menu...\n";
        return; 
    }

    Monster goblinChief("Goblin Chief", 100, 12, 4, 50, "Poisoned Dagger");

    displaySlowText("\nYou journey into the goblin's lair to retrieve the artifact...\n");
    battle(character, goblinChief);

    if (character.isAlive()) {
        displaySlowText("\nMysterious Figure: \"You have done well, hero. Here is your reward.\"\n");
        character.gainExperience(120);
        displaySlowText("You gained 120 experience points.\n");
    }
    else {
        displaySlowText("Mysterious Figure: \"Another failure... How disappointing.\"\n");
    }
}

void questHauntedVillage(Character& character) {
    displaySlowText("\nA terrified villager runs up to you, pleading for help.");
    displaySlowText("Villager: \"Our village is haunted by a terrible ghost! Please, hero, can you save us?\"\n");

    if (!confirmQuest()) {
        cout << "Returning to the main menu...\n";
        return;
    }

    cout << "\n1. Confront the Ghost\n";
    cout << "2. Search for a way to lift the curse without fighting\n";
    cout << "Choose your approach: ";

    int approach;
    cin >> approach;

    if (approach == 1) {
        // Battle with the ghost
        Monster ghost("Ghost", 80, 15, 3, 100, "Soul Drain");
        displaySlowText("\nYou decide to confront the ghost directly...\n");
        battle(character, ghost);

        if (character.isAlive()) {
            displaySlowText("\nVillager: \"Thank you, hero! The ghost has been vanquished, and our village is safe once again!\"\n");
            character.gainExperience(150);
            displaySlowText("You gained 150 experience points.\n");
        }
        else {
            displaySlowText("Villager: \"We are doomed...\"\n");
        }
    }
    else if (approach == 2) {
        // Branch to finding a special artifact to lift the curse
        displaySlowText("\nYou choose to search for the sacred artifact that could lift the curse...\n");

        Monster curseGuardian("Curse Guardian", 70, 12, 5, 80, "Curse Touch");
        displaySlowText("\nA guardian appears, protecting the artifact!\n");
        battle(character, curseGuardian);

        if (character.isAlive()) {
            displaySlowText("\nYou retrieve the sacred artifact and bring it back to the village...\n");
            displaySlowText("Villager: \"The curse has been lifted! You are a true hero!\"\n");
            character.gainExperience(180);
            displaySlowText("You gained 180 experience points.\n");
        }
        else {
            displaySlowText("Villager: \"Without the artifact, our village is lost...\"\n");
        }
    }
    else {
        cout << "Invalid choice. Returning to the main menu.\n";
    }
}

void questBanditCamp(Character& character) {
    displaySlowText("\nA traveling merchant approaches you with a tale of woe.");
    displaySlowText("Merchant: \"A group of bandits have set up camp nearby, stealing from travelers like myself. Can you stop them?\"\n");

    // Confirm quest choice
    if (!confirmQuest()) {
        cout << "Returning to the main menu...\n";
        return;
    }

    cout << "\n1. Negotiate with the bandits\n";
    cout << "2. Attack the bandit camp\n";
    cout << "Choose your approach: ";

    int approach;
    cin >> approach;

    if (approach == 1) {
        // Negotiate with the bandits
        displaySlowText("\nYou decide to try negotiating with the bandits...\n");

        int negotiationSuccess = rand() % 2; // 50% chance of success
        if (negotiationSuccess == 1) {
            displaySlowText("\nYou successfully negotiate peace with the bandits. They agree to leave the area!\n");
            character.gainExperience(100);  // Lesser reward
            displaySlowText("You gained 100 experience points.\n");
        }
        else {
            displaySlowText("\nThe negotiation fails, and the bandits attack you!\n");
            Monster banditLeader("Bandit Leader", 60, 20, 8, 150, "Savage Strike");
            battle(character, banditLeader);

            if (character.isAlive()) {
                displaySlowText("\nYou defeat the bandit leader and the rest of the camp flees!\n");
                character.gainExperience(150);  // Higher reward
                displaySlowText("You gained 150 experience points.\n");
            }
            else {
                displaySlowText("The bandits overwhelm you... The merchant will never see justice.\n");
            }
        }
    }
    else if (approach == 2) {
        // Battle the bandits directly
        Monster banditLeader("Bandit Leader", 60, 20, 8, 150, "Savage Strike");
        displaySlowText("\nYou charge into the bandit camp, ready to fight!\n");
        battle(character, banditLeader);

        if (character.isAlive()) {
            displaySlowText("\nYou defeat the bandit leader and the rest of the camp flees!\n");
            character.gainExperience(150);  // Higher reward
            displaySlowText("You gained 150 experience points.\n");
        }
        else {
            displaySlowText("The bandits overwhelm you... The merchant will never see justice.\n");
        }
    }
    else {
        cout << "Invalid choice. Returning to the main menu.\n";
    }
}

void questLostHeir(Character& character) {
    displaySlowText("\nA nobleman approaches you with a secret mission.");
    displaySlowText("Nobleman: \"My child, the heir to my fortune, has been kidnapped by a rival house. Will you rescue them for me?\"\n");

    if (!confirmQuest()) {
        cout << "Returning to the main menu...\n";
        return;
    }

    displaySlowText("\nYou infiltrate the rival house to find the heir...\n");

    Monster houseGuard("House Guard", 50, 18, 5, 80, "Heavy Slash");
    battle(character, houseGuard);

    if (character.isAlive()) {
        displaySlowText("\nYou find the heir locked away in a chamber.\n");
        cout << "\n1. Rescue the heir and return them to the nobleman\n";
        cout << "2. Betray the nobleman and ransom the heir to the rival house\n";
        cout << "Choose your action: ";

        int action;
        cin >> action;

        if (action == 1) {
            displaySlowText("\nYou decide to return the heir to the nobleman...\n");
            displaySlowText("\nNobleman: \"Thank you! You have saved my family.\"\n");
            character.gainExperience(200);
            displaySlowText("You gained 200 experience points.\n");
        }
        else if (action == 2) {
            displaySlowText("\nYou betray the nobleman and deliver the heir to the rival house for a bag of gold.\n");
            character.gainExperience(120); 
            displaySlowText("You gained 120 experience points and a bag of gold!\n");
        }
        else {
            cout << "Invalid choice. Returning to the main menu.\n";
        }
    }
    else {
        displaySlowText("\nYou were unable to rescue the heir...\n");
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "=================== Welcome to my RPG Game! ===================\n";
    string playerName;
    cout << "Enter your character's name: ";
    getline(cin, playerName);

    Character player(playerName, 100, 20, 5);

    int choice;
    do {
        cout << "\n=================== Main Menu ===================\n";
        cout << "1. View Character Stats\n";
        cout << "2. Go on a Quest\n";
        cout << "3. Rest\n";
        cout << "4. View Inventory\n";
        cout << "5. Exit Game\n";
        cout << "=================================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            player.displayStats();
            clearScreen(4000);
            break;
        case 2:
            int questChoice;
            cout << "\nChoose a Quest:\n";
            cout << "1. Defeat the Forest Beast\n";
            cout << "2. Slay the Dragon in the Cursed Cave\n";
            cout << "3. Retrieve the Lost Artifact from the Goblin Chief\n";
            cout << "4. Investigate the Haunted Village\n";
            cout << "5. Stop the Bandit Camp\n";
            cout << "6. Rescue the Lost Heir\n";
            cout << "Enter your choice: ";
            cin >> questChoice;

            switch (questChoice) {
            case 1:
                questForestBeast(player);
                break;
            case 2:
                questCursedCave(player);
                break;
            case 3:
                questLostArtifact(player);
                break;
            case 4:
                questHauntedVillage(player);
                break;
            case 5:
                questBanditCamp(player);
                break;
            case 6:
                questLostHeir(player);
                break;
            default:
                cout << "Invalid quest choice.\n";
                break;
            }
            clearScreen(3000);
            break;
        case 3:
            player.rest();
            clearScreen(2000);
            break;
        case 4:
            player.displayInventory();
            clearScreen(3000);
            break;
        case 5:
            cout << "Exiting game. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 5);

    return 0;
}