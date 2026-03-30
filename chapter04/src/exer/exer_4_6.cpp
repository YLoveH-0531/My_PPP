/**
 * @file     exer_4_6.cpp
 * @author   KaKaRot
 * @date     2026
 * @brief    PPP Chapter 4 exercise 6.
 * @details  converts a digit to its corresponding spelled-out value and in a opposed way respectively
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

const std::vector<std::string> vec_spelled{"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

bool game_One()
{
    std::cout << "\n=================================================\n";
    std::cout << "*** Welcome to game One ***\n";
    std::cout << "Input digits (0-9) separated by spaces.\n";
    std::cout << "Type 'back' to return to Main Menu, 'q' to quit.\n";  
    std::cout << "===================================================\n> " << std::flush;

    std::string line_input;

    while(true)
    {
        bool has_valid_digit = false;
        std::cout << "Game one >" << std::flush;
        if (!(std::getline(std::cin, line_input))){ return false; }

        if (line_input == "back"){ return true; }
        
        if (line_input == "quit" || line_input == "q"){ return false; }
        
        std::stringstream ss(line_input);
        int digit;

        while (ss >> digit)
        {
            if (digit >= 0 && digit <=9)
            {
                has_valid_digit = true;
                std::cout << digit << "'s spelled-out value is " << vec_spelled[digit] << "\n";
            }
            else{
                std::cout << "Out of range \n";         
            }          
        }

        if (ss.fail() && !ss.eof())
        {
            if (!has_valid_digit && line_input != "")
            {
                std::cout << "No valid digits found. Please try again.\n";
            }
            else{
                std::cout << "Illegal foramt! \n";
            }   
        }  
    }

    // ***version two
    // bool repeat = true;
    // while (repeat && std::getline(std::cin, line_input))
    // {
    //     std::stringstream ss(line_input);
    //     int digit;
    //     while (ss >> digit)
    //     {
    //         if (digit >= 0 && digit <=9)
    //         {
    //             std::cout << digit << "'s spelled-out value is " << vec_spelled[digit] << "\n";
    //         }
    //         else{
    //             std::cout << "Null \n";         
    //         }        
    //     }
    //     if (ss.flags() == ss.fail())
    //     {
    //         std::cout << "Illegel foramt! \n";
    //     }
    //     std::cout << "\n============================================\n";
    //     std::cout << "              Menu:\n";
    //     std::cout << " [1] Input 'yes' to continue\n";
    //     std::cout << " [2] Input 'no' to go back main menu\n";
    //     std::cout << " [q] Input 'quit' to exit\n";
    //     std::cout << "============================================\n> " << std::flush;

    //     std::string choise;
    //     while (std::getline(std::cin, choise))
    //     {
    //         if (choise == "yes"){
    //             std::cout << "\n============================================\n";
    //             std::cout << "*** Welcome to game One ***\n";
    //             std::cout << "Please input a sequence of digit number(0 to 9)\n";
    //             std::cout << "============================================\n> " << std::flush;
    //             break;
    //         }else if (choise == "no"){
    //             return true;
    //         }else if (choise == "quit"){
    //             return false;
    //         }else{
    //             std::cout << "Selection Input Format Error! Please Re-input:" << std::endl;
    //         }            
    //     }        
    // }
    // return true;
}

bool game_Two()
{
    std::cout << "\n=================================================\n";
    std::cout << "*** Welcome to game Two ***\n";
    std::cout << "Please input a sequence of spelled out number(eg, one, two...)\n";
    std::cout << "Type 'back' to return to Main Menu, 'q' to quit.\n";  
    std::cout << "===================================================\n> " << std::flush;
    std::string line_input;

    while(true)
    {
        std::cout << "Game two >" << std::flush;
        if (!(std::getline(std::cin, line_input))){ return false; }
        if (line_input == "back") { return true; }
        if (line_input == "quit" || line_input == "q") { return false; }

        std::stringstream ss(line_input);
        std::string spell_word;
        while(ss >> spell_word)
        {
            auto it = std::find(vec_spelled.cbegin(), vec_spelled.cend(), spell_word);
            if (it != vec_spelled.cend()){
                unsigned index = std::distance(vec_spelled.cbegin(), it);
                std::cout << spell_word << "'s digit value is " << index << "\n";             
            }else{
                std::cout << "Unkonw word \n";         
            }  
        }  
    }

    // *** Version two
    // bool repeat = true;
    // while (repeat && std::getline(std::cin, line_input))
    // {
    //     std::stringstream ss(line_input);
    //     std::string spell_word;
    //     while (ss >> spell_word)
    //     {
    //         auto it = std::find(vec_spelled.cbegin(), vec_spelled.cend(), spell_word);
    //         if (it != vec_spelled.cend()){
    //             unsigned index = std::distance(vec_spelled.cbegin(), it);
    //             std::cout << spell_word << "'s digit value is " << index << "\n";             
    //         }else{
    //             std::cout << "Null \n";         
    //         }        
    //     }

    //     std::cout << "\n============================================\n";
    //     std::cout << "              Menu:\n";
    //     std::cout << " [1] Input 'yes' to continue\n";
    //     std::cout << " [2] Input 'no' to go back main menu\n";
    //     std::cout << " [q] Input 'quit' to exit\n";
    //     std::cout << "============================================\n> " << std::flush;

    //     std::string choise;
    //     while (std::getline(std::cin, choise))
    //     {
    //         if (choise == "yes"){
    //             std::cout << "\n============================================\n";
    //             std::cout << "*** Welcome to game Two ***\n";
    //             std::cout << "Please input a sequence of spelled out number(eg, one, two...)\n";
    //             std::cout << "============================================\n> " << std::flush;
    //             break;
    //         }else if (choise == "no"){
    //             return true;
    //         }else if (choise == "quit"){
    //             return false;
    //         }else{
    //             std::cout << "Selection Input Format Error! Please Re-input:" << std::endl;
    //         }            
    //     }      
    // }
    // return true; 
}

int main()
{
    std::string game_selection;
    bool repeat = true; 

    while (repeat)
    {
        std::cout << "\n============================================\n";
        std::cout << "Choose a game to play:\n";
        std::cout << " [1] Input 'one' to convert digits to words\n";
        std::cout << " [2] Input 'two' to convert words to digits\n";
        std::cout << " [q] Input 'quit'(q) to exit\n";
        std::cout << "============================================\n> " << std::flush;

        std::getline(std::cin, game_selection);
        if (game_selection == "one")
        {
            repeat = game_One();   
        }else if (game_selection == "two")
        {
            repeat = game_Two();
        }
        else if(game_selection == "q" || game_selection == "quit")
        {
            std::cout << "Good Luck! Bye!" << std::endl;
            break;
        }
        else{
            std::cout << "Please input right format!" << std::endl;
        }
    }   
    return 0;

    /* original version
    std::string game_select;
    std::cout << "Choose game you want to play(Input onw or two, Press q to exit)\n" << std::flush;
    std::cout << " ***one*** convert a digit to its corresponding spelled-out value\n" << std::flush;
    std::cout << " ***two*** convert a spelled-out numbers into their digit form\n" << std::flush;

    while (std::cin >> game_select)
    {
        if (game_select == "one")
        {
            bool repeat_one = true;
            static const std::vector<std::string> vec_spellout{"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
            std::cout << "*** Welcome to game One ***" << std::endl;
            std::cout << "Please input a sequence of digit number(0 to 9)" << std::endl;

            std::string line;
            while(repeat_one && std::cin >> line)
            {
                std::stringstream ss(line);
                int digit;
                while (ss >> digit)
                {
                    switch(digit)
                    {
                        case 0:
                            std::cout << digit << "'s spelled-out value is " << vec_spellout[digit];
                            break;
                        case 1:
                            std::cout << digit << "'s spelled-out value is " << vec_spellout[digit];
                            break;
                        case 2:
                            std::cout << digit << "'s spelled-out value is " << vec_spellout[digit];
                            break;
                        case 3:
                            std::cout << digit << "'s spelled-out value is " << vec_spellout[digit];
                            break;
                        case 4:
                            std::cout << digit << "'s spelled-out value is " << vec_spellout[digit];
                            break;
                        case 5:
                            std::cout << digit << "'s spelled-out value is " << vec_spellout[digit];
                            break;
                        case 6:
                            std::cout << digit << "'s spelled-out value is " << vec_spellout[digit];
                            break;
                        case 7:
                            std::cout << digit << "'s spelled-out value is " << vec_spellout[digit];
                            break;
                        case 8:
                            std::cout << digit << "'s spelled-out value is " << vec_spellout[digit];
                            break;
                        case 9:
                            std::cout << digit << "'s spelled-out value is " << vec_spellout[digit];
                            break;
                        default:
                            std::cout << "Illegal input! Ignored: " << digit << std::endl;
                            break;
                    }
                }

                std::cout << " ***zero*** Go Back to main\n" << std::flush;
                std::cout << " ***one***  Continue\n" << std::flush;
                std::string select;

                while (std::cin >> select)
                {
                    if (select == "zero")
                    {
                        repeat_one = false;
                        break;
                    }
                    else if (select == "one")
                    {
                        std::cout << "*** Welcome to game One ***" << std::endl;
                        std::cout << "Please input a sequence of digit number(0 to 9)" << std::endl;
                        break;
                    }
                    else{
                        std::cout << "Selection Input Format Error! Please Re-input:" << std::endl;
                    }                    
                   
                }
            }            
        }
        else if(game_select == "two")
        {
            bool repeat_two = true;
            static const std::vector<std::string> vec_digig{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
            std::cout << "*** Welcome to game two ***" << std::endl;
            std::cout << "Please input a sequence of spelled-out number(zero to nine)" << std::endl;

            std::string line;
            while(repeat_two && std::cin >> line)
            {
                std::stringstream ss(line);
                std::string digit;
                while (ss >> digit)
                {
                    if (digit == "zero")
                    {
                        std::cout << digit << "'s spelled-out value is " << vec_digig[0];
                    }
                    else if (digit == "one")
                    {
                        std::cout << digit << "'s spelled-out value is " << vec_digig[1];
                    }
                    else if (digit == "two")
                    {
                        std::cout << digit << "'s spelled-out value is " << vec_digig[2];
                    }
                    else if (digit == "three")
                    {
                        std::cout << digit << "'s spelled-out value is " << vec_digig[3];
                    }
                    else if (digit == "four")
                    {
                        std::cout << digit << "'s spelled-out value is " << vec_digig[4];
                    }
                    else if (digit == "five")
                    {
                        std::cout << digit << "'s spelled-out value is " << vec_digig[5];
                    }
                    else if (digit == "six")
                    {
                        std::cout << digit << "'s spelled-out value is " << vec_digig[6];
                    }
                    else if (digit == "seven")
                    {
                        std::cout << digit << "'s spelled-out value is " << vec_digig[7];
                    }
                    else if (digit == "eight")
                    {
                        std::cout << digit << "'s spelled-out value is " << vec_digig[8];
                    }
                    else if (digit == "nine")
                    {
                        std::cout << digit << "'s spelled-out value is " << vec_digig[9];
                    }
                    else
                    {
                        std::cout << "Illegal input! Ignored!\n";                    
                    }                                      
                }
                
                std::cout << " ***zero*** Go Back to main\n" << std::flush;
                std::cout << " ***one***  Continue\n" << std::flush;
                std::string select;

                while (std::cin >> select)
                {
                    if (select == "zero")
                    {
                        repeat_two = false;
                        break;
                    }
                    else if (select == "one")
                    {
                        std::cout << "*** Welcome to game two ***" << std::endl;
                        std::cout << "Please input a sequence of spelled-out number(zero to nine)" << std::endl;
                        break;
                    }
                    else{
                        std::cout << "Selection Input Format Error! Please Re-input:" << std::endl;
                    }                    
                   
                }
            }
        }
        else if(game_select == "q")
        {
            std::cout << "Good Luck! Bye!" << std::endl;
            break;
        }
        else{
            std::cout << "Please input right format!" << std::endl;
        }

        std::cout << "Choose game you want to play(Input onw or two, Press q to exit)\n" << std::flush;
        std::cout << " ***one*** convert a digit to its corresponding spelled-out value\n" << std::flush;
        std::cout << " ***two*** convert a spelled-out numbers into their digit form\n" << std::flush;        

    }
    */
}

/* some thinking
* 1. The difference between std::cin and std::getline(), do not using them mixing up
*   std::cin, read data from the first no white space until white space or \n  
*   std::getline(), read a line of input from buffer (include \n).
*
* 2. flags() of IO is to return or set the format of the IO, not the state of the IO. Using fail()/good()/eof()/bad() or rdstate() instead.
*
* 3. Aviod using many more if-else, should using other method.
*
* 4. Design a user-friendly UI, for example in this program, user could input smothly, and the code also looks good.
*
* 5. using \n more often than endl or flush, since endl/flush need to force the context of the buffer to the console and wasting of time. 
* 
*
*/
    