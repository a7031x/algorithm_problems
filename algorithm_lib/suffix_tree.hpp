#pragma once
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <unordered_map>

namespace algorithm_lib
{
	class suffix_tree_t
	{
	private:
		class node_t
		{
		public:
			size_t start;
			std::shared_ptr<size_t> end;
			std::unordered_map<char, std::shared_ptr<node_t>> children;
			node_t(size_t start, std::shared_ptr<size_t> end)
				: start(start), end(end)
			{}
		};

		typedef std::shared_ptr<node_t> nodeptr;

	private:
		nodeptr _root;
		std::string _text;

	public:
		suffix_tree_t(const std::string& text)
			: _text(text)
			, _root(std::make_shared<node_t>(-1, nullptr))
		{
			size_t suffix_count = 0;
			auto end = std::make_shared<size_t>(text.size());
			size_t active_depth = 0;
			nodeptr active_node = _root;
			for (size_t k = 0; k < text.size(); ++k)
			{
				++suffix_count;
				do {
					if (false == create_internal_node(active_node, active_depth, suffix_count - 1, k, end))
						break;
					active_depth = 0;
					active_node = _root;
				} while (--suffix_count);
			}
		}

		std::string max_repeated_substring()
		{
			std::string max_path;
			size_t max_length = 0;

			traverse([&](
				const std::string& path,
				size_t path_length,
				size_t start,
				size_t end,
				const std::vector<size_t>& children_starts)
			{
				if (path_length > max_length)
				{
					max_length = path_length;
					max_path = path;
				}
			});

			return full_path(max_path);
		}

		std::string full_path(const std::string& path)
		{
			auto node = _root;
			std::string r;
			for (auto c : path)
			{
				auto child = node->children[c];
				r += _text.substr(child->start, *child->end - child->start);
				node = child;
			}
			return r;
		}

		void traverse(const std::function<void(
			const std::string& path,
			size_t path_length,
			size_t start,
			size_t end,
			const std::vector<size_t>& children_starts)>& callback)
		{
			struct item_t
			{
				std::string path;
				size_t length;
				nodeptr node;
				item_t(const std::string& path, size_t length, nodeptr node)
					: path(path), length(length), node(node)
				{}
				item_t(item_t&& other)
				{
					path = std::move(other.path);
					length = other.length;
					node = std::move(other.node);
				}
			};

			std::stack<item_t> queue;
			queue.push(item_t("", 0, _root));

			while (!queue.empty())
			{
				auto parent = std::move(queue.top());
				queue.pop();

				std::vector<size_t> starts;
				for (auto child : parent.node->children)
				{
					starts.push_back(child.second->start);
					if (child.second->children.size() == 0)
						continue;
					queue.push(
						item_t(
							parent.path + child.first,
							parent.length + *child.second->end - child.second->start,
							child.second));
				}
				if (parent.node != _root)
					callback(parent.path, parent.length, parent.node->start, *parent.node->end, starts);
			}
		}
	private:
		nodeptr create_internal_node(nodeptr parent, nodeptr child, size_t depth, size_t pos, nodeptr leaf)
		{
			auto internal_node = std::make_shared<node_t>(
				child->start,
				std::make_shared<size_t>(child->start + depth));
			parent->children[_text[pos - depth]] = internal_node;
			child->start = *internal_node->end;
			internal_node->children[_text[*internal_node->end]] = child;
			internal_node->children[_text[pos]] = leaf;
			return internal_node;
		}

		bool create_internal_node(nodeptr& active_node, size_t& active_depth, size_t depth, size_t pos, std::shared_ptr<size_t> end)
		{
			nodeptr child;
			nodeptr parent = _root;
			auto new_leaf = std::make_shared<node_t>(pos, end);

			auto saved_depth = depth;
			if (active_depth < depth)
			{
				parent = active_node;
				depth -= active_depth;
			}
			
			while (child = parent->children[_text[pos - depth]])
			{
				auto edge_length = *child->end - child->start;
				if (edge_length > depth)
				{
					if (_text[child->start + depth] == _text[pos])
					{
						active_node = parent;
						active_depth = saved_depth - depth;
						return false;
					}
					auto internal_node = create_internal_node(parent, child, depth, pos, new_leaf);
					return true;
				}
				depth -= edge_length;
				parent = child;
			}
			parent->children[_text[pos - depth]] = new_leaf;
			return true;
		}
	};
}