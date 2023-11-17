#include "World/ChunkManager.h"

ChunkManager::ChunkManager() {
	// resize the chunks array
	m_chunks.resize(m_chunksWidth * m_chunksWidth);

	// create another vector of pointers to all the chunks available for use
	for (auto& chunk : m_chunks) {
		m_availableChunks.push_back(&chunk);
	}
};

void ChunkManager::update(int chunkPosX, int chunkPosY) {
	// test if chunk data is waiting to be processed on main thread
	if (getWaitProcessData()) {
		processLoadedChunk();
	}

	// get the player chunk position
	m_chunkPosX = chunkPosX;
	m_chunkPosY = chunkPosY;

	// check if the player has moved into another chunk
	if (m_chunkPosX != m_previousChunkPosX || m_chunkPosY != m_previousChunkPosY) {
		// update the chunks
		loadChunksAt(m_chunkPosX, m_chunkPosY);
	}

	// set the previous chunk position
	m_previousChunkPosX = m_chunkPosX;
	m_previousChunkPosY = m_chunkPosY;
}

// called on the main thread when the player moves into another chunk
// sets chunks outside render distance to invisible + updates position
void ChunkManager::loadChunksAt(int chunkPosX, int chunkPosY) {
	// lock for the update chunk vector
	std::unique_lock<std::mutex> update_lock(m_accessUpdateChunks);

	// find the min and max coordinates of chunks around player
	int maxChunkX = chunkPosX + ((m_chunksWidth - 1) / 2);
	int minChunkX = chunkPosX - ((m_chunksWidth - 1) / 2);
	int maxChunkY = chunkPosY + ((m_chunksWidth - 1) / 2);
	int minChunkY = chunkPosY - ((m_chunksWidth - 1) / 2);

	//-------------Remove chunks outside render  distance-----------
	// loop over the chunks
	for (auto& chunk : m_chunks) {
		// even if chunk is in queue to be rendered remove it (if player flies fast with big render distance it would break)

		// get coordinates of chunk
		int x = chunk.getPosX();
		int y = chunk.getPosY();

		// if chunk is out of range remove it
		if (x < minChunkX || x > maxChunkX || y < minChunkY || y > maxChunkY) {
			removeChunk(&chunk);
		}
	}
	//---------------------------------------------------------------

	//--------------------------Add chunks---------------------------
	// loop over all chunk coordinates around player
	for (int x = 0; x < m_chunksWidth; x++)
	{
		for (int y = 0; y < m_chunksWidth; y++)
		{
			// get coordinates of chunk
			int chunkX = chunkPosX + (x - (m_chunksWidth / 2));
			int chunkY = chunkPosY + (y - (m_chunksWidth / 2));
			bool chunkExists = false;

			// check if chunk already exists with these coordinates
			for (auto& chunk : m_chunks) {
				// if chunk has not been assigned a new position yet ignore
				if (!chunk.getAssignedPos()) continue;
				// if chunk exists set the chunk exists flag to true
				if (chunk.getPosX() == chunkX && chunk.getPosY() == chunkY) {
					chunkExists = true;
					break;
				}
			}

			// create a new chunk if chunk doesn't exist at that position
			if (!chunkExists) addChunk(chunkX, chunkY);
		}
	}
	//---------------------------------------------------------------

	//----------------------Add chunk neighbours---------------------
	// create temp copy of update chunks as set chunk neighbours updates the update chunk vector
	std::vector<Chunk*> tmpUpdateChunks = m_updateChunks;
	for (auto& chunk : tmpUpdateChunks) {
		setChunkNeighbours(chunk);
	}
	//---------------------------------------------------------------

	// calculate new order for array of chunks in order of distance to player
	sortChunks();

	// call the function to load the nearest chunk on another thread if it's not already running
	if (!m_loadChunkRunning) {
		// set loadChunkRunning flag to true
		m_loadChunkRunning = true;

		// execute the loadNearestChunk function on another thread
		std::thread thread(&ChunkManager::loadNearestChunk, this);
		thread.detach();
	}
}

void ChunkManager::loadNearestChunk() {
	// lock the load function from running if its already running
	std::unique_lock<std::mutex> load_lock(m_loadChunkMutex);

	// set the nearest chunk to player to current chunk - lock access to sorted chunks
	{
		std::unique_lock<std::mutex> update_lock(m_accessUpdateChunks);
		m_currentChunk = m_updateChunks[0];

		// unlock vector during computational heavy code
	}

	// if the block hasn't generated its blocks yet generate the blocks
	if (!m_currentChunk->getBlocksGenerated()) m_currentChunk->generateChunkBlockData();

	// if the neighbours haven't generated their blocks yet generate their blocks
	for (unsigned char i = 0; i < 4; i++)
	{
		// get neighbour direction
		Direction dir = static_cast<Direction>(i);

		// get reference to neighbour
		Chunk* neighbour = m_currentChunk->getNeighbour(dir);
		
		// if neighbour doesn't exist skip
		if (neighbour == nullptr) continue;

		// if neighbour's blocks havent been generated yet generate them
		if (!neighbour->getBlocksGenerated()) neighbour->generateChunkBlockData();
	}

	// generate the vertex data
	m_currentChunk->generateChunkVertexData();

	// lock acess to updated chunks array again
	std::unique_lock<std::mutex> update_lock(m_accessUpdateChunks);
	// as vector may have changed on main thread since accessing first element, have to
	// find the value in the vector to remove it
	// find the iterator to the value
	auto updateChunkIterator = std::find(m_updateChunks.begin(), m_updateChunks.end(), m_currentChunk);
	// if value exists remove it
	if (updateChunkIterator != m_updateChunks.end()) { m_updateChunks.erase(updateChunkIterator); }

	// set wait for data to be processed to true - lock access to this variable
	std::unique_lock<std::mutex> data_lock(m_waitProcessDataMutex);
	m_waitProcessData = true;
}

void ChunkManager::sortChunks() {
	// if there's no chunks to sort return
	if (m_updateChunks.empty() || m_updateChunks.size() <= 0) return;

	// calculate the distance for each chunk from the player
	for (auto& chunk : m_updateChunks) {
		chunk->setDistanceFromPlayer(m_chunkPosX, m_chunkPosY);
	}

	// sort the update chunks array based on distance from player
	std::sort(m_updateChunks.begin(), m_updateChunks.end(), [](const Chunk* a, const Chunk* b) {
		return a->getDistanceFromPlayer() < b->getDistanceFromPlayer();
		});
}

// when chunk has been loaded on another thread
// process the chunk data and copy into main thread
void ChunkManager::processLoadedChunk() {
	// update the chunk buffers
	m_currentChunk->updateBuffers();

	// update chunk properties
	m_currentChunk->updateThreadToRenderProperties();
	m_currentChunk->setVisible(true);

	// reset the wait for data flag
	m_waitProcessData = false;

	// if there are more chunks to load call the next load function here
	if (!m_updateChunks.empty() && m_updateChunks.size() > 0) {
		std::thread thread(&ChunkManager::loadNearestChunk, this);
		thread.detach();
	}
	else {
		// if theres no more chunks to load set the loadChunkRunning flag to false
		m_loadChunkRunning = false;
	}
}

// find the neighbours of a chunk and pair them
void ChunkManager::setChunkNeighbours(Chunk* chunk) {
	// get chunk position
	int x = chunk->getPosX();
	int y = chunk->getPosY();

	// create array of the neighbour chunks
	Chunk* neighbours[4] = {
		getChunk(x - 1, y),
		getChunk(x + 1, y),
		getChunk(x, y - 1),
		getChunk(x, y + 1)
	};

	// loop over neighbours and set the neighbour in chunk
	for (unsigned char i = 0; i < 4; i++)
	{
		// get neighbour direction
		Direction dir = static_cast<Direction>(i);

		// if chunk already has neighbour assigned in direction skip
		if (chunk->getNeighbour(dir) != nullptr) continue;

		// if neighbour doesn't exist continue loop
		if (neighbours[i] == nullptr) continue;

		// set the neighbour in current chunk
		chunk->setNeighbour(neighbours[i], dir);

		// set current chunk as neighbour in neighbour chunk
		switch (dir)
		{
		case Left:
			neighbours[i]->setNeighbour(chunk, Right);
			break;
		case Right:
			neighbours[i]->setNeighbour(chunk, Left);
			break;
		case Back:
			neighbours[i]->setNeighbour(chunk, Front);
			break;
		case Front:
			neighbours[i]->setNeighbour(chunk, Back);
			break;
		default:
			break;
		}

		// add neighbour to update chunks
		addUpdateChunk(neighbours[i]);
	}
}

// get chunk at position
Chunk* ChunkManager::getChunk(int x, int y) {
	// loop over the chunk array
	for (auto& chunk : m_chunks) {
		// find chunk at position
		if (chunk.getPosX() != x) continue;
		if (chunk.getPosY() != y) continue;

		return &chunk;
	}

	// if no chunk return nullptr
	return nullptr;
}

// add chunk to the vector of chunks that need calculating
void ChunkManager::addUpdateChunk(Chunk* chunk) {
	// check if the chunk is already in update list
	for (auto& update_chunk : m_updateChunks) {
		// if chunk already exists return
		if (update_chunk == chunk) return;
	}

	// add the chunk to update list
	m_updateChunks.push_back(chunk);
}

void ChunkManager::removeChunk(Chunk* chunk) {
	// if chunk doesn't exist return
	if (chunk == nullptr) return;

	// find neighbour chunks and set there neighbour to nullptr
	for (char i = 0; i < 4; i++)
	{
		Direction dir = static_cast<Direction>(i);

		// get neighbour
		Chunk* neighbour = chunk->getNeighbour(dir);

		// if neighbour doesn't exist continue
		if (neighbour == nullptr) continue;

		// set current chunk as neighbour in neighbour chunk
		switch (dir)
		{
		case Left:
			neighbour->setNeighbour(nullptr, Right);
			break;
		case Right:
			neighbour->setNeighbour(nullptr, Left);
			break;
		case Back:
			neighbour->setNeighbour(nullptr, Front);
			break;
		case Front:
			neighbour->setNeighbour(nullptr, Back);
			break;
		default:
			break;
		}

		// remove neighbour from current chunk
		chunk->setNeighbour(nullptr, dir);
	}

	// add chunk to available chunks
	m_availableChunks.push_back(chunk);
	
	// set chunk visibility, assigned position and blocks generated to false
	chunk->setVisible(false);
	chunk->setAssignedPos(false);
	chunk->setBlocksGenerated(false);
}

void ChunkManager::addChunk(int x, int y) {
	// check if there aren't any available chunks and return
	if (m_availableChunks.empty() || m_availableChunks.size() <= 0) return;

	Chunk* chunk = m_availableChunks[0];

	// set the position of the chunk
	chunk->setPos(x, y);
	chunk->setAssignedPos(true);

	// add chunk to update chunks
	addUpdateChunk(chunk);

	// remove the available chunk
	m_availableChunks.erase(m_availableChunks.begin());
}

// returns m_waitProcessData, access is locked by mutex
bool ChunkManager::getWaitProcessData() {
	std::unique_lock<std::mutex> data_lock(m_waitProcessDataMutex);
	return m_waitProcessData;
}