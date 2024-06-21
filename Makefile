C = cc
C_TEST_FLAGS = -O3 -mavx
C_DEBUG_FLAGS = -pedantic -Wall -ggdb 
C_FLAGS = $(C_DEBUG_FLAGS) #$(C_TEST_FLAGS)

CF = clang-format -i

G_PLOT = gnuplot

DISA = objdump -S 

BUILD_DIR = build
BIN_DIR = $(addprefix $(BUILD_DIR)/, bin)
OBJ_DIR = $(addprefix $(BUILD_DIR)/, obj)
SRC_DIR = src

OBJS = $(addprefix $(OBJ_DIR)/, cost_branching.o cost_matmul.o)
BINS = $(addprefix $(BIN_DIR)/, exp1.out exp2.out exp3.out exp4.out exp5.out exp6.out exp7.out exp8.out exp9.out\
				test_branch_less_binary_search.out aligned_alloc.out exp1_matmul.out\
				exp2_matmul.out exp3_matmul.out exp4_matmul.out exp5_matmul.out)

.PHONY: all
all: $(BUILD_DIR) $(BIN_DIR) $(OBJ_DIR) $(OBJS) $(BINS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/*/%.c | $(OBJ_DIR)
	$(C) $(C_FLAGS) -c $< -o $@

$(BIN_DIR)/%.out: $(SRC_DIR)/*/%.c $(OBJS) | $(BIN_DIR)
	$(C) $(C_FLAGS) $(filter-out  $(BIN_DIR), $^) -o $@

%.out: $(BIN_DIR)/%.out
	./$<

disassembly_%: $(OBJ_DIR)/%.o
	$(DISA) $<

plot_%: $(BIN_DIR)/%.out
	./$< > $(BUILD_DIR)/$(notdir $(basename $<)).txt
	$(G_PLOT) -e "filename='$(BUILD_DIR)/$(notdir $(basename $<)).txt'" $(SRC_DIR)/plot_script.gp

.PHONY: format
format:
	$(CF) $(foreach ext, c h hpp cpp, $(wildcard $(SRC_DIR)/*.$(ext)))

.PHONY: clean
clean:
	rm -v -rf $(BUILD_DIR)

