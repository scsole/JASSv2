/*
	JASSV1_TO_HUMAN.CPP
	-------------------
	Copyright (c) 2016 Andrew Trotman
	Released under the 2-clause BSD license (See:https://en.wikipedia.org/wiki/BSD_licenses)
*/
/*!
	@brief Dump a human-readable version of a JASS v1 index to standard out.
*/
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>

#include <iostream>

#include "file.h"
#include "commandline.h"
#include "deserialised_jass_v1.h"

/*
	PARAMETERS
	----------
*/
bool parameter_look_like_atire = false;
bool parameter_help = false;
bool parameter_dictionary_only = false;

std::string parameters_errors;						///< Any errors as a result of command line parsing
auto parameters = std::make_tuple					///< The  command line parameter block
	(
	JASS::commandline::parameter("-?", "--help", "Print this help.", parameter_help),
	JASS::commandline::parameter("-A", "--ATIRE", "Make the output look as like 'atire_dictionary -p -q -e \"~\"')", parameter_look_like_atire),
	JASS::commandline::parameter("-d", "--dictionary", "Only print the dictionary, don't print the postings", parameter_dictionary_only)
	);

/*
	CLASS PRINTER
	-------------
*/
/*!
	@brief Implementation of add_rsv() that prints an individual posting
*/
class printer
	{
	private:
		uint64_t impact;			///< The impact score to use when push_back() is called.

	public:
		/*
			PRINTER::SET_SCORE()
			--------------------
		*/
		/*!
			@brief remember the impact score for when printing via push_back().
			@param impact [in] the impact score to rememnber.
		*/
		void set_score(uint64_t impact)
			{
			this->impact = impact;
			}

			/*
				PRINTER::PUSH_BACK()
				--------------------
			*/
			/*!
				@brief Print a bunch of <docid, impact> scores
				@param document_ids [in] The document IDs that the impact should be added to.
			*/
			void push_back(__m256i document_ids)
				{
				uint32_t each[8];
				__m256i *into = (__m256i *)each;
				_mm256_storeu_si256(into, document_ids);

				for (size_t which = 0; which < 8; which++)
					if (each[which] != 0)
						std::cout << '<' << each[which] << ',' << impact << '>';
				}

		/*
			PRINTER::ADD_RSV()
			------------------
		*/
		/*!
			@brief print the posting
			@param document [in] The document identifier (docid) used internally.
			@param impact [in] The impact score of this term in this document.
		*/
		void add_rsv(uint64_t document, uint64_t impact)
			{
			std::cout << '<' << document << ',' << impact << '>';
			}
	} ;

/*
	WALK_INDEX
	----------
*/
/*!
	@brief Walk the index, term by term, and print each posting from each postings list.
	@param index [in] Reference to a JASS v1 deserialised index object.
	@param decompressor [in] reference to an object that can decompress a postings list segment.
*/
void walk_index(JASS::deserialised_jass_v1 &index, JASS::compress_integer &decompressor)
	{
	printer out_stream;

	/*
		Walk each term
	*/
	for (const auto &term : index)
		{
		std::cout << term.term;

		if (!parameter_dictionary_only)
			{
			std::cout << ' ';
			/*
				Walk each segment
			*/
			for (uint64_t current_segment = 0; current_segment < term.impacts; current_segment++)
				{
				uint64_t *postings = (uint64_t *)term.offset + current_segment;
				const JASS::deserialised_jass_v1::segment_header &header = *reinterpret_cast<const JASS::deserialised_jass_v1::segment_header *>(index.postings() + *postings);

				decompressor.set_impact(header.impact);
				decompressor.decode_with_writer(out_stream, header.segment_frequency, index.postings() + header.offset, header.end - header.offset);
				}
			}
		std::cout << '\n';
		}
	}

/*
	USAGE()
	-------
*/
/*!
	@brief Print the usage line
*/
uint8_t usage(const std::string &exename)
	{
	std::cout << JASS::commandline::usage(exename, parameters) << "\n";
	return 1;
	}

/*
	MAIN()
	------
*/
/*!
	@brief Dump a human-readable version of a JASS v1 index to standard out.
*/
int main(int argc, const char *argv[])
	{
	/*
		Parse the commane line parameters
	*/
	auto success = JASS::commandline::parse(argc, argv, parameters, parameters_errors);
	if (!success)
		{
		std::cout << parameters_errors;
		exit(1);
		}
	if (parameter_help)
		exit(usage(argv[0]));

	/*
		Open and read the index
	*/
	JASS::deserialised_jass_v1 index(false);
	index.read_index();

	/*
		Get the encoding scheme and the d-ness of the index
	*/
	std::string codex_name;
	int32_t d_ness;
	JASS::compress_integer &decompressor = index.codex(codex_name, d_ness);

	if (!parameter_look_like_atire)
		{
		if (parameter_dictionary_only)
			std::cout << "\nDICTIONARY\n----------\n";
		else
			std::cout << "\nPOSTINGS LISTS\n-------------\n";
		}

	/*
		Print the postings lists
	*/
	walk_index(index, decompressor);

	/*
		Print the primary key list
	*/
	if (!parameter_look_like_atire && !parameter_dictionary_only)
		{
		std::cout << "\nPRIMARY KEY LIST\n----------------\n";
		for (const auto &key : index.primary_keys())
			std::cout << key << '\n';
		}

	return 0;
	}
