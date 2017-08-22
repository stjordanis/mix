#pragma once
#include <mixal/config.h>
#include <mixal/types.h>

#include <map>

namespace mixal {

class MIXAL_LIB_EXPORT Translator
{
public:
	using DefinedSymbols = std::map<Symbol, Word>;

public:
	Translator(const DefinedSymbols& symbols = {}, int current_address = 0);

	Word evaluate(const Text& text) const;
	Word evaluate(const Symbol& symbol) const;
	Word evaluate(const Number& n) const;
	Word evaluate(const BasicExpression& expr) const;
	Word evaluate(const Expression& expr) const;
	Word evaluate(const WValue& wvalue) const;

	FutureWord translate_MIX(
		Operation command,
		const Address& A, const Index& I, const Field& F,
		const Label& label = {});

	void translate_EQU(const WValue& value, const Label& label = {});
	void translate_ORIG(const WValue& address, const Label& label = {});
	AddressedWord translate_CON(const WValue& address, const Label& label = {});
	AddressedWord translate_ALF(const Text& text, const Label& label = {});
	void translate_END(const WValue& address, const Label& label = {});

	void set_current_address(int address);
	int current_address() const;

	void define_symbol(const Symbol& symbol, const Word& value);
	const Word& defined_symbol(const Symbol& symbol) const;

private:
	WordField evaluate_wvalue_field(const std::optional<Expression>& field_expr) const;
	
	void process_wvalue_token(const WValue::Token& token, Word& dest) const;
	Byte process_ALF_text_char(char ch) const;

	void define_label_if_valid(const Label& label, const Word& value);

	void increase_current_address();

private:
	int current_address_;
	DefinedSymbols defined_symbols_;
};

} // namespace mixal


