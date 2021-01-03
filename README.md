# PCDIO
PointCloud File Reader in C++ and Python
## Usage in C++
```C++
// read .ply, .stl, .xyzm
READXXX(const std::string& filename,
		std::vector<double>& points,
		std::vector<double>& normals,
		std::vector<double>& colors)
```
## Usage in Python
```Python
import pypcdio

pcd = pypcdio.pcdread("filename")
pcd.points   # points or vertices
pcd.normals  # normals
pcd.colors   # RGB values
```