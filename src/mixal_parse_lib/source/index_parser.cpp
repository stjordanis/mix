#include <mixal_parse/index_parser.h>
#include <mixal_parse/expression_parser.h>
#include <mixal_parse/parsers_utils.h>

#include <core/string.h>

using namespace mixal_parse;

std::size_t IndexParser::do_parse_stream(std::string_view str, std::size_t offset)
{
	const auto index_begin_pos = ExpectFirstNonWhiteSpaceChar(',', str, offset);
	if (index_begin_pos == str.size())
	{
		// Empty
		return offset;
	}
	
	const auto expr_end = try_parse_expression(str, index_begin_pos + 1);
	if (IsInvalidStreamPosition(expr_end))
	{
		return InvalidStreamPosition();
	}

	return expr_end;
}

std::size_t IndexParser::try_parse_expression(const std::string_view& str, std::size_t offset)
{
	ExpressionParser parser;
	const auto end = parser.parse_stream(str, offset);
	if (!IsInvalidStreamPosition(end))
	{
		index_ = parser.expression();
	}
	
	return end;
}

void IndexParser::do_clear()
{
	index_ = {};
}

const Index& IndexParser::index() const
{
	return index_;
}
