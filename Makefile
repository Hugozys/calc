CPP := g++
DEPDIR := .deps
LD = -lfruit
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
CPPFLAGS := -std=gnu++17 -O3 -Wall -Werror -pedantic -I./srcs
COMPILE.cc = $(CPP) $(DEPFLAGS) $(CPPFLAGS) -c
SRCS = $(wildcard ./**/*.cc)
MAIN = ./srcs/main.o ./tests/testToken.o ./tests/testParser.o
OBJS = $(patsubst %.cc, %.o, $(SRCS))
LIBS = $(filter-out $(MAIN) ,$(OBJS))

all: main testToken testParser

main: $(LIBS) srcs/main.o
	$(CPP) -o $@ $^ $(LD)

testToken: $(LIBS) tests/testToken.o
	$(CPP) -o $@ $^ $(LD)

testParser: $(LIBS) tests/testParser.o
	$(CPP) -o $@ $^ $(LD)

srcs/%.o : srcs/%.cc $(DEPDIR)/%.d | $(DEPDIR)
	 $(COMPILE.cc) $< -o $@

tests/%.o : tests/%.cc $(DEPDIR)/%.d | $(DEPDIR)
	 $(COMPILE.cc) $< -o $@

.PHONY: clean
clean:
	rm -rf ./**/*.o main testToken testParser ~# *~ .deps ./**/test_*

$(DEPDIR): ; @mkdir -p $@

DEPFILES = $(addprefix $(DEPDIR)/, $(addsuffix .d, $(notdir $(basename $(SRCS)))))
$(DEPFILES):

include $(DEPFILES)
