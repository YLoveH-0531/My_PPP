#include "Chrono.h"

namespace Chrono {
// member function definitions:
Date::Date(int yy, Month mm, int dd) : y{yy}, m{mm}, d{dd}{ 
    if (!is_date(yy,mm,dd)) throw Invalid{};
}

const Date& default_date(){
    static Date dd {2001,Month::jan,1}; // start of 21st century
    return dd;
}
Date::Date() : y{default_date().year()}, m{default_date().month()}, d{default_date().day()} {}

void Date::add_day(int n)
{
    static const int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    d += n;
    while (true) {
        int dim = (m == Month::feb && leapyear(y)) ? 29 : days_in_month[int(m)];
        if (d <= dim) break;
        d -= dim;
        add_month(1);
    }
} 

void Date::add_month(int n)
{
    int total = int(m) - 1 + n;       // 0-based total months
    y += total / 12;
    m = Month(total % 12 + 1);
    // clamp day if new month is shorter (e.g. Jan 31 + 1 month → Feb 28/29)
    static const int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dim = (m == Month::feb && leapyear(y)) ? 29 : days_in_month[int(m)];
    if (d > dim) d = dim;
}

void Date::add_year(int n) {
    if (m==Month::feb && d==29 && !leapyear(y+n)) { // beware of leap years!
        m = Month::mar; // use March 1 instead of February 29
        d = 1;
    }
    y+=n;
}

// helper functions:
bool is_date(int y, Month m, int d) {   // assume that y is valid
    if (d<=0) return false;     // d must be positive
    if (m<Month::jan || Month::dec<m) return false;
    int days_in_month = 31;     // most months have 31 days
    switch (m) {
        case Month::feb:        // the length of February varies
            days_in_month = (leapyear(y))?29:28;
            break;
        case Month::apr: case Month::jun: case Month::sep: case Month::nov:
            days_in_month = 30;  // the rest have 30 days
            break;
        default:
            break;
    }
    if (days_in_month<d) return false;
    return true;
}

bool leapyear(int y)
{   
    return ((y%4==0 && y%100!=0) || y%400==0);
}

bool operator==(const Date& a, const Date& b) {
    return a.year()==b.year()
        && a.month()==b.month()
        && a.day()==b.day();
}

bool operator!=(const Date& a, const Date& b){ return !(a==b); }

std::ostream& operator<<(std::ostream& os, const Date& d){ 
    return os << '(' << d.year()
              << ',' << int(d.month())
              << ',' << d.day() << ')';
}

std::istream& operator>>(std::istream& is, Date& dd){
    int y, m, d;
    char ch1, ch2, ch3, ch4;
    is >> ch1 >> y >> ch2 >> m >> ch3 >> d >> ch4;
    if (!is) return is;
    if (ch1!= '(' || ch2!=',' || ch3!=',' || ch4!=')') { // oops: format error
        is.clear(std::ios_base::failbit);   // set the fail bit
        return is;
    }
    dd = Date(y, Month(m),d);   // update dd
    return is;
}

Day day_of_week(const Date& d)
{
    // Tomohiko Sakamoto's algorithm
    static const int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    int y = d.year();
    int m = int(d.month());
    int day = d.day();
    if (m < 3) y--;
    int dow = (y + y/4 - y/100 + y/400 + t[m-1] + day) % 7;
    return Day(dow);
}

Date next_Sunday(const Date d)
{
    Date result = d;
    result.add_day(1);
    while (day_of_week(result) != Day::sunday)
        result.add_day(1);
    return result;
}

Date next_weekday(const Date& d)
{
    Date result = d;
    result.add_day(1);
    Day dow = day_of_week(result);
    while (dow == Day::saturday || dow == Day::sunday) {
        result.add_day(1);
        dow = day_of_week(result);
    }
    return result;
}

} // Chrono