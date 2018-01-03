#include <iostream>
#include <list>
#include <map>
#include <queue>

static const int NO_COIN = 9999;
static const int IMPOSSIBLE = -1;

class State {
  public:
    State();

    void Set(int slot, int coin);
    void Clear(int slot, int coin);
    int GetSmallest(int slot) const;

    bool operator < (const State &rhs) const { return state < rhs.state; }
    bool operator == (const State & rhs) const { return state == rhs.state; }

    void Dump(const int N) const {
      for (int i = 0; i < N; ++i) {
        for (int j = 0; j < 8; ++j) {
          if (state & (1ULL << (j + i * 8))) {
            printf("X");
          } else {
            printf(".");
          }
        }
        printf(" | ");
      }
      printf("\n");
    }

  private:
    uint64_t state;
};

State::State() : state(0) {}

void State::Set(int slot, int coin) {
  state |= (1ULL << (coin + slot * 8));
}

void State::Clear(int slot, int coin) {
  state &= ~(1ULL << (coin + slot * 8));
}

int State::GetSmallest(int slot) const {
  for (int i = 0; i < 8; ++i)
    if (state & (1ULL << (i + slot * 8)))
      return i;

  return NO_COIN;
}

class CoinGame {
  public:
    explicit CoinGame(int n);

    int Run(const State &initialState);

  private:
    typedef std::list<State> state_list_t;
    typedef std::queue<State> state_queue_t;
    typedef std::map<State, int> score_map_t;

    State CalculateTargetState() const;
    state_list_t GenerateSuccessors(const State &state) const;

    const int N;
};

CoinGame::CoinGame(int n) : N(n) {
}

State CoinGame::CalculateTargetState() const {
  State ret;
  for (int i = 0; i < N; ++i) {
    ret.Set(i, i);
  }
  return ret;
}

CoinGame::state_list_t CoinGame::GenerateSuccessors(const State &state) const {
  state_list_t ret;

  for (int i = 0; i < N; ++i) {
    int smallestCoin = state.GetSmallest(i);
    if (smallestCoin == NO_COIN) {
      continue;
    }

    if (i > 0 && smallestCoin < state.GetSmallest(i - 1)) {
      State leftState = state;
      leftState.Clear(i, smallestCoin);
      leftState.Set(i - 1, smallestCoin);
      ret.push_back(leftState);
    }

    if (i + 1 < N && smallestCoin < state.GetSmallest(i + 1)) {
      State rightState = state;
      rightState.Clear(i, smallestCoin);
      rightState.Set(i + 1, smallestCoin);
      ret.push_back(rightState);
    }
  }

  return ret;
}

int CoinGame::Run(const State &initialState) {
  const State target = CalculateTargetState();

  score_map_t scoreMap;
  state_queue_t queue;
  queue.push(initialState);
  scoreMap[initialState] = 0;

  while (!queue.empty()) {
    State cur = queue.front();
    queue.pop();

    if (cur == target) {
      return scoreMap[cur];
    }

    const int childScore = scoreMap[cur] + 1;
    const state_list_t nextStates = GenerateSuccessors(cur);
    for (state_list_t::const_iterator i = nextStates.begin(); i != nextStates.end(); ++i) {
      score_map_t::const_iterator score = scoreMap.find(*i);

      // Haven't visited the node yet so generate a score for it and visit it next.
      if (score == scoreMap.end()) {
        scoreMap[*i] = childScore;
        queue.push(*i);
      }
    }
  }

  return IMPOSSIBLE;
}

int main(int argc, char **argv) {
  for (;;) {
    int n;
    std::cin >> n;

    if (n == 0)
      break;

    CoinGame game(n);
    State initialState;
    for (int i = 0; i < n; ++i) {
      int coin;
      std::cin >> coin;
      --coin;
      initialState.Set(i, coin);
    }

    int score = game.Run(initialState);
    if (score == IMPOSSIBLE) {
      std::cout << "IMPOSSIBLE" << std::endl;
    } else {
      std::cout << score << std::endl;
    }
  }
  return 0;
}
