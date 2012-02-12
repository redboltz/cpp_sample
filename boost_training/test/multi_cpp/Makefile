TARGET	= multi_cpp_test
LD		= g++
LDFLAGS = -L${BOOSTLIBPATH}
CXX		= g++
CXXFLAGS= -I${BOOSTPATH}
SRCS	= target1.cpp target2.cpp main.cpp
OBJS	= $(SRCS:.cpp=.o)

all:$(TARGET)
$(TARGET):$(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ 
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET) $(OBJS)
