/**
 * @file     exer_4_10.cpp
 * @author   KaKaRot
 * @date     25/3/2026
 * @brief    PPP Chapter 4 exercise 10.
 * @details  Rock, Paper, Scissors game
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <random>

enum Choise { Rock = 0, Paper = 1, Scissors = 2, Invalid, Quit};

struct Score
{
    unsigned wins = 0;
    unsigned losses = 0;
    unsigned ties = 0;
};

Choise getComputerChoice()
{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dis(0, 2);
    return static_cast<Choise>(dis(gen));
}

int judge(Choise playerChoise, Choise computerChoice)
{
    if (playerChoise == computerChoice){ return 0;}
    int p = static_cast<int>(playerChoise);
    int c = static_cast<int>(computerChoice);
    return (p - c + 3) % 3 == 1 ? 1 : -1;
}

Choise praseInput(const std::string& line)
{
    if (line.empty()){ return Choise::Invalid; }
    
    std::stringstream ss(line);
    std::string token;

    if (ss >> token){

        if(token.size() != 1) { return Choise::Invalid; }

        char choise = static_cast<char>(std::tolower(static_cast<unsigned char>(token[0])));
        switch (choise)
        {
            case 'r':
                return Choise::Rock;
            case 'p':
                return Choise::Paper;
            case 's':
                return Choise::Scissors;
            case 'q':
                return Choise::Quit;
            default:
                return Choise::Invalid;
        }
    }
    else{
        return Choise::Invalid;
    }
}

void playRound(Choise playerChoise, Score& score)
{
    const std::vector<std::string> names{"Rock", "Paper", "Scissors"};

    Choise computerChoice = getComputerChoice();
    std::cout << "  You:      " << names[static_cast<int>(playerChoise)] << "\n";
    std::cout << "  Computer: " << names[static_cast<int>(computerChoice)] << "\n";

    int result = judge(playerChoise, computerChoice);
    if (result == 1) {
        std::cout << "  [Result]  You Win! :)\n";
        score.wins++;
    } else if (result == -1) {
        std::cout << "  [Result]  You Lose! :(\n";
        score.losses++;
    } else {
        std::cout << "  [Result]  It's a Tie! |\n";
        score.ties++;
    }
}

int main()
{
    std::cout << "===========================================\n";
    std::cout << "           Rock, Paper, Scissors           \n";
    std::cout << "        Enter r,p or s ('q' to quit )      \n";
    std::cout << "===========================================\n";

    Score score;
    std::string line_input;
    while (true)
    {
        std::cout << ">" << std::flush;
        if (!std::getline(std::cin, line_input)){
            break; 
        }
        
        Choise playerChoise = praseInput(line_input);

        if (playerChoise == Quit) { break; }
        if (playerChoise == Invalid) { std::cout << "  [!] Use r, p, or s. (q to quit)\n"; continue; }
        
        playRound(playerChoise, score);                 
    }

    int total = score.wins + score.losses + score.ties;
    if (total > 0) {
        std::cout << "\n--- Final Statistics ---\n"
                  << "  Total Rounds: " << total << "\n"
                  << "  Wins:   " << score.wins << "\n"
                  << "  Losses: " << score.losses << "\n"
                  << "  Ties:   " << score.ties << "\n";
    }

    std::cout << "Good luck! Bye~\n";
    return 0;
}
