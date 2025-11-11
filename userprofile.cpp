#include "userprofile.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib> // for system()
using namespace std;

// Function to create a new user profile and sabe all user info

void createUserProfileFile(const string& username, const string& password, int age, double weight, double height,
                           const string& gender, double bmi, const string& bmiStatus, double bmr) {
                           	
    string filename = username + "_profile.txt";				// create a filename using the username
	// used to write details about the person 
    ofstream profileFile(filename);
    if (profileFile.is_open()) {								// open the filename
        profileFile << "=== USER PROFILE ===\n";
        profileFile << "Username: " << username << endl;
        profileFile << "Age: " << age << " years\n";
        profileFile << "Weight: " << weight << " kg\n";
        profileFile << "Height: " << height << " cm\n";
        profileFile << "Gender: " << gender << endl;
        profileFile << "BMI: " << fixed << setprecision(2) << bmi << " (" << bmiStatus << ")\n";
        profileFile << "BMR (Calories Needed): " << fixed << setprecision(2) << bmr << " kcal/day\n";
        profileFile.close();									// close the file after writing all the necessary details
        
    } else {													// if file cannot be created
        cerr << "\033[31mError: Could not create profile file for " << username << "\033[0m.\n";
    }
}
// function to display the userprofile on the console
void displayUserProfile(const string& username) {
	system("cls");
    string filename = username + "_profile.txt";				// construct the filename
    ifstream profileFile(filename);								// open the filename for reading
    if (profileFile.is_open()) {								// check if the file exist
        cout << "\n";
        string line;
        while (getline(profileFile, line)) {					// read txt line by line and print out each line
            cout << line << endl;
        }
        profileFile.close();									// close the file
    } else {
        cout << "\033[31mProfile file not found for user '" << username << "'\033[0m.\n";
    }
        cout << "\n\n";
		cout << "\033[1;37m" << "Press Enter to return to the main menu...";
   		cout << "\033[0m";
    	cin.ignore();
    	cin.get(); // Wait for Enter
    
    	system("cls");
}



