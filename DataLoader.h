#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Struktura przechowuj¹ca jeden rekord danych (np. jedn¹ grupê s³upków)
struct DataRecord {
    std::string label;           // np. "Wiosna" lub "1987"
    std::vector<float> values;   // lista wartoœci do zwizualizowania
};

class DataLoader {
public:
    // Przechowuje wszystkie wczytane dane
    std::vector<DataRecord> allData;

    // Metoda wczytuj¹ca dane z pliku tekstowego
    bool load( const std::string& filename );

    // Metoda pomocnicza: znajduje najwiêksz¹ wartoœæ (przydatne do skalowania wykresu)
    float getMaxValue();
};

#endif