#ifndef DIETOPTION_H
#define DIETOPTION_H
#include <string>
using namespace std;

void setDietGoal(const string& username);
double getAdjustedBMR(const string& username, double bmr);



#endif
