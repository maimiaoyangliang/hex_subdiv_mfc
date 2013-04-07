

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <string>
#include <fstream>

namespace VolumeMesh {

namespace IO {

class FileManager {
public:
	FileManager();
	~FileManager();

	bool readFile(const std::string& _filename, Hexahedron* _mesh) const;
	bool isHexahedralMesh(const std::string& _filename) const;

private:
	void trimString(std::string& _string) const;
	bool getCleanLine(std::istream& _ifs, std::string& _string, bool _skipEmptyLines = true) const;
}; // Class FileManager

} // Namespace VolumeMesh
} // Namespace VolumeMesh


#endif