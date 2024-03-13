#include "camera.h"
#include "sphere.h"
#include "scene.h"
#include "bvh.h"
#include "texture.h"

using namespace std;

void randomSpheres() {
    shared_ptr<Scene> scene = make_shared<Scene>();
    
    const auto checker = std::make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));
    const auto materialGround = make_shared<Lambertian>(checker);
    scene->add(make_shared<Sphere>(Point(0.0, -1000.0, 0.0), 1000.0, materialGround));
    
    for (auto a = -11; a < 11; a++) {
        for (auto b = -11; b < 11; b++) {
            const auto chooseMat = randomDouble();
            const Point center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - Point(4.0, 0.2, 0.0)).length() > 0.9) {
                shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8) {
                    // diffuse
                    const auto albedo = Color::random(0.0, 1.0) * Color::random(0.0, 1.0);
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    const auto center2 = center + Vector(0.0, randomDouble(0.0, 0.5), 0.0);
                    scene->add(make_shared<Sphere>(center, center2, 0.2, sphereMaterial));
                } else if (chooseMat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1.0);
                    auto fuzz = randomDouble(0.0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    scene->add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    scene->add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    scene->add(make_shared<Sphere>(Point(0.0, 1.0, 0.0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    scene->add(make_shared<Sphere>(Point(-4.0, 1.0, 0.0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene->add(make_shared<Sphere>(Point(4.0, 1.0, 0.0), 1.0, material3));

    shared_ptr<Scene> bvh = make_shared<Scene>();
    const auto intersectables = scene->intersectables();
    bvh->add(make_shared<Bvh>(intersectables));
    
    Camera().render(bvh, "image.ppm");
}

void twoSpheres() {
    shared_ptr<Scene> scene = make_shared<Scene>();
    
    const auto checker = make_shared<CheckerTexture>(0.8, Color(.2, .3, .1), Color(.9, .9, .9));
    
    scene->add(make_shared<Sphere>(Point(0.0, -10.0, 0.0), 10.0, make_shared<Lambertian>(checker)));
    scene->add(make_shared<Sphere>(Point(0.0, 10.0, 0.0), 10.0, make_shared<Lambertian>(checker)));

    Camera camera(16.0 / 9.0, 50, 50, 400, 20.0, Vector(13.0, 2.0, 3.0), Vector(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0), 0.0, 10.0);

    camera.render(scene, "image.ppm");
}

void earth() {
    const auto earth_texture = make_shared<ImageTexture>("earthmap.jpg");
    const auto earth_surface = make_shared<Lambertian>(earth_texture);
    const auto globe = make_shared<Sphere>(Point(0.0, 0.0, 0.0), 2.0, earth_surface);
    
    shared_ptr<Scene> scene = make_shared<Scene>();
    scene->add(globe);

    Camera camera(16.0 / 9.0, 100, 50, 400, 20.0, Vector(0.0, 0.0, 12.0), Vector(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0), 0.0, 10.0);
    
    camera.render(scene, "image.ppm");
}

int main() {
    switch (3) {
        case 1: randomSpheres(); break;
        case 2: twoSpheres(); break;
        case 3: earth(); break;
    }
    return 0;
}
