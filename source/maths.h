/*
	MATHS.H
	-------
	Copyright (c) 2016 Andrew Trotman
	Released under the 2-clause BSD license (See:https://en.wikipedia.org/wiki/BSD_licenses)
*/
/*!
	@file
	@brief Basic maths functions.
	@details Some of these already exist in the C++ Standard Library or the C Runtime Library, but they are re-implemented here for portabilit and consistency reasons.
	@author Andrew Trotman
	@copyright 2016 Andrew Trotman
*/
#pragma once

#include <limits>

#include <stdio.h>
#include <stdint.h>

#include "asserts.h"

namespace JASS
	{
	/*
		CLASS MATHS
		-----------
	*/
	/*!
		@brief Basic maths functions.
	*/
	class maths
		{
		private:
			static const uint8_t ceiling_log2_answer[];
			static const uint8_t floor_log2_answer[];

		private:
			/*
				MATHS()
				-------
			*/
			/*!
				@brief Private constructor.
				@details A private constructor makes it impossable to instantiate this class - done because all methods are static.
			*/
			maths()
				{
				/*
					Nothing
				*/
				}
			
		public:
			/*
				MATHS::MAX()
				------------
			*/
			/*!
				@brief Return the maximum of the two parameters.
				@details This method is order-preserving - that is, if a == b then a is returned as the max.
				@param first [in] First of the two.
				@param second [in] Second of the two.
				@return The largest of the two (as compated using operator>=())
			*/
			template <typename TYPE>
			static const TYPE &max(const TYPE &first, const TYPE &second)
				{
				return first >= second ? first : second;
				}
			
			/*
				MATHS::MAX()
				------------
			*/
			/*!
				@brief Return the maximum of the three parameters.
				@details This method is order-preserving (if a == b == c then a is returned, etc.).
				@param first [in] First of the three.
				@param second [in] Second of the three.
				@param third [in] Third of the three.
			*/
			template <typename TYPE>
			static const TYPE &max(const TYPE &first, const TYPE &second, const TYPE &third)
				{
				return max(max(first, second), third);
				}

			/*
				MATHS::MIN()
				------------
			*/
			/*!
				@brief Return the minimum of the two parameters.
				@details This method is order-preserving - that is, if a == b then a is returned.
				@param first [in] First of the two.
				@param second [in] Second of the two.
				@return The smallest of the two (as compated using operator<=())
			*/
			template <typename TYPE>
			static const TYPE &min(const TYPE &first, const TYPE &second)
				{
				return first <= second ? first : second;
				}
				
			/*
				MATHS::MIN()
				------------
				Return the minimum of three integers (order preserving)
			*/
			/*!
				@brief Return the minimum of the three parameters.
				@details This method is order-preserving (if a == b == c then a is returned, etc.).
				@param first [in] First of the three.
				@param second [in] Second of the three.
				@param third [in] Third of the three.
				@return The smallest of the three (as compated using operator<=())
			*/
			template <typename TYPE>
			static const TYPE &min(const TYPE &first, const TYPE &second, const TYPE &third)
				{
				return min(min(first, second), third);
				}


			/*
				MATHS::FLOOR_LOG2()
				-------------------
			*/
			static size_t floor_log2(size_t x)
				{
				size_t sum, mult = 0;

				do
					{
					sum = floor_log2_answer[x & 0xFF] + mult;
					mult += 8;
					x >>= 8;
					}
				while (x != 0);

				return sum;
				}

			/*
				MATHS::CEILING_LOG2()
				---------------------
			*/
			static size_t ceiling_log2(size_t x)
				{
				size_t sum, mult = 0;

				do
					{
					sum = ceiling_log2_answer[x & 0xFF] + mult;
					mult += 8;
					x >>= 8;
					}
				while (x != 0);

				return sum;
				}


			/*
				MATHS::UNITTEST()
				-----------------
			*/
			/*!
				@brief Unit test this class
			*/
			static void unittest(void)
				{
				JASS_assert(max(2, 1) == 2);
				JASS_assert(max(1, 2) == 2);
				
				JASS_assert(max(1, 2, 3) == 3);
				JASS_assert(max(2, 1, 3) == 3);
				JASS_assert(max(1, 3, 2) == 3);
				JASS_assert(max(2, 3, 1) == 3);
				JASS_assert(max(3, 1, 2) == 3);
				JASS_assert(max(3, 2, 1) == 3);

				JASS_assert(min(2, 1) == 1);
				JASS_assert(min(1, 2) == 1);
				
				JASS_assert(min(1, 2, 3) == 1);
				JASS_assert(min(2, 1, 3) == 1);
				JASS_assert(min(1, 3, 2) == 1);
				JASS_assert(min(2, 3, 1) == 1);
				JASS_assert(min(3, 1, 2) == 1);
				JASS_assert(min(3, 2, 1) == 1);

				JASS_assert(floor_log2(10) == 3);
				JASS_assert(ceiling_log2(10) == 3);

				puts("maths::PASSED");
				}
		};
	}


