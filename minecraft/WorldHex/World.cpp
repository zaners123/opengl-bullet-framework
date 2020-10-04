//
// Created by zaners123 on 5/4/20.
//

#include "World.h"

World::World(long seed) {
	this->seed = seed;
	Chunk::noise = new FastNoise(int(seed));
	Chunk::noise->SetNoiseType(FastNoise::SimplexFractal);
}

void World::draw(glm::mat4 wvp) {
	for (auto & it : renderChunks) {
		chunks.at(it)->draw(wvp);
	}
}


void World::updatePlayerLoc(glm::vec3 loc) {

	//render distance
	int rd = 3;

//	std::cout<<"World knows where player is. Changing draw list..."<<std::endl;
	long chunkX = long((loc.x+Chunk::physicalSizeX/2)/Chunk::physicalSizeX);
	long chunkY = long((loc.y+Chunk::physicalSizeY/2)/Chunk::physicalSizeY);
	long chunkZ = long((loc.z+Chunk::physicalSizeZ/2)/Chunk::physicalSizeZ);
	//todo only update when player's chunk has changed
	if (true) {
		renderChunks.clear();
		for (long rx=chunkX-rd;rx<=chunkX+rd;rx++) {
			for (long ry=chunkY-rd;ry<=chunkY+rd;ry++) {
				for (long rz = chunkZ - rd; rz <= chunkZ + rd; rz++) {
					showChunk(rx, ry, rz);
				}
			}
		}
	}

}

bool World::isCollision(glm::vec3 loc) {
	//todo go to that chunk and see if that block collides
}

void World::showChunk(long x, long y, long z) {
	chunkLoc loc = Chunk::chunkLocTuple(x,y,z);
	if (chunks.count(loc)==0) {
		//generate if necessary
		chunks.insert(std::make_pair(loc, new Chunk(seed, x,y,z)));
	}
	//make visible
	renderChunks.push_back(loc);
}

void World::hideChunk(long x, long y, long z) {

}

