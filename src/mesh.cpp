#include "../headers/mesh.h"

mesh::mesh(int sP, const char *fileName)
{
    loadFromFile(fileName);

    shaderProgram = sP;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
}

mesh::~mesh()
{
    delete[] finalBuffer;
    delete[] vertexData;
    delete[] color;
    delete[] texCord;
    delete[] indexData;
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void mesh::Display()
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); 
    glDrawElements(GL_TRIANGLES, nIndex, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void mesh::setFinalBuffer()
{
    int iP, iV, iC, iT, i;
    iP = 0;
    iV = 0;
    iC = 0;
    iT = 0;
    while (iP < n*6)
    {
        for (i = 0; i < 3; i++)
        {
            finalBuffer[iP] = vertexData[iV];
            iP++;
            iV++;
        }

        for (i = 0; i < 3; i++)
        {
            finalBuffer[iP] = color[iC];
            iP++;
            iC++;
        }

        for (i = 0; i < 2; i++)
        {
            finalBuffer[iP] = texCord[iT];
            iP++;
            iT++;
        }
    }
}

void mesh::loadFromFile(const char* fileName)
{
    int i;
    std::fstream meshFile;
    meshFile.open(fileName, std::ios::in);
    if (!meshFile.is_open())
    {
        std::cout << "failed to open " << fileName << std::endl;
    }
    meshFile >> n;

    finalBuffer = new float[n * 6];
    vertexData  = new float[n * 3];
    color       = new float[n * 3];
    texCord     = new float[n * 2];

    for (i = 0; i < n * 3; i++)
    {
        meshFile >> vertexData[i];
    }

    for (i = 0; i < n * 3; i++)
    {
        meshFile >> color[i];
    }

    for (i = 0; i < n * 2; i++)
    {
        meshFile >> texCord[i];
    }

    meshFile >> nIndex;

    indexData = new unsigned int[nIndex];

    for (i = 0; i < nIndex; i++)
    {
        meshFile >> indexData[i];
    }

}