#pragma once

#if defined(_MSC_VER) && defined(__clang__)
#  pragma clang diagnostic push
// Comes from <regex>, -Wno-sign-compare on command line does not help
// 
// comparison of integers of different signs
#  pragma clang diagnostic ignored "-Wsign-compare"
#endif

#include <cxxopts.hpp>

#if defined(_MSC_VER) && defined(__clang__)
#  pragma clang diagnostic pop
#endif

#include <string>

struct Options
{
	cxxopts::Options raw_options;
	bool execute{false};
	bool hide_details{false};
	bool show_help{false};
	std::string file_name;

	bool mdk_stream{false};

	Options(cxxopts::Options options)
		: raw_options{std::move(options)}
	{
	}
};

inline cxxopts::Options CreateOptions()
{
	cxxopts::Options options{"mixal", "MIX interpreter/emulator"};
	options.add_options()
		("h,help",			"Show this help and exit")
		("x,hide-details",	"Hide additional information during translating")
		("e,execute",		"Compile and execute file")
		("f,file",			"File that contains MIXAL code", cxxopts::value<std::string>())
		("m,mdk",			"Interpret <file> as file with GNU MIX Development Kit (MDK) format");
		return options;
}

inline Options ParseOptions(int argc, char* argv[])
{
	Options parsed{CreateOptions()};
	auto options = parsed.raw_options.parse(argc, argv);

	parsed.execute = (options.count("execute") > 0);
	parsed.hide_details = (options.count("hide-details") > 0);
	parsed.show_help = (options.count("help") > 0);
	parsed.mdk_stream = (options.count("mdk") > 0);
	const auto& file_name_option = options["file"];

	if (file_name_option.count() > 0)
	{
		parsed.file_name = file_name_option.as<std::string>();
		//std::ios_base::openmode mode = std::ios_base::in;
		//if (parsed.mdk_stream)
		//{
		//	mode |= std::ios_base::binary;
		//}
		//std::ifstream file_input{, mode};
		//if (file_input)
		//{
		//	parsed.input_file = std::move(file_input);
		//}
	}
	return parsed;
}

