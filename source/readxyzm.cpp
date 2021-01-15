
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>

void ReadXYZM(const std::string& filename, std::vector<double>& points,
              std::vector<double>& normals,
              std::vector<double>& colors) {

    FILE* fp = fopen(filename.c_str(),"rb");
    if (fp == nullptr) {
        throw std::runtime_error("failed to open file");
    }
    int height;
    int width;
    int ret = fscanf(fp, "image size width x height = %d x %d", &width, &height);
    if (ret!=2) {
        fclose(fp);
        std::runtime_error("unsupported file format");
    }
    while(fgetc(fp)==0);
    fseek(fp,-1,SEEK_CUR);
    points.clear();
    normals.clear();
    colors.clear();
    points.resize(width*height*3);
    float buffer[3];
    for (size_t i = 0; i < (size_t)width*height; i++) {
        fread(buffer, sizeof(float), 3, fp);
        points[i*3+0] = buffer[0]; points[i*3+1] = buffer[1]; points[i*3+2] = buffer[2];
    }
    colors.resize(width*height*3);
    unsigned char color_buffer[3];
    for (size_t i = 0; i < (size_t)width*height; i++) {
        fread(color_buffer, sizeof(unsigned char), 3, fp);
        colors[i*3+0] = color_buffer[0] / 255.;
        colors[i*3+1] = color_buffer[1] / 255.;
        colors[i*3+2] = color_buffer[2] / 255.;
    }
    fclose(fp);
}
