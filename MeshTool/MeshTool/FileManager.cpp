

#define FILEMANAGER_CPP_

#include "stdafx.h"

#include <iostream>
#include <algorithm>
#include <sstream>


#include "FileManager.h"

namespace VolumeMesh {

namespace IO {

//======================================================

FileManager::FileManager() {

}

//======================================================

FileManager::~FileManager() {

}

//======================================================

void FileManager::trimString(std::string& _string) const {

	size_t start = _string.find_first_not_of(" \t\r\n");
	size_t end = _string.find_last_not_of(" \t\r\n");

	if ((std::string::npos == start) || (std::string::npos == end)) {
		_string = "";
	} else {
		_string = _string.substr(start, end - start + 1);
	}
}

//======================================================

bool FileManager::getCleanLine(std::istream& _ifs, std::string& _string, 
	bool _skipEmptyLines /* = true */) const {

		while (true) {

			std::getline(_ifs, _string);
			trimString(_string);
			if (_string.size() != 0) {

				if (_string[0] != '#') {
					return true;
				}

			} else {
				if (!_skipEmptyLines)
					return true;
			}

			if (_ifs.eof()) {
				std::cerr << "End of file reached while searching for input!" << std::endl;
				return false;
			}
		}

		return false;
}

//======================================================

bool FileManager::isHexahedralMesh(const std::string& _filename) const {

	std::ifstream iff(_filename.c_str(), std::ios::in);

	if (!iff.good()) {
		std::cerr << "Could not open file " << _filename << "for reading!" << std::endl;
		iff.close();
		return false;
	}

	std::string s;
	unsigned int n = 0u;

	while (!iff.eof()) {
		iff >> s;
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		if (s == "POLYHEDRA")
			break;
	}

	if (iff.eof()) {
		iff.close();
		return false;
	}

	iff >> n;
	if (n == 0)
		return false;
	unsigned int v = 0u;
	char tmp[256];
	for (unsigned int i = 0; i < n; ++ i) {
		iff >> v;
		iff.getline(tmp, 256);
		if (v != 6u) {
			iff.close();
			return false;
		}
	}
	iff.close();
	return true;

}

//======================================================

bool FileManager::readFile(const std::string& _filename, Hexahedron* _mesh) const {

	std::ifstream iff(_filename.c_str(), std::ios::in);

	if (!iff.good()) {
		std::cerr << "Error: Could not open file " << _filename << "for reading!" << std::endl;
		iff.close();
		return false;
	}

	std::stringstream sstr;
	std::string line;
	std::string s_tmp;
	unsigned int c = 0u;
	Point3D v = Point3D();
	unsigned int v1 = 0u;
	unsigned int v2 = 0u;
	unsigned int val;
	
	typedef std::vector<int> IntArray;
	IntArray edges_, faces_;

	bool header_found = true;

	// Get first line
	getCleanLine(iff, line);
	sstr.str(line);

	// Check header
	sstr >> s_tmp;
	std::transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "VM") {
		header_found = false;
		std::cerr << "The specified file might not be in VolumeMesh format!" << std::endl;
	}

	/*
	 * Vertices
	 */
	if (!header_found) {
		sstr.clear();
		sstr.str(line);
	} else {
		getCleanLine(iff, line);
		sstr.clear();
		sstr.str(line);  
	}

	sstr >> s_tmp;
	std::transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "VERTICES") {
		iff.close();
		std::cerr << "No vertex section defined!" << std::endl;
		return false;
	}

	getCleanLine(iff, line);
	sstr.clear();
	sstr.str(line);
	sstr >> c;

	for (unsigned int i = 0u; i < c; ++i) {
		getCleanLine(iff, line);
		sstr.clear();
		sstr.str(line);
		sstr >> v.x;
		sstr >> v.y;
		sstr >> v.z;
		_mesh->m_pHexModel->AddVertex(v);
	}

	/*
	 * Edges
	 */
	getCleanLine(iff, line);
	sstr.clear();
	sstr.str(line);
	sstr >> s_tmp;
	std::transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "EDGES") {
		iff.close();
		std::cerr << "No edges section defined!" << std::endl;
		return false;
	}

	getCleanLine(iff, line);
	sstr.clear();
	sstr.str(line);
	sstr >> c;

	for (unsigned int i = 0; i < c; ++i) {
		getCleanLine(iff, line);
		sstr.clear();
		sstr.str(line);
		sstr >> v1;
		sstr >> v2;
		_mesh->m_pHexModel->AddEdge(v1, v2);
	}

	/*
	 *Faces
	 */
	getCleanLine(iff, line);
	sstr.clear();
	sstr.str(line);
	sstr >> s_tmp;
	std::transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "FACES") {
		iff.close();
		std::cerr << "No faces defined!" << std::endl;
		return false;
	}

	getCleanLine(iff, line);
	sstr.clear();
	sstr.str(line);
	sstr >> c;
	for (unsigned int i = 0u; i < c; ++i) {
		getCleanLine(iff, line);
		sstr.clear();
		sstr.str(line);

		// Get face valence
		val = 0u;
		sstr >> val;

		for (unsigned int e = 0u; e < val; ++e) {
			sstr >> v1;
			edges_.push_back(v1);
		}

		_mesh->m_pHexModel->AddFace(edges_);
		edges_.clear();
	}

	/*
	 * Cells
	 */
	getCleanLine(iff, line);
	sstr.clear();
	sstr.str(line);
	sstr >> s_tmp;
	std::transform(s_tmp.begin(), s_tmp.end(), s_tmp.begin(), ::toupper);
	if (s_tmp != "POLYHEDRA") {
		iff.close();
		std::cerr << "No polyhedra section defined!" << std::endl;
		return false;
	}

	getCleanLine(iff, line);
	sstr.clear();
	sstr.str(line);
	sstr >> c;

	for (unsigned int i = 0u; i < c; ++i) {
		getCleanLine(iff, line);
		sstr.clear();
		sstr.str(line);

		// Get cell valence
		val = 0u;
		sstr >> val;
		for (unsigned int f = 0u; f < val; ++f) {
			sstr >> v1;
			faces_.push_back(v1);
		}

		_mesh->m_pHexModel->AddCell(faces_);
		faces_.clear();
	}

	iff.close();

	std::cerr << "########### volumemesh info ##########" << std::endl;
	std::cerr << "#vertices: " << _mesh->m_pHexModel->vertices.size() << std::endl;
	std::cerr << "#edges:    " << _mesh->m_pHexModel->edges.size() << std::endl;
	std::cerr << "#faces:    " << _mesh->m_pHexModel->faces.size() << std::endl;
	std::cerr << "#cells:    " << _mesh->m_pHexModel->cells.size() << std::endl;

	return true;

}

} // Namespace IO
}// Namespace VolumeMesh