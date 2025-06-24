.PHONY: all rv_runtime regression clean lama-rv

LAMA_RV_BUILD_DIR=$(PWD)/comp-build
LAMA_RV=$(LAMA_RV_BUILD_DIR)/lama-rv
RV_TRIPLET=riscv64-linux-gnu
RV_CC=$(RV_TRIPLET)-gcc

$(LAMA_RV_BUILD_DIR):
	mkdir -p $(LAMA_RV_BUILD_DIR)

build: lama-rv rv_runtime

lama-rv: | $(LAMA_RV_BUILD_DIR)
	cmake -DCMAKE_BUILD_TYPE=Debug -S comp -B $(LAMA_RV_BUILD_DIR)
	cmake --build $(LAMA_RV_BUILD_DIR)

rv_runtime:
	$(MAKE) -C runtime CC=$(RV_CC) build

regression: build
	$(MAKE) -C regression check LAMA_RV_BACKEND=$(LAMA_RV)

clean:
	rm -rf $(LAMA_RV_BUILD_DIR)
	$(MAKE) clean -C runtime
	$(MAKE) clean -C regression
	# $(MAKE) clean -C performance
