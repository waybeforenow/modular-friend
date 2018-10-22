CXXFLAGS=-std=c++14 -O0 -g -Wall $(FILTER_DEFINES)

.DEFAULT = canonical
.PHONY: full-differential recursive canonical

full-differential: main.cc include/full-differential.h
	$(eval FILTER_DEFINES := -DLADDER_USE_FULL_DIFFERENTIAL)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) main.cc -o main

recursive: main.cc include/recursive.h
	$(eval FILTER_DEFINES := -DLADDER_USE_RECURSIVE)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) main.cc -o main

canonical: main main.cc include/canonical.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) main.cc -o main

