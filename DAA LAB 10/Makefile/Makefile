CC     = gcc
CFLAGS = -O2 -Wall -Wextra
QS     = Q1_Online_Bipartiteness Q2_Dynamic_Islands_Grid Q3_Max_Weight_Component_Size_K \
         Q4_Reverse_Time_DSU Q5_Lex_Smallest_Permutation
BINS   = $(QS:%=%/solution)

all: $(BINS)

%/solution: %/solution.c
	$(CC) $(CFLAGS) -o $@ $<

run: all
	@for q in $(QS); do echo "=== $$q ==="; ./$$q/solution < $$q/sample_input.txt; echo; done

test:
	python3 tests/stress_test.py 2000

clean:
	rm -rf tests/bin $(BINS)

.PHONY: all run test clean
