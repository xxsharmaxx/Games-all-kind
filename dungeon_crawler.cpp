#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ctime>
#include <cctype>

using namespace std;

// ============================================================
// DUNGEON CRAWLER
// Single-file C++ Console RPG
// ============================================================

struct Position {
    int x;
    int y;
};

struct Weapon {
    string name;
    int damage;
    int price;
};

class Player {
public:
    string name;
    Position pos{1, 1};

    int hp = 100;
    int maxHp = 100;
    int attack = 15;
    int defense = 5;

    int level = 1;
    int xp = 0;
    int gold = 50;

    Weapon weapon{"Iron Sword", 15, 30};

    int potions = 2;
    int kills = 0;

    void showStats() const {
        cout << "\n====================================\n";
        cout << "           PLAYER STATS\n";
        cout << "====================================\n";
        cout << "Name      : " << name << '\n';
        cout << "Level     : " << level << '\n';
        cout << "HP        : " << hp << "/" << maxHp << '\n';
        cout << "Attack    : " << attack << '\n';
        cout << "Defense   : " << defense << '\n';
        cout << "Weapon    : " << weapon.name << '\n';
        cout << "Damage    : " << weapon.damage << '\n';
        cout << "XP        : " << xp << '\n';
        cout << "Gold      : " << gold << '\n';
        cout << "Potions   : " << potions << '\n';
        cout << "Kills     : " << kills << '\n';
        cout << "====================================\n";
    }

    void heal() {
        if (potions <= 0) {
            cout << "You have no potions!\n";
            return;
        }

        if (hp == maxHp) {
            cout << "Your health is already full.\n";
            return;
        }

        int oldHp = hp;

        hp = min(maxHp, hp + 35);
        potions--;

        cout << "You used a potion.\n";
        cout << "HP restored: " << hp - oldHp << '\n';
    }

    void gainXP(int amount) {
        xp += amount;

        cout << "You gained " << amount << " XP!\n";

        int required = level * 100;

        while (xp >= required) {
            xp -= required;
            level++;

            maxHp += 20;
            hp = maxHp;
            attack += 5;
            defense += 2;

            cout << "\n*** LEVEL UP! ***\n";
            cout << "You are now level " << level << "!\n";
            cout << "Max HP increased.\n";
            cout << "Attack increased.\n";
            cout << "Defense increased.\n";

            required = level * 100;
        }
    }
};

class Enemy {
public:
    string name;
    int hp;
    int maxHp;
    int attack;
    int defense;
    int xpReward;
    int goldReward;
    char symbol;

    Enemy(
        string n,
        int h,
        int a,
        int d,
        int xp,
        int gold,
        char s
    )
        : name(n),
          hp(h),
          maxHp(h),
          attack(a),
          defense(d),
          xpReward(xp),
          goldReward(gold),
          symbol(s) {}

    bool alive() const {
        return hp > 0;
    }
};

// ============================================================
// GLOBAL RANDOM ENGINE
// ============================================================

random_device rd;
mt19937 rng(rd());

int randomInt(int minValue, int maxValue) {
    uniform_int_distribution<int> dist(minValue, maxValue);
    return dist(rng);
}

// ============================================================
// DUNGEON
// ============================================================

class Dungeon {
private:
    vector<string> map;

public:
    int width = 31;
    int height = 15;

    Dungeon() {
        generate();
    }

    void generate() {
        map.assign(height, string(width, '#'));

        // Create basic corridors.
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {

                if (randomInt(1, 100) <= 68) {
                    map[y][x] = '.';
                }
            }
        }

        // Guarantee central paths.
        for (int x = 1; x < width - 1; x++)
            map[1][x] = '.';

        for (int y = 1; y < height - 1; y++)
            map[y][width / 2] = '.';

        map[1][1] = 'S';

        // Treasure.
        for (int i = 0; i < 8; i++) {
            int x = randomInt(2, width - 2);
            int y = randomInt(2, height - 2);

            if (map[y][x] == '.')
                map[y][x] = '$';
        }

        // Potions.
        for (int i = 0; i < 4; i++) {
            int x = randomInt(2, width - 2);
            int y = randomInt(2, height - 2);

            if (map[y][x] == '.')
                map[y][x] = 'P';
        }

        // Traps.
        for (int i = 0; i < 6; i++) {
            int x = randomInt(2, width - 2);
            int y = randomInt(2, height - 2);

            if (map[y][x] == '.')
                map[y][x] = '^';
        }

        // Exit.
        map[height - 2][width - 2] = 'E';
    }

    bool walkable(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return false;

        return map[y][x] != '#';
    }

    char getTile(int x, int y) const {
        return map[y][x];
    }

    void clearTile(int x, int y) {
        if (map[y][x] != 'S' && map[y][x] != 'E')
            map[y][x] = '.';
    }

    void display(const Player& player,
                 const vector<Position>& enemies) const {

        cout << "\n";

        for (int y = 0; y < height; y++) {

            for (int x = 0; x < width; x++) {

                if (player.pos.x == x &&
                    player.pos.y == y) {

                    cout << '@';
                    continue;
                }

                bool enemyHere = false;

                for (const auto& enemy : enemies) {

                    if (enemy.x == x &&
                        enemy.y == y) {

                        cout << 'M';
                        enemyHere = true;
                        break;
                    }
                }

                if (!enemyHere)
                    cout << map[y][x];
            }

            cout << '\n';
        }

        cout << "\n@ = Player\n";
        cout << "M = Monster\n";
        cout << "$ = Treasure\n";
        cout << "P = Potion\n";
        cout << "^ = Trap\n";
        cout << "E = Exit\n";
        cout << "# = Wall\n";
    }
};

// ============================================================
// COMBAT
// ============================================================

bool playerAttack(Player& player, Enemy& enemy) {

    int damage =
        max(
            1,
            player.weapon.damage +
            player.attack / 2 -
            enemy.defense
        );

    // Critical hit.
    bool critical = randomInt(1, 100) <= 15;

    if (critical) {
        damage *= 2;
        cout << "\n*** CRITICAL HIT! ***\n";
    }

    enemy.hp -= damage;

    cout << "You hit the "
         << enemy.name
         << " for "
         << damage
         << " damage.\n";

    if (enemy.hp <= 0) {

        cout << "\nYou defeated the "
             << enemy.name
             << "!\n";

        cout << "Gold: +" << enemy.goldReward << '\n';

        player.gold += enemy.goldReward;
        player.kills++;

        player.gainXP(enemy.xpReward);

        return true;
    }

    return false;
}

void enemyAttack(Player& player, Enemy& enemy) {

    int damage =
        max(
            1,
            enemy.attack -
            player.defense
        );

    player.hp -= damage;

    cout << enemy.name
         << " attacks you for "
         << damage
         << " damage!\n";

    if (player.hp <= 0)
        player.hp = 0;
}

// ============================================================
// ENEMY GENERATION
// ============================================================

Enemy createEnemy(int level) {

    int type = randomInt(1, 4);

    if (type == 1) {
        return Enemy(
            "Goblin",
            35 + level * 5,
            10 + level * 2,
            2 + level,
            40 + level * 10,
            15 + level * 5,
            'G'
        );
    }

    if (type == 2) {
        return Enemy(
            "Skeleton",
            45 + level * 6,
            13 + level * 2,
            4 + level,
            55 + level * 10,
            20 + level * 5,
            'S'
        );
    }

    if (type == 3) {
        return Enemy(
            "Orc",
            65 + level * 8,
            17 + level * 3,
            6 + level,
            75 + level * 12,
            30 + level * 6,
            'O'
        );
    }

    return Enemy(
        "Dark Knight",
        90 + level * 10,
        20 + level * 3,
        10 + level,
        120 + level * 15,
        50 + level * 8,
        'K'
    );
}

// ============================================================
// MAIN GAME
// ============================================================

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Player player;
    Dungeon dungeon;

    vector<Position> enemies;

    cout << "============================================\n";
    cout << "           DUNGEON CRAWLER\n";
    cout << "============================================\n";

    cout << "Enter your hero name: ";
    getline(cin, player.name);

    if (player.name.empty())
        player.name = "Hero";

    // Spawn enemies.
    for (int i = 0; i < 7; i++) {

        Position p;

        do {
            p.x = randomInt(2, dungeon.width - 2);
            p.y = randomInt(2, dungeon.height - 2);

        } while (
            !dungeon.walkable(p.x, p.y) ||
            (p.x == player.pos.x &&
             p.y == player.pos.y)
        );

        enemies.push_back(p);
    }

    vector<Enemy> enemyData;

    for (size_t i = 0; i < enemies.size(); i++)
        enemyData.push_back(
            createEnemy(player.level)
        );

    bool running = true;
    bool victory = false;

    while (running && player.hp > 0) {

        cout << "\n\n";

        dungeon.display(player, enemies);

        cout << "\nHP: "
             << player.hp
             << "/"
             << player.maxHp;

        cout << " | Gold: "
             << player.gold;

        cout << " | Level: "
             << player.level;

        cout << "\n\n";

        cout << "[W] Move Up\n";
        cout << "[S] Move Down\n";
        cout << "[A] Move Left\n";
        cout << "[D] Move Right\n";
        cout << "[I] Inventory\n";
        cout << "[P] Potion\n";
        cout << "[Q] Quit\n";

        cout << "\nChoose: ";

        char command;
        cin >> command;

        command = static_cast<char>(
            tolower(command)
        );

        if (command == 'q') {
            running = false;
            break;
        }

        if (command == 'i') {
            player.showStats();
            continue;
        }

        if (command == 'p') {
            player.heal();
            continue;
        }

        int dx = 0;
        int dy = 0;

        if (command == 'w')
            dy = -1;

        else if (command == 's')
            dy = 1;

        else if (command == 'a')
            dx = -1;

        else if (command == 'd')
            dx = 1;

        else {
            cout << "Invalid command.\n";
            continue;
        }

        int newX = player.pos.x + dx;
        int newY = player.pos.y + dy;

        if (!dungeon.walkable(newX, newY)) {

            cout << "You cannot move there.\n";
            continue;
        }

        // Check enemy collision.
        int enemyIndex = -1;

        for (size_t i = 0; i < enemies.size(); i++) {

            if (
                enemies[i].x == newX &&
                enemies[i].y == newY
            ) {
                enemyIndex = static_cast<int>(i);
                break;
            }
        }

        // Combat.
        if (enemyIndex != -1) {

            Enemy& enemy =
                enemyData[enemyIndex];

            cout << "\n====================================\n";
            cout << "         ENCOUNTER!\n";
            cout << "====================================\n";

            cout << "Enemy: "
                 << enemy.name
                 << '\n';

            cout << "HP: "
                 << enemy.hp
                 << "/"
                 << enemy.maxHp
                 << "\n\n";

            while (enemy.alive() &&
                   player.hp > 0) {

                cout << "\n[A] Attack\n";
                cout << "[P] Potion\n";
                cout << "[R] Run\n";
                cout << "Choice: ";

                char action;
                cin >> action;

                action = static_cast<char>(
                    tolower(action)
                );

                if (action == 'a') {

                    bool defeated =
                        playerAttack(
                            player,
                            enemy
                        );

                    if (defeated) {

                        enemies.erase(
                            enemies.begin() +
                            enemyIndex
                        );

                        enemyData.erase(
                            enemyData.begin() +
                            enemyIndex
                        );

                        player.pos.x = newX;
                        player.pos.y = newY;

                        break;
                    }

                    enemyAttack(
                        player,
                        enemy
                    );
                }

                else if (action == 'p') {

                    player.heal();

                    if (player.hp > 0)
                        enemyAttack(
                            player,
                            enemy
                        );
                }

                else if (action == 'r') {

                    cout << "You escaped!\n";
                    break;
                }

                else {
                    cout << "Invalid action.\n";
                }
            }

            continue;
        }

        // Move player.
        player.pos.x = newX;
        player.pos.y = newY;

        char tile =
            dungeon.getTile(newX, newY);

        // Treasure.
        if (tile == '$') {

            int gold =
                randomInt(20, 80);

            player.gold += gold;

            cout << "\nYou found a treasure chest!\n";
            cout << "Gold found: "
                 << gold
                 << '\n';

            dungeon.clearTile(
                newX,
                newY
            );
        }

        // Potion.
        else if (tile == 'P') {

            player.potions++;

            cout << "\nYou found a health potion!\n";
            cout << "Potions: "
                 << player.potions
                 << '\n';

            dungeon.clearTile(
                newX,
                newY
            );
        }

        // Trap.
        else if (tile == '^') {

            int damage =
                randomInt(10, 25);

            player.hp -= damage;

            cout << "\n!!! TRAP !!!\n";
            cout << "You took "
                 << damage
                 << " damage!\n";

            dungeon.clearTile(
                newX,
                newY
            );
        }

        // Exit.
        else if (tile == 'E') {

            if (enemies.empty()) {

                victory = true;
                running = false;

            } else {

                cout << "\nThe dungeon exit is locked!\n";
                cout << "Defeat all remaining monsters first.\n";
            }
        }
    }

    // ========================================================
    // END GAME
    // ========================================================

    cout << "\n\n";

    if (victory) {

        cout << "============================================\n";
        cout << "             🏆 VICTORY!\n";
        cout << "============================================\n";
        cout << "You escaped the dungeon!\n";
        cout << "Hero: " << player.name << '\n';
        cout << "Level: " << player.level << '\n';
        cout << "Kills: " << player.kills << '\n';
        cout << "Gold : " << player.gold << '\n';
        cout << "============================================\n";
    }

    else if (player.hp <= 0) {

        cout << "============================================\n";
        cout << "              GAME OVER\n";
        cout << "============================================\n";
        cout << "Your hero has fallen in the dungeon.\n";
        cout << "Enemies defeated: "
             << player.kills
             << '\n';
        cout << "============================================\n";
    }

    else {

        cout << "Thanks for playing!\n";
    }

    return 0;
}
