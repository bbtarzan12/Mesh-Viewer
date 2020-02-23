#include "BRDFMapCapture.h"



BRDFMapCapture::BRDFMapCapture()
	:Material
	(
		"Shaders/captureBRDFSpecularMap_vert.glsl",
		"Shaders/captureBRDFSpecularMap_frag.glsl",
		{},
		false
	)
{

}
