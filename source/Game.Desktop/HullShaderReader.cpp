#include "pch.h"
#include "HullShaderReader.h"
#include "Game.h"
#include "GameException.h"
#include "Utility.h"

namespace Library
{
	HullShaderReader::HullShaderReader(Game& game) :
		ContentTypeReader(game, HullShader::TypeIdClass())
	{
	}

	std::shared_ptr<HullShader> HullShaderReader::_Read(const std::wstring& assetName)
	{
		winrt::com_ptr<ID3D11HullShader> hullShader;
		Vector<char> compiledHullShader;
		File::LoadBinary(String::ToString(assetName), compiledHullShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateHullShader(&compiledHullShader[0], compiledHullShader.Size(), nullptr, hullShader.put()), "ID3D11Device::CreatedHullShader() failed.");
		
		return std::make_shared<HullShader>(std::move(hullShader));
	}
}