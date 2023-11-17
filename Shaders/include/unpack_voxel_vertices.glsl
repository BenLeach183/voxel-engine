void unpackData() {
    // Extract the components using bit masking and shifting.
    uint subChunk = (aPackedData >> 28u) & 0xFu;   		// 4 bits
    uint xData = (aPackedData >> 23u) & 0x1Fu;  		// 5 bits
    uint yData = (aPackedData >> 18u) & 0x1Fu;  		// 5 bits
    uint zData = (aPackedData >> 13u) & 0x1Fu;  		// 5 bits
    uint normalData = (aPackedData >> 10u) & 0x7u;  	// 3 bits
	uint textureIndex = (aPackedData >> 2u) & 0xFFu;  	// 8 bits
	uint uvData = (aPackedData) & 0x3u;  				// 2 bits

	position.x = float(xData) - 0.5f;
	position.y = float(yData) + (float(subChunk) * 16.0) - 0.5f;
	position.z = float(zData) - 0.5f;

	switch(normalData) {
		case 0u:
			normal = vec3(-1.0f, 0.0f, 0.0f);
			break;
		case 1u:
			normal = vec3(1.0f, 0.0f, 0.0f);
			break;
		case 2u:
			normal = vec3(0.0f, 0.0f, -1.0f);
			break;
		case 3u:
			normal = vec3(0.0f, 0.0f, 1.0f);
			break;
		case 4u:
			normal = vec3(0.0f, 1.0f, 0.0f);
			break;
		case 5u:
			normal = vec3(0.0f, -1.0f, 0.0f);
			break;
		default:
			normal = vec3(1.0f, 1.0f, 1.0f);
			break;
	}
	
	float textureOffsetX = float(int(textureIndex)%16) * (1.0/16.0);
	float textureOffsetY = int((float(textureIndex)/16.0)) * (1.0/16.0);
	
	switch(uvData) {
    case 0u:
        uv = vec2(textureOffsetX, textureOffsetY);
        break;
    case 1u:
        uv = vec2(textureOffsetX + (1.0/16.0), textureOffsetY);
        break;
    case 2u:
        uv = vec2(textureOffsetX, textureOffsetY + (1.0/16.0));
        break;
    case 3u:
        uv = vec2(textureOffsetX + (1.0/16.0), textureOffsetY + (1.0/16.0));
        break;
    default:
        uv = vec2(0.0, 0.0);
        break;
	}
}