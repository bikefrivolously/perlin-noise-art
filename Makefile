DEPDIR = .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

CXXFLAGS = -g -Og -Wall -Wpedantic -std=c++14
#CXXFLAGS = -g -Wall -Wpedantic -std=c++14

LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system -lnoise

LINK.o = $(LINK.cc)
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

SRCS=perlin.cc force.cc particle.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: perlin

perlin: $(OBJS)

clean:
	$(RM) $(OBJS)

%.o : %.cc
%.o : %.cc $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
