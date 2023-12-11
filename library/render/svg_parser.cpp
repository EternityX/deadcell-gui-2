#include "svg_parser.h"

#include "../../core/interfaces/interfaces.h"
#include "../../library/console/console.h"
#include "../../library/security/xorstring.h"

#include <d3d11.h>
#include <cstdint>

std::string CSvgParser::parse(const char *file_path) {
    void *file = interfaces::file_system->open(file_path, xs("r"), nullptr);
    if (!file) {
        return {};
    }

    std::vector<char> buf(interfaces::file_system->size(file));
    interfaces::file_system->read(file, buf.data(), buf.size());
    interfaces::file_system->close(file);

    std::string_view sv(buf.data(), buf.size());

    // find opening and ending svg delimiter
    std::string_view start_delimiter(xs("<svg"));
    std::string_view end_delimiter(xs("</svg>"));

    const auto start_delim_pos = sv.find(start_delimiter);
    const auto end_delim_pos = sv.find(end_delimiter, start_delim_pos);
    const auto svg_data = sv.substr(start_delim_pos, end_delim_pos - start_delim_pos + end_delimiter.size());

    // LOG("{}", svg_data);

    return std::string(svg_data);
}
