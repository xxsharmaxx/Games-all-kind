#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace std::chrono;

int main() {
    srand(time(0));

    cout << "⚡ Reaction Time Game ⚡\n";
    cout << "Wait for the signal and press ENTER as fast as possible!\n\n";

    cout << "Get ready...\n";

    // Random delay between 2 to 5 seconds
    int delay = 2 + rand() % 4;
    this_thread::sleep_for(seconds(delay));

    cout << "\n🔥 PRESS ENTER NOW! 🔥\n";

    auto start = high_resolution_clock::now();

    cin.ignore(); // wait for user input

    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);

    cout << "⏱ Your reaction time: " << duration.count() << " ms\n";

    // Feedback
    if (duration.count() < 200) {
        cout << "🚀 Lightning fast!\n";
    } else if (duration.count() < 400) {
        cout << "👍 Good reaction!\n";
    } else {
        cout << "🐢 Too slow, try again!\n";
    }

    return 0;
}
