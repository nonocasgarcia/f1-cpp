#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "F1.hpp"

using namespace std;

Driver:: Driver(int driverI, string cod, string nam, vector<int> numbe, string dateOfBir, string  nationali):  driverId(driverI), code(cod), name(nam), numbers(numbe), dateOfBirth(dateOfBir), nationality(nationali) {};
Driver:: Driver(int driverI, string cod, string nam,  string dateOfBir, string  nationali):  driverId(driverI), code(cod), name(nam), dateOfBirth(dateOfBir), nationality(nationali) {};

int Driver::getDriverId() const { return driverId; }

string Driver::getCode() const { return code; }

string Driver::getName() const { return name; }

vector<int> Driver::getNumbers() const { return numbers; }

string Driver::getDateOfBirth() const { return dateOfBirth; }

string Driver::getNationality() const { return nationality; }

Constructor::Constructor(int constructorId, string name, string country) : constructorId(constructorId), name(name), nationality(country) {};

int Constructor::getConstructorId() const { return constructorId; }

string Constructor::getName() const { return name; }

string Constructor::getCountry() const { return nationality; }

vector<DriCons*> Constructor::getDrivers() { return Drivers; }


int Constructor::deleteDriver(int driveId)
{
   
    for ( auto it=Drivers.begin();it!=Drivers.end(); it++){
        if ((*it)->driver->getDriverId()==driveId) {
           delete(*it);
	Drivers.erase(it);
	return 0;
            } 
        }
        
    
  
   return -1;
}



DriverManagement::DriverManagement(){ vectorDrivers.clear();};
DriverManagement::~DriverManagement(){
    while(vectorDrivers.size()!=0)
       {

	  	
            delete (vectorDrivers[vectorDrivers.size()-1]);
            vectorDrivers.pop_back();
                
       } 
    }


vector<Driver*> DriverManagement::getVectorDrivers() const { return vectorDrivers; }

int DriverManagement::driverInsert(Driver* driver)
{
    if (driver==nullptr) return -1;
    for (auto it = vectorDrivers.begin(); it != vectorDrivers.end(); ++it) {
        if ((*it) == driver) {
            return 1;
        }
    }
    vectorDrivers.push_back(driver);
    return -1; 
}

int DriverManagement::driverDelete(int driverId)
{
    for (auto it = vectorDrivers.begin(); it != vectorDrivers.end(); ++it) {
        if ((*it)->getDriverId() == driverId) {
            delete *it; 
            vectorDrivers.erase(it); 
            return 0; 
        }
    }
    return -1; // Series not found
}

ConstructorManagement::ConstructorManagement(){vectorConstructores.clear();};
ConstructorManagement::~ConstructorManagement(){
while(vectorConstructores.size()!=0)
   {   
        while(vectorConstructores[vectorConstructores.size()-1]->getDrivers().size()!=0) {
	vectorConstructores[vectorConstructores.size()-1]->deleteDriver(vectorConstructores[vectorConstructores.size()-1]->getDrivers()[0]->driver->getDriverId());

}
   
        delete (vectorConstructores[vectorConstructores.size()-1]);
        vectorConstructores.pop_back();
            
   } 
}
vector<Constructor*> ConstructorManagement::getVectorConstructores() const { return vectorConstructores; }

int ConstructorManagement::addConstrutor(Constructor* newConstructor)
{
    if(newConstructor==nullptr) return -1;
    auto it=find(vectorConstructores.begin(),vectorConstructores.end(),newConstructor);
    if (it==vectorConstructores.end()) {
        vectorConstructores.push_back(newConstructor);
        return 0;
    }
    return 1;
}

/**************************/
/*     A implementar      */
/**************************/

int Driver::addNumber(int num)
{
    if (find(numbers.begin(), numbers.end(), num) != numbers.end()) {
            return 1;
    } else {
        try{
            numbers.push_back(num);
            return 0;
        }catch(const std::exception& e){
            return -1;
        }
    }
        
}


void Driver::displayDriverInfo(ostream& os) const
{
    os << "Displaying driver info:\n"
    << "-----DriverId: " << driverId << "\n" 
    << "-----Code: " <<  code << "\n"
    << "-----Name: " << name << "\n"
    << "-----used numbers: ";
 

    for(int i=0; i<numbers.size(); i++){
        os << numbers.at(i);

        if(i != numbers.size()-1){
            os << " ";
        }
    }

    os << "\n"
    << "-----Date of birth: " << dateOfBirth << "\n" 
    << "-----Country: " << nationality << endl;

}



int Constructor::addDriver(int driveId, int year, DriverManagement &driverManager)
{
    auto driverRef = find_if(Drivers.begin(), Drivers.end(), [driveId](const DriCons& d) {
        return d.driver->getDriverId == driveId;
    });

    if(driverRef != Drivers.end()){
        //exists

    }else{
        //not exists
        if()
    }

   return -1;
}


int ConstructorManagement::updateConstructorDrivers(string filename, DriverManagement& manager)
{
    std::ifstream file(filename);  // Open the file
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file!\n";
        return 1;
    }

    vector<Constructor *> constructors = getVectorConstructores();

    while (std::getline(file, line)) {  // Read each line
        std::stringstream ss(line);
        std::string constructorId, driverId, year;

        std::getline(ss, constructorId, ';');
        std::getline(ss, driverId, ';');
        std::getline(ss, year, ';');

        auto constructorRef = find_if(constructors.begin(), constructors.end(), [constructorId](const Driver& d) {
            return d.getId() == constructorId;
        });

        if(constructorRef != constructors.end()){
            vector<DriCons* > drivers = (*constructorRef)->getDrivers();
            auto driverRef = find_if(drivers.begin(), drivers.end(), [driverId](const DriCons& d) {
                return d.driver->getDriverId() == driverId;
            });

            if(driverRef != drivers.end()){

            }
        }
    }

    file.close();
   
    return -1;
}


vector<string> ConstructorManagement::DriversAndConstructorsOfOneYear(int year,vector<string> &vConstr) {
  
   
    return {};
}
