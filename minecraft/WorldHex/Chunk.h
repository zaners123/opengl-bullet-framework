//
// Created by zaners123 on 5/2/20.
//

#ifndef OPENGL_CHUNK_H
#define OPENGL_CHUNK_H


#include <iostream>
#include <map>
#include "PlaneMesh.h"
#include "../libs/fastnoise/FastNoise.h"

constexpr static int deltas[] = {
		-1,0,0,     0,-1,0,     0,0,-1,
		 1,0,0,     0, 1,0,     0,0, 1,
};

//todo remove and replace with optional blockdata, but by default a block should just be a byte/short/int or something
typedef unsigned char blockType;
const blockType air  = 0;
const blockType stone= 1;
const blockType grass= 2;
const blockType dirt = 3;
const blockType wood = 5;

static std::vector<signed char> hexdelta = {
		-1,0,0,1,0,0,
		0,-1,0,0,1,0,
		0,0,-1,0,0,1,
		-1,0,1,1,0,1,
};

typedef unsigned char chunkBounds;
typedef long chunkIndexType;

typedef std::tuple<blockIndexType,blockIndexType,blockIndexType> blockLoc;
typedef std::tuple<chunkIndexType, chunkIndexType, chunkIndexType> chunkLoc;

class Chunk {

	enum class Biome{Forest, Mountains, Plains, Ocean, Desert};

	long seed;

	chunkIndexType x,y,z;

	//todo as long as chunk is 128^3 or smaller, vertices could be referenced using one byte (1/4 the memory!)

	//width of chunk. And height. Etc.

public:
//	std::map<blockLoc, Block*> block;
	static blockLoc blockLocTuple(blockIndexType x, blockIndexType y, blockIndexType z);
	static chunkLoc chunkLocTuple(chunkIndexType x, chunkIndexType y, chunkIndexType z);
	static FastNoise* noise;
	PlaneMesh* pm;
//testing what should chunk size be? Should be 0 < size <= 256
const static int size = 64;
constexpr static double physicalSizeX = size * 1.5;
constexpr static double physicalSizeY = size;
constexpr static double physicalSizeZ = size * 1.732f;//todo whats this number???

	blockType block[size][size][size];
private:
	void initChunkBlocks();
	bool blockNotAir(int x, int y, int z);
	static bool inChunk(int x, int y, int z);
public:
	Chunk(long seed, chunkIndexType x, chunkIndexType y, chunkIndexType z);
	void genMesh();
	void draw(glm::mat4 wvp);
	blockType* getBlockRef(blockLoc loc);
	blockType getBlock(blockLoc loc);
	blockType getBlock(blockIndexType x, blockIndexType y, blockIndexType z);
	static long groundHeight(long x, long z);
	long chunkSeed() const;
	Biome getBiome(chunkIndexType x, chunkIndexType y, chunkIndexType z);
	void addCubeToMesh(blockType blocktype, chunkBounds x, chunkBounds y, chunkBounds z);
};


#endif //OPENGL_CHUNK_H
