CPP := g++
DEPDIR := .deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
CPPFLAGS := -std=gnu++17 -ggdb3 -Wall -Werror -pedantic -I./srcs
COMPILE.cc = $(CPP) $(DEPFLAGS) $(CPPFLAGS) -c
SRCS = $(wildcard ./**/*.cc)
MAIN = ./srcs/main.o ./tests/test.o
OBJS = $(patsubst %.cc, %.o, $(SRCS))
LIBS = $(filter-out $(MAIN) ,$(OBJS))

all: main test

main: $(LIBS) srcs/main.o
	$(CPP) $(CPPFLAGS) -o $@ $^

test: $(LIBS) tests/test.o
	$(CPP) $(CPPFLAGS) -o $@ $^

srcs/%.o : srcs/%.cc $(DEPDIR)/%.d | $(DEPDIR)
	 $(COMPILE.cc) $< -o $@

tests/%.o : tests/%.cc $(DEPDIR)/%.d | $(DEPDIR)
	 $(COMPILE.cc) $< -o $@

.PHONY: clean
clean:
	rm -rf ./**/*.o main test ~# *~ .deps

$(DEPDIR): ; @mkdir -p $@

DEPFILES = $(addprefix $(DEPDIR)/, $(addsuffix .d, $(notdir $(basename $(SRCS)))))
$(DEPFILES):

include $(DEPFILES)