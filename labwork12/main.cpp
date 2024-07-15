#include <iostream>
#include "MyDB.h"

int main() {
    Table customers({{{"Name", "Daniil"}, {"Age", "18"}, {"City", "Saint-Petersburg"}, {"ID", "12"}},
                     {{"Name", "Sofia"},  {"Age", "18"}, {"City", "Rostov-on-Don"},    {"ID", "13"}},
                     {{"Name", "Ivan"},   {"Age", "20"}, {"City", "Moscow"},           {"ID", "14"}},
                     {{"Name", "Egor"},   {"Age", "10"}, {"City", "Samara"},           {"ID", "15"}},
                     {{"Name", "Vito"},   {"Age", "28"}, {"City", "Saint-Petersburg"}, {"ID", "17"}}});
    Table purchases({{{"Item", "Apple"},  {"Price", "0.99"}, {"Quantity", "5"}, {"ID", "12"}},
                     {{"Item", "Banana"}, {"Price", "0.50"}, {"Quantity", "3"}, {"ID", "12"}},
                     {{"Item", "Orange"}, {"Price", "0.75"}, {"Quantity", "2"}, {"ID", "15"}}});

    MyDB database({{"customers", customers},
                   {"purchases", purchases}});
    Table result = database.Request(R"(CREATE TABLE Shops (Name, City, ID, PRIMARY KEY (ID))
INSERT INTO Shops (Name, City, ID) VALUES (Dixy, Saint-Petersburg, 1)
INSERT INTO Shops (Name, City, ID) VALUES (MVideo, Moscow, 2)
SELECT * FROM Shops)");
    database.SaveIntoFile("C:\\Users\\d_chu\\CLionProjects\\labwork-12-chtonad0000\\db.txt");

};