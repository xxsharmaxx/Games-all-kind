#include <iostream>
using namespace std;

int main() {
    int choice;

    cout << "🕵️ DETECTIVE MYSTERY GAME\n";
    cout << "A murder has happened in a mansion...\n";
    cout << "You are the detective.\n\n";

    cout << "Where do you want to go first?\n";
    cout << "1. Living Room\n";
    cout << "2. Kitchen\n";
    cout << "3. Garden\n";
    cin >> choice;

    if (choice == 1) {
        cout << "\nYou found a broken glass and a note.\n";
        cout << "Do you:\n1. Read note\n2. Ignore\n";
        cin >> choice;

        if (choice == 1) {
            cout << "\nThe note says: 'He knows everything...'\n";
            cout << "Suspicious!\n";
        } else {
            cout << "\nYou missed an important clue!\n";
        }

    } else if (choice == 2) {
        cout << "\nKitchen is clean, but knife is missing!\n";
    } else {
        cout << "\nIn the garden, you find footprints.\n";
    }

    cout << "\nNow, who is the suspect?\n";
    cout << "1. Butler\n2. Chef\n3. Gardener\n";
    cin >> choice;

    if (choice == 1) {
        cout << "\n❌ Wrong! Butler is innocent.\n";
    } else if (choice == 2) {
        cout << "\n✅ Correct! Chef did it.\n";
    } else {
        cout << "\n❌ Wrong! Gardener is innocent.\n";
    }

    cout << "\nGame Over!\n";
    return 0;
}
