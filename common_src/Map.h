#ifndef MAP_H
#define MAP_H

#include <cstddef>
#include "Configuration.h"
#include <utility>
#include "Door.h"

class Map {
private:
	long int n_row, n_col;
	int** map;
public:
	Map(const Configuration& config);
	int get(int x, int y) const;
    long int get_n_row() const;
    long int get_n_col() const;
    // Retorna las coordenadas (x,y) de las puertas en el mapa.
    std::vector<class Door> getDoors() const;
	~Map();
private:
	bool outOfRange(int x, int y) const;
};

#endif
