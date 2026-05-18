#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

vector<string> happyReplies = {
    "That sounds awesome 😄",
    "I'm glad to hear that!",
    "Life is better with good vibes 🔥"
};

vector<string> sadReplies = {
    "Stay strong 💙",
    "Every storm passes eventually.",
    "Bad days don't last forever."
};

vector<string> angryReplies = {
    "Take a deep breath 😤",
    "Calm mind wins battles.",
    "Don't let anger control you."
};

vector<string> normalReplies = {
    "Interesting...",
    "Tell me more.",
    "Hmm 🤔",
    "That's cool!"
};

void typingEffect(string text) {

    cout << "\nAI: ";

    for(char c : text) {
        cout << c;
        cout.flush();

        this_thread::sleep_for(chrono::milliseconds(35));
    }

    cout << "\n";
}

string detectMood(string input) {

    for(char &c : input)
        c = tolower(c);

    if(input.find("happy") != string::npos ||
       input.find("great") != string::npos ||
       input.find("good") != string::npos)
        return "happy";

    if(input.find("sad") != string::npos ||
       input.find("depressed") != string::npos ||
       input.find("cry") != string::npos)
        return "sad";

    if(input.find("angry") != string::npos ||
       input.find("mad") != string::npos ||
       input.find("hate") != string::npos)
        return "angry";

    return "normal";
}

string randomReply(vector<string> replies) {
    return replies[rand() % replies.size()];
}

void intro() {

    cout << R"(

 █████╗ ██╗
██╔══██╗██║
███████║██║
██╔══██║██║
██║  ██║██║
╚═╝  ╚═╝╚═╝

 TERMINAL AI CHAT BOT

)";
}

int main() {

    srand(time(0));

    intro();

    string input;

    cout << "\nType 'exit' to quit.\n";

    while(true) {

        cout << "\nYou: ";
        getline(cin, input);

        if(input == "exit") {
            typingEffect("Goodbye human 👋");
            break;
        }

        string mood = detectMood(input);

        if(mood == "happy") {
            typingEffect(randomReply(happyReplies));
        }

        else if(mood == "sad") {
            typingEffect(randomReply(sadReplies));
        }

        else if(mood == "angry") {
            typingEffect(randomReply(angryReplies));
        }

        else {
            typingEffect(randomReply(normalReplies));
        }
    }

    return 0;
}
