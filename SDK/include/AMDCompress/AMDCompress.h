//=====================================================================
// Copyright (c) 2007-2014	Advanced Micro Devices, Inc. All rights reserved.
// Copyright (c) 2004-2006	ATI Technologies Inc.
//
/// \author GPU Developer Tools
/// \author gputools.support@amd.com
/// \file AMDCompress.h
/// \version 2.00
/// \brief Declares the interface to the AMDCompress library.
//
//=====================================================================

#ifndef AMDCOMPRESS
#define AMDCOMPRESS

#include <Windows.h>

typedef unsigned long	AMD_TC_DWORD;         ///< A 32-bit integer format.
typedef unsigned short	AMD_TC_WORD;          ///< A 16-bit integer format.
typedef unsigned char	AMD_TC_BYTE;          ///< An 8-bit integer format.
typedef float			AMD_TC_FLOAT;         ///< A 32-bit float format.


#define AMD_COMPRESS_VERSION_MAJOR 2         ///< The major version number of this release.
#define AMD_COMPRESS_VERSION_MINOR 0         ///< The minor version number of this release.

#if defined(WIN32) || defined(_WIN64)
#   define AMD_TC_API __cdecl
#else
#   define AMD_TC_API
#endif

/// Texture format.
typedef enum
{
   AMD_TC_FORMAT_Unknown,                    ///< Undefined texture format.
   // Channel Component formats --------------------------------------------------------------------------------
   AMD_TC_FORMAT_ARGB_8888,                  ///< ARGB format with 8-bit fixed channels.
   AMD_TC_FORMAT_RGB_888,                    ///<    RGB format with 8-bit fixed channels.
   AMD_TC_FORMAT_RG_8,                       ///<    Two component format with 8-bit fixed channels.
   AMD_TC_FORMAT_R_8,                        ///<    Single component format with 8-bit fixed channels.
   AMD_TC_FORMAT_ARGB_2101010,               ///< ARGB format with 10-bit fixed channels for color & a 2-bit fixed channel for alpha.
   AMD_TC_FORMAT_ARGB_16,                    ///<    RGB format with 16-bit fixed channels.
   AMD_TC_FORMAT_RG_16,                      ///<    Two component format with 16-bit fixed channels.
   AMD_TC_FORMAT_R_16,                       ///<    Single component format with 16-bit fixed channels.
   AMD_TC_FORMAT_ARGB_16F,                   ///< ARGB format with 16-bit floating-point channels.
   AMD_TC_FORMAT_RG_16F,                     ///<    Two component format with 16-bit floating-point channels.
   AMD_TC_FORMAT_R_16F,                      ///<    Single component with 16-bit floating-point channels.
   AMD_TC_FORMAT_ARGB_32F,                   ///< ARGB format with 32-bit floating-point channels.
   AMD_TC_FORMAT_RGB_32F,		             ///<	 RGB format with 32-bit floating-point channels.
   AMD_TC_FORMAT_RG_32F,                     ///<    Two component format with 32-bit floating-point channels.
   AMD_TC_FORMAT_R_32F,                      ///<    Single component with 32-bit floating-point channels.
   // Compression formats -----------------------------------------------------------------------------------
   AMD_TC_FORMAT_ATI1N,                      ///< Single component compression format using the same technique as DXT5 alpha. Four bits per pixel.
   AMD_TC_FORMAT_ATI2N,                      ///<	 Two component compression format using the same technique as DXT5 alpha. Designed for compression object space normal maps. Eight bits per pixel.
   AMD_TC_FORMAT_ATI2N_XY,                   ///<    Two component compression format using the same technique as DXT5 alpha. The same as ATI2N but with the channels swizzled. Eight bits per pixel.
   AMD_TC_FORMAT_ATI2N_DXT5,                 ///<    ATI2N like format using DXT5. Intended for use on GPUs that do not natively support ATI2N. Eight bits per pixel.
   AMD_TC_FORMAT_ATC_RGB,                    ///< AMD_TC - a compressed RGB format.
   AMD_TC_FORMAT_ATC_RGBA_Explicit,          ///< AMD_TC - a compressed ARGB format with explicit alpha.
   AMD_TC_FORMAT_ATC_RGBA_Interpolated,      ///< AMD_TC - a compressed ARGB format with interpolated alpha.
   AMD_TC_FORMAT_BC1,                        ///< A four component opaque (or 1-bit alpha) compressed texture format for Microsoft DirectX10. Identical to DXT1.  Four bits per pixel.
   AMD_TC_FORMAT_BC2,                        ///< A four component compressed texture format with explicit alpha for Microsoft DirectX10. Identical to DXT3. Eight bits per pixel.
   AMD_TC_FORMAT_BC3,                        ///< A four component compressed texture format with interpolated alpha for Microsoft DirectX10. Identical to DXT5. Eight bits per pixel.
   AMD_TC_FORMAT_BC4,                        ///< A single component compressed texture format for Microsoft DirectX10. Identical to ATI1N. Four bits per pixel.
   AMD_TC_FORMAT_BC5,                        ///< A two component compressed texture format for Microsoft DirectX10. Identical to ATI2N. Eight bits per pixel.
   AMD_TC_FORMAT_BC6H,                       ///< BC6H compressed texture format 
   AMD_TC_FORMAT_BC7,                        ///< BC7  compressed texture format
   AMD_TC_FORMAT_DXT1,                       ///< An DXTC compressed texture matopaque (or 1-bit alpha). Four bits per pixel.
   AMD_TC_FORMAT_DXT3,                       ///<    DXTC compressed texture format with explicit alpha. Eight bits per pixel.
   AMD_TC_FORMAT_DXT5,                       ///<    DXTC compressed texture format with interpolated alpha. Eight bits per pixel.
   AMD_TC_FORMAT_DXT5_xGBR,                  ///<    DXT5 with the red component swizzled into the alpha channel. Eight bits per pixel.
   AMD_TC_FORMAT_DXT5_RxBG,                  ///<    swizzled DXT5 format with the green component swizzled into the alpha channel. Eight bits per pixel.
   AMD_TC_FORMAT_DXT5_RBxG,                  ///<    swizzled DXT5 format with the green component swizzled into the alpha channel & the blue component swizzled into the green channel. Eight bits per pixel.
   AMD_TC_FORMAT_DXT5_xRBG,                  ///<    swizzled DXT5 format with the green component swizzled into the alpha channel & the red component swizzled into the green channel. Eight bits per pixel.
   AMD_TC_FORMAT_DXT5_RGxB,                  ///<    swizzled DXT5 format with the blue component swizzled into the alpha channel. Eight bits per pixel.
   AMD_TC_FORMAT_DXT5_xGxR,                  ///<    two-component swizzled DXT5 format with the red component swizzled into the alpha channel & the green component in the green channel. Eight bits per pixel.
   AMD_TC_FORMAT_ETC_RGB,                    ///< ETC (Ericsson Texture Compression) 
   //--------------------------------------------------------------------------------------------------------
   AMD_TC_FORMAT_MAX = AMD_TC_FORMAT_ETC_RGB
} AMD_TC_FORMAT;

/// An enum selecting the speed vs. quality trade-off.
typedef enum
{
   AMD_TC_Speed_Normal,                      ///< Highest quality mode
   AMD_TC_Speed_Fast,                        ///< Slightly lower quality but much faster compression mode - DXTn & ATInN only
   AMD_TC_Speed_SuperFast,                   ///< Slightly lower quality but much, much faster compression mode - DXTn & ATInN only
} AMD_TC_Speed;

/// AMDCompress error codes
typedef enum
{
   AMD_TC_OK = 0,                            ///< Ok.
   AMD_TC_ABORTED,                           ///< The conversion was aborted.
   AMD_TC_ERR_INVALID_SOURCE_TEXTURE,        ///< The source texture is invalid.
   AMD_TC_ERR_INVALID_DEST_TEXTURE,          ///< The destination texture is invalid.
   AMD_TC_ERR_UNSUPPORTED_SOURCE_FORMAT,     ///< The source format is not a supported format.
   AMD_TC_ERR_UNSUPPORTED_DEST_FORMAT,       ///< The destination format is not a supported format.
   AMD_TC_ERR_SIZE_MISMATCH,                 ///< The source and destination texture sizes do not match.
   AMD_TC_ERR_UNABLE_TO_INIT_CODEC,          ///< AMDCompress was unable to initialize the codec needed for conversion.
   AMD_TC_ERR_GENERIC                        ///< An unknown error occurred.
} AMD_TC_ERROR;

#define AMD_MAX_CMDS		20
#define AMD_MAX_CMD_STR		32
#define AMD_MAX_CMD_PARAM	16

typedef struct
{
  TCHAR strCommand[AMD_MAX_CMD_STR];
  TCHAR strParameter[AMD_MAX_CMD_PARAM];
} AMD_CMD_SET;

/// Options for the compression.
/// Passing this structure is optional
typedef struct
{
   AMD_TC_DWORD		dwSize;					///< The size of this structure.
   BOOL				bUseChannelWeighting;	///< Use channel weightings. With swizzled formats the weighting applies to the data within the specified channel not the channel itself.
											///< channel weigthing is not implemented for BC6H and BC7
   double			fWeightingRed;			///<	The weighting of the Red or X Channel. 
   double			fWeightingGreen;		///<	The weighting of the Green or Y Channel. 
   double			fWeightingBlue;			///<	The weighting of the Blue or Z Channel. 
   BOOL				bUseAdaptiveWeighting;	///<	Adapt weighting on a per-block basis. 
   BOOL				bDXT1UseAlpha;          ///< Encode single-bit alpha data. Only valid when compressing to DXT1 & BC1.
   AMD_TC_BYTE		nAlphaThreshold;        ///< The alpha threshold to use when compressing to DXT1 & BC1 with bDXT1UseAlpha. Texels with an alpha value less than the threshold are treated as transparent.
   BOOL				bDisableMultiThreading;	///< Disable multi-threading of the compression. This will slow the compression but can be useful if you're managing threads in your application.
											///< if set BC7 dwnumThreads will default to 1 during encoding and then return back to its original value when done.
   AMD_TC_Speed		nCompressionSpeed;		///< The trade-off between compression speed & quality.
   											///< this value is ignored for BC6H and BC7 (for BC7 the compression speed depends on fquaility value)  
   AMD_TC_DWORD		dwnumThreads;			///< Number of threads to initialize for BC7 encoding (Max up to 128). Default set to 8, 
   double			fquality;				///< Quality of encoding for BC7. This value ranges between 0.0 and 1.0.
											///< setting fquality above 0.0 gives the fastest, lowest quality encoding, 1.0 is the slowest, highest quality encoding. Default set to a low value of 0.05
   BOOL				brestrictColour;		///< This setting is a quality tuning setting for BC7 which may be necessary for convenience in some applications. Default set to false
											///< if  set and the block does not need alpha it instructs the code not to use modes that have combined colour + alpha - this
											///< avoids the possibility that the encoder might choose an alpha other than 1.0 (due to parity) and cause something to
											///< become accidentally slightly transparent (it's possible that when encoding 3-component texture applications will assume that
											///< the 4th component can safely be assumed to be 1.0 all the time.)
   BOOL				brestrictAlpha;			///< This setting is a quality tuning setting for BC7 which may be necessary for some textures. Default set to false,
											///< if set it will also apply restriction to blocks with alpha to avoid issues with punch-through or thresholded alpha encoding
   AMD_TC_DWORD		dwmodeMask;				///< Mode to set BC7 to encode blocks using any of 8 different block modes in order to obtain the highest quality. Default set to 0xCF, (Skips Color components with separate alpha component)
											///< You can combine the bits to test for which modes produce the best image quality. 
											///< The mode that produces the best image quality above a set quality level (fquality) is used and subsequent modes set in the mask 
											///< are not tested, this optimizes the performance of the compression versus the required quality. 
											///< If you prefer to check all modes regardless of the quality then set the fquality to a value of 0
   int				NumCmds;			    ///< Count of the number of command value pairs in CmdSet[].  Max value that can be set is AMD_MAX_CMDS = 20 on this release
   AMD_CMD_SET		CmdSet[AMD_MAX_CMDS];	///< Extended command options that can be set for the specified codec  			
											///< Example to set the number of threads and quality used for compression 
											///<		AMD_TC_CompressOptions Options; 
											///<		memset(Options,0,sizeof(AMD_TC_CompressOptions));
											///<		Options.dwSize = sizeof(AMD_TC_CompressOptions)
											///<		Options.CmdSet[0].strCommand   = "NumThreads"
											///<		Options.CmdSet[0].strParameter = "8";
											///<		Options.CmdSet[1].strCommand   = "Quality"
											///<		Options.CmdSet[1].strParameter = "1.0";
											///<		Options.NumCmds = 2;
} AMD_TC_CompressOptions;
//#endif // !AMDCOMPRESS_INTERNAL_BUILD

/// The structure describing a texture.
typedef struct
{
	AMD_TC_DWORD	dwSize;					///< Size of this structure.
	AMD_TC_DWORD	dwWidth;				///< Width of the texture.
	AMD_TC_DWORD	dwHeight;				///< Height of the texture.
	AMD_TC_DWORD	dwPitch;				///< Distance to start of next line - necessary only for uncompressed textures.
	AMD_TC_FORMAT	format;					///< Format of the texture.
	AMD_TC_DWORD	dwDataSize;				///< Size of the allocated texture data.
	AMD_TC_BYTE*	pData;					///< Pointer to the texture data
} AMD_TC_Texture;

#define MINIMUM_WEIGHT_VALUE 0.01f

//=================================================================================
// Number of image components
#define BC_COMPONENT_COUNT 4

// Number of bytes in a BC7 Block
#define BC_BLOCK_BYTES    (4*4)

// Number of pixels in a BC7 block
#define BC_BLOCK_PIXELS   BC_BLOCK_BYTES

// This defines the ordering in which components should be packed into
// the block for encoding
typedef enum _BC_COMPONENT
{
    BC_COMP_RED   = 0,
    BC_COMP_GREEN = 1,
    BC_COMP_BLUE =  2,
    BC_COMP_ALPHA = 3
} BC_COMPONENT;

typedef enum _BC_ERROR
{
	BC_ERROR_NONE,
	BC_ERROR_LIBRARY_NOT_INITIALIZED,
	BC_ERROR_LIBRARY_ALREADY_INITIALIZED,
	BC_ERROR_INVALID_PARAMETERS,
	BC_ERROR_OUT_OF_MEMORY,
} BC_ERROR;


class BC7BlockEncoder;
class BC6HBlockEncoder;

//=================================================================================

#ifdef __cplusplus
extern "C" {
#endif

    //=================================================================================
	//
	// InitializeBCLibrary() - Startup the BC6H or BC7 library
	//
	// Must be called before any other library methods are valid
	//
	BC_ERROR AMD_TC_InitializeBCLibrary();
	
	//
	// ShutdownBCLibrary - Shutdown the BC6H or BC7 library
	//
	BC_ERROR AMD_TC_ShutdownBCLibrary();


	//
	// AMD_TC_CreateBC6HEncoder() - Creates an encoder object with the specified quality and settings for BC6H codec
	// AMD_TC_CreateBC7Encoder()  - Creates an encoder object with the specified quality and settings for BC7  codec
	//
	// Library must be initialized before calling this function.
	//
	// Arguments :
	//		
	//      quality       - Quality of encoding. This value ranges between 0.0 and 1.0. (Valid only for BC7 in this release) default is 0.01
	//                      0.0 gives the fastest, lowest quality encoding, 1.0 is the slowest, highest quality encoding
	//                      In general even quality level 0.0 will give very good results on the vast majority of images
	//                      Higher quality settings may be needed for some difficult images (e.g. normal maps) to give good results
	//                      Encoding time will increase significantly at high quality levels. Quality levels around 0.8 will
	//                      give very close to the highest possible quality, increasing the level above this will cause large
	//                      increases in encoding time for very marginal gains in quality
	//
	//      performance   - Perfromance of encoding. This value ranges between 0.0 and 1.0. (Valid only for BC7 in this release) Typical default is 1.0
	//                      Encoding time can be reduced by incresing this value for a given Quality level. Lower values will improve overall quality with
	//						optimal setting been performed at a value of 0.
	//
	//      restrictColor - (for BC7) This setting is a quality tuning setting which may be necessary for convenience in some applications.
	//                      BC7 can be used for encoding data with up to four-components (e.g. ARGB), but the output of a BC7 decoder
	//						is effectively always 4-components, even if the original input contained less
	//                      If BC7 is used to encode three-component data (e.g. RGB) then the encoder generally assumes that it doesn't matter what
	//                      ends up in the 4th component of the data, however some applications might be written in such a way that they
	//                      expect the 4th component to always be 1.0 (this might, for example, allow mixing of textures with and without
	//                      alpha channels without special handling). In this example case the default behaviour of the encoder might cause some
	//                      unexpected results, as the alpha channel is not guaranteed to always contain exactly 1.0 (since some error may be distributed
	//                      into the 4th channel)
	//                      If the restrictColor flag is set then for any input blocks where the 4th component is always 1.0 (255) the encoder will
	//                      restrict itself to using encodings where the reconstructed 4th component is also always guaranteed to contain 1.0 (255)
	//                      This may cause a very slight loss in overall quality measured in absolute RMS error, but this will generally be negligible
	//
	//      restrictAlpha - (for BC7) This setting is a quality tuning setting which may be necessary for some textures. Some textures may need alpha values
	//                      of 1.0 and 0.0 to be exactly represented, but some BC7 block modes distribute error between the colour and alpha
	//                      channels (because they have a shared least significant bit in the encoding). This could result in the alpha values
	//                      being pulled away from zero or one by the global minimization of the error. If this flag is specified then the encoder
	//                      will restrict its behaviour so that for blocks which contain an alpha of zero or one then these values should be
	//                      precisely represented
	//                      
	//      modeMask      - This is an advanced option. (Valid only for BC7 in this release) 
	//                      BC7 can encode blocks using any of 8 different block modes in order to obtain the highest quality (for reference of how each
	//                      of these block modes work consult the BC7 specification)
	//                      Under some circumstances it is possible that it might be desired to manipulate the encoder to only produce certain modes
	//                      Using this setting it is possible to instruct the encoder to only use certain block modes.
	//                      This input is a bitmask of permitted modes for the encoder to use - for normal operation it should be set to 0xFF (all modes valid)
	//                      The bitmask is arranged such that a setting of 0x1 only allows the encoder to use block mode 0.
	//                      0x80 would only permit the use of block mode 7
	//                      Restricting the available modes will generally reduce quality, but will also increase encoding speed
	//
	//      encoder       - Address of a pointer to an encoder.
	//                      This function will allocate a BC7BlockEncoder or BC6HBlockEncoder object using new
	//
	//		isSigned	  - For BC6H this flag sets the bit layout, false = UF16 (unsigned float) and true = SF16 (signed float)
	//
	// Note: For BC6H quality and modeMask are reserved for future release
	// 
	BC_ERROR AMD_TC_CreateBC6HEncoder( bool isSigned, double quality, AMD_TC_DWORD modeMask, BC6HBlockEncoder** encoder );
	BC_ERROR AMD_TC_CreateBC7Encoder ( double quality, BOOL restrictColour, BOOL restrictAlpha,  AMD_TC_DWORD modeMask, double performance, BC7BlockEncoder** encoder );

	// AMD_TC_EncodeBC7Block()  - Enqueue a single BC7  block to the library for encoding
	// AMD_TC_EncodeBC6HBlock() - Enqueue a single BC6H block to the library for encoding
	//
	// For BC7: 
	// Input is expected to be a single 16 element block containing 4 components in the range 0.->255.
	// Pixel data in the block should be arranged in row-major order
	// For three-component input images the 4th component (BC7_COMP_ALPHA) should be set to 255 for
	// all pixels to ensure optimal encoding
	//
	// For BC6H: 
	// Input is expected to be a single 16 element block containing 4 components in Half-Float format (16bit).
	// Pixel data in the block should be arranged in row-major order.
	// the 4th component should be set to 0, since Alpha is not supported in BC6H 
	//
	BC_ERROR AMD_TC_EncodeBC7Block( BC7BlockEncoder* encoder,   double in[BC_BLOCK_PIXELS][BC_COMPONENT_COUNT], BYTE* out );
	BC_ERROR AMD_TC_EncodeBC6HBlock( BC6HBlockEncoder* encoder, AMD_TC_FLOAT  in[BC_BLOCK_PIXELS][BC_COMPONENT_COUNT], BYTE* out );

	//
	// AMD_TC_DecodeBC6HBlock() - Decode a BC6H block to an uncompressed output
	// AMD_TC_DecodeBC7Block()  - Decode a BC7 block to an uncompressed output
	//
	// This function takes a pointer to an encoded BC block as input, decodes it and writes out the result
	//
	//
	BC_ERROR AMD_TC_DecodeBC6HBlock( BYTE* in, AMD_TC_FLOAT  out[BC_BLOCK_PIXELS][BC_COMPONENT_COUNT] );
	BC_ERROR AMD_TC_DecodeBC7Block ( BYTE* in, double out[BC_BLOCK_PIXELS][BC_COMPONENT_COUNT] );

	//
	// AMD_TC_DestroyBC6HEncoder() - Deletes a previously allocated encoder object
	// AMD_TC_DestroyBC7Encoder()  - Deletes a previously allocated encoder object
	//
	//
	BC_ERROR AMD_TC_DestroyBC6HEncoder( BC6HBlockEncoder* encoder );
	BC_ERROR AMD_TC_DestroyBC7Encoder ( BC7BlockEncoder*  encoder );

   //=================================================================================

   /// AMD_TC_Feedback_Proc
   /// Feedback function for conversion.
   /// \param[in] fProgress The percentage progress of the texture compression.
   /// \param[in] pUser1 User data as passed to AMD_TC_ConvertTexture.
   /// \param[in] pUser2 User data as passed to AMD_TC_ConvertTexture.
   /// \return non-NULL(true) value to abort conversion
   typedef bool (AMD_TC_API * AMD_TC_Feedback_Proc)(float fProgress, DWORD_PTR pUser1, DWORD_PTR pUser2);

   /// Calculates the required buffer size for the specified texture
   /// \param[in] pTexture A pointer to the texture.
   /// \return    The size of the buffer required to hold the texture data.
   AMD_TC_DWORD AMD_TC_API AMD_TC_CalculateBufferSize(const AMD_TC_Texture* pTexture);

   /// Converts the source texture to the destination texture
   /// This can be compression, decompression or converting between two uncompressed formats.
   /// \param[in] pSourceTexture A pointer to the source texture.
   /// \param[in] pDestTexture A pointer to the destination texture.
   /// \param[in] pOptions A pointer to the compression options - can be NULL.
   /// \param[in] pFeedbackProc A pointer to the feedback function - can be NULL.
   /// \param[in] pUser1 User data to pass to the feedback function.
   /// \param[in] pUser2 User data to pass to the feedback function.
   /// \return    AMD_TC_OK if successful, otherwise the error code.
   AMD_TC_ERROR AMD_TC_API AMD_TC_ConvertTexture(const AMD_TC_Texture* pSourceTexture, AMD_TC_Texture* pDestTexture,
                                                 const AMD_TC_CompressOptions* pOptions,
                                                 AMD_TC_Feedback_Proc pFeedbackProc, DWORD_PTR pUser1, DWORD_PTR pUser2);

#ifdef __cplusplus
};
#endif

#endif // !AMDCOMPRESS
