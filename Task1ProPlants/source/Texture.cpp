#include "..\includes\Texture.h"
#include "GraphicsThrowMacros.h"
#include <sstream>

#define TEX_THROW_FAILED(boolean) if(boolean) throw Texture::TexException( __LINE__,__FILE__, fileName)

Texture::Texture(Graphics & gfx, const char* fileName)
{
	HRESULT hr;
	int height, width;

	//Load targa image data into memory
	LoadTarga(fileName, width, height);

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
	srvd.Texture2D.MipLevels = 1;

	//Create the shader resource view for the texture
	GFX_THROW_FAILED(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvd, &_pTextureView));

	//Generate mipmaps for the texture
	GetContext(gfx)->GenerateMips(_pTextureView.Get());

	//release the targa data
	delete[] _pTargaData;
	_pTargaData = nullptr;
}

Texture::~Texture()
{
}

void Texture::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(0u, 1u, _pTextureView.GetAddressOf());
}

ID3D11ShaderResourceView* Texture::GetTextureView() const noexcept
{
	return _pTextureView.Get();
}

//Targa loader, need to be flipped before using as targas are stored opposide in the y to dds
//MUST have an alpha channel, 24bit targas will be rejected
void Texture::LoadTarga(const char* fileName, int& width, int& height)
{
	HRESULT hr;
	int bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	//Open the targa file for reading in binary
	TEX_THROW_FAILED(fopen_s(&filePtr, fileName, "rb") != 0);

	//Read in the file header
	TEX_THROW_FAILED((unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr) != 1);

	//Get the important info from the header
	width = static_cast<int>(targaFileHeader.width);
	height = static_cast<int>(targaFileHeader.height);
	bpp = static_cast<int>(targaFileHeader.bpp);

	//Check that its 32 bit and not 24
	TEX_THROW_FAILED(bpp != 32);

	//Calc the size of the 32bit image data
	imageSize = width * height * 4;

	//allocate memory for targa image
	targaImage = new unsigned char[imageSize];

	//Read in the targa image data
	TEX_THROW_FAILED((unsigned int)fread(targaImage, 1, imageSize, filePtr) != imageSize);

	//Close the file
	TEX_THROW_FAILED(fclose(filePtr) != 0);

	//Allocate memory for the targa destination data
	_pTargaData = new unsigned char[imageSize];

	//init the index into the targa destination data array
	index = 0;

	//Initialize the index into the targa image data
	k = (width * height * 4) - (width * 4);

	//Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down
	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			_pTargaData[index + 0] = targaImage[k + 2]; //Red
			_pTargaData[index + 1] = targaImage[k + 1]; //Green
			_pTargaData[index + 2] = targaImage[k + 0]; //Blue
			_pTargaData[index + 3] = targaImage[k + 3]; //Alpha

			//Increment the indexes into the targa data
			k += 4;
			index += 4;
		}
		//Decrement down to the next row as we reading from upside down
		k -= (width * 8);
	}

	//Release the loaded in data as its stored in our local array
	delete[] targaImage;
	targaImage = nullptr;
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
