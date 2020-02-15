#include "TextureLoader.h"
#include <cassert>

#include <FreeImage/FreeImage.h>

void TextureLoader::Load(const std::string& filePath, GLuint& id, glm::ivec2& size)
{
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filePath.c_str());

	FIBITMAP* temp = FreeImage_Load(fif, filePath.c_str());
	assert(temp);

	FIBITMAP* image = FreeImage_ConvertTo24Bits(temp);
	assert(image);

	FreeImage_Unload(temp);

	size.x = FreeImage_GetWidth(image);
	size.y = FreeImage_GetHeight(image);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_BGR, GL_UNSIGNED_BYTE, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	FreeImage_Unload(image);


}
