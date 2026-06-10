

#include <iostream>   
#include <vector>     
#include <fstream>    
#include <ctime>      


using namespace std;


class Vehicle {
public:
    string plate;      
    string owner;      
    string type;      
    int slot;          
    time_t entryTime;  

   
    Vehicle() {}

   
    Vehicle(string p, string o, string t, int s) {
        plate     = p;
        owner     = o;
        type      = t;
        slot      = s;
        entryTime = time(0); 
    }
};


class ParkingLot {
private:
    vector<Vehicle> vehicles;  
    int totalSlots = 20;       

public:


    void loadFromFile() {
        ifstream file("parked.txt");  
        if (!file) return;            

        string line;
        // Ek ek line padho
        while (getline(file, line)) {
            if (line.empty()) continue;  

            Vehicle v;

           
            size_t p1 = line.find('|');
            size_t p2 = line.find('|', p1 + 1);
            size_t p3 = line.find('|', p2 + 1);
            size_t p4 = line.find('|', p3 + 1);

            v.plate     = line.substr(0, p1);
            v.owner     = line.substr(p1 + 1, p2 - p1 - 1);
            v.type      = line.substr(p2 + 1, p3 - p2 - 1);
            v.slot      = stoi(line.substr(p3 + 1, p4 - p3 - 1));
            v.entryTime = stol(line.substr(p4 + 1));

            vehicles.push_back(v);  
        }
        file.close();  
    }

   
    void saveToFile() {
        ofstream file("parked.txt");  

        for (Vehicle &v : vehicles) {
            
            file << v.plate     << "|"
                 << v.owner     << "|"
                 << v.type      << "|"
                 << v.slot      << "|"
                 << v.entryTime << "\n";
        }
        file.close(); 
    }

  
    int getFreeSlot() {
       
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
                return i; 
        }
        return -1;  
    }

   
 
    void parkVehicle() {
       
        if ((int)vehicles.size() >= totalSlots) {
            cout << "\nParking Full!\n";
            return;
        }

        string plate, owner;
        int choice;

       
        cout << "Plate Number: ";
        cin >> plate;

       
        for (Vehicle &v : vehicles) {
            if (v.plate == plate) {
                cout << "Vehicle already parked!\n";
                return;
            }
        }

      
        cout << "Owner Name: ";
        getline(cin >> ws, owner);  

       
        cout << "1. Car\n2. Bike\n3. Truck\nChoice: ";
        cin >> choice;

       
        string type = "Car";
        if (choice == 2) type = "Bike";
        if (choice == 3) type = "Truck";

       
        int slot = getFreeSlot();

        
        Vehicle v(plate, owner, type, slot);
        vehicles.push_back(v);

       
        saveToFile();

        cout << "\nVehicle Parked Successfully!\n";
        cout << "Slot Number: " << slot << endl;
    }


    void removeVehicle() {
        string plate;

        cout << "Enter Plate Number: ";
        cin >> plate;

       
        for (int i = 0; i < (int)vehicles.size(); i++) {
            if (vehicles[i].plate == plate) {

               
                time_t exitTime = time(0);

               
                double hours =
                    difftime(exitTime, vehicles[i].entryTime) / 3600.0;

               
                if (hours < 1)
                    hours = 1;

                
                double fee = hours * 30;

                cout << "\nVehicle Found\n";
                cout << "Owner: " << vehicles[i].owner << endl;
                cout << "Type : " << vehicles[i].type  << endl;
                cout << "Fee  : Rs. " << fee            << endl;

                
                vehicles.erase(vehicles.begin() + i);

               
                saveToFile();

                cout << "Vehicle Removed Successfully!\n";
                return;
            }
        }

        cout << "Vehicle Not Found!\n";
    }

   
    void searchVehicle() {
        string plate;

        cout << "Enter Plate Number: ";
        cin >> plate;

      
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

 
    void displayVehicles() {
      
        if (vehicles.empty()) {
            cout << "\nNo Vehicles Parked.\n";
            return;
        }

        cout << "\n---- Parked Vehicles ----\n";

      
        for (Vehicle &v : vehicles) {
            cout << "Plate: " << v.plate
                 << " | Owner: " << v.owner
                 << " | Type: "  << v.type
                 << " | Slot: "  << v.slot
                 << endl;
        }
    }
};


int main() {

    ParkingLot lot;
    lot.loadFromFile(); 

    int choice;

   
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
            lot.parkVehicle();      
        }
        else if (choice == 2) {
            lot.removeVehicle();    
        }
        else if (choice == 3) {
            lot.searchVehicle();     
        }
        else if (choice == 4) {
            lot.displayVehicles();   
        }
        else if (choice == 0) {
            cout << "Goodbye!\n";    
        }
        else {
            cout << "Invalid Choice!\n";  
        }

    } while (choice != 0);

    return 0;
}
