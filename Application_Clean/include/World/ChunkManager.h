#pragma once
#include <vector>
#include "World/Chunk.h"

#include <thread>
#include <mutex>
#include <algorithm>

/*
	Stores all the chunks
	Manages threaded chunk loading
	and assigning neighbouring chunks
*/

class ChunkManager {
public:
	// initialise the chunk arrays
	ChunkManager();

	// get reference to the chunk array
	std::vector<Chunk>* getChunkArray() { return &m_chunks; }

	// get the render distance width
	unsigned char* getChunksWidth() { return &m_chunksWidth; }

	// update loop, takes players chunk coordinates
	void update(int chunkPosX, int chunkPosY);

	// load the chunks, takes centre chunk coordinates
	void loadChunksAt(int chunkPosX, int chunkPosY);
private:
	// Render distance width
	unsigned char m_chunksWidth = 15;

	// vector of all chunks
	std::vector<Chunk> m_chunks;
	// vector of all available chunks to update
	std::vector<Chunk*> m_availableChunks;
	// vector of chunks that need to be loaded
	std::vector<Chunk*> m_updateChunks;

	// flag for id the load chunk function is running
	bool m_loadChunkRunning = false;
	// flag for if chunk data has been calculated and waiting
	// to be processed on the main thread
	bool m_waitProcessData = false;

	// mutex's to lock access to parts of code due to threading
	std::mutex m_loadChunkMutex;		// locks the load chunk function so it can't be called multiple times at once
	std::mutex m_accessUpdateChunks;	// locks access to the update chunk array
	std::mutex m_waitProcessDataMutex;	// locks access to the m_waitProcessData variable

	// store the current chunk position of the player
	int m_chunkPosX = 0;
	int m_chunkPosY = 0;
	// store the previous chunk position of the player
	int m_previousChunkPosX = 0;
	int m_previousChunkPosY = 0;

	// sorts the list of chunks to update based
	// on distance from player
	void sortChunks();
	// loads the nearest chunk to player
	void loadNearestChunk();

	// stores the current chunk being loaded
	Chunk* m_currentChunk = nullptr;

	// returns m_waitProcessData, access is locked by mutex
	bool getWaitProcessData();
	// processes the chunk data
	void processLoadedChunk();

	// finds the neighbouring chunks and fills the neighbour arrays
	void setChunkNeighbours(Chunk* chunk);
	
	// adds chunk to the update chunks vector
	void addUpdateChunk(Chunk* chunk);

	// remove and add chunks
	void removeChunk(Chunk* chunk);
	void addChunk(int x, int y);

	// gets chunk at coordinate
	Chunk* getChunk(int x, int y);
};