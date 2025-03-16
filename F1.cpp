#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "F1.hpp"

using namespace std;

Driver::Driver(int driverI, string cod, string nam, vector<int> numbe, string dateOfBir, string nationali) : driverId(driverI), code(cod), name(nam), numbers(numbe), dateOfBirth(dateOfBir), nationality(nationali) {};
Driver::Driver(int driverI, string cod, string nam, string dateOfBir, string nationali) : driverId(driverI), code(cod), name(nam), dateOfBirth(dateOfBir), nationality(nationali) {};

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

vector<DriCons *> Constructor::getDrivers() { return Drivers; }

int Constructor::deleteDriver(int driveId)
{

    for (auto it = Drivers.begin(); it != Drivers.end(); it++)
    {
        if ((*it)->driver->getDriverId() == driveId)
        {
            delete (*it);
            Drivers.erase(it);
            return 0;
        }
    }

    return -1;
}

DriverManagement::DriverManagement() { vectorDrivers.clear(); };
DriverManagement::~DriverManagement()
{
    while (vectorDrivers.size() != 0)
    {

        delete (vectorDrivers[vectorDrivers.size() - 1]);
        vectorDrivers.pop_back();
    }
}

vector<Driver *> DriverManagement::getVectorDrivers() const { return vectorDrivers; }

int DriverManagement::driverInsert(Driver *driver)
{
    if (driver == nullptr)
        return -1;
    for (auto it = vectorDrivers.begin(); it != vectorDrivers.end(); ++it)
    {
        if ((*it) == driver)
        {
            return 1;
        }
    }
    vectorDrivers.push_back(driver);
    return -1;
}

int DriverManagement::driverDelete(int driverId)
{
    for (auto it = vectorDrivers.begin(); it != vectorDrivers.end(); ++it)
    {
        if ((*it)->getDriverId() == driverId)
        {
            delete *it;
            vectorDrivers.erase(it);
            return 0;
        }
    }
    return -1; // Series not found
}

ConstructorManagement::ConstructorManagement() { vectorConstructores.clear(); };
ConstructorManagement::~ConstructorManagement()
{
    while (vectorConstructores.size() != 0)
    {
        while (vectorConstructores[vectorConstructores.size() - 1]->getDrivers().size() != 0)
        {
            vectorConstructores[vectorConstructores.size() - 1]->deleteDriver(vectorConstructores[vectorConstructores.size() - 1]->getDrivers()[0]->driver->getDriverId());
        }

        delete (vectorConstructores[vectorConstructores.size() - 1]);
        vectorConstructores.pop_back();
    }
}
vector<Constructor *> ConstructorManagement::getVectorConstructores() const { return vectorConstructores; }

int ConstructorManagement::addConstrutor(Constructor *newConstructor)
{
    if (newConstructor == nullptr)
        return -1;
    auto it = find(vectorConstructores.begin(), vectorConstructores.end(), newConstructor);
    if (it == vectorConstructores.end())
    {
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
    if (find(numbers.begin(), numbers.end(), num) != numbers.end())
    {
        return 1;
    }
    else
    {
        try
        {
            numbers.push_back(num);
            return 0;
        }
        catch (const std::exception &e)
        {
            return -1;
        }
    }
}

void Driver::displayDriverInfo(ostream &os) const
{
    os << "Displaying driver info:\n"
       << "-----DriverId: " << driverId << "\n"
       << "-----Code: " << code << "\n"
       << "-----Name: " << name << "\n"
       << "-----used numbers: ";

    for (int i = 0; i < numbers.size(); i++)
    {
        os << numbers.at(i);

        if (i != numbers.size() - 1)
        {
            os << " ";
        }
    }

    os << "\n"
       << "-----Date of birth: " << dateOfBirth << "\n"
       << "-----Country: " << nationality << " " << endl;
}

int Constructor::addDriver(int driveId, int year, DriverManagement &driverManager)
{
    // Validação dos parâmetros
    if (driveId <= 0 || year <= 0)
    {
        return -1; // Parâmetros inválidos
    }

    // Verifica se o piloto existe no DriverManagement
    vector<Driver *> drivers = driverManager.getVectorDrivers();

    auto driverRef = find_if(drivers.begin(), drivers.end(), [driveId](Driver *d)
                             { return d->getDriverId() == driveId; });

    // caso não exista
    if (driverRef == drivers.end())
    {
        return -1; // Piloto não encontrado
    }

    // Verifica se o piloto já foi adicionado ao Construtor para este ano
    for (const auto &driverInfo : getDrivers())
    {
        if (driverInfo->driver->getDriverId() == driveId)
        {
            // Se o piloto já está no Construtor, verifica se o ano já está associado
            if (driverInfo->begin <= year && driverInfo->end >= year)
            {
                return 1; // Ano já existe para o piloto
            }
            // Atualiza o ano de término se o piloto fica mais um ano no construtor
            else if (driverInfo->end + 1 == year)
            {
                driverInfo->end = year;
                return 0; // Sucesso
            }
            // Se o piloto voltou à equipa depois de ter saido adiciona novo registo na Construtor
            else
            {
                DriCons *driverCons = new DriCons(year, year, *driverRef);
                Drivers.push_back(driverCons);
                return 0;
            }
        }
    }

    // Se não encontrou o piloto no Construtor, adicione-o
    DriCons *driverCons = new DriCons(year, year, *driverRef);

    Drivers.push_back(driverCons);

    return 0; // Sucesso
}

int ConstructorManagement::updateConstructorDrivers(string filename, DriverManagement &manager)
{

    ifstream file(filename); // Open the file
    string line;

    // Check if the file was opened successfully
    if (!file.is_open())
    {
        cerr << "Error opening file!\n";
        return -1; // Return -1 if file can't be opened
    }

    vector<Constructor *> constructors = getVectorConstructores(); // Get all constructors

    // Read each line from the file
    while (getline(file, line)) // Read each line from the file
    {
        stringstream ss(line);
        int constructorId, driverId, year;

        string temp;

        // Parse each part of the line (constructorId, driverId, year)
        try
        {
            getline(ss, temp, ';');
            constructorId = stoi(temp); // Convert constructorId to int

            getline(ss, temp, ';');
            driverId = stoi(temp); // Convert driverId to int

            getline(ss, temp, ';');
            year = stoi(temp); // Convert year to int
        }
        catch (const std::exception &e)
        {
            return -1; // Return -1 if parsing fails
        }

        // Find the constructor by constructorId
        auto constructorRef = find_if(constructors.begin(), constructors.end(), [constructorId](Constructor *c)
                                      { return c->getConstructorId() == constructorId; });

        // If the constructor exists
        if (constructorRef != constructors.end())
        {
            Constructor *constructor = *constructorRef;

            // Find if the driver already exists for this constructor
            vector<DriCons *> driCons = constructor->getDrivers();
            auto driConsRef = find_if(driCons.begin(), driCons.end(), [driverId](DriCons *d)
                                      { return d->driver->getDriverId() == driverId; });

            constructor->addDriver(driverId, year, manager);
        }
        else
        {
            cerr << "Constructor ID " << constructorId << " not found.\n";
            return -1; // Return -1 if the constructor is not found
        }
    }

    file.close(); // Close the file after processing

    return 0; // Return 0 if everything is successful
}

vector<string> ConstructorManagement::DriversAndConstructorsOfOneYear(int year, vector<string> &vConstr)
{
    vector<string> vDrivers;

    // Itera sobre os construtores
    vector<Constructor *> constructors = getVectorConstructores();

    for (Constructor *constructor : constructors)
    {
        bool hasDriverForYear = false;
        // Verifica os pilotos do construtor
        vector<DriCons *> driverConsList = constructor->getDrivers();
        for (auto &driCons : driverConsList)
        {
            if (driCons->begin <= year && driCons->end >= year)
            { // Se o piloto participou nesse ano
                vDrivers.push_back(driCons->driver->getName());
                hasDriverForYear = true;
            }
        }
        if (hasDriverForYear)
        {
            vConstr.push_back(constructor->getName());
        }
    }

    // Ordenar os pilotos por nome
    sort(vDrivers.begin(), vDrivers.end());

    // Ordenar os construtores por nome
    sort(vConstr.begin(), vConstr.end());

    return vDrivers;
}
