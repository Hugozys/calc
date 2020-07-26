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
        static constexpr State InvalidState = -1;
        static constexpr State StartState = 0;
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
    private:
        struct StateImpl{
            std::array<State, UCHAR_MAX> edges;
            Action action;
            bool accepting;
        };
        std::vector<StateImpl> _states;
};

class DfaSimulator{
    public:
        DfaSimulator(const Dfa & dfa) : _dfa(dfa) {}
        bool Move(char c);
        void Reset();
        bool HasAcceptState() const{
            return _passedAcceptState != Dfa::InvalidState;
        }
        bool IsCurrentAccept() const{
            return _dfa.IsAccepting(_currentState);
        }
        PToken Accept(int pos);
    private:
        Dfa::State _passedAcceptState{Dfa::InvalidState};
        Dfa::State _currentState{Dfa::InvalidState};
        size_t _passedAcceptStateStrLen;
        std::string _buffer;
        const Dfa & _dfa;
};

#endif
