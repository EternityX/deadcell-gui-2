#pragma once

#include <d3d11.h>
#include <cstdint>
#include <vector>
#include <xstring>

class CSvgParser {
private:
    static std::vector<std::pair<uint32_t, ID3D11Texture2D *>> m_textures;

public:
    static std::string parse(const char *file_path);
};
