#pragma once
#include <stack>
#include <functional>
#include <iostream>
#include <string>

namespace algorithm_lib
{
	class reg_state_machine_t
	{
		std::stack<int> _states;
		std::stack<char> _symbols;
		enum ACTION { SHIFT, REDUCE, ACCEPT, ERROR };
	public:
		reg_state_machine_t()
		{
		}

		void process(const std::string& reg, const std::function<void(char, const std::string&)>& callback = nullptr)
		{
			_states = std::stack<int>();
			_states.push(0);
			_symbols = std::stack<char>();

			for (auto c : reg + '$')
			{
				char symbol = c;
				do {
					int next_state, reduce;
					auto action = determine(symbol, next_state, reduce);
					if (SHIFT == action)
					{
						_states.push(next_state);
						_symbols.push(symbol);
						if ('R' != symbol)
							break;
						symbol = c;
					}
					else if (REDUCE == action)
					{
						std::string expr;
						while (reduce--)
						{
							_states.pop();
							expr = _symbols.top() + expr;
							_symbols.pop();
						}
						if (callback)
							callback(symbol, expr);
					}
					else if (ACCEPT == action)
					{
						return;
					}
					else if (ERROR == action)
					{
						error(symbol);
						return;
					}
				} while (true);
			}
		}

	private:
		ACTION determine(char& symbol, int& next_state, int& reduce)
		{
			switch (_states.top())
			{
			case 0:
				switch (symbol)
				{
				case 'a':
				case 'b':
					next_state = 4;
					return SHIFT;
				case 'R':
					next_state = 1;
					return SHIFT;
				case '(':
					next_state = 5;
					return SHIFT;
				}
				break;
			case 1:
				switch (symbol)
				{
				case '$':
					return ACCEPT;
				}
				break;
			case 2:
				reduce = 4;
				symbol = 'R';
				return REDUCE;
			case 3:
				reduce = 5;
				symbol = 'R';
				return REDUCE;
			case 4:
				reduce = 1;
				symbol = 'R';
				return REDUCE;
			case 5:
				switch (symbol)
				{
				case 'a':
				case 'b':
					next_state = 4;
					return SHIFT;
				case '(':
					next_state = 5;
					return SHIFT;
				case 'R':
					next_state = 6;
					return SHIFT;
				}
				break;
			case 6:
				switch (symbol)
				{
				case 'a':
				case 'b':
					next_state = 4;
					return SHIFT;
				case '(':
					next_state = 5;
					return SHIFT;
				case 'R':
					next_state = 8;
					return SHIFT;
				case '|':
					next_state = 7;
					return SHIFT;
				case '*':
					next_state = 10;
					return SHIFT;
				}
				break;
			case 7:
				switch (symbol)
				{
				case 'a':
				case 'b':
					next_state = 4;
					return SHIFT;
				case 'R':
					next_state = 9;
					return SHIFT;
				case '(':
					next_state = 5;
					return SHIFT;
				}
				break;
			case 8:
				switch (symbol)
				{
				case ')':
					next_state = 2;
					return SHIFT;
				}
				break;
			case 9:
				switch (symbol)
				{
				case ')':
					next_state = 3;
					return SHIFT;
				}
				break;
			case 10:
				switch (symbol)
				{
				case ')':
					next_state = 11;
					return SHIFT;
				}
				break;
			case 11:
				reduce = 4;
				symbol = 'R';
				return REDUCE;
			}
			return ERROR;
		}
		void error(char symbol)
		{
			std::cout << "state " << _states.top() << " doesn't accept symbol " << symbol << std::endl;
		}
	};
}
