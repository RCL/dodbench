
SRC=dodbench.cpp payload.h dod_hier.h dod_hier.cpp class_hier.h class_hier1.cpp class_hier2.cpp class_hier3.cpp class_hier4.cpp class_hier5.cpp class_hier6.cpp class_hier7.cpp class_hier8.cpp
HOSTNAME=$(shell hostname)
DATE=$(shell date)

# compilation
COMPILE_FLAGS = -O3 -Wall

# parms
STARTNUM=500
ENDNUM=5000
STEPS=10
LIGHT_ITERS=1
HEAVY_ITERS=8

all: benchmark light-prefetch light-prefetch-unroll light light-unroll heavy-prefetch heavy-prefetch-unroll heavy heavy-unroll debug

clean:
	rm -f dodbench-light
	rm -f dodbench-light-unroll
	rm -f dodbench-light-prefetch
	rm -f dodbench-light-prefetch-unroll
	rm -f dodbench-heavy
	rm -f dodbench-heavy-unroll
	rm -f dodbench-heavy-prefetch
	rm -f dodbench-heavy-prefetch-unroll
	rm -f dodbench-debug

benchmark: light light-unroll light-prefetch light-prefetch-unroll heavy heavy-unroll heavy-prefetch heavy-prefetch-unroll 
	./dodbench-light $(STARTNUM) $(ENDNUM) $(STEPS)
	./dodbench-light-unroll $(STARTNUM) $(ENDNUM) $(STEPS)
	./dodbench-light-prefetch $(STARTNUM) $(ENDNUM) $(STEPS)
	./dodbench-light-prefetch-unroll $(STARTNUM) $(ENDNUM) $(STEPS)
	./dodbench-heavy $(STARTNUM) $(ENDNUM) $(STEPS)
	./dodbench-heavy-unroll $(STARTNUM) $(ENDNUM) $(STEPS)
	./dodbench-heavy-prefetch $(STARTNUM) $(ENDNUM) $(STEPS)
	./dodbench-heavy-prefetch-unroll $(STARTNUM) $(ENDNUM) $(STEPS)

record: light light-unroll light-prefetch light-prefetch-unroll heavy heavy-unroll heavy-prefetch heavy-prefetch-unroll 
	@echo "Recording benchmark on $(HOSTNAME)"
	@date >> results/$(HOSTNAME).txt
	./dodbench-light $(STARTNUM) $(ENDNUM) $(STEPS) >> results/$(HOSTNAME).txt
	./dodbench-light-unroll $(STARTNUM) $(ENDNUM) $(STEPS) >> results/$(HOSTNAME).txt
	./dodbench-light-prefetch $(STARTNUM) $(ENDNUM) $(STEPS) >> results/$(HOSTNAME).txt
	./dodbench-light-prefetch-unroll $(STARTNUM) $(ENDNUM) $(STEPS) >> results/$(HOSTNAME).txt
	./dodbench-heavy $(STARTNUM) $(ENDNUM) $(STEPS) >> results/$(HOSTNAME).txt
	./dodbench-heavy-unroll $(STARTNUM) $(ENDNUM) $(STEPS) >> results/$(HOSTNAME).txt
	./dodbench-heavy-prefetch $(STARTNUM) $(ENDNUM) $(STEPS) >> results/$(HOSTNAME).txt
	./dodbench-heavy-prefetch-unroll $(STARTNUM) $(ENDNUM) $(STEPS) >> results/$(HOSTNAME).txt
	svn add results/$(HOSTNAME).txt
	svn ci results/$(HOSTNAME).txt -m "- Auto-added results from $(HOSTNAME) on $(DATE)"

record-debug: debug
	./dodench-debug $(STARTNUM) $(ENDNUM) $(STEPS)

############################
light-prefetch: dodbench-light-prefetch

dodbench-light-prefetch: $(SRC)
	g++ $(COMPILE_FLAGS) -DPREFETCH -DPAYLOAD_ITERATIONS=$(LIGHT_ITERS) $(SRC) -o dodbench-light-prefetch

############################
light-prefetch-unroll: dodbench-light-prefetch-unroll

dodbench-light-prefetch-unroll: $(SRC)
	g++ $(COMPILE_FLAGS) -funroll-loops -DUNROLL_LOOPS -DPREFETCH -DPAYLOAD_ITERATIONS=$(LIGHT_ITERS) $(SRC) -o dodbench-light-prefetch-unroll

############################
light-unroll: dodbench-light-unroll

dodbench-light-unroll: $(SRC)
	g++ $(COMPILE_FLAGS) -funroll-loops -DUNROLL_LOOPS -DPAYLOAD_ITERATIONS=$(LIGHT_ITERS) $(SRC) -o dodbench-light-unroll


############################
light: dodbench-light

dodbench-light: $(SRC)
	g++ $(COMPILE_FLAGS) -DPAYLOAD_ITERATIONS=$(LIGHT_ITERS) $(SRC) -o dodbench-light


############################
heavy-prefetch: dodbench-heavy-prefetch

dodbench-heavy-prefetch: $(SRC)
	g++ $(COMPILE_FLAGS) -DPREFETCH -DPAYLOAD_ITERATIONS=$(HEAVY_ITERS) $(SRC) -o dodbench-heavy-prefetch

############################
heavy-prefetch-unroll: dodbench-heavy-prefetch-unroll

dodbench-heavy-prefetch-unroll: $(SRC)
	g++ $(COMPILE_FLAGS) -funroll-loops -DUNROLL_LOOPS -DPREFETCH -DPAYLOAD_ITERATIONS=$(HEAVY_ITERS) $(SRC) -o dodbench-heavy-prefetch-unroll

############################
heavy-unroll: dodbench-heavy-unroll

dodbench-heavy-unroll: $(SRC)
	g++ $(COMPILE_FLAGS) -funroll-loops -DUNROLL_LOOPS -DPAYLOAD_ITERATIONS=$(HEAVY_ITERS) $(SRC) -o dodbench-heavy-unroll

############################
heavy: dodbench-heavy

dodbench-heavy: $(SRC)
	g++ $(COMPILE_FLAGS) -DPAYLOAD_ITERATIONS=$(HEAVY_ITERS) $(SRC) -o dodbench-heavy


# debug
debug: dodbench-debug

dodbench-debug: $(SRC)
	g++ $(COMPILE_FLAGS) -DPAYLOAD_ITERATIONS=0 -DDEBUG $(SRC) -o dodbench-debug

