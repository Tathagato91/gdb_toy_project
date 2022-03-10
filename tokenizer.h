#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include <condition_variable>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

/**
 * @brief Convenience typedef for a vector of strings.
 */
typedef std::vector<std::string> Tokens;

/**
 * @brief Splits text strings into tokens on whitespace. Multithreaded.
 */
class Tokenizer {
 public:
  /**
   * Construct a Tokenizer with the given number of threads.
   *
   * Tokenizes strings from `source` as they come in, writing them to `output`.
   * @param source Deque from which to read strings to tokenize.
   * @param s_access Mutex controlling access to `source`.
   * @param output Deque into which to deposit tokenized strings.
   * @param o_access Mutex controlling access to `output`.
   * @throws `std::invalid_argument` if `num_threads` is zero.
   */
  explicit Tokenizer(unsigned int num_threads,
                     std::deque<std::string>& source, std::mutex& s_access,
                     std::deque<Tokens>& output, std::mutex& o_access);

  ~Tokenizer();

  /**
   * @brief Stop tokenizing strings. Do nothing if we already weren't.
   */
  void stop();

  static void tokenize(Tokenizer* self, std::condition_variable* waiter);

 private:
  struct Worker {
    std::thread thread;
    std::condition_variable waiter;
  };  // struct Worker

  std::deque<std::string>& source_;
  std::deque<Tokens>& output_;

  bool finish_{false};

  std::mutex& source_access_;
  std::mutex& output_access_;

  std::vector<Worker*> workers_;
};  // class Tokenizer

#endif  // _TOKENIZER_H
