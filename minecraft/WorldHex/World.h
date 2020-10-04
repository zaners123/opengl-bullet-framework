//
// Created by zaners123 on 5/4/20.
//

#ifndef OPENGL_WORLD_H
#define OPENGL_WORLD_H

#include <map>
#include <tuple>
#include "Chunk.h"

typedef std::pair<chunkLoc, blockLoc> blockWorldLoc;

class World : Collidable{
	long seed;

	//chunks that have at any point been loaded (todo could not bother saving never-modified chunks)
	std::map<chunkLoc,Chunk*> chunks;

	//chunks that should be rendered every frame (near the player, subset of chunks)
	std::vector<chunkLoc> renderChunks;

	void showChunk(chunkIndexType x, chunkIndexType y, chunkIndexType z);
	void hideChunk(chunkIndexType x, chunkIndexType y, chunkIndexType z);

public:
	explicit World(long seed);
	void draw(glm::mat4 wvp);
	void updatePlayerLoc(glm::vec3 loc);
	bool isCollision(glm::vec3 loc);

	Chunk* getChunk(chunkLoc loc) {
		return chunks.at(loc);
	}
	blockType* getBlockRef(blockWorldLoc loc) {
		return getChunk(loc.first)->getBlockRef(loc.second);
	}
	blockType getBlock(blockWorldLoc loc) {
		Chunk* c = getChunk(loc.first);
		return c->getBlock(loc.second);
	}

	static chunkLoc getChunkIndex(glm::vec3 loc) {
		//todo this wont work on hex borders
		return Chunk::chunkLocTuple(loc.x/Chunk::physicalSizeX,loc.y/Chunk::physicalSizeY,loc.z/Chunk::physicalSizeZ);
	}
	static blockLoc getBlockIndex(glm::vec3 loc) {

		//mod it inside chunk boundaries (TODO wont work for hex haha)
		loc.x = glm::mod(loc.x,float(Chunk::physicalSizeX));
		loc.y = glm::mod(loc.y,float(Chunk::physicalSizeY));
		loc.z = glm::mod(loc.z,float(Chunk::physicalSizeZ));
//		loc = glm::mod(loc, glm::vec3(float(Chunk::physicalSizeX),float(Chunk::physicalSizeY),float(Chunk::physicalSizeZ)));
		blockLoc ret = Chunk::blockLocTuple(
				loc.x * (float(Chunk::size)/Chunk::physicalSizeX),
				loc.y * (float(Chunk::size)/Chunk::physicalSizeY),
				loc.z * (float(Chunk::size)/Chunk::physicalSizeZ)
				);
		std::cout<<"LOC = "<<Chunk::size * (loc.x/Chunk::physicalSizeX)<<"<>"<<Chunk::size * (loc.y/Chunk::physicalSizeY)<<"<>"<<Chunk::size * loc.z/Chunk::physicalSizeZ<<std::endl;


		return ret;
	}

	static blockWorldLoc blockWorldTuple(chunkIndexType xc, chunkIndexType yc,chunkIndexType zc, blockIndexType xb, blockIndexType yb,blockIndexType zb) {return std::make_pair(Chunk::chunkLocTuple(xc,yc,zc), Chunk::blockLocTuple(xb,yb,zb));}
	static blockWorldLoc blockWorldTuple(chunkLoc chunk, blockLoc block) {return std::make_pair(chunk, block);}
	static blockWorldLoc getBlockWorldLoc(glm::vec3 loc) {return blockWorldTuple(getChunkIndex(loc),getBlockIndex(loc));}
	blockWorldLoc getBlockPointedAt(glm::vec3 loc, glm::vec3 lookingAt) {


		blockWorldLoc ret = getBlockWorldLoc(loc);
		return ret;


		//todo actually do this
		lookingAt = glm::normalize(lookingAt);
		const float increment = .1;
		for (auto x=0;x<10;x++) {
			blockWorldLoc absLoc = getBlockWorldLoc(loc);
			loc += lookingAt*increment;//todo is this right
			if (air != getBlock(absLoc)) return blockWorldLoc(absLoc);
		}
	}
};

#endif //OPENGL_WORLD_H
