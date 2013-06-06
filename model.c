#include "model.h" 
void SetVertex(Vertex n1, Vertex n2);
void SetNormal(Vertex n1, Vertex n2);
void SetColor(Color c1, Color c2);

Model *ModelNew(int numVertices)
{
    puts("making new model");
    Model* m = malloc(sizeof(Model));
    m->vertices = malloc(numVertices * sizeof(Vertex));
    if(m->vertices == NULL) {
        fprintf(stderr, "Error allocating vertex buffer for new model\n");
    }
    m->normals = malloc(numVertices * sizeof(Normal));
    if(m->normals == NULL) {
        fprintf(stderr, "Error allocating normal buffer for new model\n");
    }
    m->colors = malloc(numVertices * sizeof(Color));
    if(m->colors == NULL) {
        fprintf(stderr, "Error allocating color buffer for new model\n");
    }
    
    m->numFaces = 0; 
    m->numVertices = 0;
    m->subgroups = NULL;

    m->vertexVBOID = 0;
    m->colorVBOID = 0;
    return m;
}

void ModelAddTriangle(Model *m, Vertex v1, Vertex v2, Vertex v3)
{
    SetVertex(m->vertices[m->numFaces*3], v1);
    SetVertex(m->vertices[m->numFaces*3+1], v2);
    SetVertex(m->vertices[m->numFaces*3+2], v3);
    m->numFaces++;
}

void ModelAddVertex(Model *m, Vertex v)
{
    SetVertex(m->vertices[m->numVertices], v);
    m->numVertices++;
}

void ModelAddColor(Model *m, Color c)
{
    SetColor(m->colors[m->numVertices], c);
}

void ModelLoadPLY(Model *m, char *file)
{
    Vertex *vertexBuff;
    Normal *normalBuff;
    Color *colorBuff;
    int *faceBuff;
    int *faceSizeBuff;
    int curFace;

    size_t lineBuffSize;
    char *pch;
    FILE *fp;
    int numFaces, numVertices, numColors;
    int i, j;
    char *lineBuff = NULL;


    
    /* open the file and verify it is a .ply file */
    fp = NULL;
    fp = fopen(file, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error: could not open %s\n", file);
    }
    getline(&lineBuff, &lineBuffSize, fp);
    if(strncmp(lineBuff, "ply", 3) != 0) {
        fprintf(stderr, "Error: file %s is not a .ply file\n", file);
    }

    lineBuff = NULL;
    getline(&lineBuff, &lineBuffSize, fp);
    /* read the file header */
    while(strncmp(lineBuff, "end_header", 10) != 0) {
        /* begin parsing line */
        pch = strtok(lineBuff, " \t");
        if(strncmp(pch, "comment", 7) == 0) {
            /* do nothing */
        }
        /* TODO: size of vertex, faces, and colors all fixed and implied 
          i.e. "property" is not implemented */
        else if(strncmp(pch, "element", 7) == 0) {
            pch = strtok(NULL, " \t");
            /* set number of vertices */
            if(strncmp(pch, "vertex", 6) == 0) {
                numVertices = atoi(strtok(NULL, " \t"));
            }
            /* set number of faces */
            else if(strncmp(pch, "face", 4) == 0) {
                numFaces = atoi(strtok(NULL, " \t"));
            }
            /* set number of colors */
            else if(strncmp(pch, "color", 5) == 0) {
                numColors = atoi(strtok(NULL, " \t"));
            }
        }
        lineBuff = NULL;
        getline(&lineBuff, &lineBuffSize, fp);
    }
    
    /* allocate base buffer for vertices, normals, and colors */
    vertexBuff = (Vertex*)malloc(numVertices * sizeof(Vertex) * 3);
    normalBuff= (Normal*)malloc(numVertices * sizeof(Normal) * 3);
    colorBuff = (Color*)malloc(numVertices * sizeof(Color) * 4);
    faceBuff = (int*)malloc(sizeof(int) * numFaces * 4);
    faceSizeBuff = (int*)malloc(sizeof(int) * numFaces);

    /* worst case allocation TODO: */
    m->vertices = malloc(sizeof(Vertex) * 3 * numFaces * 6);
    m->normals = malloc(sizeof(Normal) * 3 * numFaces * 6);
    m->colors = malloc(sizeof(Color) * 4 * numFaces * 6);

    /* read vertex list */
    for(i = 0; i < numVertices; i++) {
        lineBuff = NULL;
        getline(&lineBuff, &lineBuffSize, fp);
        vertexBuff[i][0] = (float)atof(strtok(lineBuff, " \t"));
        vertexBuff[i][1] = (float)atof(strtok(NULL, " \t"));
        vertexBuff[i][2] = (float)atof(strtok(NULL, " \t"));
        normalBuff[i][0] = (float)atof(strtok(NULL, " \t"));
        normalBuff[i][1] = (float)atof(strtok(NULL, " \t"));
        normalBuff[i][2] = (float)atof(strtok(NULL, " \t"));
    }

    /* read colors list */
    for(i = 0; i < numColors; i++) {
        lineBuff = NULL;
        getline(&lineBuff, &lineBuffSize, fp);
        colorBuff[i][0] = (float)atof(strtok(lineBuff, " \t"));
        colorBuff[i][1] = (float)atof(strtok(NULL, " \t"));
        colorBuff[i][2] = (float)atof(strtok(NULL, " \t"));
        colorBuff[i][3] = (float)atof(strtok(NULL, " \t"));
    }

    /* read face list */
    for(i = 0, curFace = 0; curFace < numFaces; curFace++) {
        lineBuff = NULL;
        getline(&lineBuff, &lineBuffSize, fp);
        faceSizeBuff[curFace] = atoi(strtok(lineBuff, " \t"));

        if(faceSizeBuff[curFace] == 3) {
            faceBuff[i] = atoi(strtok(NULL, " \t"));
            faceBuff[i+1] = atoi(strtok(NULL, " \t"));
            faceBuff[i+2] = atoi(strtok(NULL, " \t"));
            i += 3;
        }
        else if(faceSizeBuff[curFace] == 4) {
            faceBuff[i] = atoi(strtok(NULL, " \t"));
            faceBuff[i+1] = atoi(strtok(NULL, " \t"));
            faceBuff[i+2] = atoi(strtok(NULL, " \t"));
            faceBuff[i+3] = atoi(strtok(NULL, " \t"));
            i += 4;
        }
    }

    /* expand vertex/normal/color information */
    for(i = 0, j = 0, curFace = 0; curFace < numFaces; curFace++) {
        /* if triangle, just expand each vertex */
        if(faceSizeBuff[curFace] == 3) {
            SetVertex(m->vertices[i], vertexBuff[faceBuff[j]]);
            SetNormal(m->normals[i], normalBuff[faceBuff[j]]);
            SetColor(m->colors[i], colorBuff[faceBuff[j]]);

            SetVertex(m->vertices[i+1], vertexBuff[faceBuff[j+1]]);
            SetNormal(m->normals[i+1], normalBuff[faceBuff[j+1]]);
            SetColor(m->colors[i+1], colorBuff[faceBuff[j+1]]);

            SetVertex(m->vertices[i+2], vertexBuff[faceBuff[j+2]]);
            SetNormal(m->normals[i+2], normalBuff[faceBuff[j+2]]);
            SetColor(m->colors[i+2], colorBuff[faceBuff[j+2]]);
            i += 3;
            j += 3;
        }
        /* if quad, do 0,1,2 and 0,2,3 */
        else if(faceSizeBuff[curFace] == 4) {
            SetVertex(m->vertices[i], vertexBuff[faceBuff[j]]);
            SetNormal(m->normals[i], normalBuff[faceBuff[j]]);
            SetColor(m->colors[i], colorBuff[faceBuff[j]]);

            SetVertex(m->vertices[i+1], vertexBuff[faceBuff[j+1]]);
            SetNormal(m->normals[i+1], normalBuff[faceBuff[j+1]]);
            SetColor(m->colors[i+1], colorBuff[faceBuff[j+1]]);

            SetVertex(m->vertices[i+2], vertexBuff[faceBuff[j+2]]);
            SetNormal(m->normals[i+2], normalBuff[faceBuff[j+2]]);
            SetColor(m->colors[i+2], colorBuff[faceBuff[j+2]]);

            SetVertex(m->vertices[i+3], vertexBuff[faceBuff[j]]);
            SetNormal(m->normals[i+3], normalBuff[faceBuff[j]]);
            SetColor(m->colors[i+3], colorBuff[faceBuff[j]]);

            SetVertex(m->vertices[i+4], vertexBuff[faceBuff[j+2]]);
            SetNormal(m->normals[i+4], normalBuff[faceBuff[j+2]]);
            SetColor(m->colors[i+4], colorBuff[faceBuff[j+2]]);

            SetVertex(m->vertices[i+5], vertexBuff[faceBuff[j+3]]);
            SetNormal(m->normals[i+5], normalBuff[faceBuff[j+3]]);
            SetColor(m->colors[i+5], colorBuff[faceBuff[j+3]]);
            i += 6;
            j += 4;
        }
        else {
            fprintf(stderr, "Error: unsupported number of vertices per face\n");
            break;
        }
    }
    m->numVertices = i;

    /*
    for(i = 0; i < m->numVertices; i++) {
        printf("V %f, %f, %f\n", m->vertices[i][0], m->vertices[i][1], m->vertices[i][2]);
        printf("C %f, %f, %f\n", m->colors[i][0], m->colors[i][0], m->colors[i][0]);
    }
    */

    free(vertexBuff);
    free(normalBuff);
    free(colorBuff);
    free(faceBuff);
    free(faceSizeBuff);
}

void ModelAddTriangle2(Model *m, float x1,float y1, float x2,float y2, float x3,float y3)
{
    
}

void SetVertex(Vertex v1, Vertex v2) 
{
    v1[0] = v2[0];
    v1[1] = v2[1];
    v1[2] = v2[2];
}

void SetNormal(Vertex n1, Vertex n2) 
{
    n1[0] = n2[0];
    n1[1] = n2[1];
    n1[2] = n2[2];
}

void SetColor(Color c1, Color c2)
{
    c1[0] = c2[0];
    c1[1] = c2[1];
    c1[2] = c2[2];
    c1[3] = c2[3];
}
