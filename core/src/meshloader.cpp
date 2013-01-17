/*
  Copyright � 2012 The KoRE Project

  This file is part of KoRE.

  KoRE is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  KoRE is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with KoRE.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "core/common.h"

#include "core/meshloader.h"
#include "core/log.h"

#include "assimp/scene.h"
#include "assimp/postprocess.h"

kore::MeshLoader* kore::MeshLoader::getInstance() {
    static MeshLoader clInstance;
    return &clInstance;
}

kore::MeshLoader::MeshLoader() {
}

kore::MeshLoader::~MeshLoader() {
}

std::shared_ptr<kore::Mesh>
kore::MeshLoader::loadMesh(const std::string& szMeshPath,
                           const bool bUseBuffers) {
    const aiScene* pAiScene = _aiImporter.ReadFile(szMeshPath,
        aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);

    if (!pAiScene) {
        Log::getInstance()->write("[ERROR] Mesh-file could not be loaded: %s",
                                  szMeshPath.c_str());

        return std::shared_ptr<Mesh>();
    }

    if (!pAiScene->HasMeshes()) {
        Log::getInstance()->write("[ERROR] Mesh-file does not"
                                  "contain any meshes: %s",
                                  szMeshPath.c_str());
        return std::shared_ptr<Mesh>();
    }

    if (pAiScene->mNumMeshes > 1) {
        Log::getInstance()->write("[WARNING] Mesh-file contains more than one"
                                  "mesh but it is loaded as a single mesh. %s",
                                  szMeshPath.c_str());
    }

    std::shared_ptr<kore::Mesh> pMesh(new kore::Mesh);
    aiMesh* pAiMesh = pAiScene->mMeshes[ 0 ];
    pMesh->_numVertices = pAiMesh->mNumVertices / 3;

    // TODO(dlazarek): Make more flexible here:
    pMesh->_primitiveType = GL_TRIANGLES;

    pMesh->_name = pAiMesh->mName.C_Str();

    if (pAiMesh->HasPositions()) {
        loadVertexPositions(pAiMesh, pMesh);
    }

    if (pAiMesh->HasNormals()) {
        loadVertexNormals(pAiMesh, pMesh);
    }

    if (pAiMesh->HasTangentsAndBitangents()) {
        loadVertexTangents(pAiMesh, pMesh);
    }

    // Load all texture coord-sets
    unsigned int iUVset = 0;
    while (pAiMesh->HasTextureCoords(iUVset++)) {
        loadVertexTextureCoords(pAiMesh, pMesh, iUVset);
    }

    // Load all vertex color sets
    unsigned int iColorSet = 0;
    while (pAiMesh->HasVertexColors(iColorSet++)) {
        loadVertexColors(pAiMesh, pMesh, iColorSet);
    }

    if (pAiMesh->HasFaces()) {
        loadFaceIndices(pAiMesh, pMesh);
    }

    return pMesh;
}

void kore::MeshLoader::
    loadVertexPositions(const aiMesh* pAiMesh,
                         std::shared_ptr<kore::Mesh>& pMesh ) {
    unsigned int allocSize = pAiMesh->mNumVertices * 3 * 4;
    void* pVertexData = malloc(allocSize);
    memcpy(pVertexData, pAiMesh->mVertices,
           allocSize);

    kore::Attribute att;
    att.name = "v_position";
    att.numValues = pAiMesh->mNumVertices;
    att.type = GL_FLOAT_VEC3;
    att.componentType = GL_FLOAT;
    att.byteSize = kore::DatatypeUtil::getSizeFromGLdatatype(att.type);
    att.data = pVertexData;
    pMesh->_attributes.push_back(att);
}

void kore::MeshLoader::
    loadVertexNormals(const aiMesh* pAiMesh,
                       std::shared_ptr<kore::Mesh>& pMesh ) {
    unsigned int allocSize = pAiMesh->mNumVertices * 3 * 4;
    void* pVertexData = malloc(allocSize);
    memcpy(pVertexData, pAiMesh->mNormals,
           allocSize);

    kore::Attribute att;
    att.name = "v_normal";
    att.numValues = pAiMesh->mNumVertices;
    att.type = GL_FLOAT_VEC3;
    att.componentType = GL_FLOAT;
    att.byteSize = kore::DatatypeUtil::getSizeFromGLdatatype(att.type);
    att.data = pVertexData;
    pMesh->_attributes.push_back(att);
}

void kore::MeshLoader::
    loadVertexTangents(const aiMesh* pAiMesh,
                        std::shared_ptr<kore::Mesh>& pMesh ) {
    unsigned int allocSize = pAiMesh->mNumVertices * 3 * 4;
    void* pVertexData = malloc(allocSize);
    memcpy(pVertexData, pAiMesh->mTangents,
           allocSize);

    kore::Attribute att;
    att.name = "v_tangent";
    att.numValues = pAiMesh->mNumVertices;
    att.type = GL_FLOAT_VEC3;
    att.componentType = GL_FLOAT;
    att.byteSize = kore::DatatypeUtil::getSizeFromGLdatatype(att.type);
    att.data = pVertexData;
    pMesh->_attributes.push_back(att);
}

void kore::MeshLoader::
    loadFaceIndices(const aiMesh* pAiMesh,
                     std::shared_ptr<kore::Mesh>& pMesh ) {
    for (unsigned int iFace = 0; iFace < pAiMesh->mNumFaces; ++iFace) {
        aiFace& rAiFace = pAiMesh->mFaces[iFace];
        for (unsigned int iIndex = 0; iIndex < rAiFace.mNumIndices; ++iIndex) {
            pMesh->_indices.push_back(rAiFace.mIndices[iIndex]);
        }
    }
}

void kore::MeshLoader::
    loadVertexColors(const aiMesh* pAiMesh,
                      std::shared_ptr<kore::Mesh>& pMesh,
                      unsigned int iColorSet) {
    unsigned int allocSize =
        pAiMesh->mNumVertices * 4 * pAiMesh->GetNumColorChannels();
    void* pVertexData = malloc(allocSize);
    memcpy(pVertexData, pAiMesh->mColors[iColorSet], allocSize);

    kore::Attribute att;
    char szNameBuf[20];
    sprintf(szNameBuf, "v_color%i", iColorSet);
    att.name = std::string(&szNameBuf[0]);
    att.numValues = pAiMesh->mNumVertices;

    if (pAiMesh->GetNumColorChannels() == 2) {
        att.type = GL_FLOAT_VEC2;
    } else if (pAiMesh->GetNumColorChannels() == 3) {
        att.type = GL_FLOAT_VEC3;
    } else if (pAiMesh->GetNumColorChannels() == 4) {
        att.type = GL_FLOAT_VEC4;
    } else {
        Log::getInstance()->write("[WARNING] Mesh %s has an"
                                  "unsupported number of color channels: %i",
                                  pMesh->getName().c_str());
        free(pVertexData);
        return;
    }

    att.componentType = GL_FLOAT;
    att.byteSize = kore::DatatypeUtil::getSizeFromGLdatatype(att.type);
    att.data = pVertexData;
    pMesh->_attributes.push_back(att);
}

void kore::MeshLoader::
    loadVertexTextureCoords(const aiMesh* pAiMesh,
                             std::shared_ptr<kore::Mesh>& pMesh,
                             unsigned int iUVset) {
    unsigned int allocSize =
        pAiMesh->mNumVertices * 4 * pAiMesh->GetNumUVChannels();
    void* pVertexData = malloc(allocSize);
    memcpy(pVertexData, pAiMesh->mTextureCoords[iUVset], allocSize);

    kore::Attribute att;
    char szNameBuf[20];
    sprintf(szNameBuf, "v_uv%i", iUVset);
    att.name = std::string(&szNameBuf[0]);
    att.numValues = pAiMesh->mNumVertices;

    if (pAiMesh->GetNumUVChannels() == 2) {
        att.type = GL_FLOAT_VEC2;
    } else if (pAiMesh->GetNumUVChannels() == 3) {
        att.type = GL_FLOAT_VEC3;
    } else {
        Log::getInstance()->write("[WARNING] Mesh %s has an unsupported"
                                  "number of UV channels: %i",
                                  pMesh->getName().c_str());
        free(pVertexData);
        return;
    }

    att.componentType = GL_FLOAT;
    att.byteSize = kore::DatatypeUtil::getSizeFromGLdatatype(att.type);
    att.data = pVertexData;
    pMesh->_attributes.push_back(att);
}

