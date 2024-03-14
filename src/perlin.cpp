#include "perlin.h"

void permute(int* p, int n) {
    for (int i = n-1; i > 0; i--) {
        int target = randomInt(0, i);
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

int* generatePerm() {
    auto p = new int[Perlin::POINT_COUNT];

    for (int i = 0; i < Perlin::POINT_COUNT; i++) {
        p[i] = i;
    }
    permute(p, Perlin::POINT_COUNT);
    return p;
}

Perlin::Perlin() {
    _randomData = new double[Perlin::POINT_COUNT];
    for (int i = 0; i < Perlin::POINT_COUNT; ++i) {
        _randomData[i] = randomDouble();
    }

    _permuteX = generatePerm();
    _permuteY = generatePerm();
    _permuteZ = generatePerm();
}

Perlin::~Perlin() {
    delete [] _randomData;
    delete [] _permuteX;
    delete [] _permuteY;
    delete [] _permuteZ;
}

double Perlin::noise(const Point& point) const {
    auto i = int(4.0 * point.x()) & 255;
    auto j = int(4.0 * point.y()) & 255;
    auto k = int(4.0 * point.z()) & 255;

    return _randomData[_permuteX[i] ^ _permuteY[j] ^ _permuteZ[k]];
}
