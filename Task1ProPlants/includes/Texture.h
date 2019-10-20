#pragma once
#include "Graphics.h"
#include "Bindable.h"
#include "ExceptionHandler.h"
#include <stdio.h>

class Texture : public Bindable
{
public:
	class TexException : public ExceptionHandler
	{
	public:
		TexException(int line, const char* file, const char* texName) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
	private:
		const char* _fileName;
	};
public:
	Texture(Graphics& gfx, const char* fileName);
	Texture() = delete;
	~Texture();

	void Bind(Graphics& gfx) noexcept override;

	ID3D11ShaderResourceView* GetTextureView() const noexcept;

private:
	//Targa file header structer
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	void LoadTarga(const char* fileName, int& width, int& height);

	unsigned char* _pTargaData = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pTextureView = nullptr;
};

