#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <string>
using namespace std;

// Creates a personalized text file for the user
void createUserProfileFile(const string& username, const string& password, int age, double weight, double height,
                           const string& gender, double bmi, const string& bmiStatus, double bmr);

// Displays the content of the user's profile file
void displayUserProfile(const string& username);

#endif

