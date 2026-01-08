#include "DataLoader.h"

bool DataLoader::load( const std::string& filename ) {
    std::ifstream file( filename );
    if( !file.is_open() ) {
        std::cerr << "Blad: Nie mozna otworzyc pliku " << filename << std::endl;
        return false;
    }

    allData.clear();
    std::string line;
    while( std::getline( file, line ) ) {
        if( line.empty() ) continue;

        std::stringstream ss( line );
        DataRecord record;

        // Pierwsze s³owo to etykieta (np. "Spring")
        ss >> record.label;

        // Reszta to liczby (wartoœci wykresu)
        float val;
        while( ss >> val ) {
            record.values.push_back( val );
        }

        allData.push_back( record );
    }

    file.close();
    return true;
}

float DataLoader::getMaxValue() {
    float maxV = 0.0f;
    for( const auto& record : allData ) {
        for( float v : record.values ) {
            if( v > maxV ) maxV = v;
        }
    }
    return maxV;
}