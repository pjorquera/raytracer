#include "camera.h"
#include "sphere.h"
#include "scene.h"
#include "bvh.h"
#include "texture.h"
#include "quad.h"
#include "constantmedium.h"

using namespace std;

void randomSpheres() {
    shared_ptr<Scene> scene = make_shared<Scene>();
    
    auto checker = std::make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));
    auto materialGround = make_shared<Lambertian>(checker);
    scene->add(make_shared<Sphere>(Point(0.0, -1000.0, 0.0), 1000.0, materialGround));
    
    for (auto a = -11; a < 11; a++) {
        for (auto b = -11; b < 11; b++) {
            auto chooseMat = randomDouble();
            const Point center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - Point(4.0, 0.2, 0.0)).length() > 0.9) {
                shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8) {
                    // diffuse
                    auto albedo = Color::random(0.0, 1.0) * Color::random(0.0, 1.0);
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    auto center2 = center + Vector(0.0, randomDouble(0.0, 0.5), 0.0);
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
    auto intersectables = scene->intersectables();
    bvh->add(make_shared<Bvh>(intersectables));
    
    Camera().render(bvh, "image.ppm");
}

void twoSpheres() {
    shared_ptr<Scene> scene = make_shared<Scene>();
    
    auto checker = make_shared<CheckerTexture>(0.8, Color(.2, .3, .1), Color(.9, .9, .9));
    
    scene->add(make_shared<Sphere>(Point(0.0, -10.0, 0.0), 10.0, make_shared<Lambertian>(checker)));
    scene->add(make_shared<Sphere>(Point(0.0, 10.0, 0.0), 10.0, make_shared<Lambertian>(checker)));

    Camera camera(16.0 / 9.0, 50, 50, 400, 20.0, Vector(13.0, 2.0, 3.0), Vector(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0), Color(0.7, 0.8, 1.0), 0.0, 10.0);

    camera.render(scene, "image.ppm");
}

void earth() {
    auto earth_texture = make_shared<ImageTexture>("earthmap.jpg");
    auto earth_surface = make_shared<Lambertian>(earth_texture);
    auto globe = make_shared<Sphere>(Point(0.0, 0.0, 0.0), 2.0, earth_surface);
    
    shared_ptr<Scene> scene = make_shared<Scene>();
    scene->add(globe);

    Camera camera(16.0 / 9.0, 100, 50, 400, 20.0, Vector(0.0, 0.0, 12.0), Vector(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0), Color(0.7, 0.8, 1.0), 0.0, 10.0);
    
    camera.render(scene, "image.ppm");
}

void twoPerlinSpheres() {
    shared_ptr<Scene> scene = make_shared<Scene>();
    
    auto checkerTexture = std::make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));
    auto perlinTexture = make_shared<NoiseTexture>(4.0);
    scene->add(make_shared<Sphere>(Point(0.0, -1000.0, 0.0), 1000, make_shared<Lambertian>(checkerTexture)));
    scene->add(make_shared<Sphere>(Point(0.0, 1.2, 0.0), 1.2, make_shared<Lambertian>(perlinTexture)));
    
    Camera camera(16.0 / 9.0, 500, 50, 1920, 20.0, Vector(10.0, 2.0, 3.0), Vector(0.0, 1.0, 0.0), Vector(0.0, 1.0, 0.0));
    
    camera.render(scene, "image.png");
}

void quads() {
    shared_ptr<Scene> scene = make_shared<Scene>();

    auto left_red     = make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    scene->add(make_shared<Quad>(Point(-3,-2, 5), Vector(0, 0,-4), Vector(0, 4, 0), left_red));
    scene->add(make_shared<Quad>(Point(-2,-2, 0), Vector(4, 0, 0), Vector(0, 4, 0), back_green));
    scene->add(make_shared<Quad>(Point( 3,-2, 1), Vector(0, 0, 4), Vector(0, 4, 0), right_blue));
    scene->add(make_shared<Quad>(Point(-2, 3, 1), Vector(4, 0, 0), Vector(0, 0, 4), upper_orange));
    scene->add(make_shared<Quad>(Point(-2,-3, 5), Vector(4, 0, 0), Vector(0, 0,-4), lower_teal));

    Camera camera(1.0, 100, 50, 400, 80.0, Vector(0, 0, 9), Vector(0, 0, 0), Vector(0.0, 1.0, 0.0), Color(0.7, 0.8, 1.0), 0.0);

    camera.render(scene, "image.png");
}

void simple_light() {
    shared_ptr<Scene> scene = make_shared<Scene>();

    auto pertext = make_shared<NoiseTexture>(4);
    scene->add(make_shared<Sphere>(Point(0.0, -1000.0, 0.0), 1000.0, make_shared<Lambertian>(pertext)));
    scene->add(make_shared<Sphere>(Point(0.0, 2.0, 0.0), 2.0, make_shared<Lambertian>(pertext)));

    auto difflight = make_shared<DiffuseLight>(Color(4.0, 4.0, 4.0));
    scene->add(make_shared<Sphere>(Point(0.0, 7.0, 0.0), 2.0, difflight));
    scene->add(make_shared<Quad>(Point(3.0, 1.0, -2.0), Vector(2.0, 0.0, 0.0), Vector(0.0, 2.0, 0.0), difflight));

    Camera camera(16.0 / 9.0, 500, 50, 1920, 20.0, Vector(26.0, 3.0, 6.0), Vector(0.0, 2.0, 0.0), Vector(0.0, 1.0, 0.0), Color(0.0, 0.0, 0.0), 0.0);

    camera.render(scene, "image.png");
}

void cornell_box() {
    shared_ptr<Scene> scene = make_shared<Scene>();

    auto red   = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    scene->add(make_shared<Quad>(Point(555,0,0), Vector(0,555,0), Vector(0,0,555), green));
    scene->add(make_shared<Quad>(Point(0,0,0), Vector(0,555,0), Vector(0,0,555), red));
    scene->add(make_shared<Quad>(Point(343, 554, 332), Vector(-130,0,0), Vector(0,0,-105), light));
    scene->add(make_shared<Quad>(Point(0,0,0), Vector(555,0,0), Vector(0,0,555), white));
    scene->add(make_shared<Quad>(Point(555,555,555), Vector(-555,0,0), Vector(0,0,-555), white));
    scene->add(make_shared<Quad>(Point(0,0,555), Vector(555,0,0), Vector(0,555,0), white));
    
    shared_ptr<Intersectable> box1 = createBox(Point(0,0,0), Point(165,330,165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vector(265,0,295));
    scene->add(box1);

    shared_ptr<Intersectable> box2 = createBox(Point(0,0,0), Point(165,165,165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vector(130,0,65));
    scene->add(box2);
    
    Camera camera(1.0, 500, 50, 1920, 40.0, Vector(278, 278, -800), Vector(278, 278, 0), Vector(0.0, 1.0, 0.0), Color(0.0, 0.0, 0.0), 0.0);
    
    camera.render(scene, "image.png");
}

void cornell_smoke() {
    shared_ptr<Scene> scene = make_shared<Scene>();

    auto red   = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));

    scene->add(make_shared<Quad>(Point(555,0,0), Vector(0,555,0), Vector(0,0,555), green));
    scene->add(make_shared<Quad>(Point(0,0,0), Vector(0,555,0), Vector(0,0,555), red));
    scene->add(make_shared<Quad>(Point(113,554,127), Vector(330,0,0), Vector(0,0,305), light));
    scene->add(make_shared<Quad>(Point(0,555,0), Vector(555,0,0), Vector(0,0,555), white));
    scene->add(make_shared<Quad>(Point(0,0,0), Vector(555,0,0), Vector(0,0,555), white));
    scene->add(make_shared<Quad>(Point(0,0,555), Vector(555,0,0), Vector(0,555,0), white));

    shared_ptr<Intersectable> box1 = createBox(Point(0,0,0), Point(165,330,165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vector(265,0,295));

    shared_ptr<Intersectable> box2 = createBox(Point(0,0,0), Point(165,165,165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vector(130,0,65));

    scene->add(make_shared<ConstantMedium>(box1, 0.01, Color(0,0,0)));
    scene->add(make_shared<ConstantMedium>(box2, 0.01, Color(1,1,1)));

    Camera camera(1.0, 500, 50, 1920, 40.0, Point(278, 278, -800), Point(278, 278, 0), Vector(0.0, 1.0, 0.0), Color(0.0, 0.0, 0.0), 0.0);
    
    camera.render(scene, "image.png");
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {
    shared_ptr<Scene> boxes1 = make_shared<Scene>();
    auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = randomDouble(1, 101);
            auto z1 = z0 + w;

            boxes1->add(createBox(Point(x0,y0,z0), Point(x1,y1,z1), ground));
        }
    }

    shared_ptr<Scene> scene = make_shared<Scene>();

    scene->add(make_shared<Bvh>(boxes1->intersectables()));

    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
    scene->add(make_shared<Quad>(Point(123,554,147), Vector(300,0,0), Vector(0,0,265), light));

    auto center1 = Point(400, 400, 200);
    auto center2 = center1 + Vector(30,0,0);
    auto sphere_material = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    scene->add(make_shared<Sphere>(center1, center2, 50, sphere_material));

    scene->add(make_shared<Sphere>(Point(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    scene->add(make_shared<Sphere>(
        Point(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<Sphere>(Point(360,150,145), 70, make_shared<Dielectric>(1.5));
    scene->add(boundary);
    scene->add(make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Point(0,0,0), 5000, make_shared<Dielectric>(1.5));
    scene->add(make_shared<ConstantMedium>(boundary, .0001, Color(1,1,1)));

    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("earthmap.jpg"));
    scene->add(make_shared<Sphere>(Point(400,200,400), 100, emat));
    auto pertext = make_shared<NoiseTexture>(0.1);
    scene->add(make_shared<Sphere>(Point(220,280,300), 80, make_shared<Lambertian>(pertext)));

    shared_ptr<Scene> boxes2 = make_shared<Scene>();
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2->add(make_shared<Sphere>(Point::random(0,165), 10, white));
    }

    scene->add(make_shared<Translate>(
        make_shared<RotateY>(
            make_shared<Bvh>(boxes2->intersectables()), 15),
            Vector(-100,270,395)
        )
    );

    Camera camera(1.0, samples_per_pixel, max_depth, image_width, 40.0, Point(478, 278, -600), Point(278, 278, 0), Vector(0.0, 1.0, 0.0), Color(0.0, 0.0, 0.0), 0.0);

    camera.render(scene, "image.png");
}

int main() {
    switch (9) {
        case 1: randomSpheres(); break;
        case 2: twoSpheres(); break;
        case 3: earth(); break;
        case 4: twoPerlinSpheres(); break;
        case 5: quads(); break;
        case 6: simple_light(); break;
        case 7: cornell_box(); break;
        case 8: cornell_smoke(); break;
        case 9:  final_scene(800, 10000, 40); break;
        default: final_scene(400,   250,  4); break;
    }
    return 0;
}
