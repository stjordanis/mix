#include <mixal_parse/literal_parser.h>
#include <mixal_parse/w_value_parser.h>

#include <core/string.h>

using namespace mixal_parse;

std::size_t LiteralParser::do_parse_stream(std::string_view str, std::size_t offset)
{
	const auto first_char_pos = ExpectFirstNonWhiteSpaceChar('=', str, offset);
	if (first_char_pos == str.size())
	{
		return InvalidStreamPosition();
	}

	WValueParser wvalue_parser;
	const auto expr_end = wvalue_parser.parse_stream(str, first_char_pos + 1);
	if (IsInvalidStreamPosition(expr_end))
	{
		return InvalidStreamPosition();
	}

	const auto first_char_after_expr = ExpectFirstNonWhiteSpaceChar('=', str, expr_end);
	if (first_char_after_expr == str.size())
	{
		return InvalidStreamPosition();
	}

	const auto expression_length = (first_char_after_expr - first_char_pos - 1);
	if (expression_length > k_max_symbol_length)
	{
		return InvalidStreamPosition();
	}

	value_ = wvalue_parser.value();

	return (first_char_after_expr + 1);
}

void LiteralParser::do_clear()
{
	value_ = {};
}

const WValue& LiteralParser::value() const
{
	return value_;
}

