#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Character {
public:
    Character(string name, int health, int attackPower, int defense, int experience = 0, int level = 1)
        : name(name), health(health), maxHealth(health), attackPower(attackPower), defense(defense), experience(experience), level(level),
        poisoned(false), poisonTurns(0) {}

    string getName() const { return name; }
    int getHealth() const { return health; }
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
        cout << "Character Stats:\n";
        cout << "Name: " << name << "\n";
        cout << "Health: " << health << "/" << maxHealth << "\n";
        cout << "Attack Power: " << attackPower << "\n";
        cout << "Defense: " << defense << "\n";
        cout << "Experience: " << experience << "\n";
        cout << "Level: " << level << "\n";
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

    // Future plan: fully restore HP and skip the night.
    void rest() {
        int restAmount = 30; // Amount of HP restored.
        heal(restAmount);
        cout << "You rest and restore " << restAmount << " health points. Your health is now " << health << ".\n";
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
};

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

//Combat system.
void battle(Character& character, Monster& monster) {
    cout << "\nA wild " << monster.getType() << " appeared!\n";

    while (character.isAlive() && monster.isAlive()) {
        if (character.isPoisoned()) {
            character.processPoisonDamage();
        }

        cout << "\n" << character.getName() << "'s turn:\n";
        monster.takeDamage(character.getAttackPower());
        cout << "You hit the " << monster.getType() << " for " << character.getAttackPower() << " damage!\n";

        if (!monster.isAlive()) {
            cout << "You defeated the " << monster.getType() << "!\n";
            character.gainExperience(monster.getExperienceValue());
            cout << "You gained " << monster.getExperienceValue() << " experience points.\n";
            return;
        }

        cout << "\n" << monster.getType() << "'s turn:\n";
        if (!monster.getAbility().empty()) {
            monster.useAbility(character);
        }
        character.takeDamage(monster.getAttackPower());
        cout << "The " << monster.getType() << " hit you for " << monster.getAttackPower() << " damage!\n";
        cout << "Your health is now " << character.getHealth() << "\n";

        if (!character.isAlive()) {
            cout << "You have been defeated by the " << monster.getType() << "...\n";
            cout << "Game Over. You lost.\n";
            exit(0); // End the game
        }
    }
}

//Quest system.
void questForestBeast(Character& character) {
    cout << "\nA village elder approaches you with a plea for help.\n";
    cout << "Elder: \"Please, brave adventurer, a terrible beast has been terrorizing our village. Will you help us?\"\n";

    Monster forestBeast("Forest Beast", 60, 18, 5, 70, "Ferocious Bite");

    cout << "\nYou venture into the forest to confront the beast...\n";
    battle(character, forestBeast);

    if (character.isAlive()) {
        cout << "\nElder: \"Thank you, hero! Our village is saved!\"\n";
        character.gainExperience(100);  // Quest reward experience
        cout << "You gained 100 experience points.\n";
    }
    else {
        cout << "Elder: \"Alas, our hopes are dashed...\"\n";
    }
}

void questCursedCave(Character& character) {
    cout << "\nA wounded knight stumbles towards you.\n";
    cout << "Knight: \"Adventurer, beware... A mighty dragon has taken refuge in the cursed cave. I could not defeat it. Can you?\"\n";

    Monster caveDragon("Cave Dragon", 120, 25, 10, 150, "Fire Breath");

    cout << "\nYou steel yourself and enter the cursed cave...\n";
    battle(character, caveDragon);

    if (character.isAlive()) {
        cout << "\nKnight: \"Incredible! You have slain the dragon!\"\n";
        character.gainExperience(200);  // Quest reward experience
        cout << "You gained 200 experience points.\n";
    }
    else {
        cout << "Knight: \"Another brave soul lost to the dragon...\"\n";
    }
}

void questLostArtifact(Character& character) {
    cout << "\nA mysterious figure cloaked in shadows approaches you.\n";
    cout << "Mysterious Figure: \"A powerful artifact has been stolen by the goblin chief. Retrieve it, and I shall reward you handsomely.\"\n";

    Monster goblinChief("Goblin Chief", 100, 12, 4, 50, "Poisoned Dagger");

    cout << "\nYou journey into the goblin's lair to retrieve the artifact...\n";
    battle(character, goblinChief);

    if (character.isAlive()) {
        cout << "\nMysterious Figure: \"You have done well, hero. Here is your reward.\"\n";
        character.gainExperience(120);  // Quest reward experience
        cout << "You gained 120 experience points.\n";
    }
    else {
        cout << "Mysterious Figure: \"Another failure... How disappointing.\"\n";
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "Welcome to my RPG Game!\n";
    string playerName;
    cout << "Enter your character's name: ";
    getline(cin, playerName);

    Character player(playerName, 100, 20, 5);

    int choice;
    do {
        cout << "\nMain Menu\n";
        cout << "1. View Character Stats\n";
        cout << "2. Go on a Quest\n";
        cout << "3. Rest\n";
        cout << "4. Exit Game\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            player.displayStats();
            break;
        case 2:
            int questChoice;
            cout << "\nChoose a Quest:\n";
            cout << "1. Defeat the Forest Beast\n";
            cout << "2. Slay the Dragon in the Cursed Cave\n";
            cout << "3. Retrieve the Lost Artifact from the Goblin Chief\n";
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
            default:
                cout << "Invalid quest choice.\n";
                break;
            }
            break;
        case 3:
            player.rest();
            break;
        case 4:
            cout << "Exiting game. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 4);

    return 0;
}

//To add:
// Apocalypse, make it so that you only have set amount of days to fight a final boss or the world ends.
// Add more quests and make them change every couple levels or so(also make levels incremental)
// items maybe ? Or a permanent upgrade after certain quests so you feel inclined to do it even tho it gives less exp
// Add at least 2 quest chains! (main quests)
// Crit system.
