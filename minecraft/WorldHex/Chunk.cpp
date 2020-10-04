//
// Created by zaners123 on 5/2/20.
//

#include "Chunk.h"
#include "../libs/perlin/PerlinNoise.h"
#include "../libs/fastnoise/FastNoise.h"

FastNoise* Chunk::noise = nullptr;

/**Places the blocks in the chunk based on the seed and the chunk location
 * */

long Chunk::chunkSeed() const {
	return seed + x + y*1024 + z*1024*1024;
}

long Chunk::groundHeight(long x, long z) {
	return int(size * (.5 + .2 * noise->GetNoise(x,z)));
}

Chunk::Biome Chunk::getBiome(long x, long y, long z) {
	return Chunk::Biome::Forest;
}

void Chunk::initChunkBlocks() {
	//main default value should be air (or else you get 4d artifacts haha)
	for (blockIndexType bx=0;bx<size;bx++)
		for (blockIndexType by=0;by<size;by++)
			for (blockIndexType bz=0;bz<size;bz++)
				block[bx][by][bz]=air;

	//main Ground (from noise heightmap)
	for (blockIndexType bx=0; bx < size; bx++) {
		for (blockIndexType bz=0; bz < size; bz++) {
			long height = groundHeight(bx + x * size, bz + z * size);
			for (blockIndexType by=0; by < size; by++) {
//				std::cout << bx << "," << by << "," << bz << " = " << groundHeight << std::endl;
				if (by + y*size < height) {
					if ((bx+by+bz)%2==0) {
						block[bx][by][bz] = stone;
					} else {
						block[bx][by][bz] = dirt;
					}
				}

			}
//			if (rand()%60 < 1) block[bx][rand()%size][bz] = 0;
//			if (rand()%60 < 1) block[bx][rand()%size][bz] = wood;
		}
	}

	//todo only gen trees on ground-layer chunk

	/*const int treeAttempts = 20;
	//random chunk seed based on main seed
	srand(chunkSeed());
	for (int tree = 0; tree < treeAttempts; tree++) {
		chunkBounds treeChunkX = chunkBounds(rand())%size;
		chunkBounds treeChunkZ = chunkBounds(rand())%size;

		long treeX = x*size + treeChunkX;
		long treeZ = z*size + treeChunkZ;
		long treeY = groundHeight(treeX, treeZ) + 2;

		if (treeY/size != y) continue;

		unsigned char treeChunkY = treeY % size;

		//todo multiply by biome tree factor (high mountains may have less than forests)
		if (Biome::Forest == getBiome(treeX, treeY, treeZ)) {
			while (treeChunkY>0 && block[treeChunkX][treeChunkY][treeChunkZ]==0) {
				block[treeChunkX][treeChunkY][treeChunkZ] = wood;
				treeChunkY--;
			}
		}
	}*/

}

Chunk::Chunk(long seed, long x, long y, long z) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->seed = seed;

	pm = new PlaneMesh();

	initChunkBlocks();

	genMesh();
	pm->update();
}

void Chunk::draw(glm::mat4 wvp) {
//	std::cout<<"FACES: "<<pm->planeLoc.size()/pm->planeSize<<" / "<<pm->planeMaxSize/sizeof(int)/pm->planeSize<<std::endl;
	glUseProgram(pm->getProg());
	wvp = glm::translate(wvp, glm::vec3(x*physicalSizeX, y*physicalSizeY,z*physicalSizeZ));
	glUniformMatrix4fv(glGetUniformLocation(pm->getProg(), "wvp"), 1, GL_FALSE, (GLfloat *) &wvp);
	pm->draw();
}

void Chunk::addCubeToMesh(blockType blocktype, chunkBounds x, chunkBounds y, chunkBounds z) {
	if (blocktype==0) return;
	bool draw = false;
	for (auto i=0;i<hexdelta.size();i+=3) {
		if (inChunk(x+hexdelta[i],y+hexdelta[i+1],z+hexdelta[i+2]) && getBlock(x+hexdelta[i],y+hexdelta[i+1],z+hexdelta[i+2])==air) {
			draw = true;
			break;
		}
	}
	if (!draw) return;
//	std::cout<<(int)blocktype<<std::endl;
	pm->planeLoc.emplace_back(x);
	pm->planeLoc.emplace_back(y);
	pm->planeLoc.emplace_back(z);
	pm->planeLoc.emplace_back(0);//todo remove blockdata
	pm->planeLoc.emplace_back(x%2);//todo remove blockdata
	pm->planeLoc.emplace_back(blocktype);
}

void Chunk::genMesh() {
	//todo care about memory leaks...
	pm->planeLoc.clear();
//	pm->planeLoc.emplace_back(0);
//	pm->planeLoc.emplace_back(0);
//	pm->planeLoc.emplace_back(0);
//	pm->planeLoc.emplace_back(0);
//	pm->planeLoc.emplace_back(0);
//	pm->planeLoc.emplace_back(1);
//	return;
	for (chunkBounds x=0;x<size;x++) {
		for (chunkBounds y=0;y<size;y++) {
			for (chunkBounds z=0;z<size;z++) {
				if (blockNotAir(x, y, z)) addCubeToMesh(block[x][y][z], x, y, z);
			}
		}
	}
}

bool Chunk::inChunk(int x, int y, int z) {return x >= 0 && y >= 0 && z >= 0 && x < size && y < size && z < size;}

bool Chunk::blockNotAir(int x, int y, int z) {return air != getBlock(x, y, z);}

blockType Chunk::getBlock(blockIndexType x, blockIndexType y, blockIndexType z) {
	if (x>=0&&y>=0&&z>=0&&x<size&&y<size&&z<size) {
		return block[x][y][z];
//		auto index = blockIndexTuple(x,y,z);
//		if (block.count(index)==1) return block.at(index);
	}
	return air;
}

blockLoc Chunk::blockLocTuple(blockIndexType x, blockIndexType y, blockIndexType z) {
	return std::make_tuple<>(x, y, z);
}
chunkLoc Chunk::chunkLocTuple(chunkIndexType x, chunkIndexType y,chunkIndexType z) {
	return std::make_tuple<>(x,y,z);
}

blockType* Chunk::getBlockRef(blockLoc loc) {
//	std::cout<<"BLOCK LOC "<<(int)std::get<0>(loc)<<(int)std::get<1>(loc)<<(int)std::get<2>(loc)<<std::endl;
	return &block[std::get<0>(loc)][std::get<1>(loc)][std::get<2>(loc)];
}

blockType Chunk::getBlock(blockLoc loc) {
	return block[std::get<0>(loc)][std::get<1>(loc)][std::get<2>(loc)];
}
