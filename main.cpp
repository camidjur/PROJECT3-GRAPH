#include <iostream>

using namespace std;
int main(){
    int choice;
    bool exitMenu = false;

    while(!exitMenu){
        cout << "Shortest Path Menu" << endl;
        cout << "1. Set Transportation Mode" << endl;
        cout << "2. Choose Search Algorithm" << endl;
        cout << "3. Set Start and End Points" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice){
            case 1:
                // Choose pedestrian, bus, car, bike etc
                break;
            case 2:
                // Djikstra or DFS
                break;
            case 3:
                // Set beginning and end
                break;
            case 4:
                // Exit out
                exitMenu = true;
            default:
                cout << "Invalid choice. Please try again." << endl;

        }
    }
    cout << "Exiting menu..." << endl;
    return 0;
}