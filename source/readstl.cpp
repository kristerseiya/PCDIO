
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>

void ReadSTL(const std::string& filename, std::vector<double>& points,
             std::vector<double>& normals,
             std::vector<double>& colors) {

    FILE* fp = fopen(filename.c_str(),"rb");
    if (fp == nullptr) {
        throw std::runtime_error("failed to open file");
    }
    char header[80];
    fread(header,sizeof(char),80,fp);
    unsigned int n_face;
    fread(&n_face, sizeof(unsigned int), 1, fp);
    size_t n_points = ((size_t)n_face) * 9;

    points.clear();
    normals.clear();
    colors.clear();
    points.resize(n_points);

    float buffer[9];
    for (size_t i = 0; i < n_face; i++) {
        fread(buffer, sizeof(float), 3, fp);
        fread(buffer, sizeof(float), 9, fp);
        points[i*9+0] = buffer[0]; points[i*9+1] = buffer[1]; points[i*9+2] = buffer[2];
        points[i*9+3] = buffer[3]; points[i*9+4] = buffer[4]; points[i*9+5] = buffer[5];
        points[i*9+6] = buffer[6]; points[i*9+7] = buffer[7]; points[i*9+8] = buffer[8];
        fread(buffer, sizeof(short), 1, fp);
    }

    fclose(fp);

    return;
}