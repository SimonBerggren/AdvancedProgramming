#pragma once
template <class FwdIt, class Comparator = std::less<>>
void ForwardSort(const FwdIt begin, const FwdIt end, Comparator compare = Comparator()) {
	FwdIt left = begin;
	FwdIt right = begin;
	++right;
	bool errorsFound = false;
	do {
		errorsFound = false;
		while (right != end) {
			if (compare(*left, *right)) {
				errorsFound = true;
				auto temp = *left;
				*left = *right;
				*right = temp;
			}
			++right;
			++left;
		}
		left = right = begin;
		++right;
	} while (errorsFound);
}