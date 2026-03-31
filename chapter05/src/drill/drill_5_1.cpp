/**
 * @file     drill_5_1.cpp
 * @author   KaKaRot
 * @date     31/3/2026
 * @brief    PPP Chapter 5 drill 1.
 * @details  error handle
 */

#include "std_lib_facilities.h"

int main()
{
    try {
    // code.
    keep_window_open();
    return 0;
    } 
    catch (exception& e) {
    cerr << "error: " << e.what() << '\n';
    keep_window_open();
    return 1;
    } 
    catch (...) {
    cerr << "Oops: unknown exception!\n";
    keep_window_open();
    return 2;
    }
}

/* 
1. Cout << "Success!\n";            *** Cout => cout
2. cout << "Success!\n;             *** lack of '"' in the end.
3. cout << "Success" << !\n"        *** lack of '"' in front of !.
4. cout << success << '\n';         *** lack of "" with success.
5. string res = 7; vector<int> v(10); v[5] = res; cout << "Success!\n";         *** type assignmen error.
6. vector<int> v(10); v(5) = 7; if (v(5)!=7) cout << "Success!\n";              *** containter access operator error. should use []
7. if (cond) cout << "Success!\n"; else cout << "Fail!\n";                      *** cond is not defined or declaration.
8. bool c = false; if (c) cout << "Success!\n"; else cout << "Fail!\n";         *** right.  
9. string s = "ape"; boo c = "fool"<s; if (c) cout << "Success!\n";             *** type spell wrong.
10. string s = "ape"; if (s=="fool") cout << "Success!\n";                      *** right.
11. string s = "ape"; if (s=="fool") cout < "Success!\n";                       *** cout< => cout <<
12. string s = "ape"; if (s+"fool") cout < "Success!\n";                        *** s+"fool" can not concert to bool and cout< should be cout<<
13. vector<char> v(5); for (int i=0; 0<v.size(); ++i) ; cout << "Success!\n";   *** the for loop will not nerver exit. 
14. vector<char> v(5); for (int i=0; i<=v.size(); ++i) ; cout << "Success!\n";  *** the condition causes the Memory out of bounds
15. string s = "Success!\n"; for (int i=0; i<6; ++i) cout << s[i];              *** logic error. Some contents will not be print.
16. if (true) then cout << "Success!\n"; else cout << "Fail!\n";                *** drop then.
17. int x = 2000; char c = x; if (c==2000) cout << "Success!\n";                *** int type is cut off the high bits when assigning to bool,
                                                                                    so the condition is always false.
18. string s = "Success!\n"; for (int i=0; i<10; ++i) cout << s[i];             *** right.
19. vector v(5); for (int i=0; i<=v.size(); ++i) ; cout << "Success!\n";        *** do not specify the template type for vector.
20. int i=0; int j = 9; while (i<10) ++j; if (j<i) cout << "Success!\n";        *** logic error. loop will never stop since i never increment.
21. int x = 2; double d = 5/(x–2); if (d==2*x+0.5) cout << "Success!\n";        *** devided should not be zero. prom crashed.
22. string<char> s = "Success!\n"; for (int i=0; i<=10; ++i) cout << s[i];      *** string does not have template parameters. for loop causes the Memory out of bounds
23. int i=0; while (i<10) ++j; if (j<i) cout << "Success!\n";                   *** same as 20, also j is not defined or declared.
24. int x = 4; double d = 5/(x–2); if (d=2*x+0.5) cout << "Success!\n";         *** if statement should use '==' instead of '='.
25. cin << "Success!\n"                                                         *** cin => cout.
*/