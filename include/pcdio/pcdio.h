//
// Created by Krister Ulvog on 1/2/21.
//

#pragma once

#include <string>
#include <vector>

void ReadXXX(const std::string& filename, std::vector<double>& points,
             std::vector<double>& normals,
             std::vector<double>& colors);

void ReadSTL(const std::string& filename, std::vector<double>& points,
             std::vector<double>& normals,
             std::vector<double>& colors);

void ReadXYZM(const std::string& filename, std::vector<double>& points,
              std::vector<double>& normals,
              std::vector<double>& colors);

void ReadPLY(const std::string& filename, std::vector<double>& points,
             std::vector<double>& normals,
             std::vector<double>& colors);
