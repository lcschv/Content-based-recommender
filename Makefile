# CXXFLAGS = -I /usr/local/include/htmlcxx -L /usr/local/lib -lhtmlcxx


OBJDIR = bin
OUTPUTDIR = OUTPUT
# RUNSDIR = indexout indexoutAnchors searchfiles
# OBJS := $(addprefix $(OBJDIR)/,loaders.o recommender.o searcher.o vector.o)
OBJS := $(addprefix $(OBJDIR)/, recommender.o input.o)

all:clean $(OBJS) recommender

recommender: $(OBJS)
	g++ -std=c++11 $(OBJS) -o recommender

$(OBJDIR)/recommender.o: recommender.cpp 
	g++ -std=c++11 -c recommender.cpp -o bin/recommender.o

$(OBJDIR)/input.o: src/input.cpp lib/input.h 
	g++ -std=c++11 -c src/input.cpp -o bin/input.o

# $(OBJDIR)/searcher.o: utils/searcher.cpp utils/searcher.h 
# 	g++ -std=c++11 -c utils/searcher.cpp -o bin/searcher.o

# $(OBJDIR)/vector.o: models/vector_spacemodel.cpp models/vector_spacemodel.h 
# 	g++ -std=c++11 -c models/vector_spacemodel.cpp -o bin/vector.o

$(OBJS): | $(OBJDIR)

$(OBJDIR):
		mkdir $(OBJDIR)

$(OUTPUTDIR):
	mkdir -p $(OUTPUTDIR)

clean:
	rm -f bin/*.o
	@echo Old binaries deleted...
	@echo Creating new binaries ...


