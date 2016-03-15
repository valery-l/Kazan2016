
#include <string>
#include <vector>
#include <iostream>

#include "ostream_write.h"

using namespace std;
          
struct recommendation
{
    string body;
    string author;
};


struct student_score
{
    string                  name;
    double                  av_mark;
    vector<recommendation>  recommendations;
};


REFL_STRUCT(recommendation)
    REFL_ENTRY(body)
    REFL_ENTRY(author)
REFL_END()

REFL_STRUCT(student_score)
    REFL_ENTRY(name)
    REFL_ENTRY(av_mark)
    REFL_ENTRY(recommendations)
REFL_END()

int main()
{      
    using ostream_writer::write;

    student_score sc = {"Ivan", 3.14, {{"Ales gut", "Thomas"}, {"Ales caput", "teacher"}}};
    write(cout, sc);
    
    return 0;
}