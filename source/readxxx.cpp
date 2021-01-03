
#include "pcdio/pcdio.h"

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>

void ReadXXX(const std::string& filename, std::vector<double>& points,
             std::vector<double>& normals,
             std::vector<double>& colors) {
    if (filename.compare(filename.size()-4,4,".stl")==0) {
        ReadSTL(filename, points, normals, colors);
    } else if (filename.compare(filename.size()-5,5,".xyzm")==0) {
        ReadXYZM(filename, points, normals, colors);
    } else if (filename.compare(filename.size()-4,4,".ply")==0) {
        ReadPLY(filename, points, normals, colors);
    } else {
        fprintf(stderr,"could not recognize file extension\n");
        exit(1);
    }
    return;
}