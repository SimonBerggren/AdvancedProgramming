#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#include <iostream>
#include <queue>

#define ALT_1

#ifdef ALT_1
// alt. 1 -> using queue will skip last step but will take approx. twice the memory
char* Replace(const char* originalString, const char* searchFor, const char* replaceWith)
{
	std::queue<char> queue;																// queue takes away the concern of looping through just to get the number of matches
	for (auto c1 = originalString; c1 != originalString + strlen(originalString); ++c1) {	// loop through char by char in originalString
		if (*c1 == *searchFor) {															// if first char of searchFor is matching originalString
			bool match_found = true;														// assume entire searchFor is matching
			auto c3 = c1;																	// compare searchFor and sequence in originalString char by char
			for (auto c2 = searchFor; c2 != searchFor + strlen(searchFor); ++c2) {
				if (*c3 != *c2) {															// as soon as the strings digress, break
					match_found = false;
					break;
				}
				++c3;																		// if the strings doesn't digress, keep making sure they stay that way
			}
			if (match_found) {																// if searchFor is matching a sequence inside originalString
				for (auto c = replaceWith; c != replaceWith + strlen(replaceWith); ++c)		// push searchFor to queue
					queue.push(*c);

				c1 += strlen(searchFor) - 1;												// offset originalString loop
			}
			else
				queue.push(*c1);															// if we didn't find a full match, simply push current char

		}
		else
			queue.push(*c1);																// if originalString char and searchFor char doesn't match, simply push it to the queue
	}
	queue.push('\0');																		// null-terminate queue and thus the new string
	auto result = new char[queue.size()];													// allocate memory for the new string

	for (auto c = result; queue.size() > 0; ++c) {											// transfer characters from queue to the new string
		*c = queue.front();
		queue.pop();
	}
	return result;																			// return the new string
}
#else
// alt. 2 -> not using queue will do last step but will not make much unnessecary memory allocation
char* Replace(const char* originalString, const char* searchFor, const char* replaceWith)
{
	size_t num_matches = 0;
	for (auto c1 = originalString; c1 != originalString + strlen(originalString); ++c1) {	// loop through char by char in originalString
		if (*c1 == *searchFor) {															// if first char of searchFor is matching originalString
			bool match_found = true;														// assume entire searchFor is matching
			auto c3 = c1;																	// compare searchFor and sequence in originalString char by char
			for (auto c2 = searchFor; c2 != searchFor + strlen(searchFor); ++c2) {
				if (*c3 != *c2) {															// as soon as the strings digress, break
					match_found = false;
					break;
				}
				++c3;																		// if the strings doesn't digress, keep making sure they stay that way
			}
			if (match_found) {
				++num_matches;
				c1 += strlen(searchFor) - 1;
			}
		}
	}
	size_t new_len = strlen(originalString) + (num_matches * (strlen(replaceWith) - strlen(searchFor)));
	auto result = new char[new_len + 1];													// allocate memory for the new string
	for (size_t c1 = 0; c1 < new_len; ++c1) {											// transfer characters from queue to the new string
		if (originalString[c1] == *searchFor) {															// if first char of searchFor is matching originalString
			size_t currChange = 0;
			bool match_found = true;														// assume entire searchFor is matching
			auto c3 = &originalString[c1];																	// compare searchFor and sequence in originalString char by char
			for (auto c2 = searchFor; c2 != searchFor + strlen(searchFor); ++c2) {
				if (*c3 != *c2) {															// as soon as the strings digress, break
					match_found = false;
					break;
				}
				++c3;																		// if the strings doesn't digress, keep making sure they stay that way
			}
			if (match_found)
			{
				strcpy_s(&result[c1], new_len - c1, replaceWith);
				++currChange;
				c1 += strlen(searchFor) + currChange * (strlen(replaceWith) - strlen(searchFor)) - 1;
			}
			else
				result[c1] = originalString[c1];
		}
		else
			result[c1] = originalString[c1];
	}

	result[new_len] = '\0';
	return result;																			// return the new string
}
#endif

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	setlocale(LC_ALL, "");

	auto originalString = "aaaaaaajaaajaaajaaaj";
	auto searchFor = "aa";
	auto replaceWith = "ya";

	auto result = Replace(originalString, searchFor, replaceWith);
	std::cout << result << std::endl;
	delete result;

	system("pause");
	return EXIT_SUCCESS;
}