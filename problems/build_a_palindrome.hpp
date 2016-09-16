#pragma once
#include <longest_palindromic_substring.hpp>
#include <suffix_tree.hpp>
#include <iostream>
#include <algorithm>

namespace build_a_palindrome
{
	inline std::string solve(const std::string& a, const std::string& b)
	{
		std::string rb(b.crbegin(), b.crend());
		std::string c = a + "#" + rb + "$";
		auto pa = algorithm_lib::longest_palindromic_substring_t::max_lengths(a);
		auto pb = algorithm_lib::longest_palindromic_substring_t::max_lengths(rb);
		pa.push_back(0);
		pb.push_back(0);
		algorithm_lib::suffix_tree_t tree(c);

		size_t max_length = 0;
		std::string r;

		tree.traverse([&](
			const std::string& path,
			size_t path_length,
			size_t start,
			size_t end,
			const std::vector<size_t>& children_starts)
		{
			size_t current_length = 0;
			std::string current_text;
			bool left = false;
			bool right = false;
			for (auto start : children_starts)
			{
				size_t palindrome_length;

				if (start <= a.size())
				{
					left = true;
					palindrome_length = pa[start];
				}
				else
				{
					right = true;
					palindrome_length = pb[start - a.size() - 1];
				}

				if (path_length * 2 + palindrome_length < std::max(current_length, r.size()))
					continue;

				auto full_path = tree.full_path(path);
				auto text = full_path
					+ c.substr(start, palindrome_length)
					+ std::string(full_path.crbegin(), full_path.crend());
				if (text.size() > current_length)
				{
					current_length = text.size();
					current_text = text;
				}
				else
				{
					current_text = std::min(current_text, text);
				}
			}

			if (false == left || false == right)
				return;

			if (current_length < max_length)
				return;

			if (current_length > max_length)
			{
				max_length = current_length;
				r = current_text;
			}
			else if (current_length == max_length)
			{
				r = std::min(r, current_text);
			}
		});

		return r.empty() ? "-1" : r;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			std::string a, b;
			std::cin >> a >> b;
			std::cout << solve(a, b) << std::endl;
		}
	}
}