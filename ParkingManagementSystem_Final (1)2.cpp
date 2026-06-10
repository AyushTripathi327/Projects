// ================================================
// Parking Management System
// ================================================

#include <iostream>   // cout, cin ke liye
#include <vector>     // vector (dynamic array) ke liye
#include <fstream>    // file read/write ke liye
#include <ctime>      // time() aur difftime() ke liye
#include <algorithm>  // algorithm library (future use ke liye)

using namespace std;

// ================================================
// Vehicle Class — ek gaadi ki saari info store karta hai
// ================================================
class Vehicle {
public:
    string plate;      // Gaadi ka number plate
    string owner;      // Malik ka naam
    string type;       // Car / Bike / Truck
    int slot;          // Konsi parking jagah mili
    time_t entryTime;  // Kab aayi (seconds mein)

    // Empty constructor — file se load karte waqt zarurat padti hai
    Vehicle() {}

    // Parameterized constructor — nayi gaadi park karte waqt use hota hai
    Vehicle(string p, string o, string t, int s) {
        plate     = p;
        owner     = o;
        type      = t;
        slot      = s;
        entryTime = time(0);  // Abhi ka time store karo
    }
};

// ================================================
// ParkingLot Class — saari parking operations handle karta hai
// ================================================
class ParkingLot {
private:
    vector<Vehicle> vehicles;  // Saari parked gaadiyon ki list
    int totalSlots = 20;       // Maximum 20 gaadiyaan park ho sakti hain

public:

    // ------------------------------------------------
    // loadFromFile() — program start hote hi file se data load karta hai
    // ------------------------------------------------
    void loadFromFile() {
        ifstream file("parked.txt");  // parked.txt file kholo (read mode)
        if (!file) return;            // File nahi mili? Wapas jao

        string line;
        // Ek ek line padho
        while (getline(file, line)) {
            if (line.empty()) continue;  // Khali line skip karo

            Vehicle v;

            // Line ka format: plate|owner|type|slot|entryTime
            // | se split karo taaki owner mein space ho sake
            size_t p1 = line.find('|');
            size_t p2 = line.find('|', p1 + 1);
            size_t p3 = line.find('|', p2 + 1);
            size_t p4 = line.find('|', p3 + 1);

            v.plate     = line.substr(0, p1);
            v.owner     = line.substr(p1 + 1, p2 - p1 - 1);
            v.type      = line.substr(p2 + 1, p3 - p2 - 1);
            v.slot      = stoi(line.substr(p3 + 1, p4 - p3 - 1));
            v.entryTime = stol(line.substr(p4 + 1));

            vehicles.push_back(v);  // Vector mein daalo
        }
        file.close();  // File band karo
    }

    // ------------------------------------------------
    // saveToFile() — har park/remove ke baad data file mein save karta hai
    // ------------------------------------------------
    void saveToFile() {
        ofstream file("parked.txt");  // parked.txt kholo (write mode)

        for (Vehicle &v : vehicles) {
            // | separator use karo taaki owner name mein space ho sake
            file << v.plate     << "|"
                 << v.owner     << "|"
                 << v.type      << "|"
                 << v.slot      << "|"
                 << v.entryTime << "\n";
        }
        file.close();  // File band karo
    }

    // ------------------------------------------------
    // getFreeSlot() — pehla khali slot dhundh ke return karta hai
    // ------------------------------------------------
    int getFreeSlot() {
        // Slot 1 se 20 tak ek ek check karo
        for (int i = 1; i <= totalSlots; i++) {
            bool occupied = false;

            // Dekho koi gaadi is slot mein hai ya nahi
            for (Vehicle &v : vehicles) {
                if (v.slot == i) {
                    occupied = true;
                    break;
                }
            }

            if (!occupied)
                return i;  // Khali slot mila, return karo
        }
        return -1;  // Sab slot full hain
    }

    // ------------------------------------------------
    // parkVehicle() — nayi gaadi park karta hai
    // ------------------------------------------------
    void parkVehicle() {
        // Pehle check karo parking full toh nahi
        if ((int)vehicles.size() >= totalSlots) {
            cout << "\nParking Full!\n";
            return;
        }

        string plate, owner;
        int choice;

        // Plate number lo
        cout << "Plate Number: ";
        cin >> plate;

        // Duplicate check — same plate pehle se parked toh nahi
        for (Vehicle &v : vehicles) {
            if (v.plate == plate) {
                cout << "Vehicle already parked!\n";
                return;
            }
        }

        // Owner ka naam lo — getline se taaki space wale naam bhi chalein
        cout << "Owner Name: ";
        getline(cin >> ws, owner);  // ws: leftover whitespace/newline clear karo, phir poora naam lo

        // Vehicle type choose karo
        cout << "1. Car\n2. Bike\n3. Truck\nChoice: ";
        cin >> choice;

        // Choice ke hisaab se type set karo
        string type = "Car";
        if (choice == 2) type = "Bike";
        if (choice == 3) type = "Truck";

        // Khali slot lo
        int slot = getFreeSlot();

        // Nayi gaadi banao aur list mein daalo
        Vehicle v(plate, owner, type, slot);
        vehicles.push_back(v);

        // File update karo
        saveToFile();

        cout << "\nVehicle Parked Successfully!\n";
        cout << "Slot Number: " << slot << endl;
    }

    // ------------------------------------------------
    // removeVehicle() — gaadi nikalti hai aur fee calculate karta hai
    // ------------------------------------------------
    void removeVehicle() {
        string plate;

        cout << "Enter Plate Number: ";
        cin >> plate;

        // List mein plate dhundo
        for (int i = 0; i < (int)vehicles.size(); i++) {
            if (vehicles[i].plate == plate) {

                // Abhi ka time lo (exit time)
                time_t exitTime = time(0);

                // Kitne ghante ruki gaadi — seconds ko hours mein convert
                double hours =
                    difftime(exitTime, vehicles[i].entryTime) / 3600.0;

                // Minimum 1 hour charge
                if (hours < 1)
                    hours = 1;

                // Fee calculate karo — Rs. 30 per hour
                double fee = hours * 30;

                cout << "\nVehicle Found\n";
                cout << "Owner: " << vehicles[i].owner << endl;
                cout << "Type : " << vehicles[i].type  << endl;
                cout << "Fee  : Rs. " << fee            << endl;

                // List se gaadi hatao
                vehicles.erase(vehicles.begin() + i);

                // File update karo
                saveToFile();

                cout << "Vehicle Removed Successfully!\n";
                return;
            }
        }

        cout << "Vehicle Not Found!\n";
    }

    // ------------------------------------------------
    // searchVehicle() — plate number se gaadi dhundh ke info dikhata hai
    // ------------------------------------------------
    void searchVehicle() {
        string plate;

        cout << "Enter Plate Number: ";
        cin >> plate;

        // List mein plate dhundo
        for (Vehicle &v : vehicles) {
            if (v.plate == plate) {
                cout << "\nVehicle Found\n";
                cout << "Owner : " << v.owner << endl;
                cout << "Type  : " << v.type  << endl;
                cout << "Slot  : " << v.slot  << endl;
                return;
            }
        }

        cout << "Vehicle Not Found!\n";
    }

    // ------------------------------------------------
    // displayVehicles() — saari parked gaadiyon ki list dikhata hai
    // ------------------------------------------------
    void displayVehicles() {
        // Koi gaadi nahi parked?
        if (vehicles.empty()) {
            cout << "\nNo Vehicles Parked.\n";
            return;
        }

        cout << "\n---- Parked Vehicles ----\n";

        // Har gaadi ki info print karo
        for (Vehicle &v : vehicles) {
            cout << "Plate: " << v.plate
                 << " | Owner: " << v.owner
                 << " | Type: "  << v.type
                 << " | Slot: "  << v.slot
                 << endl;
        }
    }
};

// ================================================
// main() — program ka starting point
// ================================================
int main() {

    ParkingLot lot;
    lot.loadFromFile();  // Pehle purana data file se load karo

    int choice;

    // Menu loop — jab tak user 0 na daale tab tak chalta rahe
    do {
        cout << "\n===== Parking Management System =====\n";
        cout << "1. Park Vehicle\n";
        cout << "2. Remove Vehicle\n";
        cout << "3. Search Vehicle\n";
        cout << "4. Display Vehicles\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            lot.parkVehicle();       // Nayi gaadi park karo
        }
        else if (choice == 2) {
            lot.removeVehicle();     // Gaadi nikalo aur fee lo
        }
        else if (choice == 3) {
            lot.searchVehicle();     // Gaadi dhundo
        }
        else if (choice == 4) {
            lot.displayVehicles();   // Saari gaadiyan dikhao
        }
        else if (choice == 0) {
            cout << "Goodbye!\n";    // Program band karo
        }
        else {
            cout << "Invalid Choice!\n";  // Galat input
        }

    } while (choice != 0);

    return 0;
}
