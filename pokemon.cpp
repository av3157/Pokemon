#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Move {
    string name;
    int damage;
    int maxUses;
    int remainingUses;
};

class Pokemon {
public:
    string name;
    int hp; // current hp after battle
    int maxHp;
    vector<Move> moves;

    Pokemon(string n, int h, vector<Move> m)
        : name(n), hp(h), maxHp(h), moves(m) {}

    bool isDefeated() {
        return hp <= 0;
    }

    void takeDamage(int dmg) {
        hp -= dmg;
        if (hp < 0) hp = 0;
    }

    void displayHealth() {
        cout << name << " HP: " << hp << "/" << maxHp << "\n";
    }

    void showMoves() {
        for (int i = 0; i < moves.size(); i++) {
            cout << i + 1 << ". " << moves[i].name << " (Damage: "
                 << moves[i].damage << ", Uses Left: "
                 << moves[i].remainingUses << ")\n";
        }
    }
};

Pokemon getRandomWildPokemon() {
    int index = rand() % 6;
    switch (index) {
        case 0:
            return Pokemon("Rattata", 30, {
                {"Tackle", 10, 10, 10},
                {"Quick Attack", 5, 10, 10}
            });
        case 1:
            return Pokemon("Pidgey", 35, {
                {"Gust", 10, 10, 10},
                {"Tackle", 10, 10, 10}
            });
        case 2:
            return Pokemon("Snorlax", 80, {
                {"Body Slam", 20, 10, 10},
                {"Tackle", 10, 10, 10}
            });
        case 3:
            return Pokemon("Gastly", 35, {
                {"Lick", 10, 10, 10},
                {"Hex", 20, 10, 10}
            });
        case 4:
            return Pokemon("Electrode", 50, {
                {"Thunder Shock", 20, 10, 10},
                {"Rollout", 10, 10, 10}
            });
        case 5:
        default:
            return Pokemon("Weedle", 25, {
                {"Poison Sting", 5, 10, 10},
                {"Bug Bite", 10, 10, 10}
            });
    }
}

void battle(Pokemon& player, Pokemon& wild) {
    cout << "A wild " << wild.name << " appeared!\n";

    while (!player.isDefeated() && !wild.isDefeated()) {
        // Player's Turn
        cout << "\nYour turn:\n";
        player.displayHealth();
        wild.displayHealth();
        player.showMoves();

        int choice;
        while (true) {
            cout << "Choose a move by entering 1-" << player.moves.size() << ": ";
            if (!(cin >> choice) || choice < 1 || choice > player.moves.size()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Please enter a number between 1 and " << player.moves.size() << ".\n";
                continue;
            }
            if (choice >= 1 && choice <= player.moves.size()) {
                if (player.moves[choice - 1].remainingUses > 0) {
                    break;
                } else {
                    cout << "No remaining uses for that move.\n";
                }
            }
        }
        
        Move& m = player.moves[choice - 1];
        cout << player.name << " used " << m.name << "!\n";
        wild.takeDamage(m.damage);
        m.remainingUses--;

        if (wild.isDefeated()) break;

        // Wild's Turn
        Move& wm = wild.moves[rand() % wild.moves.size()];
        cout << wild.name << " used " << wm.name << "!\n";
        player.takeDamage(wm.damage);
    }

    if (player.isDefeated()) {
        cout << "The wild " << wild.name << " defeated your " << player.name << ".\n";
        cout << "You blacked out.\n";
        exit(0);
    } else {
        cout << "Your " << player.name << " defeated the wild " << wild.name << "!\n";
    }
}

int healthX = -1;
int healthY = -1;

void spawnHealth(int playerX, int playerY) {
    do {
        healthX = rand() % 10;
        healthY = rand() % 10;
    } while (healthX == playerX && healthY == playerY);
}

void displayGrid(int playerX, int playerY) {
    const int width = 10;
    const int height = 10;

    cout << "  +" << string(width * 2, '-') << "+\n"; // top border
    for (int y = height-1; y >= 0; --y) {
        cout << y << " |";
        for (int x = 0; x < width; ++x) {
            if (x == playerX && y == playerY) {
                cout << "P ";
            } else if (x == healthX && y == healthY) {
                cout << "H "; 
            } else {
                cout << ". ";
            }
        }
        cout << "|\n";
    }
    cout << "  +" << string(width * 2, '-') << "+\n"; // bottom border
    cout << "   ";
    for (int x = 0; x < width; ++x) cout << x << " ";
    cout << "\n";
}

void movePlayer(int &x, int &y) {
    string input;
    char direction;
    while (true) {
        cout << "It's time to move around and battle wild Pokemon!\n"; 
        cout << "To move, type U, D, L, or R: ";
        cin >> input;

        if (input.length() == 1) {
            direction = toupper(input[0]);
            if (direction == 'U' || direction == 'D' || direction == 'L' || direction == 'R') {
                break;
            }
        }
        cout << "Invalid input. Please enter U, D, L, or R.\n";
    }

    switch (direction) {
        case 'U': if (y < 9) y++; else cout << "You can't move further up!\n"; break;
        case 'D': if (y > 0) y--; else cout << "You can't move further down!\n"; break;
        case 'L': if (x > 0) x--; else cout << "You can't move further left!\n"; break;
        case 'R': if (x < 9) x++; else cout << "You can't move further right!\n"; break;
    }
}

int main() {
    srand(time(nullptr));

    // Starter choice
    vector<Pokemon> starters = {
        Pokemon("Charmander", 50, {{"Ember", 15, 10, 10}, {"Scratch", 10, 10, 10}, {"Fire Fang", 20, 8, 8}, {"Flame Burst", 25, 5, 5}}),
        Pokemon("Squirtle", 55, {{"Water Gun", 15, 10, 10}, {"Tackle", 10, 10, 10}, {"Rapid Spin", 15, 10, 10}, {"Bubble Beam", 25, 5, 5}}),
        Pokemon("Bulbasaur", 60, {{"Vine Whip", 15, 10, 10}, {"Tackle", 10, 10, 10}, {"Bullet Seed", 10, 15, 15}, {"Razor Leaf", 25, 5, 5}})
    };

    int choice;
    while(true) {
        cout << "Choose your starter:\n";
        for (int i = 0; i < 3; i++) {
            cout << i + 1 << ". " << starters[i].name << "\n";
        }
        cout << "Enter 1, 2, or 3: "; 
        if (!(cin >> choice) || choice < 1 || choice > 3) {
            cin.clear();              
            cin.ignore(1000, '\n');   
            cout << "Invalid input. Please enter 1, 2, or 3.\n";
            continue;
        }
        if (choice >= 1 && choice <= 3) break;
    }
    
    Pokemon player = starters[choice - 1];
    cout << "You chose " << player.name << "!\n"; 

    // Grid Navigation
    int x = 0, y = 0;
    spawnHealth(x, y);
    while (true) {
        displayGrid(x, y);
        movePlayer(x, y);

        if (x == healthX && y == healthY) {
            int preHeal = player.hp;
            player.hp = min(player.maxHp, player.hp + 20);
            cout << "You found a health potion and healed " << player.name << " by " << player.hp - preHeal << " HP!\n";
            cout << player.name << " now has " << player.hp << " health.\n";
            spawnHealth(x, y);
        }   

        if (rand() % 5 == 0) { 
            Pokemon wild = getRandomWildPokemon();
            battle(player, wild);
        }
    }
    return 0;
}
