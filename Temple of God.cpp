#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

int health = 100;
int gold = 0;
int roomNumber = 1;

void intro() {

    cout << R"(

████████╗██████╗ ███████╗ █████╗ ███████╗██╗   ██╗██████╗ ███████╗
╚══██╔══╝██╔══██╗██╔════╝██╔══██╗██╔════╝██║   ██║██╔══██╗██╔════╝
   ██║   ██████╔╝█████╗  ███████║███████╗██║   ██║██████╔╝█████╗
   ██║   ██╔══██╗██╔══╝  ██╔══██║╚════██║██║   ██║██╔══██╗██╔══╝
   ██║   ██║  ██║███████╗██║  ██║███████║╚██████╔╝██║  ██║███████╗
   ╚═╝   ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝

              TEMPLE OF GOLD
    )";

    cout << "\n\n🏴‍☠️ Entering ancient treasure temple...\n";
}

void status() {
    cout << "\n================================";
    cout << "\n❤️ Health : " << health;
    cout << "\n💰 Gold   : " << gold;
    cout << "\n🚪 Room   : " << roomNumber;
    cout << "\n================================\n";
}

void roomArt() {

    int art = rand() % 3;

    if(art == 0) {

        cout << R"(

+-------------------+
|        💎         |
|      TREASURE     |
|                   |
|    [ CHEST ]      |
+-------------------+

)";
    }

    else if(art == 1) {

        cout << R"(

+-------------------+
|      ☠️ TRAP       |
|                   |
|    SPIKES BELOW   |
|                   |
+-------------------+

)";
    }

    else {

        cout << R"(

+-------------------+
|       👻 GHOST     |
|                   |
|    CURSED ROOM    |
|                   |
+-------------------+

)";
    }
}

void exploreRoom() {

    roomArt();

    int event = rand() % 5;

    if(event == 0) {
        cout << "\n💰 You found treasure!";
        gold += 50;
    }

    else if(event == 1) {
        cout << "\n☠️ Trap activated!";
        health -= 20;
    }

    else if(event == 2) {
        cout << "\n👻 Ghost attacked you!";
        health -= 15;
    }

    else if(event == 3) {
        cout << "\n🧪 You found a healing potion!";
        health += 20;

        if(health > 100)
            health = 100;
    }

    else {
        cout << "\n📦 Empty room...";
    }

    roomNumber++;
}

void choiceMenu() {

    int choice;

    cout << "\n1. Explore Next Room";
    cout << "\n2. Escape Temple";
    cout << "\n\nChoose: ";

    cin >> choice;

    if(choice == 1) {
        exploreRoom();
    }

    else if(choice == 2) {

        cout << "\n🏃 You escaped the temple!";

        if(gold >= 200)
            cout << "\n🏆 LEGENDARY TREASURE HUNTER!";
        else if(gold >= 100)
            cout << "\n💰 Successful treasure run!";
        else
            cout << "\n😅 Better luck next time.";

        exit(0);
    }

    else {
        cout << "\n❌ Invalid choice";
    }
}

int main() {

    srand(time(0));

    intro();

    while(health > 0) {

        status();

        choiceMenu();
    }

    cout << "\n\n💀 GAME OVER";
    cout << "\nYou were lost inside the temple...";

    return 0;
}
