
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>

static void swap_bytes(char* x, int size) {
    char* tmp = (char*)malloc(sizeof(char)*size);
    for (int i = 0; i < size; i++) {
        tmp[i] = x[size-1-i];
    }
    for (int i = 0; i < size; i++) {
        x[i] = tmp[i];
    }
    free(tmp);
}

static bool is_little_endian() {
    int x = 1;
    char* y = (char*)malloc(sizeof(int));
    memcpy(y,&x,sizeof(int));
    char z = y[0];
    free(y);
    return z;
}

void ReadPLY(const std::string& filename, std::vector<double>& points,
             std::vector<double>& normals,
             std::vector<double>& colors) {

    points.clear();
    normals.clear();
    colors.clear();

    FILE* fp = fopen(filename.c_str(), "rb");
    if (fp == nullptr) {
        throw std::runtime_error("could not open file");
    }

    char buffer[100];
    char format[100];
    int size;
    char property_type[100];
    char property_name[100];
    int n_property = 0;
    int vertex_byte_offset[3];
    int byte_size = 0;
    bool is_double = false;
    bool normal_exists = false;
    int normal_byte_offset[3];
    bool color_exists = false;
    int color_byte_offset[3];

    fgets(buffer, 99, fp);
    // char str_ply[] = "ply\n";
    if (strcmp(buffer,"ply\n")!=0) {
        fprintf(stderr,"corrupted .ply file\n");
    }

    fgets(buffer, 99, fp);
    sscanf(buffer, "format %s", format);

    fgets(buffer, 99, fp);
    // char str_element_vertex[] = "element vertex";
    while (strncmp(buffer, "element vertex", 14) != 0) {
        fgets(buffer, 99, fp);
    }

    sscanf(buffer, "element vertex %d", &size);

    // char str_float[] = "float";
    // char str_double[] = "double";
    // char str_uchar[] = "uchar";
    fgets(buffer, 99, fp);
    while (sscanf(buffer, "property %s %s",
                  property_type, property_name)==2) {
        n_property++;
        if (strcmp(property_name, "x")==0) {
            vertex_byte_offset[0] = byte_size;
            if (strcmp(property_type, "double")==0) {
                is_double = true;
            }
        }
        if (strcmp(property_name, "y")==0) {
            vertex_byte_offset[1] = byte_size;
        }
        if (strcmp(property_name, "z")==0) {
            vertex_byte_offset[2] = byte_size;
        }
        if (strcmp(property_name, "nx")==0) {
            normal_exists = true;
            normal_byte_offset[0] = byte_size;
        }
        if (strcmp(property_name, "ny")==0) {
            normal_byte_offset[1] = byte_size;
        }
        if (strcmp(property_name, "nz")==0) {
            normal_byte_offset[2] = byte_size;
        }
        if (strcmp(property_name, "red")==0) {
            color_exists = true;
            color_byte_offset[0] = byte_size;
        }
        if (strcmp(property_name, "green")==0) {
            color_byte_offset[1] = byte_size;
        }
        if (strcmp(property_name, "blue")==0) {
            color_byte_offset[2] = byte_size;
        }

        if (strcmp(property_type,"float")==0) {
            byte_size += sizeof(float);
        } else if (strcmp(property_type,"double")==0) {
            byte_size += sizeof(double);
        } else if (strcmp(property_type,"uchar")==0) {
            byte_size += sizeof(unsigned char);
        } else {
            throw std::runtime_error("unknown property type");
        }
        fgets(buffer, 99, fp);
    }

    // char str_end_header[] = "end_header\n";
    while(strcmp(buffer, "end_header\n")!=0) {
        fgets(buffer, 99, fp);
    }

    // char str_ascii[] = "ascii";
    // char str_binary[] = "binary_little_endian";
    if (strcmp(format,"binary_little_endian")==0) {
        points.resize(size*3);
        if (normal_exists) {
            normals.resize(size*3);
        }
        if (color_exists) {
            colors.resize(size*3);
        }
        if (is_double) {
            double x, y, z;
            double nx, ny, nz;
            unsigned char r, g, b;
            bool swap_endian = !is_little_endian();
            for (int i = 0; i < size; i++) {
                fread(buffer, 1, byte_size, fp);
                if (swap_endian) {
                    swap_bytes(buffer+vertex_byte_offset[0],sizeof(double));
                    swap_bytes(buffer+vertex_byte_offset[1],sizeof(double));
                    swap_bytes(buffer+vertex_byte_offset[2],sizeof(double));
                }
                memcpy(&x,buffer+vertex_byte_offset[0],sizeof(double));
                memcpy(&y,buffer+vertex_byte_offset[1],sizeof(double));
                memcpy(&z,buffer+vertex_byte_offset[2],sizeof(double));
                points[i*3+0] = x;
                points[i*3+1] = y;
                points[i*3+2] = z;
                if (normal_exists) {
                    if (swap_endian) {
                        swap_bytes(buffer+normal_byte_offset[0],sizeof(double));
                        swap_bytes(buffer+normal_byte_offset[1],sizeof(double));
                        swap_bytes(buffer+normal_byte_offset[2],sizeof(double));
                    }
                    memcpy(&nx,buffer+normal_byte_offset[0],sizeof(double));
                    memcpy(&ny,buffer+normal_byte_offset[1],sizeof(double));
                    memcpy(&nz,buffer+normal_byte_offset[2],sizeof(double));
                    normals[i*3+0] = nx;
                    normals[i*3+1] = ny;
                    normals[i*3+2] = nz;
                }
                if (color_exists) {
                    memcpy(&r,buffer+color_byte_offset[0],sizeof(unsigned char));
                    memcpy(&g,buffer+color_byte_offset[1],sizeof(unsigned char));
                    memcpy(&b,buffer+color_byte_offset[2],sizeof(unsigned char));
                    colors[i*3+0] = r / 255.;
                    colors[i*3+1] = g / 255.;
                    colors[i*3+2] = b / 255.;
                }
            }
        } else {
            float x, y, z;
            float nx, ny, nz;
            unsigned char r, g, b;
            bool swap_endian = !is_little_endian();
            for (int i = 0; i < size; i++) {
                fread(buffer, 1, byte_size, fp);
                if (swap_endian) {
                    swap_bytes(buffer+vertex_byte_offset[0],sizeof(float));
                    swap_bytes(buffer+vertex_byte_offset[1],sizeof(float));
                    swap_bytes(buffer+vertex_byte_offset[2],sizeof(float));
                }
                memcpy(&x,buffer+vertex_byte_offset[0],sizeof(float));
                memcpy(&y,buffer+vertex_byte_offset[1],sizeof(float));
                memcpy(&z,buffer+vertex_byte_offset[2],sizeof(float));
                points[i*3+0] = x;
                points[i*3+1] = y;
                points[i*3+2] = z;
                if (normal_exists) {
                    if (swap_endian) {
                        swap_bytes(buffer+normal_byte_offset[0],sizeof(float));
                        swap_bytes(buffer+normal_byte_offset[1],sizeof(float));
                        swap_bytes(buffer+normal_byte_offset[2],sizeof(float));
                    }
                    memcpy(&nx,buffer+normal_byte_offset[0],sizeof(float));
                    memcpy(&ny,buffer+normal_byte_offset[1],sizeof(float));
                    memcpy(&nz,buffer+normal_byte_offset[2],sizeof(float));
                    normals[i*3+0] = nx;
                    normals[i*3+1] = ny;
                    normals[i*3+2] = nz;
                }
                if (color_exists) {
                    memcpy(&r,buffer+color_byte_offset[0],sizeof(unsigned char));
                    memcpy(&g,buffer+color_byte_offset[1],sizeof(unsigned char));
                    memcpy(&b,buffer+color_byte_offset[2],sizeof(unsigned char));
                    colors[i*3+0] = r / 255.;
                    colors[i*3+1] = g / 255.;
                    colors[i*3+2] = b / 255.;
                }
            }
        }
    } else if (strcmp(format,"ascii")==0) {
        points.resize(3*size);
        for (int i = 0; i < size; i++) {
            fgets(buffer, 99, fp);
            sscanf(buffer, "%lf %lf %lf", &points[i*3+0], &points[i*3+1], &points[i*3+2]);
        }
    } else {
        throw std::runtime_error("unknown format");
    }

    fclose(fp);
}

void WritePLY(const std::string& filename,
              const std::vector<double>& points,
              const std::vector<double>& normals,
              const std::vector<double>& colors) {

    FILE* fp = fopen(filename.c_str(), "wb");
    fprintf(fp, "ply\n");
    if (is_little_endian()) {
      fprintf(fp, "format binary_little_endian 1.0\n");
    } else {
      fprintf(fp, "format binary_big_endian 1.0\n");
    }
    fprintf(fp, "element vertex %lu\n", points.size() / 3);
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    bool normal_exists = false;
    if (normals.size() == points.size()) {
      normal_exists = true;
      fprintf(fp, "property float nx\n");
      fprintf(fp, "property float ny\n");
      fprintf(fp, "property float nz\n");
    }
    bool color_exists = false;
    if (colors.size() == points.size()) {
      color_exists = true;
      fprintf(fp, "property uchar red\n");
      fprintf(fp, "property uchar green\n");
      fprintf(fp, "property uchar blue\n");
    }
    fprintf(fp, "end_header\n");
    float float_buffer[3];
    unsigned char uchar_buffer[3];
    for (int i = 0; i < points.size() / 3; i++) {
      float_buffer[0] = points[3*i+0];
      float_buffer[1] = points[3*i+1];
      float_buffer[2] = points[3*i+2];
      fwrite(float_buffer, sizeof(float), 3, fp);
      if (normal_exists) {
        float_buffer[0] = normals[3*i+0];
        float_buffer[1] = normals[3*i+1];
        float_buffer[2] = normals[3*i+2];
        fwrite(float_buffer, sizeof(float), 3, fp);
      }
      if (color_exists) {
        uchar_buffer[0] = colors[3*i+0] * 255;
        uchar_buffer[1] = colors[3*i+1] * 255;
        uchar_buffer[2] = colors[3*i+2] * 255;
        fwrite(uchar_buffer, sizeof(unsigned char), 3, fp);
      }
    }

    fclose(fp);
}
