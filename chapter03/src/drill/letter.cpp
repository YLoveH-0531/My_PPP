/*
    @copyright 2026 KaKaRot
    drills from chapter 3
    output a form letter
*/

#include "std_lib_facilities.h"

int main()
{
    std::string myname, friendName;
    char friend_sex = 0;
    int age = 0;
    cout << "Enter the name of the person you want to write to:" ;
    cin >> myname;
    cout << endl;
    cout << "Enter the name of the friend name:" ;
    cin >> friendName;
    cout << endl;
    cout << "Enter the sex of the friend(Press m if male or f if female):" ;
    cin >> friend_sex;
    cout << endl;
    cout << "Enter the age:" ;
    cin >> age;
    cout << endl;
    std::cout << "Dear " << myname << endl;
    std::cout << "  How are you? I am fine."<< endl;
    std::cout << "  I miss you "<< endl;
    std::cout << "  Have you seen " << friendName<< " lately? ";
    std::cout << "If you see " << friendName <<" please ask " << ((friend_sex == 'm') ? "him" : "her") <<" to call me." << endl;;
    
    if ((age <= 0) || (age >= 110)) {
        simple_error("you're kidding!");
    }else{
        std::cout << "  I hear you just had a birthday and you are "<< age <<" years old"<< std::endl;
        
        if (age < 12) {

            std::cout << "  Next year you will be "<< age + 1 << std::endl;

        }else if(age == 17)
        {
            std::cout << "  Next year you will be able to vote" << std::endl;
        }
        else if(age > 70)
        {
            std::cout << "  I hope you are enjoying retirement" << std::endl;
        }

    }

    std::cout << "Yours sincerely," << std::endl;
    std::cout << "KaKaLot" << std::endl;
}