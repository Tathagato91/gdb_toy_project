#include "tokenizer.h"

#include <sstream>
#include <stdexcept>
#include <utility>

using std::condition_variable;
using std::deque;
using std::lock_guard;
using std::move;
using std::mutex;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::thread;
using std::unique_lock;

Tokenizer::Tokenizer(unsigned int num_threads,
                     deque<string>& source, mutex& source_access,
                     deque<Tokens>& output, mutex& output_access)
  : source_(source), output_(output),
    source_access_(source_access), output_access_(output_access)
{
  for (unsigned int i = 0; i < num_threads; ++i) {
    Worker* worker = new Worker();
    worker->thread = thread(Tokenizer::tokenize, this, &worker->waiter);
    workers_.push_back(worker);
  }
}

Tokenizer::~Tokenizer() {
  finish_ = true;
  for (Worker* worker : workers_) {
    worker->waiter.notify_all();  // that we're done
  }
  for (Worker* worker : workers_) {
    worker->thread.join();
  }
}

void Tokenizer::stop() {
  finish_ = true;
  for (Worker* worker : workers_) {
    worker->waiter.notify_all();
  }
}

void Tokenizer::tokenize(Tokenizer* self, condition_variable* waiter) {
  while (not self->finish_) {
    // wait for the source to become nonempty
    unique_lock<mutex> source_lock(self->source_access_);
    if (self->source_.empty()) {
      waiter->wait(source_lock,
                   [&self]() -> bool { return self->source_.size(); });
    }
    source_lock.unlock();

    // retrieve a string from the front of the deque
    source_lock.lock();
    string raw_to_tokenize = move(self->source_.front());
    self->source_.pop_front();
    source_lock.unlock();

    // split the string on whitespace
    stringstream to_tokenize(raw_to_tokenize);
    Tokens tokens;
    string next_token;
    while (to_tokenize >> next_token) {
      tokens.emplace_back(move(next_token));
    }

    // place the string into the output deque
    lock_guard<mutex> output_lock(self->output_access_);
    self->output_.emplace_back(move(tokens));
  }
}
