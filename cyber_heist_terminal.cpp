#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

int health = 100;
int score = 0;
int level = 1;

string generatePassword() {
    string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789";
    string pass = "";

    for(int i = 0; i < 4; i++) {
        pass += chars[rand() % chars.size()];
    }

    return pass;
}

void loading() {
    cout << "\nConnecting";
    for(int i = 0; i < 5; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(400));
    }
    cout << "\n";
}

void mission() {
    string password = generatePassword();
    string guess;

    cout << "\n==============================";
    cout << "\n LEVEL " << level;
    cout << "\n==============================";

    cout << "\n\nTarget Password: ";
    
    for(char c : password) {
        cout << "* ";
    }

    cout << "\n\nHints:\n";

    for(char c : password) {
        cout << c << " ";
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    cout << "\n\nEnter Password: ";
    cin >> guess;

    if(guess == password) {
        cout << "\n✅ ACCESS GRANTED";
        score += 50;
        level++;
    } else {
        cout << "\n❌ ACCESS DENIED";
        health -= 25;
    }

    cout << "\n\nHealth: " << health;
    cout << "\nScore: " << score << "\n";
}

void intro() {
    cout << R"(

   ██████╗██╗   ██╗██████╗ ███████╗██████╗ 
  ██╔════╝╚██╗ ██╔╝██╔══██╗██╔════╝██╔══██╗
  ██║      ╚████╔╝ ██████╔╝█████╗  ██████╔╝
  ██║       ╚██╔╝  ██╔══██╗██╔══╝  ██╔══██╗
  ╚██████╗   ██║   ██████╔╝███████╗██║  ██║
   ╚═════╝   ╚═╝   ╚═════╝ ╚══════╝╚═╝  ╚═╝

        CYBER HEIST TERMINAL
    )";

    cout << "\n\n🔥 Initiating Hacker Mode...\n";
}

int main() {
    srand(time(0));

    intro();
    loading();

    while(health > 0 && level <= 5) {
        mission();
    }

    if(health <= 0) {
        cout << "\n\n💀 SYSTEM FAILURE";
    } else {
        cout << "\n\n🏆 ALL SYSTEMS HACKED";
    }

    cout << "\nFinal Score: " << score << "\n";

    return 0;
}
