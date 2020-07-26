#include <fruit/fruit.h>

#include <array>
#include <unordered_map>

#include "calcdfafactory.hpp"
#include "dfa.hpp"
#include "dfabuilder.hpp"
#include "token.hpp"
using dfa::Dfa;
using dfa::DfaFactory;
class CalcDfaFactory : public DfaFactory {
 public:
  using Inject = CalcDfaFactory();
  CalcDfaFactory() = default;
  Dfa CreateDfa() override;

 private:
  enum StateType { INITIAL, NUM, HALFFLOAT, FLOAT, SPACE, OP };
};

Dfa CalcDfaFactory::CreateDfa() {
  Dfa::DfaBuilder builder{};
  std::unordered_map<StateType, Dfa::State> state_map{};
  // add all states
  builder.WithState(state_map[INITIAL])
      .WithState(state_map[NUM], ValueTokenAction)
      .WithState(state_map[HALFFLOAT])
      .WithState(state_map[OP], OpTokenAction)
      .WithState(state_map[SPACE], SpaceTokenAction)
      .WithState(state_map[FLOAT], ValueTokenAction);
  std::array digitSet{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  std::array opSet{'+', '-', 'x', '/', '(', ')', '^', '\n'};
  std::array spaceSet{' ', '\t'};

  // add edges
  for (auto c : digitSet) {
    builder.WithEdge(state_map[INITIAL], state_map[NUM], c)
        .WithEdge(state_map[NUM], state_map[NUM], c)
        .WithEdge(state_map[FLOAT], state_map[FLOAT], c)
        .WithEdge(state_map[HALFFLOAT], state_map[FLOAT], c);
  }
  for (auto c : spaceSet) {
    builder.WithEdge(state_map[INITIAL], state_map[SPACE], c)
        .WithEdge(state_map[SPACE], state_map[SPACE], c);
  }
  for (auto c : opSet) {
    builder.WithEdge(state_map[INITIAL], state_map[OP], c);
  }
  builder.WithEdge(state_map[NUM], state_map[HALFFLOAT], '.')
      .WithStartState(state_map[INITIAL]);
  return builder.Build();
}

fruit::Component<DfaFactory> dfa::getCalcDfaFactoryComponent() {
  return fruit::createComponent().bind<DfaFactory, CalcDfaFactory>();
}
