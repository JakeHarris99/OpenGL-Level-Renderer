#pragma once

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "Texture.h"

using namespace std;


class Model {

	friend class ModelRenderer;

public:
	
	Model(string file, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool rotate) {
		this->rotate = rotate;
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		this->file = file;
	}

	virtual ~Model() {

	}

	void init() {
		Assimp::Importer importer;
		const aiScene * scene = importer.ReadFile(file,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType
		);

		if (!scene) {
			printf("assimp file failed to load: %s", file);
			return;
		}

		for (int i = 0; i < scene->mNumMeshes; i++) {
			aiMesh *mesh = scene->mMeshes[i];
			vector<GLfloat> vertices;
			vector<GLuint> indices;
			vector<GLfloat> uvMap;
			for (int j = 0; j < mesh->mNumVertices; j++) {
				const aiVector3D *pos = &mesh->mVertices[j];
				vertices.push_back(pos->x);
				vertices.push_back(pos->y);
				vertices.push_back(pos->z);
				if (mesh->HasTextureCoords(0)) {
					uvMap.push_back(mesh->mTextureCoords[0][j].x);
					uvMap.push_back(mesh->mTextureCoords[0][j].y);
				}
			}
			for (int j = 0; j < mesh->mNumFaces; j++) {
				aiFace * f = &mesh->mFaces[j];
				if (f->mNumIndices != 3) {

					continue;
				}
				indices.push_back(f->mIndices[0]);
				indices.push_back(f->mIndices[1]);
				indices.push_back(f->mIndices[2]);
			}
			
			GLuint vertexBufferObject;
			GLuint indexBufferObject;
			GLuint uvBufferObject;

			glGenBuffers(1, &vertexBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

			glGenBuffers(1, &uvBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, uvBufferObject);
			glBufferData(GL_ARRAY_BUFFER, uvMap.size() * sizeof(GLfloat), &uvMap[0], GL_STATIC_DRAW);

			glGenBuffers(1, &indexBufferObject);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
			vbos.push_back(vertexBufferObject);
			ibos.push_back(indexBufferObject);
			uvbos.push_back(uvBufferObject);
			indexCounts.push_back(indices.size());
		}

		
	}

	void update()
	{
		if (rotate)
		{
			this->rotation += (glm::vec3(0, 0, 0.00001));
		}
	}

	void setTexture(Texture * texture) {
		this->texture = texture;
	}

private:

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	bool rotate;

	string file;

	vector<GLuint> vbos;
	vector<GLuint> ibos;
	vector<GLuint> uvbos;
	vector<int> indexCounts;
	Texture * texture;
};