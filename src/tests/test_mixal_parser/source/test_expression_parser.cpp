#include <mixal/expression_parser.h>
#include <mixal/parse_exceptions.h>

#include <gtest/gtest.h>

using namespace mixal;

namespace {

ExpressionToken Token(
	std::optional<UnaryOperation> unary_op,
	BasicExpression basic_expr,
	std::optional<BinaryOperation> binary_op)
{
	return {unary_op, basic_expr, binary_op};
}

ExpressionToken Token(
	BasicExpression basic_expr)
{
	return Token({}, basic_expr, {});
}

ExpressionToken Token(
	BasicExpression basic_expr,
	std::optional<BinaryOperation> binary_op)
{
	return {{}, basic_expr, binary_op};
}

std::string MakeLongestSymbol()
{
	return std::string(k_max_symbol_length, 'A');
}

class ExpressionParserTest :
	public ::testing::Test
{
protected:
	void parse(std::string_view s)
	{
		const auto pos = parser_.parse_stream(s);
		ASSERT_NE(s.npos, pos);
		rest_of_parsed_stream_ = s.substr(pos);
	}

	void parse_error(std::string_view s)
	{
		const auto pos = parser_.parse_stream(s);
		ASSERT_EQ(s.npos, pos);
	}

	void reminder_stream_is(std::string_view rest_of_stream)
	{
		ASSERT_EQ(rest_of_stream, rest_of_parsed_stream_);
	}

	template<typename... T>
	void tokens_are(T... tokens)
	{
		std::vector<ExpressionToken> expected_tokens{tokens...};
		ASSERT_EQ(expected_tokens, parser_.expression().tokens);
	}

protected:
	ExpressionParser parser_;
	std::string_view rest_of_parsed_stream_;
};

} // namespace

TEST_F(ExpressionParserTest, Expression_Can_Contain_Only_Number)
{
	parse(" 42  ");
	tokens_are(Token("42"));
	reminder_stream_is("  ");
}

TEST_F(ExpressionParserTest, Expression_Can_Contain_Only_Symbol)
{
	parse("LABEL");
	tokens_are(Token("LABEL"));
	reminder_stream_is("");
}

TEST_F(ExpressionParserTest, Expression_Can_Contain_Only_Current_Address_Counter)
{
	parse(" *");
	tokens_are(Token("*"));
	reminder_stream_is("");
}

TEST_F(ExpressionParserTest, Expression_Can_Contain_Basic_Expression_With_Unary_Op_In_The_Beginning)
{
	parse(" -*");
	tokens_are(Token("-", "*", {}));
	reminder_stream_is("");
}

TEST_F(ExpressionParserTest, Expression_With_Only_Unary_Op_Will_Fail)
{
	parse_error("+");
}

TEST_F(ExpressionParserTest, Differentiate_Current_Address_Symbol_From_Multiply_Binary_Operation)
{
	parse("***");
	tokens_are(Token({}, "*", "*"), Token("*"));
	reminder_stream_is("");
}

TEST_F(ExpressionParserTest, Full_Token_Is_Unary_Op_With_Basic_Expression_And_Binary_Op)
{
	parse("+ * - 3");
	tokens_are(Token("+", "*", "-"), Token("3"));
	reminder_stream_is("");
}

TEST_F(ExpressionParserTest, Parses_All_Token_To_The_Vector_With_Left_To_Right_Order)
{
	//         1 |2|   3|4|
	parse("  -1 + 5*20 / 6  ");

	tokens_are(
		Token("-", "1", "+"),
		Token("5", "*"),
		Token("20", "/"),
		Token("6"));

	reminder_stream_is("  ");
}

TEST_F(ExpressionParserTest, Parses_Mix_Field_Specification_As_Binary_Op)
{
	parse("1:3");

	tokens_are(
		Token("1", ":"),
		Token("3"));

	reminder_stream_is("");
}

TEST_F(ExpressionParserTest, Parses_Special_MIXAL_Binary_Operations)
{
	parse("1 // 3");

	tokens_are(
		Token("1", "//"),
		Token("3"));
	
	reminder_stream_is("");
}

TEST_F(ExpressionParserTest, Parses_As_Much_As_Possible)
{
	auto symbol = MakeLongestSymbol();
	auto too_long_symbol = symbol + symbol;
	
	parse(too_long_symbol);

	tokens_are(Token(symbol));

	reminder_stream_is(symbol);
}
