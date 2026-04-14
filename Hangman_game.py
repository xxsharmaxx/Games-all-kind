# Day 13: Hangman Game

import random

words = ["python", "github", "developer", "computer", "programming"]
word = random.choice(words)

guessed = ["_"] * len(word)
attempts = 6
guessed_letters = []

print("🎮 Welcome to Hangman Game!")
print("Guess the word:")

while attempts > 0 and "_" in guessed:
    print("\nWord:", " ".join(guessed))
    print("Attempts left:", attempts)

    guess = input("Enter a letter: ").lower()

    if guess in guessed_letters:
        print("You already guessed that letter!")
        continue

    guessed_letters.append(guess)

    if guess in word:
        print("Correct guess! ✅")
        for i in range(len(word)):
            if word[i] == guess:
                guessed[i] = guess
    else:
        print("Wrong guess! ❌")
        attempts -= 1

# Result
if "_" not in guessed:
    print("\n🎉 You won! The word was:", word)
else:
    print("\n💀 You lost! The word was:", word)
