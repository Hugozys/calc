#ifndef _DFA_HPP__
#define _DFA_HPP__

#include <cassert>
#include <climits>
#include <array>
#include <functional>
#include <string>
#include <vector>

#include "token.hpp"

class Dfa{
    public:
        typedef size_t State;
        typedef std::function<PToken(const char *, std::size_t)> Action;

        State AddState(Action action, bool accepting);
        void LinkStates(State from, char input, State to);

        State GetLink(State from, char input) const{
            return _states[from].edges[input];
        }
        Action GetAction(State s) const{
            return _states[s].action;
        }
        bool IsAccepting(State s) const{
            return _states[s].accepting;
        }
        State Begin() const {
            return 0;
        }
        State End() const {
            return InvalidState;
        }
    private:
        static constexpr State InvalidState = -1;
        struct StateImpl{
            std::array<State, UCHAR_MAX> edges;
            Action action;
            bool accepting;
        };
        std::vector<StateImpl> _states;
};

#endif
