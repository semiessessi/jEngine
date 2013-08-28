// define constants for opengl 2.0/extention functionality
// for gl2helper class by Semi Essessi

// multisample stuff
#define GL_MULTISAMPLE										0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE							0x809E
#define GL_SAMPLE_ALPHA_TO_ONE								0x809F
#define GL_SAMPLE_COVERAGE									0x80A0

#define GL_MULTISAMPLE_BIT									0x20000000

#define GL_SAMPLE_BUFFERS									0x80A8
#define GL_SAMPLES											0x80A9
#define GL_SAMPLE_COVERAGE_VALUE							0x80AA
#define GL_SAMPLE_COVERAGE_INVERT							0x80AB

#define WGL_SAMPLE_BUFFERS									0x2041
#define WGL_SAMPLES											0x2042

#define WGL_NUMBER_PIXEL_FORMATS							0x2000

#define WGL_NO_ACCELERATION									0x2025
#define WGL_GENERIC_ACCELERATION							0x2026
#define WGL_FULL_ACCELERATION								0x2027

#define WGL_SWAP_EXCHANGE									0x2028
#define WGL_SWAP_COPY										0x2029
#define WGL_SWAP_UNDEFINED									0x202A

#define WGL_TYPE_RGBA										0x202B
#define WGL_TYPE_COLORINDEX									0x202C

#define WGL_DRAW_TO_WINDOW									0x2001
#define WGL_DRAW_TO_BITMAP									0x2002
#define WGL_ACCELERATION									0x2003
#define WGL_NEED_PALETTE									0x2004
#define WGL_NEED_SYSTEM_PALETTE								0x2005
#define WGL_SWAP_LAYER_BUFFERS								0x2006
#define WGL_SWAP_METHOD										0x2007
#define WGL_NUMBER_OVERLAYS									0x2008
#define WGL_NUMBER_UNDERLAYS								0x2009
#define WGL_TRANSPARENT										0x200A
#define WGL_TRANSPARENT_VALUE								0x200B
#define WGL_SHARE_DEPTH										0x200C
#define WGL_SHARE_STENCIL									0x200D
#define WGL_SHARE_ACCUM										0x200E
#define WGL_SUPPORT_GDI										0x200F
#define WGL_SUPPORT_OPENGL									0x2010
#define WGL_DOUBLE_BUFFER									0x2011
#define WGL_STEREO											0x2012
#define WGL_PIXEL_TYPE										0x2013
#define WGL_COLOR_BITS										0x2014
#define WGL_RED_BITS										0x2015
#define WGL_RED_SHIFT										0x2016
#define WGL_GREEN_BITS										0x2017
#define WGL_GREEN_SHIFT										0x2018
#define WGL_BLUE_BITS										0x2019
#define WGL_BLUE_SHIFT										0x201A
#define WGL_ALPHA_BITS										0x201B
#define WGL_ALPHA_SHIFT										0x201C
#define WGL_ACCUM_BITS										0x201D
#define WGL_ACCUM_RED_BITS									0x201E
#define WGL_ACCUM_GREEN_BITS								0x201F
#define WGL_ACCUM_BLUE_BITS									0x2020
#define WGL_ACCUM_ALPHA_BITS								0x2021
#define WGL_DEPTH_BITS										0x2022
#define WGL_STENCIL_BITS									0x2023
#define WGL_AUX_BUFFERS										0x2024

// floating point texture formats
#define GL_FLOAT_R_NV									   0x8880
#define GL_FLOAT_RG_NV									  0x8881
#define GL_FLOAT_RGB_NV									 0x8882
#define GL_FLOAT_RGBA_NV									0x8883
#define GL_FLOAT_R16_NV									 0x8884
#define GL_FLOAT_R32_NV									 0x8885
#define GL_FLOAT_RG16_NV									0x8886
#define GL_FLOAT_RG32_NV									0x8887
#define GL_FLOAT_RGB16_NV								   0x8888
#define GL_FLOAT_RGB32_NV								   0x8889
#define GL_FLOAT_RGBA16_NV								  0x888A
#define GL_FLOAT_RGBA32_NV								  0x888B

// draw buffers (multiple render targets)
#define GL_MAX_DRAW_BUFFERS								 0x8824
#define GL_DRAW_BUFFER0									 0x8825
#define GL_DRAW_BUFFER1									 0x8826
#define GL_DRAW_BUFFER2									 0x8827
#define GL_DRAW_BUFFER3									 0x8828
#define GL_DRAW_BUFFER4									 0x8829
#define GL_DRAW_BUFFER5									 0x882A
#define GL_DRAW_BUFFER6									 0x882B
#define GL_DRAW_BUFFER7									 0x882C
#define GL_DRAW_BUFFER8									 0x882D
#define GL_DRAW_BUFFER9									 0x882E
#define GL_DRAW_BUFFER10									0x882F
#define GL_DRAW_BUFFER11									0x8830
#define GL_DRAW_BUFFER12									0x8831
#define GL_DRAW_BUFFER13									0x8832
#define GL_DRAW_BUFFER14									0x8833
#define GL_DRAW_BUFFER15									0x8834

// texture edge clamp extension...
#define GL_CLAMP_TO_EDGE									0x812F

// texture 3d
#define GL_PACK_SKIP_IMAGES								 0x806B
#define GL_PACK_IMAGE_HEIGHT								0x806C
#define GL_UNPACK_SKIP_IMAGES							   0x806D
#define GL_UNPACK_IMAGE_HEIGHT							  0x806E
#define GL_TEXTURE_3D									   0x806F
#define GL_PROXY_TEXTURE_3D								 0x8070
#define GL_TEXTURE_DEPTH									0x8071
#define GL_TEXTURE_WRAP_R								   0x8072
#define GL_MAX_3D_TEXTURE_SIZE							  0x8073

// texture lod bias
#define GL_TEXTURE_FILTER_CONTROL						   0x8500
#define GL_TEXTURE_LOD_BIAS								 0x8501
#define GL_MAX_TEXTURE_LOD_BIAS							 0x84FD

// convolution
#define GL_CONVOLUTION_1D								   0x8010
#define GL_CONVOLUTION_2D								   0x8011
#define GL_SEPARABLE_2D									 0x8012

#define GL_CONVOLUTION_BORDER_MODE						  0x8013

#define GL_CONVOLUTION_FILTER_SCALE						 0x8014
#define GL_CONVOLUTION_FILTER_BIAS						  0x8015

#define GL_REDUCE										   0x8016

#define GL_CONVOLUTION_FORMAT							   0x8017
#define GL_CONVOLUTION_WIDTH								0x8018
#define GL_CONVOLUTION_HEIGHT							   0x8019
#define GL_MAX_CONVOLUTION_WIDTH							0x801A
#define GL_MAX_CONVOLUTION_HEIGHT						   0x801B

#define GL_POST_CONVOLUTION_RED_SCALE					   0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE					 0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE					  0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE					 0x801F
#define GL_POST_CONVOLUTION_RED_BIAS						0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS					  0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS					   0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS					  0x8023

// annoying texture rectangle
#define GL_TEXTURE_RECTANGLE								0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE						0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE						  0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE					   0x84F8

// depth textures
#define GL_DEPTH_COMPONENT16								0x81A5
#define GL_DEPTH_COMPONENT24								0x81A6
#define GL_DEPTH_COMPONENT32								0x81A7
#define GL_TEXTURE_DEPTH_SIZE							   0x884A
#define GL_DEPTH_TEXTURE_MODE							   0x884B

// cube maps
#define GL_NORMAL_MAP									   0x8511
#define GL_REFLECTION_MAP								   0x8512
#define GL_TEXTURE_CUBE_MAP								 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP						 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X					  0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X					  0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y					  0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y					  0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z					  0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z					  0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP						   0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE						0x851C

// mip map generation
#define GL_GENERATE_MIPMAP								  0x8191
#define GL_GENERATE_MIPMAP_HINT							 0x8192

// point parameters
#define GL_POINT_SIZE_MIN								   0x8126
#define GL_POINT_SIZE_MAX								   0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE						0x8128
#define GL_POINT_DISTANCE_ATTENUATION					   0x8129

// point sprites
#define GL_POINT_SPRITE									 0x8861
#define GL_COORD_REPLACE									0x8862
// stupid gl2 only constant... not sure what to do with it
#define GL_POINT_SPRITE_COORD_ORIGIN						0x8CA0

// shader defines
#define GL_CURRENT_PROGRAM								  0x8B8D
#define GL_SHADER_TYPE									  0x8B4E
#define GL_DELETE_STATUS									0x8B80
#define GL_COMPILE_STATUS								   0x8B81
#define GL_LINK_STATUS									  0x8B82
#define GL_VALIDATE_STATUS								  0x8B83
#define GL_INFO_LOG_LENGTH								  0x8B84
#define GL_ATTACHED_SHADERS								 0x8B85
#define GL_ACTIVE_UNIFORMS								  0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH						0x8B87
#define GL_SHADER_SOURCE_LENGTH							 0x8B88
#define GL_VERTEX_SHADER									0x8B31
#define GL_ACTIVE_ATTRIBUTES								0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH					  0x8B8A
#define GL_FRAGMENT_SHADER								  0x8B30

#define GL_FLOAT_VEC2									   0x8B50
#define GL_FLOAT_VEC3									   0x8B51
#define GL_FLOAT_VEC4									   0x8B52
#define GL_INT_VEC2										 0x8B53
#define GL_INT_VEC3										 0x8B54
#define GL_INT_VEC4										 0x8B55
#define GL_BOOL											 0x8B56
#define GL_BOOL_VEC2										0x8B57
#define GL_BOOL_VEC3										0x8B58
#define GL_BOOL_VEC4										0x8B59
#define GL_FLOAT_MAT2									   0x8B5A
#define GL_FLOAT_MAT3									   0x8B5B
#define GL_FLOAT_MAT4									   0x8B5C
#define GL_SAMPLER_1D									   0x8B5D
#define GL_SAMPLER_2D									   0x8B5E
#define GL_SAMPLER_3D									   0x8B5F
#define GL_SAMPLER_CUBE									 0x8B60
#define GL_SAMPLER_1D_SHADOW								0x8B61
#define GL_SAMPLER_2D_SHADOW								0x8B62

#define GL_VERTEX_ATTRIB_ARRAY_ENABLED					  0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE						 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE					   0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE						 0x8625
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED				   0x886A
#define GL_CURRENT_VERTEX_ATTRIB							0x8626
#define GL_VERTEX_ATTRIB_ARRAY_POINTER					  0x8645
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING			   0x889F

#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT				  0x8B8B

#define GL_VERTEX_PROGRAM_POINT_SIZE						0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE						  0x8643

#define GL_SHADING_LANGUAGE_VERSION						 0x8B8C
#define GL_MAX_VERTEX_ATTRIBS							   0x8869
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS				  0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS					0x8B4A
#define GL_MAX_VARYING_FLOATS							   0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS				   0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS				 0x8B4D
#define GL_MAX_TEXTURE_COORDS							   0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS						  0x8872

// fbo defines
#define GL_FRAMEBUFFER									  0x8D40
#define GL_RENDERBUFFER									 0x8D41

#define GL_STENCIL_INDEX1								   0x8D46
#define GL_STENCIL_INDEX4								   0x8D47
#define GL_STENCIL_INDEX8								   0x8D48
#define GL_STENCIL_INDEX16								  0x8D49

#define GL_RENDERBUFFER_WIDTH							   0x8D42
#define GL_RENDERBUFFER_HEIGHT							  0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT					 0x8D44
#define GL_RENDERBUFFER_RED_SIZE							0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE						  0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE						   0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE						  0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE						  0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE						0x8D55

#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE			   0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME			   0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL			 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE	 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET		0x8CD4

#define GL_COLOR_ATTACHMENT0								0x8CE0
#define GL_COLOR_ATTACHMENT1								0x8CE1
#define GL_COLOR_ATTACHMENT2								0x8CE2
#define GL_COLOR_ATTACHMENT3								0x8CE3
#define GL_COLOR_ATTACHMENT4								0x8CE4
#define GL_COLOR_ATTACHMENT5								0x8CE5
#define GL_COLOR_ATTACHMENT6								0x8CE6
#define GL_COLOR_ATTACHMENT7								0x8CE7
#define GL_COLOR_ATTACHMENT8								0x8CE8
#define GL_COLOR_ATTACHMENT9								0x8CE9
#define GL_COLOR_ATTACHMENT10							   0x8CEA
#define GL_COLOR_ATTACHMENT11							   0x8CEB
#define GL_COLOR_ATTACHMENT12							   0x8CEC
#define GL_COLOR_ATTACHMENT13							   0x8CED
#define GL_COLOR_ATTACHMENT14							   0x8CEE
#define GL_COLOR_ATTACHMENT15							   0x8CEF
#define GL_DEPTH_ATTACHMENT								 0x8D00
#define GL_STENCIL_ATTACHMENT							   0x8D20

#define GL_FRAMEBUFFER_COMPLETE							 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT				0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT		0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT	  0x8CD8
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS				0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				   0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER			   0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER			   0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED						  0x8CDD

#define GL_FRAMEBUFFER_BINDING							  0x8CA6
#define GL_RENDERBUFFER_BINDING							 0x8CA7
#define GL_MAX_COLOR_ATTACHMENTS							0x8CDF
#define GL_MAX_RENDERBUFFER_SIZE							0x84E8

#define GL_INVALID_FRAMEBUFFER_OPERATION					0x0506

// multitexture defines
#define GL_ACTIVE_TEXTURE								   0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE							0x84E1
#define GL_MAX_TEXTURE_UNITS								0x84E2

#define GL_TEXTURE0										 0x84C0
#define GL_TEXTURE1										 0x84C1
#define GL_TEXTURE2										 0x84C2
#define GL_TEXTURE3										 0x84C3
#define GL_TEXTURE4										 0x84C4
#define GL_TEXTURE5										 0x84C5
#define GL_TEXTURE6										 0x84C6
#define GL_TEXTURE7										 0x84C7
