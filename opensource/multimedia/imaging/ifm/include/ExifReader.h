#ifndef _EXIFREADER_H_
#define _EXIFREADER_H_

typedef enum
{
	eExif_UNKNOW   = 0, // For not defined type
	eExif_BYTE     = 1,
	eExif_ASCII    = 2,
	eExif_SHORT    = 3,
	eExif_LONG     = 4,
	eExif_RATIONAL = 5,
	eExif_SBYTE    = 6,
	eExif_UNDEFINED= 7,
	eExif_SSHORT   = 8,
	eExif_SLONG    = 9,
	eExif_SRATIONAL= 10,
	eExif_FLOAT    = 11,
	eExif_DOUBLE   = 12,
	eExif_MAX      = 13,
}enumExifType;

typedef enum
{
/* +CR370700 */
    eExifTag_Make                    = 271,
    eExifTag_Model                   = 272,
    eExifTag_Orientation             = 274,
    eExifTag_XResolution             = 282,
    eExifTag_YResolution             = 283,
    eExifTag_ResolutionUnit          = 296,
    eExifTag_Software                = 305,
    eExifTag_YCbCrPositioning        = 531,
    eExifTag_ExposureTime            = 33434,
    eExifTag_FNumber                 = 33437,
    eExifTag_IdfExifPointer          = 34665, //From ifd0
    eExifTag_IdfGpsPointer           = 34853,
    eExifTag_ISOSpeedRatings         = 34855,
    eExifTag_ExifVersion             = 36864,
    eExifTag_DateTimeOriginal        = 36867,
    eExifTag_DateTimeDigitized       = 36868,
    eExifTag_ComponentsConfiguration = 37121,
    eExifTag_ShutterSpeedValue       = 37377,
    eExifTag_ApertureValue           = 37378,
    eExifTag_BrightnessValue         = 37379,
    eExifTag_SubjectDistance         = 37382,
    eExifTag_MeteringMode            = 37383,
    eExifTag_LightSource             = 37384,
    eExifTag_Flash                   = 37385,
    eExifTag_FocalLength             = 37386,
    eExifTag_SubjectArea             = 37396,
    eExifTag_MakerNote               = 37500,
    eExifTag_FlashpixVersion         = 40960,
    eExifTag_ColorSpace              = 40961,
    eExifTag_PixelXDimension         = 40962,
    eExifTag_PixelYDimension         = 40963,
    eExifTag_IdfInteroperabilityPointer  = 40965,
    eExifTag_FlashEnergy             = 41483,
    eExifTag_ExposureIndex           = 41493,
    eExifTag_CustomRendered          = 41985,
    eExifTag_ExposureMode            = 41986,
    eExifTag_WhiteBalance            = 41987,
    eExifTag_DigitalZoomRatio        = 41988,
    eExifTag_SceneCaptureType        = 41990,
    eExifTag_GainControl             = 41991,
    eExifTag_Contrast                = 41992
/* -CR370700 */
}enumExifTag;


typedef struct
{
	unsigned short   tag ;
	enumExifType     type;
	unsigned short   count;
	const char      *comment;
}ExifTagDecription;


typedef struct
{
	unsigned char  MarkerPrefix ;
	unsigned char  APP1;
	unsigned short LengthOfField;
	unsigned char  ExifIndentifierCoder[6];
	unsigned short TiffHeader_ByteOrder; //endianness
	unsigned short TiffHeader_filed_42;
	unsigned short TiffHeader_OffsetOfIFD_0;
	unsigned short TiffHeader_OffsetOfIFD_1;
}TExifAPP1Marker;

const ExifTagDecription *GetExifTagDecription(const unsigned short val, enumExifType type= eExif_UNKNOW);
const ExifTagDecription *GetExifTagDecription(const char *name, enumExifType type= eExif_UNKNOW);


//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************

union ExifPtrDataValue
{ // Union for managing all type of ptr for value access
	/*const*/ unsigned int   *uiValue;
	/*const*/ unsigned short *usValue;
	/*const*/ unsigned char  *ucValue;
	/*const*/ int            *iValue;
	/*const*/ short          *sValue;
	/*const*/ char           *cValue;
	/*const*/ double         *dValue;
	/*const*/ float          *fValue;
};

struct ExifTag
{
	unsigned short Tag;         // Bytes 0-1
	unsigned short Type;        // Bytes 2-3
	unsigned int   Count;       // Bytes 4-7
	unsigned int   Value_Offset;// Bytes 8-11
	int DisplayItem(const char *pExifBase) const;
	int GetDataAddress(const char *pExifBase, ExifPtrDataValue &DataAddr, size_t &size) const;
	size_t GetDataSize() const;
	unsigned int GetOffset() const;
	unsigned int GetOffset( size_t &size) const;


};

class ExifIfd_Manager
//*************************************************************************************************************
{
public:
	ExifIfd_Manager(char *ptr=NULL, char * exif_base=NULL);
	void SetBaseAddr( char *ptr, char * exif_base=NULL);
	unsigned short GetNbTag() const;
	const char *GetNextIfd() const;
	ExifTag *GetAt(unsigned short index) const;
	ExifTag *FindTag(unsigned short tag) const;
	unsigned int GetEnd() const;
	unsigned int GetStartOffset() const;


protected:
	char * m_BasePtr;
	char * m_ExifBase;
};

class ExifReader
//*************************************************************************************************************
{
public:
	ExifReader();
	int  AttachBuffer(char *pBuff, size_t size);
	size_t ComputeMaxOffset();

	bool IsLittleEndian() const;
	int  GetNbOfIfd() const;
	char *GetIfd0() const;
	char *GetIfd(unsigned int index) const;
	char *GetIfd_Exif() const;             //Return specific IDF Exif pointer from IDF0
	char *GetIfd_Gps() const;              //Return specific IDF GPS pointer from IDF0
	char *GetIfd_Interoperability() const; //Return specific IDF Interoperability pointer from IDF0

	char *GetExifBase() const;

	int DumpAll();
	//int DisplayItem(ExifTag &ExifItem);
	int DumpIFD(ExifIfd_Manager &Ifd, const char *header=NULL) const;

	ExifTag *FindTag(const char *name, char **pIfd=NULL) const;
	ExifTag *FindTag(unsigned short tag, char **pIfd=NULL) const;
	//ExifTag *FindTag(const char *name, enumExifType type= eExif_UNKNOW, char **ppIfd=NULL) const;

	int ModifyTag(unsigned short tag, unsigned short type, unsigned short count, const char *pNewData, ExifTag **ppReturnTag=NULL);

	enum
	{
		eExif_AppMarker_0= 0xFF,
		eExif_AppMarker_1= 0xE1,
		eHeader_byte_42  = 0x2A,

		eLittleEndian= 0x4949,
		eBigEndian   = 0x4D4D,

	};
	enum enumError
	{
		eError_HeaderIsNull= -2000,
		eError_NoData,
	};

protected:
	size_t           m_Size;
	size_t           m_UsedSize;
	size_t           m_AllocSize;
	TExifAPP1Marker *m_pExifMarker;
};


#endif // _EXIFREADER_H_
