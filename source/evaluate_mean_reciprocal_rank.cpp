/*
	EVALUATE_MEAN_RECIPROCAL_RANK.CPP
	---------------------------------
	Copyright (c) 2019 Andrew Trotman
	Released under the 2-clause BSD license (See:https://en.wikipedia.org/wiki/BSD_licenses)
*/
#include <cmath>

#include "asserts.h"
#include "unittest_data.h"
#include "evaluate_mean_reciprocal_rank.h"

namespace JASS
	{
	/*
		EVALUATE_MEAN_RECIPROCAL_RANK::COMPUTE()
		----------------------------------------
	*/
	double evaluate_mean_reciprocal_rank::compute(const std::string &query_id, const std::vector<std::string> &results_list, size_t depth)
		{
		size_t which = 1;

		for (const auto &result : results_list)
			{
			auto assessment = assessments.find(query_id, result);

			if (assessment.score != 0)
				return 1.0 / static_cast<double>(which);

			which++;

			if (which > depth)
				break;
			}

		return 0;
		}

	/*
		EVALUATE_MEAN_RECIPROCAL_RANK::UNITTEST()
		-----------------------------------------
	*/
	void evaluate_mean_reciprocal_rank::unittest(void)
		{
		/*
			Example results list with one relevant document
		*/
		std::vector<std::string> results_list =
			{
			"AP880217-0026",
			"AP880216-0139",			// RELEVANT (all others are not).
			"AP880212-0161",
			"AP880216-0169",
			"AP880217-0030",
			};

		/*
			One of the 5 documents is relevant
		*/
		double true_precision = 1.0 / 2.0;

		/*
			Load the sample data
		*/
		evaluate container;
		std::string copy = unittest_data::five_trec_assessments;
		container.decode_assessments_trec_qrels(copy);

		/*
			Evaluate the results list
		*/
		evaluate_mean_reciprocal_rank calculator(container);
		double calculated_precision = calculator.compute("1", results_list);

		/*
			Compare to 5 decimal places
		*/
		JASS_assert(std::round(calculated_precision * 10000) == std::round(true_precision * 10000));

		puts("evaluate_mean_reciprocal_rank::PASSED");
		}
	}
