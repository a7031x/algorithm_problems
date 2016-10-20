#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

/*
assert("aaa", "a", 4, "aaa", "");
assert("ab", "ccd", 12, "ab", "cc");
assert("aa", "aaa", 3, "a", "a");
assert("ab", "cd", 5, "a", "cd");
assert("ab", "ccd", 9, "a", "d");
assert("", "", 1, "no", "solution");
assert("aab", "ccd", 2, "", "ccd");
assert("abc", "ab", 4, "a", "");
assert("jagsjdhgkj", "dfsdfjhghs", 1287, "s", "fs");
*/

namespace two_strings_game
{
	class suffix_automation_t
	{
		struct state_t
		{
			size_t length = 0;
			size_t link = -1;
			int grundy_value = -1;
			int64_t grundy_sum;
			std::map<char, size_t> next;
		};

		std::vector<state_t> _states;
		std::vector<state_t*> _sorted_states;
		size_t _last;

	public:
		suffix_automation_t()
		{
			state_t start;
			_states.push_back(start);
			_last = 0;
		}

		suffix_automation_t(const std::string& s) : suffix_automation_t()
		{
			extend(s);
			_sorted_states.resize(_states.size());
			for (size_t k = 0; k < _states.size(); ++k)
			{
				_sorted_states[k] = &_states[k];
			}

			std::sort(_sorted_states.begin(), _sorted_states.end(), [](const state_t* s0, const state_t* s1)
			{
				return s0->length > s1->length;
			});

			update_grundy_values();
		}

		void extend(const std::string& s)
		{
			for (auto c : s)
				extend_one(c);
		}

		void extend_one(char c)
		{
			size_t current = _states.size();
			_states.push_back(state_t());
			_states[current].length = _states[_last].length + 1;
			size_t parent = _last;
			for (; -1 != parent && 0 == _states[parent].next.count(c); parent = _states[parent].link)
				_states[parent].next[c] = current;

			if (-1 == parent)
				_states[current].link = 0;
			else
			{
				size_t next = _states[parent].next[c];
				if (_states[next].length - 1 == _states[parent].length)
					_states[current].link = next;
				else
				{
					size_t clone = _states.size();
					state_t state = _states[next];
					state.length = _states[parent].length + 1;
					_states.push_back(state);
					for (; -1 != parent && _states[parent].next[c] == next; parent = _states[parent].link)
						_states[parent].next[c] = clone;
					_states[next].link = _states[current].link = clone;
				}
			}
			_last = current;
		}

		std::string kth_substring(int64_t k)const
		{
			auto cache = all_substrings();
			std::string s;
			kth_substring(cache, 0, k, s);
			return s;
		}

		void update_grundy_values()
		{
			for (auto state : _sorted_states)
			{
				bool mark[27] = { false };
				for (auto& kv : state->next)
				{
					auto& next = _states[kv.second];
					mark[next.grundy_value] = true;
				}
				for (size_t grundy_value = 0; grundy_value < sizeof(mark); ++grundy_value)
					if (!mark[grundy_value])
					{
						state->grundy_value = grundy_value;
						break;
					}
			}
		}

		std::vector<int64_t> calculate_grundy_sums(int64_t grundy_value)
		{
			std::vector<int64_t> r(_states.size(), 0);
			for (auto state : _sorted_states)
			{
				size_t index = state - _states.data();
				for(auto& kv : state->next)
				{
					r[index] += r[kv.second];
				}
				if (state->grundy_value == grundy_value)
					++r[index];
			}
			return r;
		}

		std::vector<int64_t> root_grundy_sum()const
		{
			std::vector<int64_t> r(27, 0);
			auto grundy_sum = calculate_grundy_sums();
			for (auto& g : grundy_sum[0])
				r[g.first] = g.second;
			return r;
		}

		std::vector<std::map<int, int64_t>> calculate_grundy_sums()const
		{
			std::vector<std::map<int, int64_t>> grundy_sum(_states.size());
			for (auto state : _sorted_states)
			{
				size_t index = state - _states.data();
				for (auto& kv : state->next)
				{
					for (auto& g : grundy_sum[kv.second])
						grundy_sum[index][g.first] += g.second;
				}
				++grundy_sum[index][state->grundy_value];
			}
			return grundy_sum;
		}

		static std::pair<std::string, std::string> solve(const std::string& a, const std::string& b, int64_t k)
		{
			suffix_automation_t sa(a);
			suffix_automation_t sb(b);

			std::vector<int64_t> counts = sb.all_substrings();
			auto grundy_sum_b = sb.root_grundy_sum();
			auto grundy_sum_a = sa.calculate_grundy_sums();
			std::string ra, rb;
			size_t left_index = 0;
			while (true)
			{
				const state_t& root_b = sb._states[0];
				int64_t strings = counts[0] - grundy_sum_b[sa._states[left_index].grundy_value];
				if (strings >= k)
					break;
				k -= strings;
				if (left_index + 1 == sa._states.size())
					return std::make_pair("no", "solution");
				for (auto& kv : sa._states[left_index].next)
				{
					const state_t& state = sa._states[kv.second];
					int64_t total = 0;
					left_index = kv.second;
					for (auto g : grundy_sum_a[kv.second])
					{
						int64_t count0 = g.second;
						int64_t count1 = counts[0] - grundy_sum_b[g.first];
						int64_t strings = count0 * count1;
						if (0 != count0 && strings / count0 != count1)
							total = k + 1;
						else
							total += strings;
						if (total >= k)
							break;
					}
					if (total >= k)
					{
						ra += kv.first;
						break;
					}
					k -= total;
					left_index = sa._states.size();
				}
				if (sa._states.size() <= left_index)
					return std::make_pair("no", "solution");
			}

			size_t right_index = 0;
			int grundy_value = sa._states[left_index].grundy_value;
			auto grundy_sum = sb.calculate_grundy_sums(grundy_value);
			if (counts[0] - grundy_sum[0] < k)
				return std::make_pair("no", "solution");
			while (k)
			{
				const state_t& state = sb._states[right_index];
				if (state.grundy_value != grundy_value)
					--k;
				else if (sb._states.size() == right_index + 1)
					return std::make_pair("no", "solution");
				if (0 == k)
					break;
				for (auto& kv : state.next)
				{
					int64_t strings = counts[kv.second] - grundy_sum[kv.second];
					if (strings >= k)
					{
						right_index = kv.second;
						rb.push_back(kv.first);
						break;
					}
					k -= strings;
				}
			}

			return std::make_pair(ra, rb);
		}

	private:
		std::vector<int64_t> all_substrings()const
		{
			std::vector<int64_t> cache(_states.size(), 0);
			for (auto state : _sorted_states)
			{
				auto& c = cache[state - _states.data()];
				c = 1;
				for (auto& kv : state->next)
					c += cache[kv.second];
			}
			return cache;
		}

		void kth_substring(const std::vector<int64_t>& counts, size_t root, int64_t k, std::string& s)const
		{
			if (0 == k)
				return;
			for (auto& kv : _states[root].next)
			{
				if (counts[kv.second] >= k)
				{
					s.push_back(kv.first);
					kth_substring(counts, kv.second, k - 1, s);
					return;
				}
				k -= counts[kv.second];
			}
		}
	};

	static std::pair<std::string, std::string> solve(const std::string& a, const std::string& b, int64_t k)
	{
		return suffix_automation_t::solve(a, b, k);
	}

	inline void solve()
	{
		int a, b;
		int64_t k;
		std::cin >> a >> b >> k;
		std::string sa, sb;
		std::cin >> sa >> sb;
		auto r = suffix_automation_t::solve(sa, sb, k);
		if ("no" == r.first && "solution" == r.second)
			std::cout << r.first << " " << r.second << std::endl;
		else
			std::cout << r.first << std::endl << r.second << std::endl;
	}

	inline void assert(const std::string& s0, const std::string& s1, int64_t k, const std::string& t0, const std::string& t1)
	{
		auto actual = solve(s0, s1, k);
		std::cout << "==============================" << std::endl;
		std::cout << "a: " << s0 << " b: " << s1 << " k: " << k << std::endl;
		if (t0 == actual.first && t1 == actual.second)
		{
			std::cout << "--success: " << t0 << " " << t1 << std::endl;
		}
		else
		{
			std::cout << "--expect: " << t0 << " " << t1 << std::endl;
			std::cout << "--actual: " << actual.first << " " << actual.second << std::endl;
		}
	}
}
