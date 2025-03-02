#include "..\includes\Texture.h"
#include "GraphicsThrowMacros.h"
#include <sstream>

#define TEX_THROW_FAILED(boolean) if(boolean) throw Texture::TexException( __LINE__,__FILE__, fileName)

Texture::Texture(Graphics & gfx, const char* fileName)
{
	ChangeTextureView(gfx, fileName);
}

Texture::~Texture()
{
	//release the targa data
	if (_pTargaData != nullptr)
	{
		delete[] _pTargaData;
		_pTargaData = nullptr;
	}
}

void Texture::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(0u, 1u, _pTextureView.GetAddressOf());
}

ID3D11ShaderResourceView* Texture::GetTextureView() const noexcept
{
	return _pTextureView.Get();
}

void Texture::ChangeTextureView(Graphics& gfx, const char* fileName)
{
	HRESULT hr;
	int height, width;

	//Load targa image data into memory
	if (!LoadTarga(fileName, width, height))
	{
		return;
	}

	//Ensure the base pointer is freed
	_pTextureView = nullptr;

	//Creation of texture resource
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture = nullptr;
	D3D11_TEXTURE2D_DESC td = {};
	td.Height = height;
	td.Width = width;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = _pTargaData;
	sd.SysMemPitch = sizeof(unsigned char) * (width * 4);

	//Create the texture
	GFX_THROW_FAILED(GetDevice(gfx)->CreateTexture2D(&td, &sd, &pTexture));

	//Setup the shader resource view desc
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = -1;

	//Create the shader resource view for the texture
	GFX_THROW_FAILED(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvd, &_pTextureView));

	//release the targa data
	delete[] _pTargaData;
	_pTargaData = nullptr;
}

//Targa loader, need to be flipped before using as targas are stored opposide in the y to dds
//MUST have an alpha channel, 24bit targas will be rejected
bool Texture::LoadTarga(const char* fileName, int& width, int& height)
{
	int bpp, imageSize, index, targaIndex;
	FILE* filePtr;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	//Open the targa file for reading in binary
	if (fopen_s(&filePtr, fileName, "rb"))
	{
		return false;
	}

	//Read in the file header
	if ((unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr) != 1)
	{
		return false;
	}

	//Get the important info from the header
	width = static_cast<int>(targaFileHeader.width);
	height = static_cast<int>(targaFileHeader.height);
	bpp = static_cast<int>(targaFileHeader.bpp);

	//Check that its 32 bit and not 24
	if (bpp != 32)
	{
		return false;
	}

	//Calc the size of the 32bit image data, 4 channels for each pixel
	imageSize = width * height * 4;

	//allocate memory for targa image
	targaImage = new unsigned char[imageSize];

	//Read in the targa image data
	if ((unsigned int)fread(targaImage, 1, imageSize, filePtr) != imageSize)
	{
		return false;
	}

	//Close the file
	if (fclose(filePtr) != 0)
	{
		return false;
	}

	//Allocate memory for the targa destination data
	_pTargaData = new unsigned char[imageSize];

	//init the index into the targa destination data array
	index = 0;

	//Initialize the index into the targa image data. Go to the last row and shift to the start of that row
	targaIndex = (width * height * 4) - (width * 4) + (9*4); //9*4 cause fucked offset for some reason

	//Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			_pTargaData[index + 0] = targaImage[targaIndex + 2]; //Red
			_pTargaData[index + 1] = targaImage[targaIndex + 1]; //Green
			_pTargaData[index + 2] = targaImage[targaIndex + 0]; //Blue
			_pTargaData[index + 3] = targaImage[targaIndex + 3]; //Alpha

			//Increment the indexes into the targa data
			targaIndex += 4;
			index += 4;
		}
		//Decrement down to the next row as we reading from upside down
		targaIndex -= (width * 8);
	}

	//Release the loaded in data as its stored in our local array
	delete[] targaImage;
	targaImage = nullptr;
	return true;
}

//Texture exceptions
Texture::TexException::TexException(int line, const char* file, const char* texName) noexcept : ExceptionHandler(line, file), _fileName(texName)
{
}

const char* Texture::TexException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Description] " << "Error with loading targa" << std::endl
		<< "[Error File Name] " << _fileName << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Texture::TexException::GetType() const noexcept
{
	return "Targa Load Error";
}
