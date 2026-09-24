.PHONY: all phase1 phase2 test test-p1 test-p2 clean

all: phase1 phase2

phase1:
	@echo "=========================================="
	@echo "       Building Phase 1 Compiler          "
	@echo "=========================================="
	@$(MAKE) -C phase1 all

phase2:
	@echo "=========================================="
	@echo "       Building Phase 2 Compiler          "
	@echo "=========================================="
	@$(MAKE) -C phase2 all

test: test-p1 test-p2

test-p1: phase1
	@echo "=========================================="
	@echo "       Running Phase 1 Test Suite         "
	@echo "=========================================="
	@$(MAKE) -C phase1 test

test-p2: phase2
	@echo "=========================================="
	@echo "       Running Phase 2 Test Suite         "
	@echo "=========================================="
	@$(MAKE) -C phase2 test

clean:
	@$(MAKE) -C phase1 clean
	@$(MAKE) -C phase2 clean
	@echo "All build outputs cleaned."
