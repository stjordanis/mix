#pragma once
#include <mixal_parse/expression_parser.h>
#include <mixal_parse/field_parser.h>

namespace mixal_parse {

class MIXAL_PARSE_LIB_EXPORT WValueParser final :
	public ParserBase
{
public:
	const WValue& value() const;

private:
	virtual std::size_t do_parse_stream(std::string_view str, std::size_t offset) override;
	virtual void do_clear() override;

	std::size_t parse_expr_with_field(std::string_view str, std::size_t offset);
	void add_token(ExpressionParser&& expr_parser, FieldParser&& field_parser);

private:
	WValue value_;
};

} // namespace mixal_parse



