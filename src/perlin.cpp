#include "perlin.h"

#include <cmath>

using namespace std;

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

double interpolation(Vector c[2][2][2], double u, double v, double w) {
    auto uu = u * u * (3.0 - 2.0 * u);
    auto vv = v * v * (3.0 - 2.0 * v);
    auto ww = w * w * (3.0 - 2.0 * w);
    auto accum = 0.0;

    for (int i=0; i < 2; i++)
        for (int j=0; j < 2; j++)
            for (int k=0; k < 2; k++) {
                Vector weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1.0 - i) * (1.0 - uu))
                       * (j * vv + (1.0 - j) * (1.0 - vv))
                       * (k * ww + (1.0 - k) * (1.0 - ww))
                       * Vector::dot(c[i][j][k], weight_v);
            }

    return accum;
}

Perlin::Perlin() {
    _randomData = new Point[Perlin::POINT_COUNT];
    for (int i = 0; i < Perlin::POINT_COUNT; ++i) {
        _randomData[i] = Vector::random(-1.0, 1.0).unit();
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
    auto u = point.x() - floor(point.x());
    auto v = point.y() - floor(point.y());
    auto w = point.z() - floor(point.z());
    
    u = u * u * (3.0 - 2.0 * u);
    v = v * v * (3.0 - 2.0 * v);
    w = w * w * (3.0 - 2.0 * w);

    auto i = int(floor(point.x()));
    auto j = int(floor(point.y()));
    auto k = int(floor(point.z()));
    Vector c[2][2][2];

    for (int di=0; di < 2; di++)
        for (int dj=0; dj < 2; dj++)
            for (int dk=0; dk < 2; dk++)
                c[di][dj][dk] = _randomData[
                    _permuteX[(i + di) & 255] ^
                    _permuteY[(j + dj) & 255] ^
                    _permuteZ[(k + dk) & 255]
                ];

    return interpolation(c, u, v, w);
}

double Perlin::turbulence(const Point& p, int depth) const {
    auto accum = 0.0;
    auto tmp = p;
    auto weight = 1.0;

    for (int i = 0; i < depth; i++) {
        accum += weight * noise(tmp);
        weight *= 0.5;
        tmp *= 2.0;
    }

    return fabs(accum);
}
