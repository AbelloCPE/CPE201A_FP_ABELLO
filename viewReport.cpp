#include "viewReport.h"
#include "dietoption.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

using namespace std;

// Function to display a daily nutrition report for a user
void viewReport(const string& username, double bmr) {

 	system("cls"); // to clear the display
 	
    cout << "\n=== \033[36mDAILY REPORT\033[0m ===\n";

    ifstream logFile(username + "_foodlog.txt"); // Open the user foodlog text
    if (!logFile.is_open()) { 
        cout << "\033[31mNo food logs found for today.\033[0m\n";    // If file cannot be opened, inform user and return to previous menu
        cout << "\nPress Enter to go back...";
        cin.ignore();                                                // discard invalid input
        cin.get();                                                   // waiting for enter
        system("cls");
        return;                                                      // return to main menu
    }
    // declare variables
    string line, date, food; 
    double cal, prot, carb;
    int qty;

    // Match your log date format (no leading zeros)
    time_t now = time(0);                                      // Get the current system time
    tm *ltm = localtime(&now);                                 // Convert the current time to local time structure
    string today = to_string(1900 + ltm->tm_year) + "-" +      // Add 1900 to get the current year
                   to_string(1 + ltm->tm_mon) + "-" +           // Add 1 to month because tm_mon starts from 0
                   to_string(ltm->tm_mday);                     // Get the current day of the month

    double totalCal = 0, totalProt = 0, totalCarb = 0;
    bool hasLogs = false;
    // read each line from the log file 
    while (getline(logFile, line)) { 
        stringstream ss(line);               // analyze the line
        getline(ss, date, ',');
        getline(ss, food, ',');              //extract the food name
        ss >> cal;                           //extract calories
        ss.ignore();
        ss >> prot;                          // extract protein
        ss.ignore();
        ss >> carb;                          // extract carbs
        ss.ignore();
        ss >> qty;                           // extract quantity

        // Remove any spaces from date string (trim)
        string trimmedDate = "";
        for (char c : date) {
            if (!isspace(static_cast<unsigned char>(c)))
                trimmedDate += c;
        }
        date = trimmedDate;
    // If the log matches today's date
        if (date == today) {
            hasLogs = true;   // Mark that we have logs for today
   // display the food entry
            cout << "- " << food << " " << qty << "x: "
                 << cal << " kcal, " << prot << "g protein, "
                 << carb << "g carbs\n";
 // calculate the total
            totalCal += cal;
            totalProt += prot;
            totalCarb += carb;
        }
    }
    logFile.close();   // close the file
 
// If no logs were found for today
    if (!hasLogs) {
        cout << "\033[31m\nNo logs found for today (" << today << ").\033[0m\n";
        cout << "\nPress Enter to go back...";
        cin.ignore();
        cin.get();
        system("cls");
        return;               // return to the main menu
    }
// Get the adjusted BMR for the user
    double adjustedBMR = getAdjustedBMR(username, bmr);

    cout << fixed << setprecision(2);
    cout << "\n=== \033[36mTOTALS FOR TODAY\033[0m ===\n";
    cout << "Date: " << today << endl;
    cout << "Calories: " << totalCal << " kcal\n";
    cout << "Protein:  " << totalProt << " g\n";
    cout << "Carbs:    " << totalCarb << " g\n";
    cout << "BMR (Goal): " << adjustedBMR << " kcal/day\n\n";

    // ==================== CLEAN ASCII PROGRESS BAR ====================
    double progress = (adjustedBMR > 0) ? (totalCal / adjustedBMR) : 0; // Calculate progress toward calorie goal
    if (progress > 1.5) progress = 1.5; // cap display at 150%

    int barWidth = 40; // total width of the bar 
    int filled = static_cast<int>(barWidth * (progress < 1.0 ? progress : 1.0));  // Filled portion

    string color;
    if (totalCal < adjustedBMR * 0.8)
        color = "\033[33m"; // Yellow (below goal)
    else if (totalCal > adjustedBMR * 1.1)
        color = "\033[31m"; // Red (above goal)
    else
        color = "\033[32m"; // Green (goal met)

 // Print the progress bar
    cout << "   " << color << "[";
    for (int i = 0; i < barWidth; ++i)
        cout << (i < filled ? "=" : " ");
    cout << "] " << fixed << setprecision(1)
         << (progress * 100) << "% of goal\033[0m\n";

    // ==================== STATUS MESSAGE ====================
    cout << "\n";
    if (totalCal == 0)
        cout << "\033[31mNo foods logged today.\033[0m\n";
    else if (totalCal < adjustedBMR)
        cout << "You are below your calorie goal.\n";
    else if (totalCal >= adjustedBMR + 200)
        cout << "You exceeded your calorie goal.\n";
    else
        cout << "You reached your calorie goal.\n";
 
    // Wait for user input before returning to previous menu
    cout << "\nPress Enter to go back...";
    cin.ignore();
    cin.get();
    system("cls");
}

