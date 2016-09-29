#pragma once
#include <memory>
#include <vector>
#include <set>
#include <list>
#include <algorithm>
#include <reg_state_machine.hpp>

namespace count_strings
{
	typedef std::shared_ptr<struct node_t> nodeptr;
	typedef std::vector<std::vector<int64_t>> matrix_t;
	struct node_t
	{
		char symbol;
		std::set<nodeptr> next;
		node_t(char symbol) : symbol(symbol) {}
	};

	struct expression_t
	{
		std::set<nodeptr> first;
		std::set<nodeptr> last;
	};

	typedef std::shared_ptr<struct state_t> stateptr;

	struct state_t
	{
		int id;
		std::set<nodeptr> nodes;
		stateptr a;
		stateptr b;
		bool terminal = false;
	};

	struct state_less_than_t
	{
		bool operator()(const stateptr& left, const stateptr& right)
		{
			if (left->nodes.size() < right->nodes.size())
				return true;
			else if (left->nodes.size() > right->nodes.size())
				return false;
			for (auto j = left->nodes.begin(), k = right->nodes.begin(); left->nodes.end() != j; ++j, ++k)
			{
				if (*j < *k)
					return true;
				else if (*j > *k)
					return false;
			}
			return false;
		}
	};

	inline void determine_terminal(const expression_t& expression, const stateptr state)
	{
		for (auto& node : state->nodes)
			if (expression.last.find(node) != expression.last.end())
			{
				state->terminal = true;
				return;
			}
	}

	inline expression_t nfa(const std::string& reg, nodeptr nullable)
	{
		algorithm_lib::reg_state_machine_t sm;
		std::stack<expression_t> expression_stack;

		sm.process(reg, [&](char symbol, const std::string& production)
		{
			if ("a" == production || "b" == production)
			{
				expression_t expression;
				nodeptr node = std::make_shared<node_t>(production[0]);
				expression.first.insert(node);
				expression.last.insert(node);
				expression_stack.push(expression);
			}
			else if ("(RR)" == production)
			{
				expression_t right = expression_stack.top();
				expression_stack.pop();
				expression_t left = expression_stack.top();
				expression_stack.pop();
				expression_t merge;
				for (auto& from : left.last)
				{
					if (from == nullable)
						continue;
					for (auto& to : right.first)
						if (to != nullable)
							from->next.insert(to);
				}
				for (auto& first : left.first)
				{
					if (nullable == first)
						merge.first.insert(right.first.begin(), right.first.end());
					else
						merge.first.insert(first);
				}
				for (auto& last : right.last)
				{
					if (nullable == last)
						merge.last.insert(left.last.begin(), left.last.end());
					else
						merge.last.insert(last);
				}
				expression_stack.push(merge);
			}
			else if ("(R|R)" == production)
			{
				expression_t right = expression_stack.top();
				expression_stack.pop();
				expression_t left = expression_stack.top();
				expression_stack.pop();
				expression_t merge;
				merge.first.insert(left.first.begin(), left.first.end());
				merge.first.insert(right.first.begin(), right.first.end());
				merge.last.insert(left.last.begin(), left.last.end());
				merge.last.insert(right.last.begin(), right.last.end());
				expression_stack.push(merge);
			}
			else if ("(R*)" == production)
			{
				expression_t& expression = expression_stack.top();
				for (auto& from : expression.last)
				{
					if (from == nullable)
						continue;
					for (auto& to : expression.first)
						if (to != nullable)
							from->next.insert(to);
				}
				expression.first.insert(nullable);
				expression.last.insert(nullable);
			}
		});
		return expression_stack.top();
	}

	inline std::vector<stateptr> dfa(const expression_t& expression, nodeptr nullable)
	{
		stateptr start = std::make_shared<state_t>();
		start->a = std::make_shared<state_t>();
		start->b = std::make_shared<state_t>();
		int state_id = 0;
		start->id = state_id++;
		for (auto& node : expression.first)
		{
			if (node == nullable)
			{
				start->terminal = true;
				continue;
			}
			if ('a' == node->symbol)
				start->a->nodes.insert(node);
			else
				start->b->nodes.insert(node);
		}
		if (start->a->nodes.empty())
			start->a = nullptr;
		if (start->b->nodes.empty())
			start->b = nullptr;

		std::list<stateptr> queue;
		std::set<stateptr, state_less_than_t> states;
		states.insert(start);
		if (start->a)
		{
			states.insert(start->a);
			queue.push_back(start->a);
			determine_terminal(expression, start->a);
		}
		if (start->b)
		{
			states.insert(start->b);
			queue.push_back(start->b);
			determine_terminal(expression, start->b);
		}
		while (queue.size())
		{
			auto state = queue.front();
			state->id = state_id++;
			auto a = std::make_shared<state_t>();
			auto b = std::make_shared<state_t>();
			for (auto& node : state->nodes)
				for (auto& next : node->next)
				{
					if ('a' == next->symbol)
						a->nodes.insert(next);
					else
						b->nodes.insert(next);
				}
			if (false == a->nodes.empty())
			{
				auto it = states.insert(a);
				if (it.second)
				{
					queue.push_back(a);
					determine_terminal(expression, a);
				}
				state->a = *it.first;
			}
			if (false == b->nodes.empty())
			{
				auto it = states.insert(b);
				if (it.second)
				{
					queue.push_back(b);
					determine_terminal(expression, b);
				}
				state->b = *it.first;
			}
			queue.pop_front();
		}
		std::vector<stateptr> r(states.begin(), states.end());
		std::sort(r.begin(), r.end(), [](const stateptr& x, const stateptr& y)
		{
			return x->id < y->id;
		});
		return r;
	}

	inline matrix_t multiply(const matrix_t& left, const matrix_t& right)
	{
		const int64_t module = 1000000007;
		matrix_t m(left.size(), std::vector<int64_t>(right[0].size(), 0));
		for (size_t i = 0; i < left.size(); ++i)
		{
			for (size_t j = 0; j < right[0].size(); ++j)
			{
				int64_t r = 0;
				for (size_t k = 0; k < right.size(); ++k)
					r += (left[i][k] * right[k][j]) % module;
				m[i][j] = r % module;
			}
		}
		return m;
	}

	inline int64_t solve(const std::string& reg, size_t length)
	{
		const int64_t module = 1000000007;
		nodeptr nullable = std::make_shared<node_t>(0);
		auto expression = nfa(reg, nullable);
		auto states = dfa(expression, nullable);
		if (0 == length)
			return states[0]->terminal ? 1 : 0;

		matrix_t matrix(states.size(), std::vector<int64_t>(states.size(), 0));
		matrix_t coef(states.size(), std::vector<int64_t>(states.size(), 0));
		for (size_t k = 0; k < states.size(); ++k)
			coef[k][k] = 1;
		for (size_t row = 0; row < states.size(); ++row)
		{
			auto ca = states[row]->a;
			auto cb = states[row]->b;
			if (ca)
				matrix[row][ca->id] = 1;
			if (cb)
				matrix[row][cb->id] = 1;
		}
		size_t index = 1;
		while (index <= length)
		{
			if (length & index)
			{
				coef = multiply(coef, matrix);
			}
			matrix = multiply(matrix, matrix);
			index <<= 1;
		}
		int64_t r = 0;
		for (size_t k = 1; k < states.size(); ++k)
			if (states[k]->terminal)
				r += coef[0][k];
		return r % module;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			std::string reg;
			size_t length;
			std::cin >> reg >> length;
			std::cout << solve(reg, length) << std::endl;
		}
	}
}