#include "chunk_mesh.h"

#include <common/world/world_constants.h>

namespace {
template <typename T>
size_t vecSize(const std::vector<T> vect)
{
    return vect.size() * sizeof(vect[0]);
}
} // namespace

ChunkMesh::ChunkMesh(const ChunkPosition& chunkPosition)
    : position(chunkPosition)
{
    vertices.reserve(CHUNK_VOLUME * 2);
    textureCoords.reserve(CHUNK_VOLUME * 2);
    indices.reserve(CHUNK_VOLUME * 2);
    //cardinalLights.reserve(CHUNK_VOLUME * 2);
}

void ChunkMesh::addFace(const MeshFace& face, const BlockPosition& blockPosition,
                        int texture)
{
    int index = 0;
    for (int i = 0; i < 4; i++) {
        GLubyte x = face.vertices[index++] + blockPosition.x;
        GLubyte y = face.vertices[index++] + blockPosition.y;
        GLubyte z = face.vertices[index++] + blockPosition.z;

        GLuint vertex = x | y << 6 | z << 12 | face.lightLevel << 18;

        vertices.push_back(vertex);

        //cardinalLights.push_back(face.lightLevel);
    }
    textureCoords.insert(textureCoords.end(),
                         {0.0f, 0.0f, (float)texture, 1.0f, 0.0f, (float)texture, 1.0f,
                          1.0f, (float)texture, 0.0f, 1.0f, (float)texture});
    indices.push_back(indicesCount);
    indices.push_back(indicesCount + 1);
    indices.push_back(indicesCount + 2);
    indices.push_back(indicesCount + 2);
    indices.push_back(indicesCount + 3);
    indices.push_back(indicesCount);
    indicesCount += 4;
}

gl::VertexArray ChunkMesh::createBuffer()
{
    gl::VertexArray vao;
    vao.bind();
    vao.addVertexBuffer(1, vertices);
    vao.addVertexBuffer(3, textureCoords);
   // vao.addVertexBuffer(1, cardinalLights);
    vao.addIndexBuffer(indices);

    return vao;
}

size_t ChunkMesh::calculateBufferSize() const
{
    return vecSize(vertices) + vecSize(textureCoords) + vecSize(indices);
    
           //vecSize(cardinalLights);
}

ChunkMeshCollection::ChunkMeshCollection(const ChunkPosition& chunkPosition)
    : blockMesh(chunkPosition)
    , fluidMesh(chunkPosition)
    , floraMesh(chunkPosition)
{
}
