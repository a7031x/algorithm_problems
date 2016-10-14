#pragma once
#include <vector>
#include <string>
#include <algorithm>

namespace algorithm_lib
{
	inline std::vector<size_t> suffix_array(const std::string& text)
	{
		std::vector<size_t> r(text.size() + 1);
		std::vector<size_t> rank(text.size() + 1);
		for (size_t k = 0; k < text.size(); ++k)
		{
			rank[k] = text[k];
			r[k] = k;
		}
		rank[text.size()] = 0;
		r[text.size()] = text.size();

		for (size_t k = 1; k <= text.size(); k *= 2)
		{
			auto comparer = [&](size_t index0, size_t index1)
			{
				if (rank[index0] < rank[index1])
					return true;
				else if (rank[index1] < rank[index0])
					return false;
				auto v0 = (index0 + k) < text.size() ? rank[index0 + k] : 0;
				auto v1 = (index1 + k) < text.size() ? rank[index1 + k] : 0;
				return v0 < v1;
			};
			std::sort(r.begin(), r.end(), comparer);
			std::vector<size_t> rank1(rank.size());
			rank1[r[0]] = 0;
			for (size_t i = 1; i <= text.size(); ++i)
				rank1[r[i]] = rank1[r[i - 1]] + comparer(r[i - 1], r[i]);
			rank = std::move(rank1);
		}

		return r;
	}

	std::vector<size_t> longest_common_prefix(const std::string& text, const std::vector<size_t>& sa) {
		std::vector<size_t> rank(text.size() + 1);
		std::vector<size_t> lcp(text.size());
		for (size_t k = 0; k <= text.size(); ++k)
			rank[sa[k]] = k;

		int common_length = 0;
		lcp[0] = 0;
		for (size_t i = 0; i < text.size(); ++i) {
			size_t j = sa[rank[i] - 1];
			if (common_length > 0) --common_length;
			for (; i + common_length < text.size() && j + common_length < text.size(); ++common_length)
				if (text[i + common_length] != text[j + common_length])
					break;
			lcp[rank[i] - 1] = common_length;
		}

		return lcp;
	}

}