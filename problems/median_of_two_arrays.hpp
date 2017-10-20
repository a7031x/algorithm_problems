//https://leetcode.com/problems/median-of-two-sorted-arrays/description/
#pragma once
#include <vector>
#include <algorithm>

class Solution {
public:
	double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
		if (0 == nums1.size())
			return findMedian(nums2);
		if (0 == nums2.size())
			return findMedian(nums1);
		if (nums1.front() > nums2.front())
			return findMedianSortedArrays(nums2, nums1);
		auto k = (nums1.size() + nums2.size() - 1) / 2;
		auto kth = findKth(nums1.begin(), nums1.end(), nums2.begin(), nums2.end(), k);
		if ((nums1.size() + nums2.size()) % 2 == 0)
		{
			auto k1th = findKth(nums1.begin(), nums1.end(), nums2.begin(), nums2.end(), k + 1);
			return (kth + k1th) / 2.0;
		}
		else
			return kth;
	}

	double findMedian(const std::vector<int>& nums)
	{
		auto middle = (nums.size() - 1) / 2;
		if (nums.size() % 2 == 0)
			return (nums[middle] + nums[middle + 1]) / 2.0;
		else
			return nums[middle];
	}

	template<typename Iterator>
	int findKth(Iterator first1, Iterator last1, Iterator first2, Iterator last2, size_t k)
	{
		if (first1 == last1)
			return *(first2 + k);
		else if (first2 == last2)
			return *(first1 + k);

		auto mid = first1 + std::distance(first1, last1) / 2;
		auto rank1 = findRank(first1, last1, *mid);
		auto rank2 = findRank(first2, last2, *mid);
		if (rank2.first + rank1.first > k)
		{
			return findKth(first1, first1 + rank1.first, first2, first2 + rank2.first, k);
		}
		else if (rank2.second + rank1.second <= k)
		{
			return findKth(first1 + rank1.second, last1, first2 + rank2.second, last2, k - rank2.second - rank1.second);
		}
		else
		{
			if (rank1.second + rank2.first > k)
				return *mid;
			else
				return findKth(mid, mid, first2 + rank2.first, first2 + rank2.second, k - rank1.second - rank2.first);
		}
	}

	template<typename Iterator>
	std::pair<size_t, size_t> findRank(Iterator first, Iterator last, int val)
	{
		auto itr = std::lower_bound(first, last, val);
		if (last == itr)
		{
			return std::pair<size_t, size_t>(last - first, last - first);
		}
		auto itr2 = std::upper_bound(itr, last, val);
		return std::pair<size_t, size_t>(itr - first, itr2 - first);
	}
};