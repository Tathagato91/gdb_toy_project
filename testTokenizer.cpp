#include "tokenizer.h"

#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

bool goesBefore(const Tokens& lhs, const Tokens& rhs);

void testWorks();
// and there would be other tests, realistically

int main() {
  cout << "Running Tokenizer tests." << endl;
  testWorks();
  cout << "All tests passed!" << endl;
}

/**
 * @brief Compare vectors of tokens lexicographically on their first element.
 */
bool goesBefore(const Tokens& lhs, const Tokens& rhs) {
  if (lhs.empty()) {
    return true;
  }
  if (rhs.empty()) {
    return false;
  }
  return lhs.front() < rhs.front();
}

/**
 * @brief Equality comparator for vectors of tokens.
 */
bool operator==(const Tokens& lhs, const Tokens& rhs) {
  if (lhs.size() != rhs.size()) { return false; }
  return equal(lhs.begin(), lhs.end(), rhs.begin());
}

ostream& operator<<(ostream& os, const Tokens& tokens) {
  for (const string& token : tokens) {
    os << "{" << token << "}";
  }
  return os;
}

void testWorks() {
  // Tokenize an equal number of "0" and "1" strings.
  // Basically just makes sure that Tokenizer doesn't crash.

  cout << "testWorks" << endl;
  deque<string> source;
  mutex s_access;

  deque<Tokens> output;
  mutex o_access;

  Tokenizer tokenizer(2, source, s_access, output, o_access);

  const int NUM_ELTS = 100;
  for (int i = 0; i < NUM_ELTS; ++i) {
    lock_guard<mutex> lock(s_access);
    source.emplace_back(to_string(i % 2));
  }

  while (source.size()) {  // wait until it's drained
    this_thread::sleep_for(chrono::milliseconds(5));
  }
  tokenizer.stop();

  // sort lexicographically to ignore nondeterministic output order
  sort(output.begin(), output.end(), goesBefore);

  vector<Tokens> expected;
  for (int i = 0; i < NUM_ELTS / 2; ++i) {
    expected.emplace_back(Tokens{"0"});
  }
  for (int i = 0; i < NUM_ELTS / 2; ++i) {
    expected.emplace_back(Tokens{"1"});
  }

  cout << "Produced " << output.size() << " token vectors." << endl;
  for (auto& ts : output) {
    cout << ts << endl;
  }
  assert(std::equal(output.begin(), output.end(), expected.begin()));

  cout << "testWorks passed!" << endl;
}
