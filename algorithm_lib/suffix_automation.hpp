#include <vector>
#include <map>

namespace algorithm_lib
{
	class suffix_automation_t
	{
		struct state_t
		{
			size_t length = 0;
			size_t link = -1;
			std::map<char, size_t> next;
		};

		std::vector<state_t> _states;
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

		int64_t all_substrings()const
		{
			std::vector<int64_t> cache(_states.size(), 0);
			return all_substrings(cache, 0);
		}

		std::string kth_substring(int64_t k)const
		{
			std::vector<int64_t> cache(_states.size(), 0);
			all_substrings(cache, 0);
			std::string s;
			kth_substring(cache, 0, k, s);
			return s;
		}

	private:
		int64_t all_substrings(std::vector<int64_t>& cache, size_t root)const
		{
			auto& c = cache[root];
			if (c)
				return c;
			c = 1;
			for (auto& kv : _states[root].next)
				c += all_substrings(cache, kv.second);
			return c;
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
}