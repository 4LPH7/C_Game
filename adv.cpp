#include <iostream>
#include <string>
#include <map>
#include <vector>

// Forward declarations
class Room;
class Player;

// Item class
class Item {
public:
    Item(const std::string& name, const std::string& description)
        : name(name), description(description) {}

    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

private:
    std::string name;
    std::string description;
};

// Room class
class Room {
public:
    Room(const std::string& name, const std::string& description)
        : name(name), description(description) {}

    void addExit(const std::string& direction, Room* room) {
        exits[direction] = room;
    }

    void addItem(const Item& item) {
        items.push_back(item);
    }

    void describe() const {
        std::cout << "You are in " << name << ". " << description << std::endl;
        if (!items.empty()) {
            std::cout << "You see: ";
            for (const auto& item : items) {
                std::cout << item.getName() << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Exits: ";
        for (const auto& exit : exits) {
            std::cout << exit.first << " ";
        }
        std::cout << std::endl;
    }

    Room* getExit(const std::string& direction) const {
        auto it = exits.find(direction);
        if (it != exits.end()) {
            return it->second;
        }
        return nullptr;
    }

    Item takeItem(const std::string& itemName) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->getName() == itemName) {
                Item item = *it;
                items.erase(it);
                return item;
            }
        }
        return Item("", ""); // Return an empty item if not found
    }

private:
    std::string name;
    std::string description;
    std::map<std::string, Room*> exits;
    std::vector<Item> items;
};

// Player class
class Player {
public:
    Player(Room* startingRoom) : currentRoom(startingRoom) {}

    void move(const std::string& direction) {
        Room* nextRoom = currentRoom->getExit(direction);
        if (nextRoom) {
            currentRoom = nextRoom;
            currentRoom->describe();
        } else {
            std::cout << "You can't go that way." << std::endl;
        }
    }

    void takeItem(const std::string& itemName) {
        Item item = currentRoom->takeItem(itemName);
        if (item.getName() != "") {
            inventory.push_back(item);
            std::cout << "You picked up the " << item.getName() << "." << std::endl;
        } else {
            std::cout << "There is no " << itemName << " here." << std::endl;
        }
    }

    void showInventory() const {
        if (inventory.empty()) {
            std::cout << "Your inventory is empty." << std::endl;
        } else {
            std::cout << "Your inventory: ";
            for (const auto& item : inventory) {
                std::cout << item.getName() << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    Room* currentRoom;
    std::vector<Item> inventory;
};

// Game class
class Game {
public:
    Game() {
        // Create rooms
        Room* room1 = new Room("Forest", "A dense forest with tall trees.");
        Room* room2 = new Room("Cave", "A dark cave with a faint glow.");
        Room* room3 = new Room("Lake", "A serene lake surrounded by mountains.");

        // Connect rooms
        room1->addExit("north", room2);
        room2->addExit("south", room1);
        room2->addExit("east", room3);
        room3->addExit("west", room2);

        // Add items
        room1->addItem(Item("Sword", "A sharp sword."));
        room2->addItem(Item("Lantern", "A glowing lantern."));

        // Set starting room
        player = new Player(room1);
    }

    ~Game() {
        delete player;
    }

    void start() {
        std::cout << "Welcome to the Text-Based Adventure Game!" << std::endl;
        player->getCurrentRoom()->describe();

        std::string input;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);

            if (input == "quit") {
                std::cout << "Goodbye!" << std::endl;
                break;
            } else if (input.substr(0, 3) == "go ") {
                player->move(input.substr(3));
            } else if (input.substr(0, 5) == "take ") {
                player->takeItem(input.substr(5));
            } else if (input == "inventory") {
                player->showInventory();
            } else {
                std::cout << "Invalid command. Try 'go [direction]', 'take [item]', or 'inventory'." << std::endl;
            }
        }
    }

private:
    Player* player;
};

// Main function
int main() {
    Game game;
    game.start();
    return 0;
}
