#include <mixal/address_parser.h>
#include <mixal/parse_exceptions.h>

using namespace mixal;

void AddressParser::parse(std::string_view /*str*/)
{
	throw NotImplemented{};
}

std::string_view AddressParser::str() const
{
	throw NotImplemented{};
}
