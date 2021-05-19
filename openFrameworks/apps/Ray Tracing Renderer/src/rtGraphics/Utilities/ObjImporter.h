#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "../Objects/rtMesh.h"

namespace rtGraphics
{
	class ObjImoprter
	{
	private:
		static void validateFile(string filePath)
		{
			int pathLength = filePath.length();

			if (pathLength < 4)
				throw ("Cannot import OBJ. '" + filePath + "' is not a valid OBJ file");

			string extension = filePath.substr(pathLength - 4, pathLength);

			if (extension != ".obj")
				throw ("Cannot import file with extension '" + extension + "' as an OBJ file");
		}

		static void parseVertex(rtMesh& mesh, string vertexString)
		{
			stringstream sstream(vertexString);
			float vx;
			float vy;
			float vz;

			//Skip the first character
			sstream.get();
			//Get the vertex x, y, and z
			sstream >> vx;
			sstream >> vy;
			sstream >> vz;

			mesh.addVert(rtVec3f(vx, vy, vz));
		}

		static void parseFace(rtMesh& mesh, string faceString)
		{
			stringstream sstream(faceString);
			string discard;
			string vertex1;
			string vertex2;
			string vertex3;

			//Skip the first character
			sstream.get();

			//Vertex 1
			getline(sstream, vertex1, '/');
			getline(sstream, discard, ' ');
			//Vertex 2
			getline(sstream, vertex2, '/');
			getline(sstream, discard, ' ');
			//Vertex 3
			getline(sstream, vertex3, '/');
			getline(sstream, discard, ' ');

			//Convert the strings to ints
			int v1 = stoi(vertex1) - 1;
			int v2 = stoi(vertex2) - 1;
			int v3 = stoi(vertex3) - 1;

			mesh.addFace(v1, v2, v3);
		}

	public:
		//Imports OBJ files as an rtMesh object. Only tri faces are supported.
		static rtMesh loadOBJ(string filePath)
		{
			//Ensure that the file is an OBJ file
			validateFile(filePath);

			ifstream objFile(filePath);

			if (!objFile.is_open())
				throw "Unable to open file: '" + filePath + "'";

			rtMesh newMesh;
			string line;

			//Parse the obj file
			while (getline(objFile, line))
			{
				switch (line[0])
				{
					case 'v':
						parseVertex(newMesh, line);
						break;

					case 'f':
						parseFace(newMesh, line);
						break;

					//Ignore any lines that aren't a vertex or face
					default:
						continue;
						break;
				}
			}

			objFile.close();

			return newMesh;
		}
	};
}