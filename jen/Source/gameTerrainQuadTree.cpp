/* gameTerrainQuadTree class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "gameTerrain.h"

#define INDEX_X_UP          0
#define INDEX_X_MIDDLE      4
#define INDEX_X_DOWN        1
#define INDEX_Y_UP          2
#define INDEX_Y_MIDDLE      5
#define INDEX_Y_DOWN        3

gameTerrainQuadTree::gameTerrainQuadTree(float** heightmap, const unsigned int& size)
{
    heights = heightmap;
    nodeCount = 0;
    for(unsigned int i = 1; i < size; i <<= 1)
    {
        nodeCount += i*i;
    }
    nodes = new gameTerrainQuadTreeNode[2*(size - 1)*(size - 1) - 1];
    
    for(unsigned int i = 1; i < nodeCount; ++i)
    {
        nodes[i].ownIndex = i;
    }

    // create top node
    unsigned int d = size - 1;
    nodes[0].indices[INDEX_X_UP] = 0;
    nodes[0].indices[INDEX_X_DOWN] = d;
    nodes[0].indices[INDEX_Y_UP] = 0;
    nodes[0].indices[INDEX_Y_DOWN] = d;
    nodes[0].indices[INDEX_X_MIDDLE] = d >> 1;
    nodes[0].indices[INDEX_Y_MIDDLE] = d >> 1;

    nodes[0].recurseSetup(nodes);
}

gameTerrainQuadTree::~gameTerrainQuadTree()
{
    delete[] nodes;
}

/*
    the nodes are stored like this:
    0
    1   2   3   4
    5   6   7   8   9   10  11  12  13  14  15  16
    17 .. 64
    etc...
*/

void gameTerrainQuadTreeNode::recurseSetup(gameTerrainQuadTreeNode* nodes)
{
    if(ownIndex == 0)
    {
        children[0] = &(nodes[1]);
    }
    else
    {
        // find start of "row" this node is in
        unsigned int idx = 1;
        while(idx < ownIndex)
        {
            idx = ((idx - 1) << 2) + 1;
        }

        // work out position relative to start of this "row"
        unsigned int temp = ownIndex - idx;

        // move to the start of the next "row"
        idx = ((idx - 1) << 2) + 1;
        
        // add 4* the relative position since there are four children per node
        children[0] = &(nodes[idx]);
    }

    children[1] = children[0] + sizeof(gameTerrainQuadTreeNode);
    children[2] = children[1] + sizeof(gameTerrainQuadTreeNode);
    children[3] = children[2] + sizeof(gameTerrainQuadTreeNode);

    // fill out corners
    children[0]->indices[INDEX_X_UP] = indices[INDEX_X_UP];
    children[0]->indices[INDEX_X_DOWN] = indices[INDEX_X_MIDDLE];
    children[0]->indices[INDEX_Y_UP] = indices[INDEX_Y_UP];
    children[0]->indices[INDEX_Y_DOWN] = indices[INDEX_Y_MIDDLE];
 
    // work out edges
    children[0]->indices[INDEX_X_MIDDLE] = (indices[INDEX_X_UP] + indices[INDEX_X_MIDDLE]) >> 1;
    children[0]->indices[INDEX_Y_MIDDLE] = (indices[INDEX_Y_UP] + indices[INDEX_Y_MIDDLE]) >> 1;

    // fill out corners
    children[1]->indices[INDEX_X_UP] = indices[INDEX_X_MIDDLE];
    children[1]->indices[INDEX_X_DOWN] = indices[INDEX_X_DOWN];
    children[1]->indices[INDEX_Y_UP] = indices[INDEX_Y_UP];
    children[1]->indices[INDEX_Y_DOWN] = indices[INDEX_Y_MIDDLE];
 
    // work out edges
    children[1]->indices[INDEX_X_MIDDLE] = (indices[INDEX_X_MIDDLE] + indices[INDEX_X_DOWN]) >> 1;
    children[1]->indices[INDEX_Y_MIDDLE] = (indices[INDEX_Y_UP] + indices[INDEX_Y_MIDDLE]) >> 1;

    // fill out corners
    children[2]->indices[INDEX_X_UP] = indices[INDEX_X_UP];
    children[2]->indices[INDEX_X_DOWN] = indices[INDEX_X_MIDDLE];
    children[2]->indices[INDEX_Y_UP] = indices[INDEX_Y_MIDDLE];
    children[2]->indices[INDEX_Y_DOWN] = indices[INDEX_Y_DOWN];
 
    // work out edges
    children[2]->indices[INDEX_X_MIDDLE] = (indices[INDEX_X_UP] + indices[INDEX_X_MIDDLE]) >> 1;
    children[2]->indices[INDEX_Y_MIDDLE] = (indices[INDEX_Y_MIDDLE] + indices[INDEX_Y_DOWN]) >> 1;

    // fill out corners
    children[3]->indices[INDEX_X_UP] = indices[INDEX_X_MIDDLE];
    children[3]->indices[INDEX_X_DOWN] = indices[INDEX_X_DOWN];
    children[3]->indices[INDEX_Y_UP] = indices[INDEX_Y_MIDDLE];
    children[3]->indices[INDEX_Y_DOWN] = indices[INDEX_Y_DOWN];
 
    // work out edges
    children[3]->indices[INDEX_X_MIDDLE] = (indices[INDEX_X_MIDDLE] + indices[INDEX_X_DOWN]) >> 1;
    children[3]->indices[INDEX_Y_MIDDLE] = (indices[INDEX_Y_MIDDLE] + indices[INDEX_Y_DOWN]) >> 1;

    if((indices[INDEX_X_DOWN] - indices[INDEX_X_UP]) > 1)
    {
        children[0]->recurseSetup(nodes);
        children[1]->recurseSetup(nodes);
        children[2]->recurseSetup(nodes);
        children[3]->recurseSetup(nodes);
    }
    else
    {
        children[0]->nullChildren();
        children[1]->nullChildren();
        children[2]->nullChildren();
        children[3]->nullChildren();
    }
}