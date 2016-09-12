#pragma once
#include <vector>
#include <string>

namespace algorithm_lib
{
	class longest_palindromic_substring_t
	{
	public:
		static std::vector<size_t> max_lengths(const std::string& input)
		{
			if (input.empty())
				return std::vector<size_t>();

			auto input_with_boundaries = add_boundaries(input);
			std::vector<size_t> radius(input_with_boundaries.size());
			size_t reference_boundary = 2;
			size_t reference_center = 1;
			radius[0] = 0;
			radius[1] = 1;
			for (size_t k = 2; k < input_with_boundaries.size(); ++k)
			{
				size_t mirror_radius = radius[reference_center * 2 - k];
				if (k + mirror_radius < reference_boundary)
				{
					radius[k] = mirror_radius;
				}
				else
				{
					reference_center = k;
					size_t r = std::max(reference_boundary, k) - k;
					while (reference_center > r &&
						reference_center + r < input_with_boundaries.size() - 1 &&
						input_with_boundaries[reference_center - r - 1] == input_with_boundaries[reference_center + r + 1])
						++r;
					radius[k] = r;
					reference_boundary = k + r;
				}
			}
			reference_boundary = input_with_boundaries.size() - 1;
			std::vector<size_t> output(input.size());
			for (size_t k = input_with_boundaries.size() - 2; k != 0; --k)
			{
				size_t new_boundary = k - radius[k];
				while (reference_boundary > new_boundary)
				{
					reference_boundary -= 2;
					output[reference_boundary / 2] = k - reference_boundary;
				}
			}
			return output;
		}

	private:
		static std::string add_boundaries(const std::string& input)
		{
			std::string output;
			for (auto c : input)
			{
				output.push_back('|');
				output.push_back(c);
			}
			output += '|';
			return output;
		}
	};
}