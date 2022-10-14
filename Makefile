CC = gcc
CXX = g++
CXXFLAGS = -pthread

SRC = $(wildcard *.cpp)
BIN = $(SRC:%.cpp=%)
LOG = $(BIN:%=%.log)

MAX_THREADS = 128
TEST_ROUNDS = 3

all: ${BIN}

log: ${LOG}

${LOG}:%.log:%
	>$@
	for thread in `seq 1 ${MAX_THREADS}`; do \
		total_time=0.0; \
		for round in `seq 1 ${TEST_ROUNDS}`; do \
			this_time=`/usr/bin/time -f "%e" ./$^ $$thread 2>&1 >/dev/null`; \
			total_time=`echo $$total_time $$this_time | awk '{printf("%.2f",$$1+$$2)}'`; \
		done; \
		average_time=`echo $$total_time | awk '{printf("%.2f",$$1/${TEST_ROUNDS}.0)}'`; \
		printf "%d\t%.2f\n" $$thread $$average_time >>$@; \
	done

${BIN}:%:%.cpp
	${CXX} $^ ${CXXFLAGS} -o $@

clean:
	rm -rf ${BIN}
	rm -rf ${LOG}

.PHONY: all log clean
