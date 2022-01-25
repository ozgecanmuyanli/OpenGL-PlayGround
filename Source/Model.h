#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model
{
public:
   Model() {};
   vector<Texture> textures_loaded;
   vector<Mesh> meshes;
   string directory;
   bool gammaCorrection;

   Model(char* path)
   {
      loadModel(path);
   }
   void Draw(Shader& shader)
   {
      for (unsigned int i = 0; i < meshes.size(); i++)
         meshes[i].Draw(shader);
   }

private: 
   void loadModel(string path)
   {
      Assimp::Importer import;
      const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
      if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
         !scene->mRootNode)
      {
         cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
         return;
      }
      directory = path.substr(0, path.find_last_of('/'));
      processNode(scene->mRootNode, scene);
   }
   void processNode(aiNode* node, const aiScene* scene)
   {
      // process all the node’s meshes (if any)
      for (unsigned int i = 0; i < node->mNumMeshes; i++)
      {
         aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
         meshes.push_back(processMesh(mesh, scene));
      }
      // then do the same for each of its children
      for (unsigned int i = 0; i < node->mNumChildren; i++)
      {
         processNode(node->mChildren[i], scene);
      }
   }
   Mesh processMesh(aiMesh* mesh, const aiScene* scene)
   {
      vector<Vertex> vertices;
      vector<unsigned int> indices;
      vector<Texture> textures;
      for (unsigned int i = 0; i < mesh->mNumVertices; i++)
      {
         Vertex vertex;
         glm::vec3 vector;
         vector.x = mesh->mVertices[i].x;
         vector.y = mesh->mVertices[i].y;
         vector.z = mesh->mVertices[i].z;
         vertex.Position = vector;

         if (mesh->HasNormals())
         {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
         }

         if (mesh->mTextureCoords[0])
         {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

            if (mesh->mTangents)
            {
               //tangent
               vector.x = mesh->mTangents[i].x;
               vector.y = mesh->mTangents[i].y;
               vector.z = mesh->mTangents[i].z;
               vertex.Tangent = vector;
               //bitangent
               vector.x = mesh->mBitangents[i].x;
               vector.y = mesh->mBitangents[i].y;
               vector.z = mesh->mBitangents[i].z;
               vertex.Bitangent = vector;
            }

         }
         else
         {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
         }
         vertices.push_back(vertex);
      }

      for (unsigned int i = 0; i < mesh->mNumFaces; i++)
      {
         aiFace face = mesh->mFaces[i];
         for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
      }

      // process material
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

      vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

      vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

      vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
      textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

      vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
      textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
      
      return Mesh(vertices, indices, textures);
   }

   vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
   {
      vector<Texture> textures;
      for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
      {
         aiString str;
         mat->GetTexture(type, i, &str);
         bool skip = false;
         for (unsigned int j = 0; j < textures_loaded.size(); j++)
         {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
               textures.push_back(textures_loaded[j]);
               skip = true;
               break;
            }
         }
         if (!skip)
         {
            Texture texture = Texture(str.C_Str(), directory);
            texture.type = typeName;
            textures.push_back(texture);
            textures_loaded.push_back(texture);
         }

      }
      return textures;
   }
};

#endif // !MODEL_H