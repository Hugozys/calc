#ifndef _DFA_HPP__
#define _DFA_HPP__
#include <cassert>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <string_view>
#include "token.hpp"

class Dfa{
    public:
        static std::unique_ptr<Dfa> BuildDfa();
        bool Move(char c);
        void Reset();
        bool HasAcceptState() const;
        bool IsCurrentAccept() const;
        PToken Accept(int pos);
    private:
        class State;
        class Builder;
        Dfa() = default;
        Dfa(const Dfa & rhs) = delete;
        Dfa & operator = (const Dfa & rhs) = delete;
        Dfa(Dfa && rhs) = default;
        Dfa & operator = (Dfa && rhs) = default;
        State * _currentState{nullptr};
        State * _startState{nullptr};
        State * _passedAcceptState{nullptr};
        std::string _passedAcceptStateStr;
        std::string _buffer;
        std::unordered_map<int,std::unique_ptr<State>> _states;     
        std::unordered_set<State *> _acceptStates;
};

class Dfa::Builder{
    public:
        Builder() = default;
        Builder & AddStates();
        Builder & LinkStates();
        Builder & SetAcceptStates();
        Builder & SetStartState();
        std::unique_ptr<Dfa> Build();
        void Reset();
    private:
        State * _startState;
        std::unordered_set<State *> _acceptStates;
        std::unordered_map<int, std::unique_ptr<State>> _states;
};

class Dfa::State{
    public:
        State(std::function<PToken(const char *, std::size_t)> action):
        _action(action), _table{} {}

        const std::unordered_map<char, State *> & Table() const{
            return _table;
        }
        PToken Execute(const char * payload, std::size_t pos){
            return _action(payload, pos);
        }
        friend Dfa::Builder & Dfa::Builder::AddStates();
        friend Dfa::Builder & Dfa::Builder::LinkStates();       
    private:
        State(const State & rhs) = delete;
        State & operator =(const State & rhs) = delete;
        State(State && rhs) = default;
        State & operator = (State && rhs) = default;
        std::function<PToken(const char *, std::size_t)> _action;
        std::unordered_map<char, State *> _table;
};

#endif
