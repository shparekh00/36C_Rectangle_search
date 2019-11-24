//
// Created by shivani on 4/8/19.
//

#include <fstream>
#include <iostream>
#include <random>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <num_rectangles> <num_areas> <suffix> [seed]"
                  << std::endl;
        exit(1);
    }

    int nrects = std::stoi(argv[1]);
    int nareas = std::stoi(argv[2]);
    if (nrects <= 0 || nareas <= 0) {
        std::cerr << "Error: wrong arguments" << std::endl;
        exit(1);
    }

    std::ofstream rects, areas;
    rects.open(std::string("rectangles_") + argv[3] + ".dat",
               std::ofstream::trunc);
    areas.open(std::string("areas_") + argv[3] + ".dat",
               std::ofstream::trunc);
    if (!rects.good() || !areas.good()) {
        std::cerr << "Error: cannot open output file(s)" << std::endl;
        exit(1);
    }

    /* random generator */
    std::random_device rd;
    std::mt19937 mt(rd());

    if (argc == 5) {
        int seed = std::stoi(argv[4]);
        if (seed < 0) {
            std::cerr << "Error: wrong seed" << std::endl;
            exit(1);
        }
        mt.seed(seed);
    }

    /* random number distribution for rectangles */
    std::uniform_int_distribution<int> rect_dist(0, nrects);
    for (int i = 0; i < nrects; i++) {
        int ax = rect_dist(mt);
        int ay = rect_dist(mt);
        int bx = ax + rect_dist(mt);
        int by = ay + rect_dist(mt);
        rects << ax << " " << ay << " " << bx << " " << by << "\n";
    }

    /* random number distribution for areas */
    std::uniform_int_distribution<int> area_dist(0, nrects * nrects);
    for (int i = 0; i < nareas; i++)
        areas << area_dist(mt) << "\n";

    rects.close();
    areas.close();

    return 0;
}
