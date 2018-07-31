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
    glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(vertexData[0]), &vertexData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), &indexData[0], GL_STATIC_DRAW);

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

    float tempF;
    unsigned int tempI;

    for (i = 0; i < n * 3; i++)
    {
        meshFile >> tempF;
        vertexData.push_back(tempF);
    }

    for (i = 0; i < n * 3; i++)
    {
        meshFile >> tempF;
        color.push_back(tempF);
    }

    for (i = 0; i < n * 2; i++)
    {
        meshFile >> tempF;
        texCord.push_back(tempF);
    }

    meshFile >> nIndex;


    for (i = 0; i < nIndex; i++)
    {
        meshFile >> tempI;
        indexData.push_back(tempI);
    }

}