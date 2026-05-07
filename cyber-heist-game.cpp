#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(0));

    int firewallCode = rand() % 9000 + 1000;
    int guess;
    int attempts = 5;

    cout << "=============================\n";
    cout << "   CYBER HEIST TERMINAL\n";
    cout << "=============================\n\n";

    cout << "Mission: Crack the firewall code.\n";
    cout << "You have 5 attempts.\n\n";

    while (attempts > 0) {

        cout << "Enter 4-digit code: ";
        cin >> guess;

        if (guess == firewallCode) {
            cout << "\n✅ ACCESS GRANTED\n";
            cout << "💰 Data stolen successfully!\n";
            break;
        }

        if (guess < firewallCode) {
            cout << "📉 Code too LOW\n";
        } else {
            cout << "📈 Code too HIGH\n";
        }

        attempts--;

        cout << "Attempts left: " << attempts << "\n\n";

        if (attempts == 0) {
            cout << "❌ ACCESS DENIED\n";
            cout << "🚨 Security traced your location!\n";
            cout << "Firewall Code was: " << firewallCode << "\n";
        }
    }

    return 0;
}
