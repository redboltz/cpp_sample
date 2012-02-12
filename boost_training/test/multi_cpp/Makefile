TARGET	= multi_cpp_test
LD		= g++
LDFLAGS = -L${BOOSTLIBPATH}
LIBS	= -lboost_test_exec_monitor
CXX		= g++
CXXFLAGS= -I${BOOSTPATH}
SRCS	= target1.cpp target2.cpp main.cpp
OBJS	= $(SRCS:.cpp=.o)

all:$(TARGET)
$(TARGET):$(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET) $(OBJS)
