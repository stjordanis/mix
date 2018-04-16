#pragma once
#include <mixal_format/line_formatter.h>

#include <iosfwd>

namespace mixal_format {

struct FormatOptions
{
	bool mdk_compatible = false;
	bool make_all_uppercase = true;
	std::string title_comment;
};

void FormatStream(std::istream& in, std::ostream& out, const FormatOptions& options = {});

} // namespace mixal_format


